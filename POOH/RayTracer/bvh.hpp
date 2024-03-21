#ifndef BVH_H
#define BVH_H

#include "ray_tracing_common.hpp"
// Include the ray_tracing_common header file for common ray tracing utilities

#include "hittable.hpp"
// Include the hittable header file for hittable object representation
#include "hittable_list.hpp"
// Include the hittable list header file

#include <algorithm>
// Include the algorithm header file for STL algorithms

class bvh_node : public hittable {
    // Bounding Volume Hierarchy
  public:
    bvh_node(const hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}

    /*
    bvh_node(const std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
        // To be implemented later.
    }
    */

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Function to check if a ray hits the bounding volume hierarchy
        if (!bbox.hit(r, ray_t))
        // If the ray does not hit the bounding box, return false
            return false;

        bool hit_left = left->hit(r, ray_t, rec);
        // Check if the ray hits the left child node
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);
        // Check if the ray hits the right child node

        return hit_left || hit_right;
        // Return true if the ray hits either the left or right child node
    }

    aabb bounding_box() const override { return bbox; }
    // Function to compute the bounding box of the bounding volume hierarchy

    bvh_node(const std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
        // Function to construct a bounding volume hierarchy from a list of hittable objects
        auto objects = src_objects; // Create a modifiable array of the source scene objects

        int axis = random_int(0,2);
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare;
        // Choose the axis along which to split the objects

        size_t object_span = end - start;
        // Compute the number of objects in the span

        if (object_span == 1) {
            left = right = objects[start];
            // If there is only one object, set both left and right child nodes to the object
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start+1])) {
                left = objects[start];
                right = objects[start+1];
                // If the objects are not sorted along the axis, swap them
            } else {
                left = objects[start+1];
                right = objects[start];
                // If the objects are sorted along the axis, set the left and right child nodes
            }
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);
            // Sort the objects along the axis

            auto mid = start + object_span/2;
            // Compute the midpoint of the span
            left = make_shared<bvh_node>(objects, start, mid);
            // Recursively construct the left child node
            right = make_shared<bvh_node>(objects, mid, end);
            // Recursively construct the right child node
        }

        bbox = aabb(left->bounding_box(), right->bounding_box());
        // Compute the bounding box of the bounding volume hierarchy
    }

  private:
    shared_ptr<hittable> left;
    // Shared pointer to the left child node
    shared_ptr<hittable> right;
    // Shared pointer to the right child node
    aabb bbox;
    // Axis-aligned bounding box of the bounding volume hierarchy

    static bool box_compare(
        // Function to compare two hittable objects along an axis
        const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index
        // Two hittable objects and the axis along which to compare them
    ) {
        return a->bounding_box().axis(axis_index).min < b->bounding_box().axis(axis_index).min;
        // Compare the minimum values of the bounding boxes along the axis
    }

    static bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        // Function to compare two hittable objects along the x-axis
        return box_compare(a, b, 0);
        // Compare the two hittable objects along the x-axis
    }

    static bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        // Function to compare two hittable objects along the y-axis
        return box_compare(a, b, 1);
    }

    static bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        // Function to compare two hittable objects along the z-axis
        return box_compare(a, b, 2);
    }
};

#endif