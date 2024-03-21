// RayTracer.hpp

#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP

#include <vector>
#include <string>

namespace RayTracing {
    void traceRays(const std::vector<std::string>& shapeTypes,
                   const std::vector<std::vector<double>>& Colors,
                   const std::vector<std::vector<double>>& Colors2,
                   const std::vector<std::string>& Materials,
                   const std::vector<double>& Radio, 
                   const std::vector<std::vector<double>>& Position,
                   const std::vector<std::vector<double>>& Position2,
                   int vfov,
                   const std::vector<double>& lookfrom,
                   const std::vector<double>& lookat,
                   const std::vector<double>& vup,
                   int RenderType,
                   const std::vector<double>& backGrounColor,
                   const std::vector<std::vector<double>>& Origen,
                   const std::vector<std::vector<double>>& Vect_1,
                   const std::vector<std::vector<double>>& Vect_2,
                   const std::vector<std::vector<double>>& Point_1,
                   const std::vector<std::vector<double>>& Point_2);
}

#endif // RAY_TRACER_H