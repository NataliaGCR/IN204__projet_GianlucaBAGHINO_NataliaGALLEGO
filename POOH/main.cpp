#include <iostream>
#include <thread>
#include <vector>
#include <future> 
#include <string>
#include "ImGui/ImGuiExample.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "RayTracer/RayTracer.hpp"
#include "Application.h"
#include "xml_reader.h"

std::mutex dataMutex;
std::thread renderThread; 



std::vector<double> parseDoubleArray(const std::string& str) {
    std::vector<double> result;
    std::string num;
    for (char c : str) {
        if (c == '[' || c == ' ' || c == ']') continue;
        if (c == ',') {
            result.push_back(std::stod(num));
            num.clear();
        } else {
            num += c;
        }
    }
    if (!num.empty()) result.push_back(std::stod(num));
    return result;
}


std::vector<std::vector<double>> Position;
std::vector<std::vector<double>> Position2;
std::string ShapeType;
std::string Material;
std::vector<std::vector<double>> Colors;
std::vector<std::vector<double>> Colors2;

std::vector<std::vector<double>> Origen;
std::vector<std::vector<double>> Vect_1;
std::vector<std::vector<double>> Vect_2;

std::vector<std::vector<double>> Point_1;
std::vector<std::vector<double>> Point_2;
double Ratio;

std::vector<double> Ratio_fin;
std::vector<std::string> Shape_fin;
std::vector<std::string> Material_fin;

void runImGui(int argc, char** argv) {
    ImGuiExample::RunImGuiExample(argc, argv);
    ImGui::DestroyContext();
}

void data_organisation(std::vector<ObjectInfo> objectInfo) {
    std::lock_guard<std::mutex> lock(dataMutex); // Se adquiere el mutex para asegurar que los datos estén protegidos
    for (const auto& object : objectInfo)
    {
        std::vector<double> position = parseDoubleArray(object.position);
        Position.push_back(position);
        std::vector<double> position2 = parseDoubleArray(object.position2);
        Position2.push_back(position2);
        ShapeType = object.shapeType;
        Shape_fin.push_back(ShapeType);
        Material = object.material;
        Material_fin.push_back(Material);   
        std::vector<double> colors = parseDoubleArray(object.colors);
        Colors.push_back(colors);
        std::vector<double> colors2 = parseDoubleArray(object.colors2);
        Colors2.push_back(colors2);
        Ratio_fin.push_back(object.ratio);

        std::vector<double> origen = parseDoubleArray(object.origen);
        Origen.push_back(origen);
        std::vector<double> vect1 = parseDoubleArray(object.vect_1);
        Vect_1.push_back(vect1);
        std::vector<double> vect2 = parseDoubleArray(object.vect_2);
        Vect_2.push_back(vect2);
        std::vector<double> point1 = parseDoubleArray(object.point_1);
        Point_1.push_back(point1);
        std::vector<double> point2 = parseDoubleArray(object.point_2);
        Point_2.push_back(point2);
    }
}


std::vector<double> lookFrom;
std::vector<double> lookAt;
std::vector<double> vup;
double vfov;
std::vector<double> backgroundColor;


void organise_camera_data(CameraInfo cameraInfo) {
    lookFrom = parseDoubleArray(cameraInfo.lookFrom);
    lookAt = parseDoubleArray(cameraInfo.lookAt);
    vup = parseDoubleArray(cameraInfo.vup);
    vfov = cameraInfo.vfov;
    backgroundColor = parseDoubleArray(cameraInfo.backgroundColor); //New
}



void renderScene(const std::vector<std::string>& shapeType,
                 const std::vector<std::vector<double>>& colors,
                 const std::vector<std::vector<double>>& colors2,
                 const std::vector<std::string>& material,
                 const std::vector<double>& Radio, // Changed to std::vector<std::vector<double>>
                 const std::vector<std::vector<double>>& Position,
                 const std::vector<std::vector<double>>& Positio2,
                 double vfov,
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



                std::cout << "backGrounColor";
                for (const auto& vec : backGrounColor) {
                    std::cout << vec << " ";
                }
                std::cout << std::endl;
                
    

    RayTracing::traceRays(shapeType, colors, colors2, material, Radio, Position, Positio2, vfov, lookfrom, lookat, vup, RenderType, backGrounColor, Origen, Vect_1, Vect_2, Point_1, Point_2);
}


int main(int argc, char** argv)
{   
    SceneInfo sceneInfo;
    std::vector<ObjectInfo> objectInfo;
    CameraInfo cameraInfo;

    bool quit = false;

    // Run ImGui in a separate thread
    std::thread imguiThread(runImGui, argc, argv);

    // Move the content of the mainThread to the main thread
    while (!quit) {
        if (MyApp::loadXMLFlag)
        {
            XMLReader xmlReader("C:\\Users\\natyo\\OneDrive - Universidad EIA\\Escritorio\\POOH\\output.xml");
            sceneInfo = xmlReader.readSceneInfo();
            objectInfo = xmlReader.readObjectInfo();

            data_organisation(objectInfo);
            if (!sceneInfo.cameraInfoVector.empty()) {
                organise_camera_data(sceneInfo.cameraInfoVector[0]); // Accede al primer elemento de cameraInfoVector
            }


            MyApp::loadXMLFlag = false;
        }

        if (MyApp::loadRenderFlag == 1){            
            int RenderType = MyApp::current_item;
            
            std::thread renderThread(renderScene, Shape_fin, Colors, Colors2, Material_fin, Ratio_fin, Position, Position2, vfov, lookFrom, lookAt, vup, RenderType, backgroundColor, Origen, Vect_1, Vect_2, Point_1, Point_2);
            renderThread.join(); // Wait for the rendering to finish
            MyApp::loadRenderFlag = 2;
            MyApp::imageReady = true;
        }
    }

    imguiThread.join(); // Wait for the ImGui thread to finish

    return 0;
}