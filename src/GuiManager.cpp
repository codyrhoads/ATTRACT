/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GuiManager.cpp
 * Author: root
 * 
 * Created on February 9, 2017, 3:36 PM
 */

#include "GuiManager.hpp"

using namespace std;
using namespace glm;

GuiManager::GuiManager(string resource) :
selectedName("play"),
RESOURCE_DIR(resource) {
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    static const GLfloat tex_buffer_data[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    guiShader = make_shared<Program>();
    guiShader->setShaderNames(RESOURCE_DIR + "guiVert.glsl", RESOURCE_DIR + "guiFrag.glsl");
    guiShader->init();
    guiShader->addAttribute("pos");

    guiShader->addAttribute("posTex");
    guiShader->addUniform("guiTex");
    guiShader->addUniform("M");

    shared_ptr<Texture> playTex = make_shared<Texture>();
    playTex->setFilename(RESOURCE_DIR + "play.jpg");
    playTex->init();
    playTex->setUnit(0);
    playTex->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("play", playTex));
    scales.insert(make_pair("play", vec3(0.5, 0.5, 1)));
    translates.insert(make_pair("play", vec3(0, -0.2, 0)));

    shared_ptr<Texture> playTex1 = make_shared<Texture>();
    playTex1->setFilename(RESOURCE_DIR + "attract.jpg");
    playTex1->init();
    playTex1->setUnit(0);
    playTex1->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("attract", playTex1));
    scales.insert(make_pair("attract", vec3(1, 0.55, 1)));
    translates.insert(make_pair("attract", vec3(0, 0.5, 0)));

    shared_ptr<Texture> playTex2 = make_shared<Texture>();
    playTex2->setFilename(RESOURCE_DIR + "quit.jpg");
    playTex2->init();
    playTex2->setUnit(0);
    playTex2->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("quit", playTex2));
    scales.insert(make_pair("quit", vec3(1, 0.35, 1)));
    translates.insert(make_pair("quit", vec3(0, -0.8, 0)));

    shared_ptr<Texture> playTex3 = make_shared<Texture>();
    playTex3->setFilename(RESOURCE_DIR + "arrow.jpg");
    playTex3->init();
    playTex3->setUnit(0);
    playTex3->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("arrow", playTex3));
    scales.insert(make_pair("arrow", vec3(0.3, 0.2, 1)));
    translates.insert(make_pair("arrow", vec3(-0.5, -0.2, 0)));

    shared_ptr<Texture> playTex4 = make_shared<Texture>();
    playTex4->setFilename(RESOURCE_DIR + "pause.jpg");
    playTex4->init();
    playTex4->setUnit(0);
    playTex4->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("pause", playTex4));
    scales.insert(make_pair("pause", vec3(1, 1, 1)));
    translates.insert(make_pair("pause", vec3(-0.5, 0.5, 0)));

    shared_ptr<Texture> playTex5 = make_shared<Texture>();
    playTex5->setFilename(RESOURCE_DIR + "shipparts0.jpg");
    playTex5->init();
    playTex5->setUnit(0);
    playTex5->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("shipparts0", playTex5));
    scales.insert(make_pair("shipparts0", vec3(0.7, 0.7, 1)));
    translates.insert(make_pair("shipparts0", vec3(0.5, 0.5, 0)));

    playTex5 = make_shared<Texture>();
    playTex5->setFilename(RESOURCE_DIR + "death.jpg");
    playTex5->init();
    playTex5->setUnit(0);
    playTex5->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("death", playTex5));
    scales.insert(make_pair("death", vec3(0.7, 0.7, 1)));
    translates.insert(make_pair("death", vec3(0.5, 0.5, 0)));

    playTex5 = make_shared<Texture>();
    playTex5->setFilename(RESOURCE_DIR + "win.jpg");
    playTex5->init();
    playTex5->setUnit(0);
    playTex5->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("win", playTex5));
    scales.insert(make_pair("win", vec3(0.7, 0.7, 1)));
    translates.insert(make_pair("win", vec3(0.5, 0.5, 0)));

    playTex5 = make_shared<Texture>();
    playTex5->setFilename(RESOURCE_DIR + "nextlevel.jpg");
    playTex5->init();
    playTex5->setUnit(0);
    playTex5->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("nextlevel", playTex5));
    scales.insert(make_pair("nextlevel", vec3(0.7, 0.7, 1)));
    translates.insert(make_pair("nextlevel", vec3(-0.5, -0.5, 0)));

    playTex5 = make_shared<Texture>();
    playTex5->setFilename(RESOURCE_DIR + "tryagain.jpg");
    playTex5->init();
    playTex5->setUnit(0);
    playTex5->setWrapModes(GL_REPEAT, GL_REPEAT);
    guiTextures.insert(make_pair("tryagain", playTex5));
    scales.insert(make_pair("tryagain", vec3(0.7, 0.7, 1)));
    translates.insert(make_pair("tryagain", vec3(-0.5, -0.5, 0)));







    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &texbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof (g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (tex_buffer_data), tex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);



}

GuiManager::GuiManager(const GuiManager& orig) {
}

GuiManager::~GuiManager() {
}

State GuiManager::interpretMenuPressedKeys(vector<char> pressedKeys) {
    if (find(pressedKeys.begin(), pressedKeys.end(), 's') != pressedKeys.end()) {
        translates["arrow"] = vec3(-0.8, -0.8, 0);
        selectedName = "quit";
    } else if (find(pressedKeys.begin(), pressedKeys.end(), 'w') != pressedKeys.end()) {
        translates["arrow"] = vec3(-0.5, -0.2, 0);
        selectedName = "play";
    } else if (find(pressedKeys.begin(), pressedKeys.end(), '\n') != pressedKeys.end()) {
        if (selectedName == "play")
            return GAME;
        else if (selectedName == "quit")
            exit(0);
    }
    return MENU;
}

State GuiManager::interpretPausePressedKeys(std::vector<char> pressedKeys) {
    if (find(pressedKeys.begin(), pressedKeys.end(), '<') != pressedKeys.end()) {
        return GAME;
    } else if (find(pressedKeys.begin(), pressedKeys.end(), '\n') != pressedKeys.end()) {
        if (selectedName == "quit")
            exit(0);
    }

    return PAUSE;
}

State GuiManager::interpretDeathPressedKeys(std::vector<char> pressedKeys) {
    if (find(pressedKeys.begin(), pressedKeys.end(), 's') != pressedKeys.end()) {
        translates["arrow"] = vec3(-0.8, -0.8, 0);
        selectedName = "quit";
    } else if (find(pressedKeys.begin(), pressedKeys.end(), 'w') != pressedKeys.end()) {
        translates["arrow"] = vec3(-0.5, -0.2, 0);
        selectedName = "tryagain";
    } else if (find(pressedKeys.begin(), pressedKeys.end(), '\n') != pressedKeys.end()) {
        if (selectedName == "quit")
            exit(0);
        else if (selectedName == "tryagain")
            return GAME;
    }
    return DEATH;
}

State GuiManager::interpretWinPressedKeys(std::vector<char> pressedKeys) {
    if (find(pressedKeys.begin(), pressedKeys.end(), 's') != pressedKeys.end()) {
        translates["arrow"] = vec3(-0.8, -0.8, 0);
        selectedName = "quit";
    } else if (find(pressedKeys.begin(), pressedKeys.end(), 'w') != pressedKeys.end()) {
        translates["arrow"] = vec3(-0.5, -0.2, 0);
        selectedName = "nextlevel";
    } else if (find(pressedKeys.begin(), pressedKeys.end(), '\n') != pressedKeys.end()) {
        if (selectedName == "quit")
            exit(0);
        else if (selectedName == "nextlevel")
            return GAME;
    }
    return WIN;
}

void GuiManager::drawMenu() {
    draw("attract");
    draw("play");
    draw("quit");
    draw("arrow");

}

void GuiManager::drawPause() {
    translates["arrow"] = vec3(-0.8, -0.8, 0);
    draw("arrow");
    draw("pause");
    draw("shipparts0");
    draw("quit");
    selectedName = "quit";
}

void GuiManager::drawDeath() {

    draw("death");
    draw("tryagain");
    draw("quit");
    draw("arrow");
}

void GuiManager::drawWin() {

    draw("win");
    draw("nextlevel");
    draw("quit");
    draw("arrow");
}

void GuiManager::drawAll() {
    for (std::map<string, shared_ptr < Texture>>::iterator it = guiTextures.begin(); it != guiTextures.end(); ++it) {
        draw(it->first);
    }

}

void GuiManager::draw(string name) {

    auto M = make_shared<MatrixStack>();
    M->pushMatrix();
    M->loadIdentity();

    M->translate(translates[name]);
    M->scale(scales[name]);

    glDisable(GL_DEPTH_TEST);

    guiShader->bind();
    guiTextures[name]->bind(guiShader->getUniform("guiTex"));


    glUniformMatrix4fv(guiShader->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));

    int pos = guiShader->getAttribute("pos");
    glEnableVertexAttribArray(pos);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            pos, // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3, // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*) 0 // array buffer offset
            );

    glEnableVertexAttribArray(guiShader->getAttribute("posTex"));
    glBindBuffer(GL_ARRAY_BUFFER, texbuffer);
    glVertexAttribPointer(guiShader->getAttribute("posTex"), 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);


    // Draw the triangle !
    glDrawArrays(GL_QUADS, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(pos);


    guiShader->unbind();
    glEnable(GL_DEPTH_TEST);
    M->popMatrix();
}

