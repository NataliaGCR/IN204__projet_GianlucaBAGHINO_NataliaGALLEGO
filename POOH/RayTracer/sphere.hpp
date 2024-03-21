#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
// Include the hittable header file for hittable object representation
#include "ray.hpp"
// Include the ray header file for ray representation
#include "vec3.hpp"
// Include the vec3 header file for point and vector representations

class sphere : public hittable {
// Define a class representing a sphere as a hittable object
  public:
    // Stationary Sphere
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
      : center1(_center), radius(_radius), mat(_material), is_moving(false)
      // Constructor initializing the sphere with a center, radius, and material
    {
        auto rvec = vec3(radius, radius, radius);
        // Vector representing the radius of the sphere
        bbox = aabb(center1 - rvec, center1 + rvec);
        // Axis-aligned bounding box of the sphere
    }

    // Moving Sphere
    sphere(point3 _center1, point3 _center2, double _radius, shared_ptr<material> _material)
      : center1(_center1), radius(_radius), mat(_material), is_moving(true)
      // Constructor initializing the sphere with two centers, radius, and material
    {
        auto rvec = vec3(radius, radius, radius);
        // Vector representing the radius of the sphere
        aabb box1(_center1 - rvec, _center1 + rvec);
        // Axis-aligned bounding box of the sphere at center1
        aabb box2(_center2 - rvec, _center2 + rvec);
        // Axis-aligned bounding box of the sphere at center2
        bbox = aabb(box1, box2);
        // Axis-aligned bounding box of the sphere

        center_vec = _center2 - _center1;
        // Vector from center1 to center2
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Function to check if a ray intersects the sphere
        point3 center = is_moving ? sphere_center(r.time()) : center1;
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = half_b*half_b - a*c;

        if (discriminant < 0) return false;
        // If the discriminant is negative, the ray misses the sphere
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            // If the first root is outside the acceptable range, try the second root
            if (!ray_t.surrounds(root))
                return false;
                // If both roots are outside the acceptable range, the ray misses the sphere
        }

        rec.t = root;
        // Set the parameter value of the intersection point
        rec.p = r.at(rec.t);
        // Set the point of intersection
        vec3 outward_normal = (rec.p - center) / radius;
        // Calculate the outward normal vector at the point of intersection
        rec.set_face_normal(r, outward_normal);
        // Set the normal vector at the point of intersection
        get_sphere_uv(outward_normal, rec.u, rec.v);
        // Compute the texture coordinates at the point of intersection
        rec.mat = mat;
        // Set the material of the sphere

        return true;
    }

    aabb bounding_box() const override { return bbox; }
    // Function to compute the bounding box of the sphere

  private:
    point3 center1;
    // Center of the sphere
    double radius;
    // Radius of the sphere
    shared_ptr<material> mat;
    // Material of the sphere
    bool is_moving;
    // Boolean flag indicating if the sphere is moving
    vec3 center_vec;
    // Vector from center1 to center2
    aabb bbox;
    // Axis-aligned bounding box of the sphere

    point3 sphere_center(double time) const {
        // Linearly interpolate from center1 to center2 according to time, 
        // where t=0 yields center1, and t=1 yields center2.
        return center1 + time*center_vec;
    }

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        // Compute the polar angle theta
        auto phi = atan2(-p.z(), p.x()) + pi;
        // Compute the azimuthal angle phi

        u = phi / (2*pi);
        // Normalize phi to [0,1]
        v = theta / pi;
        // Normalize theta to [0,1]
    }
};

#endif