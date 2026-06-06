#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "sstvprocessor.hpp"
#include "langs.hpp"


MidiNoteToImage::MidiNoteToImage(const std::string& midi_file_path, unsigned char track_number, sstvformats_ sstvformat_)
{
	smf::MidiFile midifile;
	midifile.linkNotePairs();
	midifile.doTimeAnalysis();
	double ticks_per_quarter = midifile.getTicksPerQuarterNote();
    double current_tempo = 120; //default tempo is 120;
    for (int event = 0; event < midifile[track_number].size(); event++) {
        smf::MidiEvent& mev = midifile[track_number][event];
        if (mev.isTempo())  {
            current_tempo = mev.getTempoBPM();
            break;
        }
	} //ÎÞÄÎÖ®¾Ù£¡

    for (int event = 0; event < midifile[track_number].size(); event++) {
        smf::MidiEvent& mev = midifile[track_number][event];
        Note note;
        if (mev.isNoteOn()) {
            if (mev[1] <= BLACK_AUDIO_FREQ || mev[1] >= WHITE_AUDIO_FREQ) continue;
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

const std::list<MidiNoteToImage::Note>& MidiNoteToImage::getNotes() const
{
	return notes;
}

const MidiNoteToImage::sstvformats_& MidiNoteToImage::getSSTVformat() const
{
	return sstvformat;
}

inline double MidiNoteToImage::tickToSeconds(int tick, double ticks_per_quarter, double tempo_bpm)
{
    return tick * (60.0 / tempo_bpm) / ticks_per_quarter;
}

inline double MidiNoteToImage::midiPitchToFrequency(int pitch, double a4_freq)
{
    return a4_freq * pow(2.0, (pitch - 69) / 12.0);
}