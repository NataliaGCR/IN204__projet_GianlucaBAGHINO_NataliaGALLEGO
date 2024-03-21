#ifndef AABB_H
#define AABB_H

#include "ray_tracing_common.hpp"

class aabb {
  public:
    interval x, y, z;

    aabb() {} // The default AABB is empty, since intervals are empty by default.

    aabb(const interval& ix, const interval& iy, const interval& iz)
    // Construct an AABB from three intervals.
      : x(ix), y(iy), z(iz) { }

    aabb(const point3& a, const point3& b) {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a
        // particular minimum/maximum coordinate order.
        x = interval(fmin(a[0],b[0]), fmax(a[0],b[0]));
        y = interval(fmin(a[1],b[1]), fmax(a[1],b[1]));
        z = interval(fmin(a[2],b[2]), fmax(a[2],b[2]));
    }

    aabb(const aabb& box0, const aabb& box1) {
        // Construct the bounding box that contains both box0 and box1.
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    aabb pad() {
        // Return an AABB that has no side narrower than some delta, padding if necessary.
        double delta = 0.0001;
        // Set the padding delta.
        interval new_x = (x.size() >= delta) ? x : x.expand(delta);
        interval new_y = (y.size() >= delta) ? y : y.expand(delta);
        interval new_z = (z.size() >= delta) ? z : z.expand(delta);

        return aabb(new_x, new_y, new_z);
        // Return the padded bounding box.
    }

    const interval& axis(int n) const {
        // Return the nth axis of the bounding box.
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval ray_t) const {
        // Check if the ray intersects the bounding box.
        for (int a = 0; a < 3; a++) {
            auto invD = 1 / r.direction()[a];
            // Compute the inverse of the ray direction component to avoid division in the loop.
            auto orig = r.origin()[a];
            // Compute the origin of the ray.

            auto t0 = (axis(a).min - orig) * invD;
            // Compute the parameter value of the intersection with the minimum plane.
            auto t1 = (axis(a).max - orig) * invD;
            // Compute the parameter value of the intersection with the maximum plane.

            if (invD < 0)
            // If the ray is moving in the negative direction, swap the intersection points.
                std::swap(t0, t1);

            if (t0 > ray_t.min) ray_t.min = t0;
            // Update the minimum intersection parameter.
            if (t1 < ray_t.max) ray_t.max = t1;
            // Update the maximum intersection parameter.

            if (ray_t.max <= ray_t.min)
            // If the ray misses the bounding box, return false.
                return false;
                // The ray misses the bounding box.
        }
        return true;
    }
};

#endif