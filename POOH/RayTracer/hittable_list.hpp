#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.hpp"
// Include the hittable header file for hittable object representation

#include "aabb.hpp"
// Include the aabb header file for axis-aligned bounding box representation

#include <memory>
// Include the memory header file for shared_ptr and make_shared
#include <vector>
// Include the vector header file for vector representation

using std::shared_ptr;
// Using declaration for shared pointer
using std::make_shared;
// Using declaration for make_shared

/*
The 'using' statements tell the compiler that we'll be getting 'shared_ptr' and 'make_shared' from the std library,
so we don't need to prefix these with 'std::' every time we reference them. 
*/

/*
We'll use shared pointers in our code, because it allows multiple geometries to share a common instance 
(for example, a bunch of spheres that all use the same color material), 
and because it makes memory management automatic and easier to reason about. 
*/

class hittable_list : public hittable {
// Define a class representing a list of hittable objects
  public:
    std::vector<shared_ptr<hittable>> objects;
    // Vector of shared pointers to hittable objects

    hittable_list() {}
    // Default constructor

    hittable_list(shared_ptr<hittable> object) { add(object); }
    // Constructor initializing the list with a hittable object

    void clear() { objects.clear(); }
    // Function to clear the list of hittable objects

    void add(shared_ptr<hittable> object) {
        // Function to add a hittable object to the list
        objects.push_back(object);
        // Adds a value to the end of the std::vector member variable objects
        bbox = aabb(bbox, object->bounding_box());
        // Update the bounding box of the list of hittable objects
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Function to check if a ray hits any of the objects in the list
        hit_record temp_rec;
        // Temporary hit record
        bool hit_anything = false;
        // Flag indicating if the ray hits any object
        auto closest_so_far = ray_t.max;
        // Set the closest hit so far to the maximum ray parameter

        for (const auto& object : objects) {
            // Iterate over the list of hittable objects
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                // Check if the ray hits the current object
                hit_anything = true;
                // Set the hit flag to true
                closest_so_far = temp_rec.t;
                // Update the closest hit so far
                rec = temp_rec;
                // Set the hit record to the temporary hit record
            }
        }

        return hit_anything;
    }

    aabb bounding_box() const override { return bbox; }
    // Function to compute the bounding box of the list of hittable objects

  private:
    aabb bbox;
    // Axis-aligned bounding box of the list of hittable objects
};

#endif