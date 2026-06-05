#pragma once
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "midifile/include/MidiFile.h"

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
		int velocity;
		double start_time;
		double end_time;
	};
public:
	std::vector<Note> notes;

	MidiNoteReader(const std::string& midi_file_path, unsigned char track_number = 0);

	const std::vector<Note>& getNotes() const;

};