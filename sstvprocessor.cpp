#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "sstvprocessor.hpp"
#include "langs.hpp"


MidiNoteReader::MidiNoteReader(const std::string& midi_file_path, unsigned char track_number)
{
	smf::MidiFile midifile;
	midifile.linkNotePairs();
	midifile.doTimeAnalysis();
	double ticks_per_quarter = midifile.getTicksPerQuarterNote();
    double current_tempo = 120; //default tempo is 120;
    for (int event = 0; event < midifile[track_number].size(); event++) {
        smf::MidiEvent& mev = midifile[track_number][event];
        if (mev.isTempo()) current_tempo = mev.getTempoBPM();
	} //ÎÞÄÎÖ®¾Ù£¡

    for (int event = 0; event < midifile[track_number].size(); event++) {
        smf::MidiEvent& mev = midifile[track_number][event];
        if (mev.isNoteOn()) {
            Note note;
            note.pitch = mev[1];
            note.start_time = tickToSeconds(mev.tick, midifile.getTicksPerQuarterNote(), current_tempo);

            smf::MidiEvent* note_off = mev.getLinkedEvent();
            if (note_off) {
                note.end_time = tickToSeconds(mev.tick, midifile.getTicksPerQuarterNote(), current_tempo);
            }
            else {
                note.end_time = note.start_time + tickToSeconds(midifile.getTicksPerQuarterNote(), midifile.getTicksPerQuarterNote(), current_tempo);
            }

            notes.push_back(note);
        }
        else if (mev.isTempo()) current_tempo = mev.getTempoBPM();
    }
}

const std::list<MidiNoteReader::Note>& MidiNoteReader::getNotes() const
{
	return notes;
}

inline double MidiNoteReader::tickToSeconds(int tick, double ticks_per_quarter, double tempo_bpm)
{
    return tick * (60.0 / tempo_bpm) / ticks_per_quarter;
}