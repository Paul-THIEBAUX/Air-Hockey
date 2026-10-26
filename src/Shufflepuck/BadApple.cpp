#include "pch.h"
#include <fstream>

//empty video
BadAppleVideo::BadAppleVideo()
{
	m_width = 0;
	m_height = 0;
	m_fps = 30;
	m_count = 0;
	m_gray = false;
}

void BadAppleVideo::Clear()
{
	m_width = 0;
	m_height = 0;
	m_fps = 30;
	m_count = 0;
	m_gray = false;
	std::vector<unsigned char>().swap(m_data);
	std::vector<size_t>().swap(m_offset);
}

//Reads a little endian 16 bit value
static unsigned int ReadU16(const unsigned char* p)
{
	return (unsigned int)p[0] | ((unsigned int)p[1] << 8);
}

//Reads a little endian 32 bit value
static unsigned int ReadU32(const unsigned char* p)
{
	return (unsigned int)p[0] | ((unsigned int)p[1] << 8) | ((unsigned int)p[2] << 16) | ((unsigned int)p[3] << 24);
}

bool BadAppleVideo::Load(const char* path)
{
	Clear();

	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file)
		return false;
	const std::streamoff fileSize = file.tellg();
	if (fileSize<16 || fileSize>(std::streamoff)0x7FFFFFFF)
		return false;
	std::vector<unsigned char> all((size_t)fileSize);
	file.seekg(0);
	file.read((char*)all.data(), fileSize);
	if (!file)
		return false;

	//Header
	const bool gray = memcmp(all.data(), "BAD2", 4) == 0;
	if (gray == false && memcmp(all.data(), "BAD1", 4) != 0)
		return false;
	const int width = (int)ReadU16(&all[4]);
	const int height = (int)ReadU16(&all[6]);
	const int fps = (int)ReadU16(&all[8]);
	const unsigned int count = ReadU32(&all[12]);
	if (width <= 0 || height <= 0 || fps <= 0 || count == 0 || count > 1000000)
		return false;
	if ((long long)width * (long long)height > 64LL * 1024LL * 1024LL)
		return false;

	//Frames, only the payloads are kept
	std::vector<unsigned char> data;
	std::vector<size_t> offset;
	data.reserve(all.size());
	offset.reserve(count + 1);
	size_t pos = 16;
	for (unsigned int i = 0; i < count; i++)
	{
		if (pos + 4 > all.size())
			return false;
		const size_t size = ReadU32(&all[pos]);
		pos += 4;
		if (size > all.size() - pos)
			return false;
		offset.push_back(data.size());
		data.insert(data.end(), all.begin() + pos, all.begin() + pos + size);
		pos += size;
	}
	offset.push_back(data.size());

	m_width = width;
	m_height = height;
	m_fps = fps;
	m_count = (int)count;
	m_gray = gray;
	m_data.swap(data);
	m_offset.swap(offset);
	return true;
}

bool BadAppleVideo::Decode(int frame, std::vector<unsigned char>& out) const
{
	if (frame < 0 || frame >= m_count)
		return false;

	const size_t total = (size_t)m_width * (size_t)m_height;
	out.assign(total, 0);

	const unsigned char* p = m_data.data() + m_offset[frame];
	const unsigned char* end = m_data.data() + m_offset[frame + 1];
	size_t pixel = 0;

	//LEB128 varint
	auto readVarint = [&](size_t& value)
		{
			value = 0;
			int shift = 0;
			while (p < end)
			{
				const unsigned char b = *p++;
				value |= (size_t)(b & 0x7F) << shift;
				if ((b & 0x80) == 0)
					return true;
				shift += 7;
				if (shift > 28)
					return false;
			}
			return false;
		};

	if (m_gray)
	{
		while (p < end && pixel < total)
		{
			const unsigned char b = *p++;
			const auto gray = (unsigned char)((b >> 4) * 17);
			size_t run = (size_t)(b & 15) + 1;
			if ((b & 15) == 15)
			{
				size_t extra = 0;
				if (readVarint(extra) == false)
					return false;
				run = 16 + extra;
			}
			if (run > total - pixel)
				run = total - pixel;
			memset(&out[pixel], gray, run);
			pixel += run;
		}
		return pixel == total;
	}

	unsigned char color = 0;
	while (p < end && pixel < total)
	{
		size_t run = 0;
		if (readVarint(run) == false)
			return false;
		if (run > total - pixel)
			run = total - pixel;
		if (color)
			memset(&out[pixel], 255, run);
		pixel += run;
		color ^= 1;
	}
	return pixel == total;
}