#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.hpp"
// Include the ray header file for ray representation
#include "ray_tracing_common.hpp"
// Include the ray_tracing_common header file for common ray tracing utilities
#include "aabb.hpp"
// Include the aabb header file for axis-aligned bounding box representation

class material;

class hit_record {
  public:
    point3 p;
    // Point of intersection
    vec3 normal;
    // Normal vector at the point of intersection
    shared_ptr<material> mat;
    // Shared pointer to the material of the object
    double t;
    // Parameter value of the intersection point
    double u;
    // Texture coordinate u
    double v;
    // Texture coordinate v
    bool front_face;
    // Flag indicating if the normal vector is pointing towards the ray

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector
        // NOTE: the parameter 'outward_normal' is assumed to have unit length

        front_face = dot(r.direction(), outward_normal) < 0;
        // Check if the normal vector is pointing towards the ray
        normal = front_face ? outward_normal : -outward_normal;
        // Set the normal vector based on the front face flag;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;
    // Virtual destructor to ensure proper cleanup of derived classes

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    // Pure virtual function to check if a ray hits the object

    virtual aabb bounding_box() const = 0;
    // Pure virtual function to compute the bounding box of the object
};

#endif