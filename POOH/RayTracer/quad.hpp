#ifndef QUAD_H
#define QUAD_H

#include "ray_tracing_common.hpp"

#include "hittable.hpp"

#include "hittable_list.hpp"

#include <cmath>

class quad : public hittable {
  public:
    quad(const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> m)
    // Constructor for the quad class
      : Q(_Q), u(_u), v(_v), mat(m)
    {
        auto n = cross(u, v);
        // Compute the normal vector of the quad
        normal = unit_vector(n);
        // Normalize the normal vector
        D = dot(normal, Q);
        // Compute the D parameter of the plane equation
        w = n / dot(n,n);
        // Compute the w vector for the plane coordinates

        set_bounding_box();
        // Set the bounding box of the quad
    }

    virtual void set_bounding_box() {
        // Set the bounding box of the quad.
        bbox = aabb(Q, Q + u + v).pad();
    }

    aabb bounding_box() const override { return bbox; }
    // Return the bounding box of the quad

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Check if the ray intersects the quad.
        auto denom = dot(normal, r.direction());
        // Compute the denominator of the ray-plane intersection formula.

        // No hit if the ray is parallel to the plane.
        if (fabs(denom) < 1e-8)
        // Check if the ray is parallel to the plane
            return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (D - dot(normal, r.origin())) / denom;
        // Compute the parameter value of the intersection point
        if (!ray_t.contains(t))
        // Check if the intersection point is within the ray interval
            return false;

        // Determine the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        // Compute the intersection point
        vec3 planar_hitpt_vector = intersection - Q;
        // Compute the vector from the quad origin to the intersection point
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        // Compute the alpha coordinate of the intersection point
        auto beta = dot(w, cross(u, planar_hitpt_vector));
        // Compute the beta coordinate of the intersection point

        if (!is_interior(alpha, beta, rec))
        // Check if the intersection point is within the planar shape
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.

        rec.t = t;
        // Set the parameter value of the intersection point
        rec.p = intersection;
        // Set the intersection point
        rec.mat = mat;
        // Set the material of the object
        rec.set_face_normal(r, normal);
        // Set the normal vector of the intersection point

        return true;
    }

    virtual bool is_interior(double a, double b, hit_record& rec) const {
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
        // Check if the intersection point is outside the planar shape
            return false;

        rec.u = a;
        // Set the u coordinate of the hit record
        rec.v = b;
        // Set the v coordinate of the hit record
        return true;
    }

  private:
    point3 Q;
    // Origin of the quad
    vec3 u, v;
    // Vectors defining the quad
    shared_ptr<material> mat;
    // Material of the quad
    aabb bbox;
    // Bounding box of the quad
    vec3 normal;
    // Normal vector of the quad
    double D;
    // D parameter of the plane equation
    vec3 w;
    // w vector for the plane coordinates
};

inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = make_shared<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}

#endif