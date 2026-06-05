/*
* BMSSTV - Build Music SSTV
* This project MUST be built with or with above C++ 20 standard
*/

#include "langs.hpp"
#include "sstvprocessor.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include <bitset>
#include <vector>
#include <list>
#include <set>

#include "CLI11/include/CLI/CLI.hpp"

#define VERSION_C "ver build20260531"
#define VERSION std::string(VERSION_C)

const std::set<std::string> sstv_formats = {
	"martin1","mt1",
	"martin2", "mt2"
	"scottie1", "sct1"
	"scottie2", "sct2"
	"scottiedx", "sctdx"
};

int main(int argc, char** argv)
{
	CLI::App app{ (std::string("BMSSTV - Build Music SSTV ") + VERSION + " by BH6BMJ") };
	argv = app.ensure_utf8(argv);
	app.require_subcommand();

	std::string
		midi_file_path,
		output_image_file_path,
		sstv_format_argc;
	unsigned char track_number;

	app.add_option("-m,-i,--midiinput", midi_file_path, midiinput_bmj)->check(CLI::ExistingFile);
	app.add_option("-o,--outputfile", output_image_file_path, outputfile_bmj);
	app.add_option("-f,--sstvformat", sstv_format_argc, sstvformat_bmj)->check(CLI::IsMember(sstv_formats));
	app.add_option("-t,--track", track_number, tracknumber_bmj)
		->default_val(0)
		->check(CLI::Range(0, 255));

	try {
		app.parse(argc, argv);
		
	}
	catch (const CLI::ParseError& e) {
		return app.exit(e);
	}

	sstvformats_ sstvformat;
	{
		if (sstv_format_argc == "martin1" || sstv_format_argc == "mt1") sstvformat = martin1;
		else if (sstv_format_argc == "martin2" || sstv_format_argc == "mt2") sstvformat = martin2;
		else if (sstv_format_argc == "scottie1" || sstv_format_argc == "sct1") sstvformat = scottie1;
		else if (sstv_format_argc == "scottie2" || sstv_format_argc == "sct2") sstvformat = scottie2;
		else if (sstv_format_argc == "scottiedx" || sstv_format_argc == "sctdx") sstvformat = scottiedx;
	}
	
	return 0;
}