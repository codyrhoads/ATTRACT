//
//  MotionBlur.cpp
//  ATTRACT
//
//  Created by Cody Rhoads on 3/13/17.
//
//

#include <iostream>

#include "MotionBlur.hpp"

using namespace std;

MotionBlur::MotionBlur()
{
    
}

MotionBlur::~MotionBlur()
{
    
}

void MotionBlur::init()
{
    // Set up a quad.
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);
    
    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };
    
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    
    // Create the frame buffer object.
    glGenFramebuffers(1, &frameBuf);
    glGenTextures(1, &texBuf);
    glGenTextures(1, &motionBuf);
    glGenRenderbuffers(1, &depthBuf);
    createFBO();
    GLSL::checkError(GET_FILE_LINE);
    
    // set up depth
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
    
    //bind with framebuffer's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBuf, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, motionBuf, 0);
    
    GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, DrawBuffers);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLSL::checkError(GET_FILE_LINE);
}

/*
 Helper function to create the framebuffer object and associated texture to write to
 */
void MotionBlur::createFBO() {
    //set up framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    //set up texture
    glBindTexture(GL_TEXTURE_2D, texBuf);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBuf, 0);
    
    // Buffer for holding motion vectors
    glBindTexture(GL_TEXTURE_2D, motionBuf);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, motionBuf, 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cout << "Error setting up frame buffer - exiting" << endl;
        exit(0);
    }
}

void MotionBlur::bindFramebuffer()
{
    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void MotionBlur::unbindFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MotionBlur::bindTex(GLint handle)
{
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, texBuf);
    glUniform1i(handle, texUnit);
}

void MotionBlur::unbindTex()
{
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MotionBlur::bindMotion(GLint handle)
{
    glActiveTexture(GL_TEXTURE0 + motionUnit);
    glBindTexture(GL_TEXTURE_2D, motionBuf);
    glUniform1i(handle, motionUnit);
}

void MotionBlur::unbindMotion()
{
    glActiveTexture(GL_TEXTURE0 + motionUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MotionBlur::draw()
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
}
