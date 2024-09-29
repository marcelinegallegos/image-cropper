# Image Cropper Tool

## Overview
The **Image Cropper Tool** is a command-line application developed in C/C++ that crops 8-bit indexed bitmap images to the smallest bounding rectangle based on non-transparent pixels. Useful for efficiently removing excess space around images, making it ideal for image processing tasks.

## Features
- **Crops Images**: Efficiently crops images to the smallest bounding rectangle defined by non-transparent pixels, minimizing excess space.
- **No External Libraries**: Implements image processing without relying on external image libraries.

## Getting Started

### Prerequisites
- C/C++ compiler (e.g., GCC, Clang)
- Basic knowledge of command-line interface usage

### Installation
1. Clone the repository:
```bash
git clone https://github.com/<yourusername>/image-cropper.git
cd image-cropper
```
2. Compile the source code:
```bash
make
```

### Usage
**Background Requirement**: Requires a solid (non-transparent) background color for accurate cropping.

Run the tool using the command line:
```bash
./image_cropper input_file.bmp output_file.bmp
```
- `input_file.bmp`: Input image file (**MUST** be an 8-bit indexed BMP)
- `output_file.bmp`: Output cropped image file

### Example
For demonstration, the input file `knight_in.bmp` is included in the repository.

## License
This project is licensed under the MIT License. See the LICENSE file for details.