#include "io_png.hh"
#include <err.h>

png_bytep* read_png(const char* filename, int *width, int *height)
{
    char header[8];

    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
        errx(1, "Input file not found");
    }

    const auto nread = fread(header, 1, 8, fp);
    if (png_sig_cmp((png_bytep)header, 0, 8) || nread != 8)
    {
        fclose(fp);
        errx(1, "Input file is not a PNG");
    }

    png_structp png_ptr =
            png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
    {
        errx(1, "Failed creation of png struct reader");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        errx(1, "Failed creation of png info png struct reader");
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    *width = png_get_image_width(png_ptr, info_ptr);
    *height = png_get_image_height(png_ptr, info_ptr);
    const auto color_type = png_get_color_type(png_ptr, info_ptr);
    const auto bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if (bit_depth == 16)
        png_set_strip_16(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    png_color_16 my_background = {0, 0, 0, 0, 0};
    png_set_background(png_ptr, &my_background, PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);

    png_read_update_info(png_ptr, info_ptr);

    auto row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * *height);
    const auto row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    for (int i = 0; i < *height; ++i)
        row_pointers[i] = (png_bytep)malloc(row_bytes);

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return row_pointers;
}