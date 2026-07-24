/*
* BMSSTV - Build Music SSTV
* This project MUST be built with or with above C++ 20 standard
*/

#include "langs.hpp"
#include "sstvprocessor.hpp"
#include "constvalue.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include <bitset>
#include <vector>
#include <list>
#include <set>

#include "include/CLI/CLI.hpp"

#define VERSION_C "ver 0.21alpha Jun,24,2026"
#define COPYRIGHT "BMSSTV Copyright (c) 2026 BH6BMJ. Licensed under the MIT License"

const std::set<std::string> sstv_formats = {
	"martin1","mt1",
	"martin2", "mt2",
	"scottie1", "sct1",
	"scottie2", "sct2",
	"scottiedx", "sctdx"
};

const std::set<std::string> output_image_formats = { "png", "bmp" ,"jpg", "jpeg" };
enum output_image_formatse {png, bmp, jpg};

int main(int argc, char** argv)
{
	CLI::App app{ (std::string("BMSSTV - Build Music SSTV ") + VERSION_C + " by BH6BMJ") };
	argv = app.ensure_utf8(argv);
	app.allow_windows_style_options();

	std::string
		midi_file,
		output_image_file,
		sstv_format_argc;
	unsigned char track_number;
	double
		a4_freq = 1800.0,
		timescale = 1.0;
	bool check_mode = false;
	std::string error;

	app.add_option("-m,-i,--midiinput", midi_file, midiinput_bmj)->check(
		[](const std::string& filename) -> std::string {
			if (!std::filesystem::exists(filename)) return midifile404_bmj + filename;
			smf::MidiFile midifile;
			if (!midifile.read(filename)) return cannotreadmidifile_bmj + filename;
			if (midifile.getTrackCount() == 0) return emptytrack_bmj + filename;
			return "";
		}
	);
	app.add_option("-o,--outputfile", output_image_file, outputfile_bmj)->check(
		[](const std::string& imagefileext)->std::string
		{
			std::filesystem::path path(imagefileext);
			if (path.has_parent_path()) return filename_is_a_path_bmj;
			auto ext = path.extension().string();
			if (!ext.empty()) {
				ext = ext.substr(1);
				if (output_image_formats.find(ext) == output_image_formats.end()) return invaildimageformat_bmj + ext;
				return "";
			}
			else return invaildimageformat_bmj + ext;
		}
	);
	app.add_option("-f,--sstvformat", sstv_format_argc, sstvformat_bmj)->check(CLI::IsMember(sstv_formats));
	
	app.add_option("-t,--track", track_number, tracknumber_bmj)
		->default_val(2)
		->check(CLI::Range(0, 255));

	app.add_option("--a4equals", a4_freq, a4freq_bmj)
		->default_val(1800.0)
		->check(CLI::Range(20.0, 20000.0));

	app.add_option("--timescale", timescale, timescale_bmj)
		->default_val(1.0)
		->check(CLI::Range(0.1, 4.0));

	app.add_flag("-c,--check", check_mode, checkmode_bmj);

	app.add_flag_callback("-v,--about", []() 
		{
			std::cout << VERSION_C << '\n' << COPYRIGHT << std::endl;
			throw CLI::Success();
		}, version_bmj);
	app.set_version_flag("--version", std::string(VERSION_C) + '\n' + COPYRIGHT);

	try {
		if (argc == 1) throw CLI::CallForHelp();
		app.parse(argc, argv);
		
	}
	catch (const CLI::ParseError& e) {
		return app.exit(e);
	}
	
	std::cout << "Reading MIDI file: " << midi_file << std::endl;
	if (check_mode) {
		std::cout << "TESTING MODE ON" << std::endl;
		std::cout << "A4 frequency: " << a4_freq << "Hz" << std::endl;
		std::cout << "Time scale: " << std::fixed << std::setprecision(2) << timescale << std::endl << std::endl;
	}
	MidiNoteToImage noteslist(
		midi_file,
		track_number,
		[&]() -> auto {
			if (sstv_format_argc == "martin1" || sstv_format_argc == "mt1") return MidiNoteToImage::sstvformats_::martin1;
			if (sstv_format_argc == "martin2" || sstv_format_argc == "mt2") return MidiNoteToImage::sstvformats_::martin2;
			if (sstv_format_argc == "scottie1" || sstv_format_argc == "sct1") return MidiNoteToImage::sstvformats_::scottie1;
			if (sstv_format_argc == "scottie2" || sstv_format_argc == "sct2") return MidiNoteToImage::sstvformats_::scottie2;
			if (sstv_format_argc == "scottiedx" || sstv_format_argc == "sctdx") return MidiNoteToImage::sstvformats_::scottiedx;
			return MidiNoteToImage::sstvformats_::martin1;
		}(),
		&error,
		timescale,
		a4_freq,
		check_mode
		);

	if (!error.empty()) 
		if (error == test_mode_over_bmj)
		{
			std::cout << test_mode_over_bmj << std::endl;
			return app.exit(CLI::Success());
		}
		return app.exit(CLI::ValidationError(error, 0)); //Test mode stops here.
	noteslist.generateBitImage();

	std::cout << generating_image_bmj << std::endl;
	auto ext = std::filesystem::path(output_image_file).extension().string().substr(1);
	if (ext == "png") {
		if (!stbi_write_png(output_image_file.c_str(), SSTV_WIDTH, SSTV_HEIGHT, 3, noteslist.getSSTVbitimage(), SSTV_WIDTH * 3))
			return app.exit(CLI::ValidationError(generate_image_failed_bmj, 0));
	}
	else if (ext == "bmp") {
		if (!stbi_write_bmp(output_image_file.c_str(), SSTV_WIDTH, SSTV_HEIGHT, 3, noteslist.getSSTVbitimage()))
			return app.exit(CLI::ValidationError(generate_image_failed_bmj, 0));
	}
	else if (ext == "jpg" || ext == "jpeg") {
		if (!stbi_write_jpg(output_image_file.c_str(), SSTV_WIDTH, SSTV_HEIGHT, 3, noteslist.getSSTVbitimage(), 100))
			return app.exit(CLI::ValidationError(generate_image_failed_bmj, 0));
	}
	else
		return app.exit(CLI::ParseError(invaildimageformat_bmj + ext, 0));
	std::cout << generate_image_success_bmj << output_image_file << std::endl;
	return app.exit(CLI::Success());
}