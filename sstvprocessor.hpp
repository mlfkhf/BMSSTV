#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "include/stb/stb_image.h"
#include "include/stb/stb_image_write.h"
#include "include/midifile/MidiFile.h"
#include "constvalue.hpp"
#include <list>

using sstvbitimage_ = unsigned char[SSTV_HEIGHT_MARTIN_AND_SCOTTIE][SSTV_WIDTH_MARTIN_AND_SCOTTIE * 3]; // 256x320, 3 bytes per pixel (RGB)

class MidiNoteToImage
{
public:
	struct Note
	{
		int pitch;
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
	MidiNoteToImage(const std::string& midi_file_path, unsigned char track_number, sstvformats_ sstvformat_);

	const std::list<Note>& getNotes() const;
	const sstvformats_& getSSTVformat() const;
	static double tickToSeconds(int tick, double ticks_per_quarter, double tempo_bpm);
	static double midiPitchToFrequency(int pitch, double a4_freq = 440.0);
	void generateBitImage();
	sstvbitimage_& getBitImage();
private:
	std::list<Note> notes;
	sstvformats_ sstvformat;
	sstvbitimage_ sstvbitimage;
};