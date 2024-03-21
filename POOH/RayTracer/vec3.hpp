#ifndef VEC3_H
#define VEC3_H

#include <cmath>        
// Standard C++ math library
#include <iostream>     
// Standard C++ I/O stream library

using std::sqrt;        
// Bring sqrt function into the global namespace

class vec3 {
public:
    double e[3];        
    // Array to store the three components of the vector

    // Constructors
    vec3() : e{0,0,0} {}   
    // Default constructor initializes all elements to 0
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {} 
    // Constructor with three parameters to set elements

    // Getter functions for individual components
    double x() const { return e[0]; }   
    // Getter for the x-coordinate
    double y() const { return e[1]; }   
    // Getter for the y-coordinate
    double z() const { return e[2]; }   
    // Getter for the z-coordinate

    // Unary minus operator
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    // When you use the unary minus '-' with a 'vec3' object, it returns a new 'vec3' object where each component is negated

    // Subscript operator overloading for const and non-const objects
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    // Compound addition assignment operator
    // Adds each component of the input vector 'v' to the corresponding component of the current vector
    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Compound multiplication assignment operator by a scalar
    // Multiplies each component of the vector by the scalar 't'
    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    // Compound division assignment operator by a scalar
    // Divides each component of the vector by the scalar 't'
    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    // Compute the length of the vector
    // Returns the magnitude (length) of the vector
    double length() const {
        return sqrt(length_squared());
    }

    // Compute the squared length of the vector (faster than computing the length)
    // Returns the squared magnitude of the vector
    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions
        auto s = 1e-8;
        // Define a small value 's' to compare with
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        // Return true if all components are less than 's' in magnitude
    }

    static vec3 random() {
        // Returns a random vector with each component in the range [0,1)
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        // Returns a random vector with each component in the range [min,max)
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// 'point3' is just an alias for 'vec3', but useful for geometric clarity in the code
using point3 = vec3;

// Vector Utility Functions

// Output operator for vec3
// Outputs the vector components to the output stream 'out'
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Addition operator for vec3
// Returns the sum of two vectors 'u' and 'v'
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Subtraction operator for vec3
// Returns the difference between two vectors 'u' and 'v'
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Element-wise multiplication operator for vec3
// Returns the element-wise product of two vectors 'u' and 'v'
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Scalar multiplication operator for vec3 (scalar on the left-hand side)
// Returns the product of a scalar 't' and a vector 'v'
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

// Scalar multiplication operator for vec3 (scalar on the right-hand side)
// Returns the product of a vector 'v' and a scalar 't'
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

// Scalar division operator for vec3
// Returns the result of dividing a vector 'v' by a scalar 't'
inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

// Computes the dot product of two vectors u and v
// Returns the scalar dot product of the two vectors
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

// Computes the cross product of two vectors u and v
// Returns a new vector that is perpendicular to both u and v
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Computes the unit vector of a given vector v
// Returns a new vector with the same direction as v, but with unit length (magnitude)
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 random_in_unit_disk() {
// Returns a random point in the unit disk
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        // Generate a random point in the square [-1,1] x [-1,1] x {0}
        if (p.length_squared() < 1)
        // Check if the point is inside the unit disk
            return p;
    }
}

inline vec3 random_in_unit_sphere() {
// Returns a random point in the unit sphere
    while (true) {
        auto p = vec3::random(-1,1);
        // Generate a random point in the cube [-1,1] x [-1,1] x [-1,1]
        if (p.length_squared() < 1)
        // Check if the point is inside the unit sphere
            return p;
    }
}

inline vec3 random_unit_vector() {
// Returns a random unit vector
    return unit_vector(random_in_unit_sphere());
    // Return a random point in the unit sphere, normalized to a unit vector
}

inline vec3 random_on_hemisphere(const vec3& normal) {
// Returns a random point on the hemisphere with the given normal
    vec3 on_unit_sphere = random_unit_vector();
    // Generate a random point on the unit sphere
    if (dot(on_unit_sphere, normal) > 0.0) 
    // Check if the point is in the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n) {
// Reflects the vector v around the normal n
    return v - 2*dot(v,n)*n;
    // The reflection of a vector v around a normal n is given by v - 2*dot(v,n)*n
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
// Refracts the vector uv through the normal n with the given refractive index ratio etai_over_etat
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    // Compute the cosine of the angle between the vectors
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    // Compute the perpendicular component of the refracted ray
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    // Compute the parallel component of the refracted ray
    return r_out_perp + r_out_parallel;
    // The refracted ray is the sum of the perpendicular and parallel components
}

#endif
