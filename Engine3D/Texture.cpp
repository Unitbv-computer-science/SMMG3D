#include "stdafx.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {
	_isCreated = false;
	_isAlpha = false;
	_width = 0;
	_height = 0;
	_target = Target::Tex2D;
	_format = Format::RGB;
	_internalFormat = InternalFormat::RGB;
	_textureID = 0;
}


Texture::Texture(const std::string& path)
{
	load(path);
}

Texture::Texture(Texture&& texture) : Texture() {
	std::swap(_isAlpha, texture._isAlpha);
	std::swap(_isCreated, texture._isCreated);
	std::swap(_width, texture._width);
	std::swap(_height, texture._height);
	std::swap(_target, texture._target);
	std::swap(_format, texture._format);
	std::swap(_textureID, texture._textureID);
	std::swap(_internalFormat, texture._internalFormat);
}

Texture::~Texture() {
	destroy();
}

Texture& Texture::operator=(Texture&& texture) {
	std::swap(_isAlpha, texture._isAlpha);
	std::swap(_isCreated, texture._isCreated);
	std::swap(_width, texture._width);
	std::swap(_height, texture._height);
	std::swap(_target, texture._target);
	std::swap(_format, texture._format);
	std::swap(_textureID, texture._textureID);
	std::swap(_internalFormat, texture._internalFormat);

	return *this;
}

void Texture::create() {
	destroy();

	glGenTextures(1, &_textureID);
	_isCreated = true;

	setWidth(0);
	setHeight(0);
}

void Texture::destroy() {
	if (isCreated()) {
		glDeleteTextures(1, &_textureID);

		_isCreated = false;
	}
}

void Texture::bind() const {
	glBindTexture(static_cast<GLenum>(getTarget()), getID());
}

void Texture::unbind() const {
	glBindTexture(static_cast<GLenum>(getTarget()), 0);
}

void Texture::load(const std::string& path) {
	int width, height, nrChannels;
	unsigned char* pImage =
		stbi_load("image.png", &width, &height, &nrChannels, 0);
	if (nrChannels == 1) {
		setInternalFromat(InternalFormat::Red);
		setFormat(Format::Red);
	}
	else {
		if (nrChannels == 3) {
			setInternalFromat(InternalFormat::RGB);
			setFormat(Format::RGB);
		}
	} if (nrChannels == 4) { 
		setInternalFromat(InternalFormat::RGBA);
		setFormat(Format::RGBA);
	}

	setData2D(width, height, GL_UNSIGNED_BYTE, pImage);

	stbi_image_free(pImage);
			
	setTarget(Target::Tex2D);
	bind();

	setParameters();
	generateMipmap();
	unbind();
}

void Texture::generateMipmap() {
	glGenerateMipmap(static_cast<GLenum>(getTarget()));
}

void Texture::setData1D(GLsizei width, GLenum dataType, const GLvoid* data, GLint level) {
	setWidth(width);

	glTexImage1D(
		static_cast<GLenum>(getTarget()),
		level,
		static_cast<GLenum>(getInternalFromat()),
		width,
		0,
		static_cast<GLenum>(getFormat()),
		dataType,
		data
	);
}

void Texture::setData2D(GLsizei width, GLsizei height, GLenum dataType, const GLvoid* data, GLint level) {
	setWidth(width);
	setHeight(height);

	glTexImage2D(
		static_cast<GLenum>(getTarget()),
		level,
		static_cast<GLenum>(getInternalFromat()),
		width,
		height,
		0,
		static_cast<GLenum>(getFormat()),
		dataType,
		data
	);
}

void Texture::setData3D(GLsizei width, GLsizei height, GLsizei depth, GLenum dataType, const GLvoid* data, GLint level) {
	setWidth(width);
	setHeight(height);

	glTexImage3D(
		static_cast<GLenum>(getTarget()),
		level,
		static_cast<GLenum>(getInternalFromat()),
		width,
		height,
		depth,
		0,
		static_cast<GLenum>(getFormat()),
		dataType,
		data
	);
}

void Texture::setSubData1D(GLint xoffset, GLsizei width, GLenum dataType, const GLvoid* data, GLint level) {
	glTexSubImage1D(
		static_cast<GLenum>(getTarget()),
		level,
		xoffset,
		width,
		static_cast<GLenum>(getFormat()),
		dataType,
		data
	);
}

void Texture::setSubData2D(GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum dataType, const GLvoid* data, GLint level) {
	glTexSubImage2D(
		static_cast<GLenum>(getTarget()),
		level,
		xoffset,
		yoffset,
		width,
		height,
		static_cast<GLenum>(getFormat()),
		dataType,
		data
	);
}

void Texture::setSubData3D(GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
	GLenum dataType, const GLvoid * data, GLint level)
{
	glTexSubImage3D(
		static_cast<GLenum>(getTarget()),
		level,
		xoffset,
		yoffset,
		zoffset,
		width,
		height,
		depth,
		static_cast<GLenum>(getFormat()),
		dataType,
		data
	);
}

void Texture::setTarget(Target target) {
	_target = target;
}

void Texture::setParameters() {
	glTexParameteri(static_cast<GLenum>(getTarget()), GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(static_cast<GLenum>(getTarget()), GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(static_cast<GLenum>(getTarget()), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(static_cast<GLenum>(getTarget()), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Texture::setParameters(std::list<std::pair<GLenum, GLint>> parameters) {
	for (auto& parameter : parameters)
		glTexParameteri(static_cast<GLenum>(getTarget()), parameter.first, parameter.second);
}

void Texture::setParameters(std::list<std::pair<GLenum, GLfloat>> parameters) {
	for (auto& parameter : parameters)
		glTexParameterf(static_cast<GLenum>(getTarget()), parameter.first, parameter.second);
}

void Texture::setFormat(Format format) {
	_format = format;
}

void Texture::setInternalFromat(InternalFormat format) {
	_internalFormat = format;
}

bool Texture::isAlpha() const {
	return _isAlpha;
}

unsigned int Texture::getWidth() const {
	return _width;
}

unsigned int Texture::getHeight() const {
	return _height;
}

Texture::Target Texture::getTarget() const {
	return _target;
}

Texture::Format Texture::getFormat() const {
	return _format;
}

Texture::InternalFormat Texture::getInternalFromat() const {
	return _internalFormat;
}

GLuint Texture::getID() const {
	if (!isCreated())
		const_cast<Texture*>(this)->create();

	return _textureID;
}

void Texture::setWidth(unsigned int width) {
	_width = width;
}

void Texture::setHeight(unsigned int height) {
	_height = height;
}

bool Texture::isCreated() const {
	return _isCreated;
}
