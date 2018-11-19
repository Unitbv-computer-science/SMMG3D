#include "stdafx.h"
#include "TerrainSector.h"
#include "FileUtils.h"


TerrainSector::TerrainSector() {
	setLatitude(0);
	setLongtitude(0);

	_mapSize = 1201;

	resize(_mapSize);
}

TerrainSector::TerrainSector(TerrainSector&& TerrainSector) : 
	_heightMap(std::move(TerrainSector._heightMap)), _texture(std::move(TerrainSector._texture)) {
	std::swap(_latitude, TerrainSector._latitude);
	std::swap(_longitude, TerrainSector._longitude);
	std::swap(_mapSize, TerrainSector._mapSize);
}

TerrainSector& TerrainSector::operator=(TerrainSector&& TerrainSector) {
	_heightMap = std::move(TerrainSector._heightMap);
	_texture = std::move(TerrainSector._texture);

	std::swap(_latitude, TerrainSector._latitude);
	std::swap(_longitude, TerrainSector._longitude);
	std::swap(_mapSize, TerrainSector._mapSize);

	return *this;
}

void TerrainSector::clear() {
	_heightMap.clear();
}

void TerrainSector::loadData(const std::string& filePath) {
	std::ifstream fileStream;
	std::vector<short int> texData;
	short int height;

	std::cerr << "Loading TerrainSector " << filePath << "... ";

	fileStream.open(filePath, std::ifstream::binary | std::ifstream::in);

	if (!fileStream.is_open())
		throw new std::exception((std::string("Unable to load heightmap from file: ") + filePath).c_str());

	for (unsigned int y = _mapSize - 1; y >= 0; y--) {
		for (unsigned int x = 0; x < _mapSize; ++x) {
			fileStream.read(reinterpret_cast<char*>(&height), sizeof(height));

			_heightMap[y][x] = swapEndiannes(height);
			texData.push_back(_heightMap[y][x]);
		}
	}

	fileStream.close();

	parseLocation(FileUtils::getFilenameExtensionless(filePath));

	/**
	* Texture
	*/
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

	std::list<std::pair<GLenum, GLint>> parameters;
	parameters.push_back(std::make_pair<GLenum, GLint>(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	parameters.push_back(std::make_pair<GLenum, GLint>(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	_texture.setTarget(Texture::Target::Tex2D);
	_texture.setFormat(Texture::Format::Red);
	_texture.setInternalFromat(Texture::InternalFormat::R16);
	_texture.bind();
	_texture.setData2D(1201, 1201, GL_SHORT, &texData[0]);
	_texture.setParameters();
	_texture.setParameters(parameters);
	_texture.generateMipmap();
	_texture.unbind();

	std::cout << "done." << std::endl;
}

int TerrainSector::getLatitude() const {
	return _latitude;
}

int TerrainSector::getLongtitude() const {
	return _longitude;
}

short int TerrainSector::getHeight(unsigned int x, unsigned int y) const {
	return _heightMap[y][x];
}

const TerrainSector::HeightMap& TerrainSector::getHeightMap() const {
	return _heightMap;
}

const Texture& TerrainSector::getTexture() const {
	return _texture;
}

void TerrainSector::resize(unsigned int size) {
	_heightMap.resize(size);

	for (unsigned int y = 0; y < size; ++y)
		_heightMap[y].resize(size);
}

void TerrainSector::setLatitude(int value) {
	_latitude = value;
}

void TerrainSector::setLongtitude(int value) {
	_longitude = value;
}

void TerrainSector::parseLocation(const std::string& fileName) {
	char xDir, yDir;
	int x, y;

	sscanf_s(fileName.c_str(), "%c%d%c%d", &yDir, &y, &xDir, &x);

	if (xDir == 'W' || xDir == 'w')
		x = -x;

	if (yDir == 'S' || yDir == 's')
		y = -y;

	setLatitude(y);
	setLongtitude(x);
}

short int TerrainSector::swapEndiannes(short int value) {
	static union {
		int16_t value;
		int8_t  byte[2];
	} eUnion;

	eUnion.value = value;
	std::swap(eUnion.byte[0], eUnion.byte[1]);

	return eUnion.value;
}