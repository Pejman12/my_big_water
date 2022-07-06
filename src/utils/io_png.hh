#pragma once
#include <stdlib.h>
#include <png.h>

png_bytep *read_png(const char* filename, int *width, int *height);
