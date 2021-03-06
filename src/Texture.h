#pragma once
#ifndef __Texture__
#define __Texture__

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>

class Texture {
public:

    /*data structure for the image used for  texture mapping */
    typedef struct Image {
        int sizeX;
        int sizeY;
        char *data;
    } Image;

    typedef struct RGB {
        GLubyte r;
        GLubyte g;
        GLubyte b;
    } RGB;
    static int ImageLoad(const char *filename, Image *image);

    
    Texture();
    virtual ~Texture();

    void setFilename(const std::string &f) {
        filename = f;
    }
    void init();
    
    void initBMP();

    void setUnit(GLint u) {
        unit = u;
    }

    GLint getUnit() const {
        return unit;
    }
    void bind(GLint handle);
    void unbind();
    void setWrapModes(GLint wrapS, GLint wrapT); // Must be called after init()
    void loadDDS(const char * imagepath);
private:
    std::string filename;
    int width;
    int height;
    GLuint tid;

    GLint unit;

};

#endif
