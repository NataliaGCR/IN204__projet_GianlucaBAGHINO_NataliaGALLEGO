
#include "RayTracer.hpp"
#include "ray_tracing_common.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "bvh.hpp"
#include "texture.hpp"
#include "quad.hpp"

#include <iostream>


// #include "stb_image_write.h"

namespace RayTracing {
    // Función que realiza el trazado de rayos
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
           const std::vector<std::vector<double>>& Point_2) {


        hittable_list world;  
                 


        for (int i = 0; i < Materials.size(); i++) {
            shared_ptr<material> material;

            std::string materialType = Materials[i];
            if (Materials[i] == "lambertian") {
                auto checker = make_shared<checker_texture>(2.0, color(Colors[i][0], Colors[i][1], Colors[i][2]), color(Colors2[i][0], Colors2[i][1], Colors2[i][2]));
                material = make_shared<lambertian>(checker);           
            } else if (Materials[i] == "metal") {
                material = make_shared<metal>(color(Colors[i][0], Colors[i][1], Colors[i][2]), Colors[i][3]);
            } else if (Materials[i] == "dielectric") {
                material = make_shared<dielectric>(Colors[i][0]);
            } else if (Materials[i] == "difflight") {
                material = make_shared<diffuse_light>(color(Colors[i][0], Colors[i][1], Colors[i][2]));
            }

            if (shapeTypes[i] == "sphere"){
                world.add(make_shared<sphere>(point3(Position[i][0], Position[i][1], Position[i][2]), point3(Position2[i][0], Position2[i][1], Position2[i][2]), Radio[i], material));
            } else if(shapeTypes[i] == "quad"){
                world.add(make_shared<quad>(point3(Origen[i][0], Origen[i][1], Origen[i][2]), vec3(Vect_1[i][0], Vect_1[i][1], Vect_1[i][2]), vec3(Vect_2[i][0], Vect_2[i][1], Vect_2[i][2]), material));
            } else if(shapeTypes[i] == "box"){
                world.add(box(point3(Point_1[i][0], Point_1[i][1], Point_1[i][2]), point3(Point_2[i][0], Point_2[i][1], Point_2[i][2]), material));
            }     
        }
        
        camera cam;

        cam.aspect_ratio = 16.0 / 9.0;
        cam.image_width  = 300;
        cam.background = color(backGrounColor[0], backGrounColor[1], backGrounColor[2]);

        if (RenderType == 1) {
            cam.max_depth  = 10;
            cam.samples_per_pixel = 30; 
        } else if (RenderType == 2) {
            cam.max_depth  = 100;
            cam.samples_per_pixel = 300;
        } else if (RenderType == 3) {
            cam.max_depth  = 200;
            cam.samples_per_pixel = 800;
        }

        cam.vfov = vfov;
        cam.lookfrom = point3(lookfrom[0],lookfrom[1],lookfrom[2]);
        cam.lookat = point3(lookat[0],lookat[1],lookat[2]);
        cam.vup = vec3(vup[0],vup[1],vup[2]);

        cam.render(world);
    }
}