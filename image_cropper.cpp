/**
 * @file main.cc
 * @author Marceline Gallegos (marceline.gallegos@icloud.com)
 * @brief Crops an image to the smallest bounding rectangle
 * @version 1.0
 * @date 2023-04-16
 */

#include "bitmap.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file.bmp> <output_file.bmp>" << std::endl;
        return 1; // Exit if no input file is provided
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    Bitmap image;
    image.read(inputFileName);
    Bitmap copy = image.crop();
    copy.write(outputFileName);

    std::cout << "Cropped image saved as: " << outputFileName << std::endl;

    return 0;
}
