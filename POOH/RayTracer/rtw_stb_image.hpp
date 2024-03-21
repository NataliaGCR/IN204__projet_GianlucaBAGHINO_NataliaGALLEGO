#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

// Disable strict warnings for this header from the Microsoft Visual C++ compiler.
#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "external_stb_image.h"

#include <cstdlib>
#include <iostream>

class rtw_image {
  public:
    rtw_image() : data(nullptr) {}
    // Default constructor initializing the image data to nullptr

    rtw_image(const char* image_filename) {
        // Loads image data from the specified file. If the RTW_IMAGES environment variable is
        // defined, looks only in that directory for the image file. If the image was not found,
        // searches for the specified image file first from the current directory, then in the
        // images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
        // parent, on so on, for six levels up. If the image was not loaded successfully,
        // width() and height() will return 0.

        auto filename = std::string(image_filename);
        // Convert the image filename to a string
        auto imagedir = getenv("RTW_IMAGES");
        // Get the RTW_IMAGES environment variable

        // Hunt for the image file in some likely locations.
        if (imagedir && load(std::string(imagedir) + "/" + image_filename)) return;
        if (load(filename)) return;
        if (load("images/" + filename)) return;
        if (load("../images/" + filename)) return;
        if (load("../../images/" + filename)) return;
        if (load("../../../images/" + filename)) return;
        if (load("../../../../images/" + filename)) return;
        if (load("../../../../../images/" + filename)) return;
        if (load("../../../../../../images/" + filename)) return;

        std::cerr << "ERROR: Could not load image file '" << image_filename << "'.\n";
    }

    ~rtw_image() { STBI_FREE(data); }
    // Destructor to free the image data

    bool load(const std::string filename) {
        // Loads image data from the given file name. Returns true if the load succeeded.
        auto n = bytes_per_pixel; // Dummy out parameter: original components per pixel
        data = stbi_load(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
        // Load the image data from the file
        bytes_per_scanline = image_width * bytes_per_pixel;
        // Compute the number of bytes per scanline
        return data != nullptr;
        // Return true if the image data was loaded successfully
    }

    int width()  const { return (data == nullptr) ? 0 : image_width; }
    // Function to return the width of the image
    int height() const { return (data == nullptr) ? 0 : image_height; }
    // Function to return the height of the image

    const unsigned char* pixel_data(int x, int y) const {
        // Return the address of the three bytes of the pixel at x,y (or magenta if no data).
        static unsigned char magenta[] = { 255, 0, 255 };
        // Define the color magenta
        if (data == nullptr) return magenta;
        // If the image data is not loaded, return magenta

        x = clamp(x, 0, image_width);
        // Clamp the x-coordinate to the image width
        y = clamp(y, 0, image_height);
        // Clamp the y-coordinate to the image height

        return data + y*bytes_per_scanline + x*bytes_per_pixel;
        // Return the address of the pixel data
    }

  private:
    const int bytes_per_pixel = 3;
    // Number of bytes per pixel in the image
    unsigned char *data;
    // Pointer to the image data
    int image_width, image_height;
    // Width and height of the image
    int bytes_per_scanline;
    // Number of bytes per scanline

    static int clamp(int x, int low, int high) {
        // Return the value clamped to the range [low, high).
        if (x < low) return low;
        if (x < high) return x;
        return high - 1;
        // Return the clamped value
    }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif