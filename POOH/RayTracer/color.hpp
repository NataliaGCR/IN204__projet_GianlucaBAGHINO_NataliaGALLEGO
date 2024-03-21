#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp" 
// Include the vec3 header file for color representation
#include <iostream> 
// Include the input/output stream library for console output

using color = vec3; 
// Alias 'color' to 'vec3' for simplicity

inline double linear_to_gamma(double linear_component)
// Function to convert linear color to gamma-corrected color
{
    return sqrt(linear_component);
    // Return the square root of the linear component
}

// Function to write the color components to the output stream
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    // Write the translated [0,255] value of each color component
    auto r = pixel_color.x();
    // Red component
    auto g = pixel_color.y();
    // Green component
    auto b = pixel_color.z();
    // Blue component

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply the linear to gamma transform.
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Write the translated [0,255] value of each color component.
    static const interval intensity(0.000, 0.999);
    // Define an interval for clamping the color intensity
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
    // Output the clamped red component
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
    // Output the clamped green component
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
    // Output the clamped blue component
}

#endif
