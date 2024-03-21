#ifndef TEXTURE_H
#define TEXTURE_H

#include "ray_tracing_common.hpp"
// Include the ray_tracing_common header file for common ray tracing utilities
#include "color.hpp"
// Include the color header file for color representation
#include "rtw_stb_image.hpp"
// Include the rtw_stb_image header file for image loading utilities
#include "perlin.hpp"
// Include the perlin header file for perlin noise utilities

class texture {
  public:
    virtual ~texture() = default;
    // Virtual destructor to ensure proper cleanup of derived classes

    virtual color value(double u, double v, const point3& p) const = 0;
    // Pure virtual function to compute the texture value at a given point
};

class solid_color : public texture {
  public:
    solid_color(color c) : color_value(c) {}
    // Constructor initializing the color value of the texture

    solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)) {}
    // Constructor initializing the color value of the texture

    color value(double u, double v, const point3& p) const override {
        // Return the color value of the texture
        return color_value;
    }

  private:
    color color_value;
    // Color value of the texture
};

class checker_texture : public texture {
  public:
    checker_texture(double _scale, shared_ptr<texture> _even, shared_ptr<texture> _odd)
      : inv_scale(1.0 / _scale), even(_even), odd(_odd) {}
      // Constructor initializing the scale and textures of the checker texture

    checker_texture(double _scale, color c1, color c2)
      : inv_scale(1.0 / _scale),
        even(make_shared<solid_color>(c1)),
        odd(make_shared<solid_color>(c2))
    {}
    // Constructor initializing the scale and colors of the checker texture

    color value(double u, double v, const point3& p) const override {
        // Compute the integer coordinates of the point
        auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
        // Check if the sum of the integer coordinates is even

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        // Return the texture value based on the parity of the integer coordinates
    }

  private:
    double inv_scale;
    // Inverse scale of the checker texture
    shared_ptr<texture> even;
    // Shared pointer to the even texture
    shared_ptr<texture> odd;
    // Shared pointer to the odd texture
};

class image_texture : public texture {
  public:
    image_texture(const char* filename) : image(filename) {}
    // Constructor initializing the image texture from a file

    color value(double u, double v, const point3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return color(0,1,1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = interval(0,1).clamp(u);
        v = 1.0 - interval(0,1).clamp(v);  // Flip V to image coordinates

        auto i = static_cast<int>(u * image.width());
        // Compute the horizontal pixel coordinate
        auto j = static_cast<int>(v * image.height());
        // Compute the vertical pixel coordinate
        auto pixel = image.pixel_data(i,j);
        // Get the pixel data at the given coordinates

        auto color_scale = 1.0 / 255.0;
        // Scale the color values to [0,1]
        return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
        // Return the color value of the image texture
    }

  private:
    rtw_image image;
    // Image data for the texture
};

class noise_texture : public texture {
  // Noise texture class
  public:
    noise_texture() {}
    // Default constructor

    noise_texture(double sc) : scale(sc) {}
    // Constructor initializing the scale of the noise texture

    color value(double u, double v, const point3& p) const override {
        auto s = scale * p;
        // Scale the point by the noise texture scale
        return color(1,1,1) * 0.5 * (1 + sin(s.z() + 10*noise.turb(s)));
        // Return the noise texture value
    }

  private:
    perlin noise;
    // Perlin noise object for the texture
    double scale;
    // Scale of the noise texture
};

#endif