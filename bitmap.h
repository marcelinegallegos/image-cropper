/**
 * @file bitmap.h
 * @author Marceline Gallegos (marceline.gallegos@icloud.com)
 * @brief Contains the necessary programming interfaces to work with a 8bpp bitmap
 * @version 1.0
 * @date 2023-04-16
 *
 */
#include <vector> // std::vector
#include <string> // std::string
#include <cstdint>

#ifndef BITMAP_H
#define BITMAP_H

#pragma pack(push, 1)
/**
 * @brief Contains general information about the bitmap image file
 *
 */
struct Bitmap_file_header
{
    uint16_t type{0x4D42}; // File type; must be BM
    uint32_t size{0};      // Size, in bytes, of the bitmap file
    uint16_t reserved1{0}; // Reserved; must be 0
    uint16_t reserved2{0}; // Reserved; must be 0
    uint32_t off_bits{0};  // Offset, in bytes, from the beginning of the Bitmap_file_header structure to the bitmap bits
};

/**
 * @brief Contains detailed information about the bitmap image and define the pixel format
 *
 */
struct DIB_header
{
    uint32_t size{0};              // No. of bytes required by the structure
    int32_t width{0};              // Width of the bitmap, in pixels
    int32_t height{0};             // Height of the bitmap, in pixels
                                   //       (if positive, the bitmap is a bottom-up DIB with the origin at the lower left corner)
                                   //       (if negative, the bitmap is a top-down DIB with the origin at the upper left corner)
    uint16_t planes{1};            // No. of planes for the target device; must be 1
    uint16_t bit_count{0};         // No. of bits per pixel
    uint32_t compression{0};       // Compression method being used
    uint32_t size_image{0};        // Size, in bytes, of the image; 0 for uncompressed RGB bitmaps
    int32_t x_pixels_per_meter{0}; // Horizontal resolution, in pixels per meter, of the target device for the bitmap
    int32_t y_pixels_per_meter{0}; // Vertical resolution, in pixels per meter, of the target device for the bitmap
    uint32_t colors_used{0};       // No. of color indices in the color table that are actually used by the bitmap
    uint32_t colors_important{0};  // No. of color indices that are considered important for displaying the bitmap
};
#pragma pack(pop)

/**
 * @brief Provides methods for loading, modifying, and saving bitmaps
 * 
 */
class Bitmap
{
private:
    const int num_colors = 256; // The 8bpp format supports 256 distinct colors
    int num_padding;            // The size of each row is rounded up to a multiple of 4 bytes by padding
    unsigned char padding[3] = {0, 0, 0};
    Bitmap_file_header file_header;
    DIB_header info_header;
    unsigned char *color_table;                    // Define colors used by the bitmap image data (Pixel array)
    std::vector<std::vector<uint8_t>> pixel_array; // Define the actual values of the pixels

public:
    Bitmap() {}
    void read(const std::string &filename);
    void write(const std::string &filename) const;
    Bitmap crop();
};

#endif
