#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "sstvprocessor.hpp"
#include "langs.hpp"

MidiNoteReader::MidiNoteReader(const std::string& midi_file_path, double tempo, unsigned char track_number)
{
	smf::MidiFile midifile;
	midifile.linkNotePairs();
	midifile.doTimeAnalysis();
	double ticks_per_quarter = midifile.getTicksPerQuarterNote();
}

const std::vector<MidiNoteReader::Note>& MidiNoteReader::getNotes() const
{
	return notes;
}
