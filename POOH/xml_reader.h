#ifndef XML_READER_H
#define XML_READER_H

#include <string>
#include <vector>
#include "pugixml.hpp"

struct CameraInfo {
    std::string lookFrom;
    std::string lookAt;
    std::string vup;
    double vfov;
    std::string backgroundColor;
};

struct SceneInfo {
    std::vector<CameraInfo> cameraInfoVector;
};

struct ObjectInfo {
    std::string position;
    std::string position2;
    std::string shapeType;
    std::string material;
    std::string colors;
    std::string colors2;
    double ratio;
    std::string origen;
    std::string vect_1;
    std::string vect_2;
    std::string point_1;
    std::string point_2;
};

class XMLReader {
public:
    XMLReader(const std::string& fileName);
    SceneInfo readSceneInfo();
    std::vector<ObjectInfo> readObjectInfo();

private:
    std::string fileName_;
};

#endif // XML_READER_H