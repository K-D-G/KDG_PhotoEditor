#pragma once

//The font base path is used to access the fonts that are
//in OS specific locations
#ifdef _WIN32
#define FONT_BASE_PATH "C:/Windows/Fonts/"
#endif

#ifdef __APPLE__
#define FONT_BASE_PATH "/System/Library/Fonts/"
#endif

#ifdef __linux__
#define FONT_BASE_PATH "/usr/share/fonts/truetype/"
#endif