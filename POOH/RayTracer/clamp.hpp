#ifndef CLAMP_HPP
#define CLAMP_HPP

// Define a template function named 'clamp' that takes three parameters: value, min, and max
template<typename T>
constexpr const T& clamp(const T& value, const T& min, const T& max) {
    // Return the value if it falls within the range defined by min and max
    // Otherwise, return the minimum value if 'value' is less than 'min'
    // or return the maximum value if 'value' is greater than 'max'
    return (value < min) ? min : (max < value) ? max : value;
}

#endif // CLAMP_HPP
