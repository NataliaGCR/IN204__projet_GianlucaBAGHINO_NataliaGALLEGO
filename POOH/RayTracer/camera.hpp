#ifndef CAMERA_H
#define CAMERA_H

#define STB_IMAGE_WRITE_IMPLEMENTATION   
// Define the STB_IMAGE_WRITE_IMPLEMENTATION macro
#include "stb_image_write.h"             
// Include the stb_image_write header file for writing images to disk

#include "ray_tracing_common.hpp"
// Include the ray tracing common header file for common ray tracing utilities

#include "color.hpp"
// Include the color header file for color representation
#include "hittable.hpp"
// Include the hittable header file for hittable object representation
#include "clamp.hpp"
// Include the clamp header file for clamping utility
#include "material.hpp"
// Include the material header file for material representation

#include <iostream>                 
// Include the standard input-output stream library for console I/O
#include <vector>                   
// Include the vector container from the standard template library (STL)
#include <omp.h>  

class camera {
  public:
    double aspect_ratio = 1.0;
    // Define the aspect ratio of the image
    int image_width = 100;
    // Define the width of the image in pixels
    int samples_per_pixel = 10;
    // Define the number of samples per pixel
    int max_depth = 10;
    // Define the maximum depth of the ray
    color  background;               
    // Scene background color

    double vfov = 90;  
    // Vertical view angle (field of view)
    point3 lookfrom = point3(0,0,-1);
    // Point to look from  
    point3 lookat = point3(0,0,0);  
    // Point to look at 
    vec3 vup = vec3(0,1,0);
    // Camera-relative "up" direction

    double defocus_angle = 0; 
    // Variation angle of rays through each pixel
    double focus_dist = 10;    
    // Distance from camera lookfrom point to plane of perfect focus

    void rende2(const hittable& world) {
        initialize();
        // Initialize the camera

        // Create a buffer to store the image data
        std::vector<unsigned char> image_buffer(image_width * image_height * 3); 
        // Create a vector to hold pixel data
        // The size is calculated based on width, height, and 3 channels (RGB)

        // Progress indicator
        // Print the message indicating the start of the rendering process

        // Fill the image buffer with pixel data
        for (int j = 0; j < image_height; ++j) {  
        // Iterate through each row of the image
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush; 
            // Print the progress indicator for the current scanline
            for (int i = 0; i < image_width; ++i) {  
            // Iterate through each column (pixel) of the image
                color pixel_color(0, 0, 0);
                // Initialize the color of the current pixel to black
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                // Iterate through each sample per pixel
                    ray r = get_ray(i, j);
                    // Create a ray from the camera center to the current pixel
                    pixel_color += ray_color(r, max_depth, world);
                    // Calculate the color of the current pixel
                }
                // write_color(std::cout, pixel_color, samples_per_pixel);

                // Calculate the average color of the current pixel
                pixel_color /= samples_per_pixel;

                // Write the color to the buffer
                image_buffer[(j * image_width + i) * 3 + 0] = static_cast<unsigned char>(255.999 * clamp(pixel_color.x(), 0.0, 1.0));
                // Store the clamped red component of the current pixel in the image buffer
                image_buffer[(j * image_width + i) * 3 + 1] = static_cast<unsigned char>(255.999 * clamp(pixel_color.y(), 0.0, 1.0));
                // Store the clamped green component of the current pixel in the image buffer
                image_buffer[(j * image_width + i) * 3 + 2] = static_cast<unsigned char>(255.999 * clamp(pixel_color.z(), 0.0, 1.0));
                // Store the clamped blue component of the current pixel in the image buffer
            }
        }

        // Write the image to a PNG file using stb_image_write.h
        stbi_write_png("C:\\Users\\natyo\\OneDrive - Universidad EIA\\Escritorio\\POOH\\RayTracer\\output.png", image_width, image_height, 3, image_buffer.data(), image_width * 3); 
        // Write the image data to a PNG file

        // Progress indicator
        std::clog << "\rRendering completed!         \n";  
        // Print the completion message indicating the end of the rendering process
    }

    void render(const hittable& world) {
        initialize();
        std::vector<unsigned char> image_buffer(image_width * image_height * 3); 

        std::clog << "Rendering Progress:\n";   
        int num_threads = 4;  // Specify the number of threads you want to use
        omp_set_num_threads(num_threads);  // Set the number of threads

        
        for (int j = 0; j < image_height; ++j) {  
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush; 
            #pragma omp parallel for schedule(dynamic, 1) // OpenMP
            for (int i = 0; i < image_width; ++i) {  
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                pixel_color /= samples_per_pixel;

                image_buffer[(j * image_width + i) * 3 + 0] = static_cast<unsigned char>(255.999 * clamp(pixel_color.x(), 0.0, 1.0));
                image_buffer[(j * image_width + i) * 3 + 1] = static_cast<unsigned char>(255.999 * clamp(pixel_color.y(), 0.0, 1.0));
                image_buffer[(j * image_width + i) * 3 + 2] = static_cast<unsigned char>(255.999 * clamp(pixel_color.z(), 0.0, 1.0));
            }
        }

        stbi_write_png("C:\\Users\\natyo\\OneDrive - Universidad EIA\\Escritorio\\POOH\\RayTracer\\output.png", image_width, image_height, 3, image_buffer.data(), image_width * 3); 

        std::clog << "\rRendering completed!         \n";  
    }

  private:
    int image_height;   
    // Rendered image height
    point3 center;         
    // Camera center
    point3 pixel00_loc;    
    // Location of pixel 0, 0
    vec3 pixel_delta_u;  
    // Offset to pixel to the right
    vec3 pixel_delta_v;  
    // Offset to pixel below
    vec3 u, v, w;
    // Camera coordinate system (basis vectors)
    vec3 defocus_disk_u;  
    // Defocus disk horizontal radius
    vec3 defocus_disk_v;  
    // Defocus disk vertical radius

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        // Calculate the height of the image based on the aspect ratio
        image_height = (image_height < 1) ? 1 : image_height;
        // Ensure the height is at least 1 pixel

        center = lookfrom;
        // Set the camera center to the lookfrom point

        // Determine viewport dimensions
        auto theta = degrees_to_radians(vfov);
        // Convert the vertical field of view to radians
        auto h = tan(theta/2);
        // Calculate the half-height of the viewport
        auto viewport_height = 2 * h * focus_dist;
        // Calculate the height of the viewport based on the field of view
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
        // Calculate the width of the viewport based on the aspect ratio of the image

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        vec3 viewport_u = viewport_width * u;    
        // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  
        // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        // Calculate the change in position along the horizontal direction for each pixel
        pixel_delta_v = viewport_v / image_height;
        // Calculate the change in position along the vertical direction for each pixel

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        // Calculate the radius of the defocus disk
        defocus_disk_u = u * defocus_radius;
        // Calculate the horizontal radius of the defocus disk
        defocus_disk_v = v * defocus_radius;
        // Calculate the vertical radius of the defocus disk
    }

    ray get_ray(int i, int j) const {
    // Get a randomly sampled camera ray for the pixel at location i,j, 
    // originating from the camera defocus disk and passing through the pixel

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        // Calculate the center of the pixel
        auto pixel_sample = pixel_center + pixel_sample_square();
        // Calculate a random point in the pixel

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        // Set the ray origin to the center of the camera if defocus is disabled
        auto ray_direction = pixel_sample - ray_origin;
        // Set the ray direction to the point in the pixel

        auto ray_time = random_double();
        // Set the ray time to a random value

        return ray(ray_origin, ray_direction, ray_time);
        // Return the ray from the camera origin to the pixel
    }

    point3 defocus_disk_sample() const {
    // Returns a random point in the camera defocus disk
        auto p = random_in_unit_disk();
        // Generate a random point in the unit disk
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    vec3 pixel_sample_square() const {
    // Returns a random point in the square surrounding a pixel at the origin
        auto px = -0.5 + random_double();
        // Randomly sample a point in the range [-0.5, 0.5]
        auto py = -0.5 + random_double();
        // Randomly sample a point in the range [-0.5, 0.5]
        return (px * pixel_delta_u) + (py * pixel_delta_v);
        // Return the point in the square surrounding the pixel
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        // Ray scattered from the hit point
        color attenuation;
        // Attenuation factor of the scattered ray
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);
        // Emitted color from the hit point

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
        // If the ray is not scattered
            return color_from_emission;
            // Return the emitted color

        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);
        // Color from the scattered ray

        return color_from_emission + color_from_scatter;
        // Return the sum of the emitted and scattered colors
    }
};

#endif