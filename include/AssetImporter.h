#pragma once
#include <string_view>
#include <fstream>

class AssetImporter {
public:
	AssetImporter(const std::string& asset) : m_asset{asset} {}
	~AssetImporter() noexcept {
		m_asset.close();
	}
	void readObj();
	void readGltf();
	void readGlb();
private:
	std::ifstream m_asset{};
};