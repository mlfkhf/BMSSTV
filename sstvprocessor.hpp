#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "include/stb/stb_image.h"
#include "include/stb/stb_image_write.h"
#include "include/midifile/MidiFile.h"
#include <list>

enum sstvformats_
{
	martin1,
	martin2,
	scottie1,
	scottie2,
	scottiedx
};


class MidiNoteReader
{
	struct Note
	{
		int pitch;
		double start_time;
		double end_time;
	};
public:
	std::list<Note> notes;

	MidiNoteReader(const std::string& midi_file_path, unsigned char track_number);

	const std::list<Note>& getNotes() const;
	static double tickToSeconds(int tick, double ticks_per_quarter, double tempo_bpm);
};