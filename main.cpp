/*
* BMSSTV - Build Music SSTV
* This project MUST be built with or with above C++ 20 standard
*/

#define BMSSTV_LANG BMSSTV_LANG_EN
#include "langs.hpp"

#include <iostream>
#include <string>
#include "libremidi/include/libremidi/api.hpp"
#include "CLI11/include/CLI/CLI.hpp"
#include <bitset>
#include <vector>
#include <list>
#include <set>

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
		sstv_format
		;

	app.add_option("-m,-i,--midiinput", midi_file_path,
		"A midi file contains the reference music.");
	app.add_option("-o,--outputfile", output_image_file_path, outputfile_bmj);
	app.add_option("-f,--sstvformat", midi_file_path, sstvformat_bmj);

	try {
		app.parse(argc, argv);

		if (sstv_formats.find(sstv_format) == sstv_formats.end())
			throw CLI::ParseError(invailesstvformat_bmj + sstv_format, 1);
	}
	catch (const CLI::ParseError& e) {
		return app.exit(e);
	}
	return 0;
}