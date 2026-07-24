#pragma once
#define BMSSTV_LANG_ZH_CN 0
#define BMSSTV_LANG_EN 1

#ifndef BMSSTV_LANG
//#define BMSSTV_LANG BMSSTV_LANG_ZH_CN
#define BMSSTV_LANG BMSSTV_LANG_EN
#endif
#if BMSSTV_LANG != BMSSTV_LANG_ZH_CN && BMSSTV_LANG != BMSSTV_LANG_EN
#error Language not supported
#endif

#if BMSSTV_LANG == BMSSTV_LANG_ZH_CN
#define midiinput_bmj "一个包含参考音乐的 midi 文件。"
#define outputfile_bmj "图像输出的路径和文件名，文件扩展名决定图像格式（支持 .png .bmp .jpeg）。"
#define sstvformat_bmj "SSTV 格式。\n\
		supprts:\n\
		....martin1 - 输入martin1/mt1\n\
		....martin2 - 输入martin2/mt2\n\
		....scottie1 - 输入scottie1/sct1\n\
		....scottie2 - 输入scottie2/sct2\n\
		....scottiedx - 输入scottiedx/sctdx\n\
		"
#define invailesstvformat_bmj "无效的 SSTV 格式: "
#define tracknumber_bmj "音轨编号（单音轨时无需指定）"
#define midifilecannotberead_bmj "无法读取的 MIDI 文件: "
#define timescale_bmj "MIDI音乐文件的时间倍率，以小数表示，范围为0.1到4.0。默认为1.0"
#define midifile404_bmj "文件不存在: "
#define cannotreadmidifile_bmj "无法读取 MIDI 文件 (文件可能损坏或不是有效的 MIDI 格式): "
#define emptytrack_bmj "MIDI 文件没有音轨: "
#define noteon_bmj "是否将 MIDI 文件中的 Note On 事件视为 SSTV 图像的有效音符（默认启用）。"
#define invaildimageformat_bmj "无效的图像格式: "
#define generate_image_failed_bmj "生成图像失败。"
#define a4freq_bmj "A4 音符的频率，单位为赫兹（默认是 1800Hz）。"
#define filename_is_a_path_bmj "文件名是一个路径，请仅指定文件名。"
#define track_has_no_note_bmj "指定的音轨没有音符："
#define checkmode_bmj "检查模式，检查 MIDI 文件是否可读，而不生成图像。"
#define version_bmj "输出程序的版本信息。"
#define test_mode_over_bmj "测试模式结束"


#elif BMSSTV_LANG == BMSSTV_LANG_EN
#define midiinput_bmj "A midi file contains the reference music."
#define outputfile_bmj "The filename of image output, file suffix decides image format(support .png .bmp .jpeg)."
#define sstvformat_bmj "SSTV formats.\n\
		supprts:\n\
		....martin1 - type martin1/mt1\n\
		....martin2 - type martin2/mt2\n\
		....scottie1 - type scottie1/sct1\n\
		....scottie2 - type scottie2/sct2\n\
		....scottiedx -type scottiedx/sctdx\
		"
#define invailesstvformat_bmj "Invalid SSTV format: "
#define tracknumber_bmj "Track number (no need to specify when single track)."
#define midifilecannotberead_bmj "Failed to read MIDI file: "
#define timescale_bmj "MIDI music file time scale, expressed as a decimal, ranging from 0.1 to 4.0. Default as 1.0."
#define midifile404_bmj "File not found: "
#define cannotreadmidifile_bmj "Failed to read MIDI file (file may be damaged or not a valid MIDI format): "
#define emptytrack_bmj "MIDI file has no track: "
#define noteon_bmj "Whether to consider Note On events in MIDI file as valid notes for SSTV image (enabled by default)."
#define invaildimageformat_bmj "Invalid image format: "
#define generate_image_failed_bmj "Failed to generate image."
#define a4freq_bmj "The frequency of A4 note in Hertz (default is 1800Hz)."
#define filename_is_a_path_bmj "The filename is a path, please specify the filename only."
#define track_has_no_note_bmj "The specified track has no notes: "
#define checkmode_bmj "Check mode, check if the MIDI file is readable without generating an image."
#define version_bmj "Output the version information of the program."
#define test_mode_over_bmj "TEST MODE OVER"

#endif