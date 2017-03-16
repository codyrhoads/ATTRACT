//
//  MotionBlur.hpp
//  ATTRACT
//
//  Created by Cody Rhoads on 3/13/17.
//
//

#ifndef MotionBlur_hpp
#define MotionBlur_hpp

#include "GLSL.h"

class MotionBlur
{
public:
    MotionBlur();
    virtual ~MotionBlur();
    void init();
    void bindFramebuffer();
    void unbindFramebuffer();
    void bindTex(GLint handle);
    void unbindTex();
    void bindMotion(GLint handle);
    void unbindMotion();
    void draw();
    
    void setTexUnit(GLint u) { texUnit = u; }
    GLint getTexUnit() const { return texUnit; }
    
    void setMotionUnit(GLint u) { motionUnit = u; }
    GLint getMotionUnit() const { return motionUnit; }
    
    GLuint texBuf;
    GLuint motionBuf;
    GLuint depthBuf;
    
private:
    void createFBO();
    
    GLint texUnit;
    GLint motionUnit;
    GLuint frameBuf;
//    GLuint texBuf;
//    GLuint motionBuf;
//    GLuint depthBuf;
    const GLuint WIDTH = 1024, HEIGHT = 1024;
    GLuint quad_VertexArrayID;
    GLuint quad_vertexbuffer;
};

#endif /* MotionBlur_hpp */
