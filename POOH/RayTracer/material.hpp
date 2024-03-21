#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray_tracing_common.hpp"
// Include the ray_tracing_common header file for common ray tracing utilities
#include "color.hpp"
// Include the color header file for color representation
#include "hittable.hpp"
// Include the hittable header file for hittable object representation
#include "texture.hpp"
// Include the texture header file for texture representation

class hit_record;

class material {
  public:
    virtual ~material() = default;
    // Virtual destructor to ensure proper cleanup of derived classes

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
    // Pure virtual function to compute the scattered ray and attenuation

    virtual color emitted(double u, double v, const point3& p) const {
      // Function to compute the emitted color
        return color(0,0,0);
        // Return black color
    }
};

class lambertian : public material {
// Define a class representing a Lambertian material
  public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    // Constructor initializing the albedo of the material
    lambertian(shared_ptr<texture> a) : albedo(a) {}
    // Constructor initializing the albedo of the material

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
      // Function to compute the scattered ray and attenuation
        auto scatter_direction = rec.normal + random_unit_vector();
        // Compute the scattered ray direction
        if (scatter_direction.near_zero())
        // If the scattered ray direction is near zero
            scatter_direction = rec.normal;
            // Set the scattered ray direction to the normal
        scattered = ray(rec.p, scatter_direction, r_in.time());
        // Set the scattered ray
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        // Set the attenuation
        return true;
    }

  private:
    shared_ptr<texture> albedo;
    // Texture of the material
};

class metal : public material {
// Define a class representing a metal material
  public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    // Constructor initializing the albedo and fuzziness of the material

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    // Function to compute the scattered ray and attenuation
    const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);   
        // Compute the reflected ray
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time());
        // Set the scattered ray
        attenuation = albedo;
        // Set the attenuation
        return (dot(scattered.direction(), rec.normal) > 0);
        // Check if the scattered ray is in the same hemisphere as the normal
    }

  private:
    color albedo;
    // Albedo of the material
    double fuzz;
    // Fuzziness of the material
};

// typically air = 1.0, glass = 1.3–1.7, diamond = 2.4

class dielectric : public material {
// Define a class representing a dielectric material
  public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}
    // Constructor initializing the index of refraction of the material

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    // Function to compute the scattered ray and attenuation
    const override {
        attenuation = color(1.0, 1.0, 1.0);
        // Set the attenuation
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
        // Set the refraction ratio based on the front face flag

        vec3 unit_direction = unit_vector(r_in.direction());
        // Convert the ray direction to a unit vector
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        // Compute the cosine of the angle between the vectors
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        // Compute the sine of the angle between the vectors

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        // Check if the ray cannot be refracted
        vec3 direction;
        // Create a direction vector for the scattered ray

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        // If the ray cannot be refracted or the reflectance is greater than a random value
            direction = reflect(unit_direction, rec.normal);
            // Compute the reflected ray
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);
            // Compute the refracted ray

        scattered = ray(rec.p, direction, r_in.time());
        // Set the scattered ray
        return true;
    }

  private:
    double ir; 
    // Index of refraction of the material
    static double reflectance(double cosine, double ref_idx) {
    // Use Schlick's approximation for reflectance
        auto r0 = (1-ref_idx) / (1+ref_idx);
        // Compute the reflectance at normal incidence
        r0 = r0*r0;
        // Square the reflectance at normal incidence
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

class diffuse_light : public material {
  public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        return false;
    }

    color emitted(double u, double v, const point3& p) const override {
        return emit->value(u, v, p);
    }

  private:
    shared_ptr<texture> emit;
};

#endif