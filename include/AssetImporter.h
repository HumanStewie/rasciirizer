#pragma once
#include <fribidi/fribidi.h>

#include <vector>

#include "sgm.h"

class AssetImporter {
   public:
    AssetImporter(std::vector<sgm::Vec3D>& vertices,
                  std::vector<sgm::Vec<int, 3>>& faces)
        : m_vertices { vertices }, m_faces { faces }
    {
    }
    ~AssetImporter() noexcept {}
    void parseObj(const std::string& assetPath);
    void parseGltf(const std::string& assetPath);
    void parseGlb(const std::string& assetPath);

   private:
    std::vector<sgm::Vec3D>& m_vertices;
    std::vector<sgm::Vec<int, 3>>& m_faces;
};
