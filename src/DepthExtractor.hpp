//
//  DepthExtractor.hpp
//  ATTRACT
//
//  Created by Cody Rhoads on 3/10/17.
//
//

#ifndef DepthExtractor_hpp
#define DepthExtractor_hpp

#define GLEW_STATIC
#include <GL/glew.h>

class DepthExtractor
{
public:
    DepthExtractor();
    virtual ~DepthExtractor();
    void init();
    void bindFramebuffer();         // used when creating the texture
    void unbindFramebuffer();
    void bind(GLint handle);        // used when applying the texture
    void unbind();
    void drawDebug();               // used to draw the debug texture
    
    void setUnit(GLint u) { unit = u; }
    GLint getUnit() const { return unit; }
    
private:
    GLint unit;
    GLuint depthMapFBO;
    const GLuint WIDTH = 1024, HEIGHT = 1024;
    GLuint depthMap;
    //geometry for texture render
    GLuint quad_VertexArrayID;
    GLuint quad_vertexbuffer;
};

#endif /* DepthExtractor_hpp */
