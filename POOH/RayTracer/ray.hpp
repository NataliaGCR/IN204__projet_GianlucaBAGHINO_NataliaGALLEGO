/*
This file encapsulates the functionality and properties required to represent and manipulate rays
*/

#ifndef RAY_H
#define RAY_H

#include "vec3.hpp" 
// Include the vec3 header file for point and vector representations

// Define a class representing a ray in 3D space
class ray {
public:
    // Default constructor
    ray() {}

    // Constructor initializing the ray with an origin and direction
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction), tm(0)
    {}

    // Constructor initializing the ray with an origin, direction, and time
    ray(const point3& origin, const vec3& direction, double time = 0.0)
      : orig(origin), dir(direction), tm(time)
    {}

    // Member function to retrieve the origin of the ray
    point3 origin() const  { return orig; }

    // Member function to retrieve the direction of the ray
    vec3 direction() const { return dir; }

    // Member function to retrieve the time of the ray
    double time() const    { return tm; }

    // Function to compute a point along the ray for a given parameter t
    point3 at(double t) const {
        return orig + t * dir; 
        // P(t) = A + tb, where A is the origin and b is the direction
    }

private:
    point3 orig; 
    // Origin point of the ray
    vec3 dir;    
    // Direction vector of the ray
    double tm;
    // Time of the ray
};

#endif
