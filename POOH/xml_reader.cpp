#include "pugixml.hpp"
#include <iostream>
#include <vector>
#include "xml_reader.h"

XMLReader::XMLReader(const std::string& fileName) : fileName_(fileName) {}


SceneInfo XMLReader::readSceneInfo() {
    SceneInfo sceneInfo;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName_.c_str());
    if (!result) {
        std::cerr << "Error al cargar el archivo XML. Detalles: " << result.description() << std::endl;
        return sceneInfo;
    }

    pugi::xml_node sceneNode = doc.child("scene");
    if (sceneNode) {
        pugi::xml_node cameraNode = sceneNode.child("camara");
        if (cameraNode) {
            CameraInfo cameraInfo;
            cameraInfo.lookFrom = cameraNode.child_value("look_from");
            cameraInfo.lookAt = cameraNode.child_value("look_at");
            cameraInfo.vup = cameraNode.child_value("vup");
            cameraInfo.vfov = std::stod(cameraNode.child_value("vfov"));
            cameraInfo.backgroundColor = cameraNode.child_value("background_color");

            try {
                cameraInfo.vfov = std::stod(cameraNode.child_value("vfov"));
            } catch (std::invalid_argument&) {
                std::cerr << "Invalid argument: Unable to convert vfov to double 1  : " << std::endl;
            }

            sceneInfo.cameraInfoVector.push_back(cameraInfo);
        }
    }

    return sceneInfo;
}

std::vector<ObjectInfo> XMLReader::readObjectInfo() {
    std::vector<ObjectInfo> objectInfoVector;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName_.c_str());
    if (!result) {
        std::cerr << "Error al cargar el archivo XML. Detalles: " << result.description() << std::endl;
        return objectInfoVector;
    }

    pugi::xml_node sceneNode = doc.child("scene");
    if (sceneNode) {
        for (pugi::xml_node objectNode = sceneNode.child("object"); objectNode; objectNode = objectNode.next_sibling("object")) {
            ObjectInfo objectInfo;

            if (!objectNode.child("Position").empty())
                objectInfo.position = objectNode.child_value("Position");

            if (!objectNode.child("Position2").empty())
                objectInfo.position2 = objectNode.child_value("Position2");

            if (!objectNode.child("Shape_type").empty())
                objectInfo.shapeType = objectNode.child_value("Shape_type");

            if (!objectNode.child("Material").empty())
                objectInfo.material = objectNode.child_value("Material");

            if (!objectNode.child("Colors").empty())
                objectInfo.colors = objectNode.child_value("Colors");

            if (!objectNode.child("Colors2").empty())
                objectInfo.colors2 = objectNode.child_value("Colors2");

            if (!objectNode.child("Ratio").empty()) {
                try {
                    objectInfo.ratio = std::stod(objectNode.child_value("Ratio"));
                } catch (std::invalid_argument&) {
                    std::cerr << "Invalid argument: Unable to convert Ratio to double" << std::endl;
                }
            }

            if (!objectNode.child("Origen").empty())
                objectInfo.origen = objectNode.child_value("Origen");

            if (!objectNode.child("Vector1").empty())
                objectInfo.vect_1 = objectNode.child_value("Vector1");

            if (!objectNode.child("Vector2").empty())
                objectInfo.vect_2 = objectNode.child_value("Vector2");

            if (!objectNode.child("Point1").empty())
                objectInfo.point_1 = objectNode.child_value("Point1");

            if (!objectNode.child("Point2").empty())
                objectInfo.point_2 = objectNode.child_value("Point2");

            objectInfoVector.push_back(objectInfo);
        }
    }

    return objectInfoVector;
}