#pragma once

#include <string>
#include <vector>

#include "Texture.h"

class TerrainSector {
public:
	typedef std::vector<std::vector<short int>> HeightMap;

public:
	TerrainSector();
	TerrainSector(TerrainSector&& sector);

	TerrainSector& operator=(TerrainSector&& sector);

	void clear();
	void loadData(const std::string& filePath);

	int getLatitude() const;
	int getLongtitude() const;
	short int getHeight(unsigned int x, unsigned int y) const;
	const HeightMap& getHeightMap() const;
	const Texture& getTexture() const;

private:
	void resize(unsigned int size);

	void setLatitude(int value);
	void setLongtitude(int value);

	void parseLocation(const std::string& fileName);

	short int swapEndiannes(short int value);

	int _latitude;
	int _longitude;
	unsigned int _mapSize;
	HeightMap _heightMap;
	Texture _texture;
};
