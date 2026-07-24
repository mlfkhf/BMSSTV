#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "sstvprocessor.hpp"
#include "langs.hpp"

MidiNoteToImage::MidiNoteToImage(
	const std::string& midi_file_path,
	unsigned char track_number,
	sstvformats_ sstvformat_,
	std::string* error,
	const double timescale,
	const double a4_freq,
	const bool check_mode
	)
{
	error->clear();
	smf::MidiFile midifile;
	if (!midifile.read(midi_file_path)) {
		*error = std::string(midifilecannotberead_bmj) + midi_file_path;
		return;
	}

	midifile.doTimeAnalysis();
	midifile.linkNotePairs();

	if (check_mode) //CHECK MODE
	{
		std::cout << "Tempo (BPM): " << midifile.getTicksPerQuarterNote() << std::endl
		<< "Number of tracks: " << midifile.getTrackCount() << std::endl;
		bool has_not_event = true;
		for (int track = 0; track < midifile.getTrackCount(); ++track) {
			has_not_event = true;
			std::cout << "Track " << track << " has " << midifile[track].size() << " events." << std::endl
			 << "Notes in track " << track << ":" << std::endl;
			for (unsigned int event = 0, note_count = 0; event < midifile[track].size(); event++) {
				smf::MidiEvent& mev = midifile[track][event];
				if (mev.isNoteOn()) {
					has_not_event = false;

					std::cout << "note " << note_count <<
						" :\tstart time:" <<
						std::fixed << std::setprecision(4) << mev.seconds * timescale << "s ";

					if (mev.getLinkedEvent()) {

						std::cout << "\tend time:" <<
							std::fixed << std::setprecision(4) << mev.getLinkedEvent()->seconds * timescale << "s ";

					}
					else std::cout << "\t\tno linked note off event";

					std::cout << "\tfreq: " << midiPitchToFrequency(mev[1], a4_freq) << "Hz" << std::endl;
					++note_count;
				}
			}
			if (has_not_event) std::cout << "No notes found in this track." << std::endl;
			std::cout << std::endl;
		}
		*error = test_mode_over_bmj;
		return;
	}

	std::cout << std::string(reading_track_bmj) << (int)track_number << std::endl;
	bool track_has_no_note = true;
	for (unsigned int event = 0; event < midifile[track_number].size(); event++) {
		smf::MidiEvent& mev = midifile[track_number][event];
		if (mev.isNoteOn()) {
			track_has_no_note = false;
			if (
				midiPitchToFrequency(mev[1], a4_freq) < BLACK_AUDIO_FREQ ||
				midiPitchToFrequency(mev[1], a4_freq) > WHITE_AUDIO_FREQ
			) continue;

			// mev.seconds 返回值为秒，将其转换为毫秒以保持项目内时间单位一致
			double start_time_ms = mev.seconds * 1000.0 * timescale;

			smf::MidiEvent* note_off = mev.getLinkedEvent();
			if (note_off) {
				double end_time_ms = note_off->seconds * 1000.0 * timescale;
				notes.push_back({ midiPitchToFrequency(mev[1], a4_freq), start_time_ms, end_time_ms });
			}
		}
	}
	std::cout << notes.size() << notes_have_been_translated_bmj << std::endl;
	if (track_has_no_note) {
		*error = std::string(track_has_no_note_bmj) + std::to_string((int)track_number);
		return;
	}
	if (notes.empty()) {
		*error = std::string(no_note_has_been_translated_bmj);
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

const MidiNoteToImage::sstvformats_* MidiNoteToImage::getSSTVbitimage() const
{
	return reinterpret_cast<const sstvformats_*>(sstvbitimage);
}

inline double MidiNoteToImage::midiPitchToFrequency(int pitch, double a4_freq)
{
	return a4_freq * std::pow(2.0, (pitch - 57) / 12.0);
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
	default: color_time = MARTIN1_SINGLEPIXEL_TIME; break;
	}

	constexpr double eps = 1e-9; // 容差，避免浮点比较问题
	const size_t total_pixels = static_cast<size_t>(SSTV_WIDTH) * static_cast<size_t>(SSTV_HEIGHT) * 3;
	if (total_pixels == 0) return;

	// notes -> pixels 映射，使用更精确的时间到像素区间映射
	for (const auto& note : notes)
	{
		// 忽略无效时长
		if (note.end_time <= note.start_time) continue;

		// 计算包含与 note 时间段有交叠的所有像素索引 [start_idx, end_idx]
		double start_d = (note.start_time + eps) / color_time;
		double end_d = (note.end_time - eps) / color_time;

		// 如果整个区间在像素时间之前，则跳过
		if (end_d < 0.0) continue;

		// 使用 floor/ceil 提高精度
		size_t start_idx = 0;
		if (start_d > 0.0) start_idx = static_cast<size_t>(std::floor(start_d));

		// 计算 end_idx = ceil(end_d) - 1，保证覆盖到 partial pixel
		size_t end_idx = 0;
		if (end_d > 0.0) {
			double ce = std::ceil(end_d);
			end_idx = (ce > 0.0) ? static_cast<size_t>(ce - 1.0) : 0;
		}

		// 裁剪到合法范围
		if (start_idx >= total_pixels) continue;
		if (end_idx >= total_pixels) end_idx = total_pixels - 1;
		if (start_idx > end_idx) continue;

		// 写入像素值，使用 clamp 避免溢出
		for (size_t j = start_idx; j <= end_idx; ++j)
		{
			double normalized = ((note.pitch - 1500.0) / (2300.0 - 1500.0)) * 255.0;
			double clamped = std::clamp(normalized, 0.0, 255.0);
			sstvbitimage[j] = static_cast<unsigned char>(clamped);
		}
	}

	// Martin: GBR
	// Scottie: GRB
	for (unsigned int i = 0; i < SSTV_WIDTH * SSTV_HEIGHT; i++)
	{
		switch (sstvformat)
		{
		case martin1: [[fallthrough]];
		case martin2:
			std::swap(sstvbitimage[i * 3], sstvbitimage[i * 3 + 2]); //(R) G (B) -> (B) G (R)
			[[fallthrough]]; //(B) (G) R -> (G) (B) R, fallthrough to scottiedx case.
		case scottie1: [[fallthrough]];
		case scottie2: [[fallthrough]];
		case scottiedx:
			std::swap(sstvbitimage[i * 3], sstvbitimage[i * 3 + 1]); //(R) (G) B -> (G) (R) B
			break;
		default:
			break;
		}
	}
}