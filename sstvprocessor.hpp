#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "include/stb/stb_image.h"
#include "include/stb/stb_image_write.h"
#include "include/midifile/MidiFile.h"
#include "constvalue.hpp"

#include <list>
#include <algorithm>
#include <map>
#include <iomanip>

using sstvbitimage_ = unsigned char[SSTV_HEIGHT * SSTV_WIDTH * 3]; // 256x320, 3 bytes per pixel

class MidiNoteToImage
{
public:
	struct Note
	{
		double pitch;
		double start_time;
		double end_time;
	};
	enum sstvformats_
	{
		martin1,
		martin2,
		scottie1,
		scottie2,
		scottiedx
	};
	MidiNoteToImage(
		const std::string& midifile_,
		unsigned char track_number,
		sstvformats_ sstvformat_,
		std::string* error,
		const double timescale = 1.0,
		const double a4_herz = 1800,
		const short key = 0,
		const bool check_mode = false);

	~MidiNoteToImage();

	const sstvformats_& getSSTVformat() const;
	const sstvformats_* getSSTVbitimage() const;
	static double midiPitchToFrequency(unsigned short pitch, double a4_freq = 440.0, signed short key = 0);
	void generateBitImage();
private:
	std::list<Note> notes;
	sstvformats_ sstvformat = sstvformats_::martin1;
	sstvbitimage_* sstvbitimage = nullptr;
};