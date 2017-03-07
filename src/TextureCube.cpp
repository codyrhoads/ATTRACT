#include "TextureCube.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "stb_image.h"
#include "GLSL.h"

using namespace std;

TextureCube::TextureCube() :
	tid(0),
	unit(0)
{
	
}

TextureCube::~TextureCube()
{
	
}

void TextureCube::setFilenames(const string &negx,
							   const string &posx,
							   const string &negy,
							   const string &posy,
							   const string &negz,
							   const string &posz)
{
	filenames[0] = negx;
	filenames[1] = posx;
	filenames[2] = negy;
	filenames[3] = posy;
	filenames[4] = negz;
	filenames[5] = posz;
}

void TextureCube::init()
{
	// Generate a texture buffer object
	glGenTextures(1, &tid);
	// Bind the current texture to be the newly generated texture object
	glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
    GLSL::checkError();
	
	// Load textures
	GLenum targets[] = {
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	};
    GLSL::checkError();
	for(int i = 0; i < 6; ++i) {
		int w, h, ncomps;
		//stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filenames[i].c_str(), &w, &h, &ncomps, 0);
        GLSL::checkError();
		if(!data) {
			cerr << filenames[i] << " not found" << endl;
		}
		if(ncomps != 3) {
			cerr << filenames[i] << " must have 3 components (RGB)" << endl;
		}
		if((w & (w - 1)) != 0 || (h & (h - 1)) != 0) {
			cerr << filenames[i] << " must be a power of 2" << endl;
		}
		width = w;
		height = h;
		// Load the actual texture data
		// Base level is 0, number of channels is 3, and border is 0.
        GLSL::checkError();
		glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        GLSL::checkError();
		// Free image, since the data is now on the GPU
		stbi_image_free(data);
        GLSL::checkError();
	}
    GLSL::checkError();
	
	// Generate image pyramid
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    GLSL::checkError();
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	// Set wrap modes
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Set mipmapping
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCube::bind(GLint handle)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
	glUniform1i(handle, unit);
}

void TextureCube::unbind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
