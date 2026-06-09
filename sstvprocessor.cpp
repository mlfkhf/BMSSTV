#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "sstvprocessor.hpp"
#include "langs.hpp"


MidiNoteToImage::MidiNoteToImage(const std::string& midi_file_path, unsigned char track_number, sstvformats_ sstvformat_)
{
	smf::MidiFile midifile;
	if (!midifile.read(midi_file_path)) {
		throw std::runtime_error(midifilecannotberead_bmj + midi_file_path);
	}

	midifile.linkNotePairs();
	midifile.doTimeAnalysis();

	for (int event = 0; event < midifile[track_number].size(); event++) {
		smf::MidiEvent& mev = midifile[track_number][event];

		if (mev.isNoteOn()) {
			if (midiPitchToFrequency(mev[1]) <= BLACK_AUDIO_FREQ || midiPitchToFrequency(mev[1]) >= WHITE_AUDIO_FREQ) continue;

			double start_time = mev.seconds;

			smf::MidiEvent* note_off = mev.getLinkedEvent();
			if (note_off) {
				double end_time = mev.seconds;
				notes.push_back({ midiPitchToFrequency(mev[1]), start_time, end_time });
			}
		}
	}

	notes.sort([](const Note& a, const Note& b) {
		return a.start_time < b.start_time;
		});

	auto it = notes.begin();
	while (it != notes.end()) {
		auto next = std::next(it);
		while (next != notes.end() && std::abs(next->start_time - it->start_time) < 0.001) {
			it->pitch = std::max(it->pitch, next->pitch);
			it->end_time = std::max(it->end_time, next->end_time);
			next = notes.erase(next);
		}
		++it;
	}
}

const MidiNoteToImage::sstvformats_& MidiNoteToImage::getSSTVformat() const
{
	return sstvformat;
}

inline double MidiNoteToImage::midiPitchToFrequency(int pitch, double a4_freq)
{
	return a4_freq * pow(2.0, (pitch - 69) / 12.0);
}

void MidiNoteToImage::generateBitImage()
{
	double color_time;
	switch (sstvformat)
	{
	case martin1: color_time = MARTIN1_SINGLEPIXEL_TIME; break;
	case martin2: color_time = MARTIN2_SINGLEPIXEL_TIME; break;
	case scottie1: color_time = SCOTTIE1_SINGLEPIXEL_TIME; break;
	case scottie2: color_time = SCOTTIE2_SINGLEPIXEL_TIME; break;
	case scottiedx: color_time = SCOTTIEDX_SINGLEPIXEL_TIME; break;
	}
	for (auto i = notes.begin(); i != notes.end(); ++i)
	{
		if ((unsigned int)((i->start_time) / color_time) >= SSTV_WIDTH * SSTV_HEIGHT * 3) break;
		for (unsigned int j = (unsigned int)((i->start_time) / color_time);
			(j < SSTV_WIDTH * SSTV_HEIGHT * 3) && (j < (unsigned int)((i->end_time) / color_time));
			++j)
		{
			sstvbitimage[j] = (unsigned char)(((i->pitch - 1500.0) / (2300.0 - 1500.0)) * 255.0);
		}
	}
}