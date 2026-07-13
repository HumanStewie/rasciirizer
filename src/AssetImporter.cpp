#include "AssetImporter.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <array>

#include "sgm.h"
#include "json.hpp"

using json = nlohmann::json;

void AssetImporter::parseObj(const std::string& assetPath)
{
    std::ifstream asset { assetPath };
    std::cout << assetPath << '\n';
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
    // .gltf
    std::ifstream asset { assetPath };
    json data{ json::parse(asset) };

    int positionAccessor{ data["accessors"][data["meshes"][0]["primitives"][0]["attributes"]["POSITION"]]};
    int positionBufferView{ data["bufferViews"][positionAccessor["bufferView"]]};
    int positionBuffer{ data["buffers"][positionBufferView["buffer"]] };

    std::string binaryPath{ assetPath };
    binaryPath.replace(assetPath.size() - 3, assetPath.size(), "bin");
    std::ifstream bin{ binaryPath, std::ios::binary };

    bin.seekg(0, std::ios::end);
    int binaryLength{ static_cast<int>(bin.tellg()) };
    bin.seekg(0, std::ios::beg);

    std::vector<char> binaryFile(binaryLength);
    bin.read(binaryFile.data(), binaryLength);

}