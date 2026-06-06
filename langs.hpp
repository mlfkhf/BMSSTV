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
#define tempo_bmj "要应用的 MIDI 文件音乐的速度，过快的音符会被忽略。"
#define midifile404_bmj "文件不存在:"
#define cannotreadmidifile_bmj "无法读取 MIDI 文件 (文件可能损坏或不是有效的 MIDI 格式): "
#define emptytrack_bmj "MIDI 文件没有音轨: "


#elif BMSSTV_LANG == BMSSTV_LANG_EN
#define midiinput_bmj "A midi file contains the reference music."
#define outputfile_bmj "The path and filename of image output, file suffix decides image format(support .png .bmp .jpeg)."
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
#define tempo_bmj "Tempo to be modified.Too fast notes will be ignored."
#define midifile404_bmj "File not found:"
#define cannotreadmidifile_bmj "Failed to read MIDI file (file may be damaged or not a valid MIDI format): "
#define emptytrack_bmj "MIDI file has no track: "
#endif