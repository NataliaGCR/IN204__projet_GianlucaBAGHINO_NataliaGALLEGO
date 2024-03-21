#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
// Standard C++ math library
#include <cstdlib>
// Standard C++ general utilities library
#include <limits>
// Standard C++ library for numeric limits
#include <memory>
// Standard C++ memory management library


// Usings

using std::shared_ptr;
// Using declaration for shared pointer
using std::make_shared;
// Using declaration for make_shared
using std::sqrt;
// Bring sqrt function into the global namespace

// Constants

const double infinity = std::numeric_limits<double>::infinity();
// Set the constant 'infinity' to the maximum value of a double
const double pi = 3.1415926535897932385;
// Set the constant 'pi' to the value of pi

// Utility Functions

inline double degrees_to_radians(double degrees) {
    // Function to convert degrees to radians
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max+1));
}

// Common Headers

#include "ray.hpp"
// Include the ray header file for ray representation
#include "vec3.hpp"
// Include the vec3 header file for point and vector representations
#include "interval.hpp"
// Include the interval header file for interval representations

#endif