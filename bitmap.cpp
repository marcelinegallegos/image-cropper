/**
 * @file bitmap.cc
 * @author Marceline Gallegos (marceline.gallegos@icloud.com)
 * @brief Contains the definitions for the methods prototyped in bitmap.h
 * @version 1.0
 * @date 2023-04-16
 *
 */
#include <iostream> // std::cout, std::cerr
#include <fstream>  // std::ifstream, std::ofstream
#include "bitmap.h"

/**
 * @brief Creates a Bitmap object based on an image file
 *
 * @param filename String with the name of the file to open
 */
void Bitmap::read(const std::string &filename)
{
    std::ifstream ifs;

    // Open the input file and test for failure
    ifs.open(filename, std::ios::in | std::ios::binary);
    if (!ifs.is_open())
    {
        std::cerr << "Error - unable to open input file " << filename << std::endl;
        exit(1);
    }

    // Read the first 14 bytes into file_header
    ifs.read((char *)&file_header, sizeof(file_header));
    if (file_header.type != 0x4D42) // Verify file type
    {
        std::cerr << "Error - unrecognized file format\n";
        ifs.close();
        exit(1);
    }

    // Read the next 40 bytes into info_header
    ifs.read((char *)&info_header, sizeof(info_header));
    if (info_header.bit_count != 8) // Verify bits per pixel
    {
        std::cerr << "Error - incorrect bit per pixel format\n";
        ifs.close();
        exit(1);
    }

    // Read the next 1024 bytes into color_table
    color_table = new unsigned char[(uint8_t)4 * num_colors];
    ifs.read((char *)color_table, (uint8_t)4 * num_colors); // 4 bytes per color

    pixel_array.resize(info_header.height, std::vector<uint8_t>(info_header.width));
    num_padding = (4 - info_header.width % 4) % 4; // Calculate the number of padding bytes at the end of each row

    // Read the remaining bytes into pixel_array
    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            unsigned char color_index;                  // Each byte is an index into color_table
            ifs.read((char *)&color_index, (uint8_t)1); // 1 byte per pixel
            pixel_array[y][x] = color_index;
        }
        ifs.ignore(num_padding); // Ignore padding bytes
    }

    ifs.close();
    std::cout << "File read\n";
}

/**
 * @brief  Creates a bitmap file from this Bitmap object
 *
 * @param filename String with the name of the file to open
 */
void Bitmap::write(const std::string &filename) const
{
    std::ofstream ofs;

    // Open the output file and test for failure
    ofs.open(filename, std::ios::out | std::ios::binary);
    if (!ofs.is_open())
    {
        std::cerr << "Error - unable to open output file " << filename << std::endl;
        exit(1);
    }

    ofs.write((const char *)&file_header, sizeof(file_header));        // Write file_header into the first 14 bytes of the output file
    ofs.write((const char *)&info_header, sizeof(info_header));        // Write info_header into the next 40 bytes of the output file
    ofs.write((const char *)&color_table[0], (uint8_t)4 * num_colors); // Write color_table into the first 1024 bytes of the output file

    // Write pixel_array into the remaining bytes of the output file
    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            unsigned char color_index = pixel_array[y][x];
            ofs.write((const char *)&color_index, (uint8_t)1);
        }
        ofs.write((const char *)&padding, num_padding); // Append padding bytes to the end of a row
    }

    ofs.close();
    std::cout << "File created\n";
}

/**
 * @brief Creates a new Bitmap object by copying a portion of this bitmap
 *
 * @return Bitmap A bitmap object
 */
Bitmap Bitmap::crop()
{
    int y_lower_bound;
    int y_upper_bound;
    int x_lower_bound;
    int x_upper_bound;
    bool flag; // Used to break out of nested loops

    unsigned char bkg_color_index = pixel_array[0][0]; // Color index of the lowest left pixel

    // Find lower boundries
    flag = false;
    for (int y = 0; y < info_header.height && !flag; y++)
    {
        for (int x = 0; x < info_header.width && !flag; x++)
        {
            if (bkg_color_index != pixel_array[y][x])
            {
                y_lower_bound = y;
                flag = true;
            }
        }
    }

    flag = false;
    for (int x = 0; x < info_header.width && !flag; x++)
    {
        for (int y = y_lower_bound; y < info_header.height && !flag; y++)
        {
            if (bkg_color_index != pixel_array[y][x])
            {
                x_lower_bound = x;
                flag = true;
            }
        }
    }

    bkg_color_index = pixel_array[info_header.height - 1][info_header.width - 1]; // Color index of the up most right pixel

    // Find upper boundries
    flag = false;
    for (int y = info_header.height - 1; y >= y_lower_bound && !flag; y--)
    {
        for (int x = x_lower_bound; x < info_header.width && !flag; x++)
        {
            if (bkg_color_index != pixel_array[y][x])
            {
                y_upper_bound = y;
                flag = true;
            }
        }
    }

    flag = false;
    for (int x = info_header.width - 1; x >= x_lower_bound && !flag; x--)
    {
        for (int y = y_lower_bound; y <= y_upper_bound && !flag; y++)
        {
            if (bkg_color_index != pixel_array[y][x])
            {
                x_upper_bound = x;
                flag = true;
            }
        }
    }

    // Create a new bitmap object
    Bitmap copy;

    // Copy this file structure to the new object
    copy.file_header = file_header;
    copy.info_header = info_header;
    copy.color_table = color_table;

    // Update header values
    copy.info_header.height = y_upper_bound + 1 - y_lower_bound;
    copy.info_header.width = x_upper_bound + 1 - x_lower_bound;
    copy.num_padding = (4 - copy.info_header.width % 4) % 4;
    copy.file_header.off_bits = sizeof(Bitmap_file_header) + sizeof(DIB_header) + (uint8_t)4 * num_colors;
    copy.file_header.size = copy.file_header.off_bits + (copy.info_header.width + copy.num_padding) * copy.info_header.height;

    // Copy a portion of this pixel_array to the new object
    copy.pixel_array.resize(copy.info_header.height, std::vector<uint8_t>(copy.info_header.width));
    for (int y = 0; y < copy.info_header.height; y++)
    {
        for (int x = 0; x < copy.info_header.width; x++)
        {
            copy.pixel_array[y][x] = pixel_array[y + y_lower_bound][x + x_lower_bound];
        }
    }

    std::cout << "Image cropped\n";
    return copy;
}
