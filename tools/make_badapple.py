#!/usr/bin/env python3
"""
Converts a video into the badapple.bin file read by BadAppleVideo (BadApple.h).
Normal output : 
  python make_badapple.py badapple.mp4 badapple.bin

Gray level ouput:
  python make_badapple.py badapple.mp4 badapple.bin --gray
"""
import argparse
import struct
import subprocess
import sys


def varint(value, out):
    while True:
        b = value & 0x7F
        value >>= 7
        if value:
            out.append(b | 0x80)
        else:
            out.append(b)
            return


def runs(frame):
    """(value, length) runs over the whole frame, raster order."""
    out = []
    cur = frame[0]
    n = 0
    for v in frame:
        if v == cur:
            n += 1
        else:
            out.append((cur, n))
            cur, n = v, 1
    out.append((cur, n))
    return out


def encode_bw(frame, threshold):
    bits = [1 if v >= threshold else 0 for v in frame]
    out = bytearray()
    color = 0
    cur = 0
    n = 0
    for b in bits:
        if b == cur:
            n += 1
        else:
            varint(n, out)
            cur = b
            n = 1
    varint(n, out)
    return bytes(out)


def encode_gray(frame):
    levels = bytes((v * 15 + 127) // 255 for v in frame)  # 0..15
    out = bytearray()
    for level, n in runs(levels):
        while n > 0:
            if n >= 16:
                take = n
                out.append((level << 4) | 15)
                varint(take - 16, out)
                n = 0
            else:
                out.append((level << 4) | (n - 1))
                n = 0
    return bytes(out)


def main():
    ap = argparse.ArgumentParser(description="video -> badapple.bin (BAD1 / BAD2)")
    ap.add_argument("input", help="source video (anything ffmpeg reads)")
    ap.add_argument("output", help="badapple.bin")
    ap.add_argument("--size", default="240x180", help="WIDTHxHEIGHT of the frames (default 240x180)")
    ap.add_argument("--fps", type=int, default=30, help="frames per second (default 30)")
    ap.add_argument("--gray", action="store_true", help="16 gray levels (BAD2) instead of black and white (BAD1)")
    ap.add_argument("--threshold", type=int, default=128, help="black / white threshold for BAD1 (default 128)")
    args = ap.parse_args()

    try:
        width, height = (int(x) for x in args.size.lower().split("x"))
    except ValueError:
        sys.exit("--size must look like 240x180")
    if not (0 < width < 65536 and 0 < height < 65536 and 0 < args.fps < 65536):
        sys.exit("size / fps out of range")

    cmd = ["ffmpeg", "-v", "error", "-i", args.input, "-an",
           "-vf", "fps=%d,scale=%d:%d:flags=area,format=gray" % (args.fps, width, height),
           "-f", "rawvideo", "-pix_fmt", "gray", "-"]
    try:
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    except FileNotFoundError:
        sys.exit("ffmpeg not found (install it and put it in the PATH)")

    size = width * height
    payloads = []
    while True:
        frame = proc.stdout.read(size)
        if len(frame) < size:
            break
        payloads.append(encode_gray(frame) if args.gray else encode_bw(frame, args.threshold))
    proc.wait()
    if proc.returncode != 0 or not payloads:
        sys.exit("ffmpeg failed or the video has no frame")

    with open(args.output, "wb") as f:
        f.write(b"BAD2" if args.gray else b"BAD1")
        f.write(struct.pack("<HHHHI", width, height, args.fps, 0, len(payloads)))
        for p in payloads:
            f.write(struct.pack("<I", len(p)))
            f.write(p)
    total = sum(len(p) for p in payloads) + 16 + 4 * len(payloads)
    print("%s: %d frames %dx%d @ %d fps, %s, %.1f KB" %
          (args.output, len(payloads), width, height, args.fps, "16 gray levels" if args.gray else "black and white", total / 1024))

    if args.audio:
        r = subprocess.run(["ffmpeg", "-v", "error", "-y", "-i", args.input, "-vn", "-ac", "1", "-ar", "22050",
                            "-c:a", "pcm_s16le", args.audio])
        if r.returncode != 0:
            sys.exit("audio export failed (does the video have an audio track?)")
        print("audio: " + args.audio)


if __name__ == "__main__":
    main()
