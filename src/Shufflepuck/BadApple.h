#pragma once
class BadAppleVideo
{
public:
	BadAppleVideo();

	/// Loads video file in memory
	bool Load(const char* path);
	//frees memory
	void Clear();

	bool IsLoaded() const { return m_count > 0; }
	int Width() const { return m_width; }
	int Height() const { return m_height; }
	int Fps() const { return m_fps; }
	int FrameCount() const { return m_count; }
	// true for 16 gray levels, false for a black and white
	bool IsGray() const { return m_gray; }

	/// Decodes one frame.
	bool Decode(int frame, std::vector<unsigned char>& out) const;

private:
	int m_width;							//pixels
	int m_height;							//pixels
	int m_fps;								//frames per second
	int m_count;							//frames
	bool m_gray;							//16 gray levels instead of black and white
	std::vector<unsigned char> m_data;		//every payload, one after the other
	std::vector<size_t> m_offset;			//start of each payload in m_data
};