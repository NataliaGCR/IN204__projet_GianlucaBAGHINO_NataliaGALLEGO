#ifndef INTERVAL_H
#define INTERVAL_H

#include "ray_tracing_common.hpp"
// Include the ray_tracing_common header file for common ray tracing utilities

class interval {
  public:
    double min, max;
    // Minimum and maximum values of the interval

    interval() : min(+infinity), max(-infinity) {} 
    // Constructor initializing the interval to empty

    interval(double _min, double _max) : min(_min), max(_max) {}
    // Constructor initializing the interval with a minimum and maximum value

    interval(const interval& a, const interval& b)
    // Constructor initializing the interval with the union of two intervals
      : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

    double size() const {
        // Function to compute the size of the interval
        return max - min;
    }

    interval expand(double delta) const {
        // Function to expand the interval by a given amount
        auto padding = delta/2;
        // Compute the padding as half the expansion amount
        return interval(min - padding, max + padding);
    }

    bool contains(double x) const {
        // Function to check if the interval contains a value
        return min <= x && x <= max;
        // Return true if the value is within the interval
    }

    bool surrounds(double x) const {
        // Function to check if the interval surrounds a value
        return min < x && x < max;
        // Return true if the value is strictly within the interval
    }

    double clamp(double x) const {
        // Function to clamp a value to the interval
        if (x < min) return min;
        // If the value is less than the minimum, return the minimum
        if (x > max) return max;
        // If the value is greater than the maximum, return the maximum
        return x;
    }

    static const interval empty, universe;
    // Static constants representing the empty and universal intervals
};

const static interval empty   (+infinity, -infinity);
// Set the constant 'empty' to the empty interval
const static interval universe(-infinity, +infinity);
// Set the constant 'universe' to the universal interval

#endif