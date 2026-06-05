#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "sstvprocessor.hpp"
#include "langs.hpp"

MidiNoteReader::MidiNoteReader(const std::string& midi_file_path, unsigned char track_number)
{
	smf::MidiFile midifile;
	if (!midifile.read(midi_file_path))
	{
		throw std::runtime_error(midifilecannotberead_bmj + midi_file_path);
	}
	midifile.linkNotePairs();
	midifile.doTimeAnalysis();
	double ticks_per_quarter = midifile.getTicksPerQuarterNote();
	double tempo = 120.0;
}

const std::vector<MidiNoteReader::Note>& MidiNoteReader::getNotes() const
{
	return notes;
}
