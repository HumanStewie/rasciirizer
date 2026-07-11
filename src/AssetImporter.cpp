#include "AssetImporter.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "sgm.h"

void AssetImporter::parseObj(const std::string& assetPath)
{
    std::ifstream asset { assetPath };
    std::string line {};
    std::stringstream str {};
    while (std::getline(asset, line)) {
        if (line.starts_with("v ")) {
            str.clear();
            sgm::Vec3D vertex {};
            line.erase(0, 2);
            str.str(line);
            str >> vertex.x >> vertex.y >> vertex.z;
            m_vertices.push_back(vertex);
            continue;
        }
        else if (line.starts_with("f ")) {
            str.clear();
            sgm::Vec<int, 3> face {};
            line.erase(0, 2);
            str.str(line);
            str >> face.x;
            str.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
            str >> face.y;
            str.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
            str >> face.z;
            face.x--;
            face.y--;
            face.z--;
            m_faces.push_back(face);
        }
    }
}

void AssetImporter::parseGltf(const std::string& assetPath)
{
    std::ifstream asset { assetPath };
}
