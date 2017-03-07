//
//  GameManager.cpp
//  Lab1
//
//  Created by Cody Rhoads on 1/17/17.
//
//

#include <iostream>
#include <math.h>
#include <time.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameManager.hpp"

#include "AABoundingBox.hpp"
#include "BoundingSphere.hpp"

#include "InputManager.hpp"
#include "Camera.hpp"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"
#include "Material.hpp"
#include "GameObject.hpp"
#include "Texture.h"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "BulletManager.hpp"
#include "Cuboid.hpp"
#include "RayTrace.hpp"
#include "FmodManager.hpp"
#include "VfcManager.hpp"
#include "SpaceShipPart.hpp"
#include "Skybox.hpp"
#include "ShadowManager.hpp"
#include <fstream>

#include "GuiManager.hpp"
#include "ParticleManager.hpp"

#include "KDTree.hpp"
#include "BVH.hpp"

#define MAX_NUM_OBJECTS 15
#define GRID_SIZE 8
#define OBJ_SPAWN_INTERVAL 2
#define BUNNY_SPHERE_RADIUS 0.5f
#define MAGNET_RANGE 13.0f
#define MAGNET_STRENGTH 7.0f
#define CUBE_HALF_EXTENTS vec3(0.5f, 0.5f, 0.5f)

using namespace std;
using namespace glm;

GameManager::GameManager(GLFWwindow *window, const string &resourceDir) :
window(window),
RESOURCE_DIR(resourceDir),
gameState(MENU),
level(1) {
    objIntervalCounter = 0.0f;
    numObjCollected = 0;
    gameWon = false;
    camera = make_shared<Camera>();

    // Set vsync.
    glfwSwapInterval(1);
    // Set keyboard callback.
    glfwSetKeyCallback(window, Keyboard::key_callback);
    // Sets cursor movement to unlimited.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Set cursor position callback.
    glfwSetCursorPosCallback(window, Mouse::cursor_position_callback);
    // Set the mouse button callback.
    glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
    // Set the window resize call back.
    glfwSetFramebufferSizeCallback(window, resize_callback);
    // Set up the manager for keyboard and mouse input.
    inputManager = make_shared<InputManager>(camera);
    //init vfc
    vfc = make_shared<VfcManager>();
    fmod = make_shared<FmodManager>(RESOURCE_DIR);
    gui = make_shared<GuiManager>(RESOURCE_DIR);
    psystem = make_shared<ParticleManager>(RESOURCE_DIR);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    shadowManager = make_shared<ShadowManager>(RESOURCE_DIR);

    // Initialize the scene.
    initScene();
}

GameManager::~GameManager() {

}

void GameManager::initScene() {
    // Initialize time.
    glfwSetTime(0.0);

    // Set background color.
    glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
    // Enable z-buffer test.
    glEnable(GL_DEPTH_TEST);

    //
    // Objects
    //
    program = make_shared<Program>();
    program->setShaderNames(RESOURCE_DIR + "objectVert.glsl", RESOURCE_DIR + "objectFrag.glsl");
    program->setVerbose(true);
    GLSL::checkError(GET_FILE_LINE);
    program->init();
    GLSL::checkError(GET_FILE_LINE);
    program->addAttribute("aPos");
    program->addAttribute("aNor");
    program->addUniform("M");
    program->addUniform("V");
    program->addUniform("P");
    program->addUniform("lightPos");
    program->addUniform("lightIntensity");
    program->addUniform("ka");
    program->addUniform("kd");
    program->addUniform("ks");
    program->addUniform("s");
    GLSL::checkError(GET_FILE_LINE);
    program->addUniform("LS");
    program->addUniform("shadowDepth");
    GLSL::checkError(GET_FILE_LINE);
    
    //
    // Skyscrapers
    //
    skyscraperProgram = make_shared<Program>();
    skyscraperProgram->setShaderNames(RESOURCE_DIR + "skyscraperVert.glsl", RESOURCE_DIR + "skyscraperFrag.glsl");
    skyscraperProgram->setVerbose(true);
    skyscraperProgram->init();
    skyscraperProgram->addAttribute("aPos");
    skyscraperProgram->addAttribute("aNor");
    skyscraperProgram->addAttribute("aTex");
    skyscraperProgram->addUniform("M");
    skyscraperProgram->addUniform("V");
    skyscraperProgram->addUniform("P");
    skyscraperProgram->addUniform("diffuseTex");
    skyscraperProgram->addUniform("specularTex");
    skyscraperProgram->addUniform("lightPos");
    skyscraperProgram->addUniform("lightIntensity");
    skyscraperProgram->addUniform("objTransMatrix");
    skyscraperProgram->addUniform("scalingFactor");

    //
    // Ship Parts
    //
    shipPartProgram = make_shared<Program>();
    shipPartProgram->setShaderNames(RESOURCE_DIR + "shipPartVert.glsl",
            RESOURCE_DIR + "shipPartFrag.glsl");
    shipPartProgram->setVerbose(true);
    shipPartProgram->init();

    shipPartProgram->addAttribute("aPos");
    shipPartProgram->addAttribute("aNor");
    shipPartProgram->addAttribute("aTex");
    shipPartProgram->addUniform("M");
    shipPartProgram->addUniform("V");
    shipPartProgram->addUniform("P");
    shipPartProgram->addUniform("diffuseTex");
    shipPartProgram->addUniform("specularTex");
    shipPartProgram->addUniform("lightPos");
    shipPartProgram->addUniform("shadowDepth");
    shipPartProgram->addUniform("LS");

    shipPartColorTexture = make_shared<Texture>();
    shipPartColorTexture->setFilename(RESOURCE_DIR + "shipPartColor.jpg");
    shipPartColorTexture->init();
    shipPartColorTexture->setUnit(2);
    shipPartColorTexture->setWrapModes(GL_REPEAT, GL_REPEAT);
    shipPartSpecularTexture = make_shared<Texture>();
    shipPartSpecularTexture->setFilename(RESOURCE_DIR + "shipPartSpecular.jpg");
    shipPartSpecularTexture->init();
    shipPartSpecularTexture->setUnit(3);
    shipPartSpecularTexture->setWrapModes(GL_REPEAT, GL_REPEAT);
    
    //
    // Loading obj files
    //
    shared_ptr<Shape> temp = make_shared<Shape>();
    
    temp->loadMesh(RESOURCE_DIR + "cube.obj", RESOURCE_DIR);
    temp->fitToUnitBox();
    temp->init();
    shapes.insert(make_pair("plainCube", temp));
    
    temp = make_shared<Shape>();
    temp->loadMesh(RESOURCE_DIR + "texturedSkyscraper.obj", RESOURCE_DIR);
    temp->fitToUnitBox();
    temp->init();
    shapes.insert(make_pair("skyscraper", temp));

    temp = make_shared<Shape>();
    temp->loadMesh(RESOURCE_DIR + "shipPart.obj", RESOURCE_DIR);
    temp->fitToUnitBox();
    temp->init();
    shapes.insert(make_pair("shipPart", temp));

    temp = make_shared<Shape>();
    temp->loadMesh(RESOURCE_DIR + "Magnagun.obj", RESOURCE_DIR);
    temp->fitToUnitBox();
    temp->init();
    shapes.insert(make_pair("magnetGun", temp));
    
    temp = make_shared<Shape>();
    temp->loadMesh(RESOURCE_DIR + "cylinder.obj", RESOURCE_DIR);
    temp->fitToUnitBox();
    temp->init();
    shapes.insert(make_pair("cylinder", temp));
    
    temp = make_shared<Shape>();
    temp->loadMesh(RESOURCE_DIR + "sphere.obj", RESOURCE_DIR);
    temp->fitToUnitBox();
    temp->init();
    shapes.insert(make_pair("sphere", temp));
    
    //
    // Make Skybox
    //
    skybox = make_shared<Skybox>(RESOURCE_DIR, shapes["sphere"]);

    lightPos = vec3(20.0f, 10.0f, 1.0f);
    lightIntensity = 0.6f;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLSL::checkError(GET_FILE_LINE);

    // Set up the bullet manager and create ground plane and camera.
    //bullet = make_shared<BulletManager>();
    //bullet->createPlane("ground", 0, 0, 0);

    shared_ptr<Material> material = make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(0.0f, 0.5f, 0.5f), vec3(1.0f, 0.9f, 0.8f), 200.0f);
    magnetGun = make_shared<GameObject>(vec3(0.4, -0.2, -1), vec3(0.4, 0, -0.2), vec3(1, 1, 1), 0, shapes["magnetGun"], nullptr);
    magnetBeam = make_shared<GameObject>(vec3(0.4, -0.2, -3), vec3(0.4, 0, -0.2), vec3(0.5, 0.5, 0.5), 0, shapes["cylinder"], material);
}

bool GameManager::toBool(string s) {
    return s != "0";
}

void GameManager::parseCamera(string objectString) {
    char *str = new char[objectString.length() + 1];
    strcpy(str, objectString.c_str());

    vector<string> elems;
    char *tok = strtok(str, ",");
    while (tok != NULL) {
        elems.push_back(tok);
        tok = strtok(NULL, ",");
    }
    //split(objectString, ',', back_inserter(elems));
    string::size_type sz;
    vec3 pos = vec3(stof(elems[0], &sz), stof(elems[1]), stof(elems[2]));
    vec3 scale = vec3(stof(elems[3]), stof(elems[4]), stof(elems[5]));
    vec3 rot = vec3(stof(elems[6]), stof(elems[7]), stof(elems[8]));
    bool magnetic = toBool(elems[9]);
    bool deadly = toBool(elems[10]);
    bool playerSpawn = toBool(elems[11]);
    bool collectable = toBool(elems[12]);

    //    cerr << "new camera" << endl;
    //    cerr << pos.x << " " << pos.y << endl;
    //    cerr << scale.x << " " << scale.y << endl;

    camera = make_shared<Camera>(pos, window);
    inputManager->setCamera(camera);
    bullet->createBox("cam", pos, CUBE_HALF_EXTENTS, scale, 1);
}

void GameManager::parseObject(string objectString, shared_ptr<Material> greyBox,
        shared_ptr<Material> magnetSurface,
        shared_ptr<Material> spacePart) {
    static int name = 0;

    char *str = new char[objectString.length() + 1];
    strcpy(str, objectString.c_str());

    vector<string> elems;
    char *tok = strtok(str, ",");
    while (tok != NULL) {
        elems.push_back(tok);
        tok = strtok(NULL, ",");
    }
    //split(objectString, ',', back_inserter(elems));
    string::size_type sz;
    vec3 pos = vec3(stof(elems[0], &sz), stof(elems[1]), stof(elems[2]));
    vec3 scale = vec3(stof(elems[3]), stof(elems[4]), stof(elems[5]));
    vec3 rot = vec3(stof(elems[6]), stof(elems[7]), stof(elems[8]));
    bool magnetic = toBool(elems[9]);
    bool deadly = toBool(elems[10]);
    bool playerSpawn = toBool(elems[11]);
    bool collectable = toBool(elems[12]);
    //    cerr << "new obj" << endl;
    //    cerr << pos.x << " " << pos.y << endl;
    //    cerr << scale.x << " " << scale.y << endl;

    /*TODO: MAGNETIC AND DEADLY*/
    if (magnetic) {
        shared_ptr<Cuboid> magnet = make_shared<Cuboid>(pos, vec3(0, 0, 0),
                CUBE_HALF_EXTENTS,
                scale, 0, shapes["plainCube"],
                magnetSurface, true);
        objects.push_back(magnet);
        bullet->createMagneticBox(to_string(name++), pos, CUBE_HALF_EXTENTS, scale, 0);
    } else if (deadly) {
        shared_ptr<Cuboid> dobj1 = make_shared<Cuboid>(pos, vec3(0, 0, 0),
                CUBE_HALF_EXTENTS,
                scale, 0, shapes["plainCube"],
                greyBox, false);
        deathObjects.push_back(dobj1);

        //cout << "death box" << endl;
    } else if (collectable) {
        spaceShipPart = make_shared<SpaceShipPart>(pos, vec3(0, 0, 0),
                CUBE_HALF_EXTENTS, scale,
                shapes["shipPart"], spacePart);
    } else {
        shared_ptr<Cuboid> groundPlane = make_shared<Cuboid>(pos, vec3(0, 0, 0),
                CUBE_HALF_EXTENTS,
                scale, 0, shapes["skyscraper"],
                nullptr, false);
        objects.push_back(groundPlane);
        bullet->createBox(to_string(name++), pos, CUBE_HALF_EXTENTS, scale, 0);
    }
}

void GameManager::importLevel(string level) {
    string line;
    ifstream file;
    bullet = make_shared<BulletManager>();
    file.open(RESOURCE_DIR + "levels/" + level);

    shared_ptr<Material> greyBox = make_shared<Material>(vec3(0.9f, 0.9f, 0.9f),
            vec3(1.0f, 1.0f, 1.0f),
            vec3(0.0f, 0.0f, 0.0f),
            200.0f);
    shared_ptr<Material> magnetSurface = make_shared<Material>(vec3(0.2f, 0.2f, 0.2f),
            vec3(1.0f, 0.0f, 0.0f),
            vec3(1.0f, 0.9f, 0.8f),
            200.0f);
    shared_ptr<Material> spacePart = make_shared<Material>(vec3(0.2f, 0.2f, 0.2f),
            vec3(1.0f, 1.0f, 0.0f),
            vec3(1.0f, 0.9f, 0.8f),
            200.0f);

    if (file.is_open()) {
        if (getline(file, line)) {
            /*get camera position*/
            parseCamera(line);
        }
        if (getline(file, line)) {
            /*get spaceship positions*/
            parseObject(line, greyBox, magnetSurface, spacePart);
        }
        while (getline(file, line)) {
            /*objects here*/
            parseObject(line, greyBox, magnetSurface, spacePart);
        }
        cout << "Level '" << level << "' successfully imported." << endl;
        file.close();
    } else {
        cout << "Unable to open level '" << level << "'" << endl;
    }

    //kdtree = make_shared<KDTree>(objects);
    bvh = make_shared<BVH>(objects);
    //bvh->printTree();
}

State GameManager::processInputs() {
    if (gameState == GAME) {
        if (!fmod->isPlaying("game"))
            fmod->playSound("game", true);
        gameState = inputManager->processGameInputs(bullet, fmod);
    } else if (gameState == PAUSE) {
        gameState = inputManager->processPauseInputs(gui, fmod);
    } else if (gameState == MENU) {
        gameState = inputManager->processMenuInputs(gui, fmod);
        if (fmod->isPlaying("game"))
            fmod->stopSound("game");
        if (!fmod->isPlaying("menu"))
            fmod->playSound("menu", true);
        if (gameState == GAME) {
            fmod->stopSound("menu");
            importLevel(to_string(level));
        }
    } else if (gameState == DEATH) {
        gameState = inputManager->processDeathInputs(gui, fmod);
        if (gameState == GAME) {
            importLevel(to_string(level));
        }
    } else if (gameState == WIN) {
        gameState = inputManager->processWinInputs(gui, fmod);
        if (gameState == GAME) {
            level++;
            importLevel(to_string(level));
        } else if (gameState == MENU) {
            fmod->stopSound("menu");
        }
    }

    return gameState;
}

void GameManager::updateGame(double dt) {
    //bullet->rayTrace(camera->getPosition(), camera->getPosition() + (camera->getDirection() * MAGNET_RANGE));
    resolveMagneticInteractions();

    spaceShipPart->update(dt);

    //step the bullet, update test obj
    bullet->step(dt);

    camera->setPosition(bullet->getBulletObjectState("cam"));

    psystem->update(dt, camera->getPosition());

    if (camera->checkForCollision(spaceShipPart)) {
        //cout << "Collision" << endl;
        kdtree = nullptr;
        bvh = nullptr;
        objects.clear();
        deathObjects.clear();
        fmod->playSound("win", false);
        gameState = WIN;
    }
    /*check for collision with death objects*/
    for (unsigned int i = 0; i < deathObjects.size(); i++) {
        if (camera->checkForCollision(deathObjects.at(i))) {
            objects.clear();
            deathObjects.clear();
            fmod->playSound("death", false);
            gameState = DEATH;
        }
    }
}

void GameManager::renderGame(int fps) {
    int objectsDrawn = 0;

    // Clear framebuffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (Keyboard::isPressed(GLFW_KEY_C)) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }

    if (Keyboard::isPressed(GLFW_KEY_L)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Get current frame buffer size.
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    camera->setAspect((float) width / (float) height);
    gui->setWindowSize(width, height);

    // Matrix stacks
    auto P = make_shared<MatrixStack>();
    auto V = make_shared<MatrixStack>();

    /*if in gamestate menu render menu*/
    if (gameState == MENU) {
        //cout << "SHOULD BE HERE" << endl;
        gui->drawMenu();
    }/* else its in pause menu/game*/
    else if (gameState == DEATH) {
        gui->drawDeath();
    } else if (gameState == WIN) {
        gui->drawWin(level);
    } else {
        // Apply camera transforms
        P->pushMatrix();
        camera->applyProjectionMatrix(P);
        V->pushMatrix();
        camera->applyViewMatrix(V);
        
        skybox->render(P, V);

        //lightPos = vec4(camera->getPosition(), 1.0);
        vec3 l = vec3(V->topMatrix() * vec4(lightPos, 0.0));
        
        // Get shadows
        shadowManager->bindDepthMap(l);
        drawScene(P, V, l, true);
        shadowManager->unbindDepthMap();
        glViewport(0, 0, width, height);
        // Clear framebuffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render scene
        drawScene(P, V, l, false);
        
        /*
        // SHADOWS
        

        // Draw ship part
        shipPartProgram->bind();
        shipPartColorTexture->bind(shipPartProgram->getUniform("diffuseTex"));
        shipPartSpecularTexture->bind(shipPartProgram->getUniform("specularTex"));
        glUniformMatrix4fv(shipPartProgram->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(shipPartProgram->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
        glUniform3fv(shipPartProgram->getUniform("lightPos"), 1, value_ptr(vec3(l)));
        spaceShipPart->draw(shipPartProgram);
        shipPartSpecularTexture->unbind();
        shipPartColorTexture->unbind();
        shipPartProgram->unbind();

        // Render skyscrapers
//        skyscraperProgram->bind();
//        glUniformMatrix4fv(skyscraperProgram->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
//        glUniformMatrix4fv(skyscraperProgram->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
//        glUniform3fv(skyscraperProgram->getUniform("lightPos"), 1, value_ptr(vec3(l)));
//        glUniform1f(skyscraperProgram->getUniform("lightIntensity"), lightIntensity);

        vfc->extractVFPlanes(P->topMatrix(), V->topMatrix());
        for (unsigned int i = 0; i < objects.size(); i++) {
            if (objects.at(i)->isCuboid()) {
                std::shared_ptr<Cuboid> cub = dynamic_pointer_cast<Cuboid>(objects.at(i));
                std::vector<vec3> *temp = cub->getAabbMinsMaxs();

                if (!vfc->viewFrustCull(temp)) {
                    objectsDrawn++;
                    if (cub->isMagnetic()) {
                        program->bind();
                        glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
                        glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
                        glUniform4f(program->getUniform("lightPos"), l[0], l[1], l[2], l[3]);
                        glUniform1f(program->getUniform("lightIntensity"), lightIntensity);
                        cub->draw(program);
                        program->unbind();
                    }
                    else {
                        skyscraperProgram->bind();
                        glUniformMatrix4fv(skyscraperProgram->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
                        glUniformMatrix4fv(skyscraperProgram->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
                        glUniform3fv(skyscraperProgram->getUniform("lightPos"), 1, value_ptr(vec3(l)));
                        glUniform1f(skyscraperProgram->getUniform("lightIntensity"), lightIntensity);
                        glUniform3fv(skyscraperProgram->getUniform("scalingFactor"), 1, value_ptr(cub->getScale()));
                        cub->draw(skyscraperProgram);
                        skyscraperProgram->unbind();
                    }
                }

                delete temp;
            }
        }
        
        skyscraperProgram->unbind();
        
//        if (bullet->getDebugFlag()) {
//            //DRAW DEATH OBJECTS
//            for (unsigned int i = 0; i < deathObjects.size(); i++) {
//                deathObjects.at(i)->draw(program);
//
//            }
//        }
        
        // Render magnet gun
        program->bind();
        glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniform4f(program->getUniform("lightPos"), l[0], l[1], l[2], l[3]);
        glUniform1f(program->getUniform("lightIntensity"), lightIntensity);
        V->pushMatrix();
        V->loadIdentity();
        glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
        glClear(GL_DEPTH_BUFFER_BIT);
        magnetGun->draw(program);
        V->popMatrix();

        magnetBeam->draw(program);
        glEnable(GL_DEPTH_TEST);
        program->unbind();

        psystem->draw(V->topMatrix() , P->topMatrix(), 0);
        
        if (bullet->getDebugFlag()) {
            bullet->renderDebug(P->topMatrix(), V->topMatrix());
        }

        V->popMatrix();
        P->popMatrix();
        */

        if (gameState == PAUSE) {
            gui->drawPause(level);
        } else {
            gui->drawHUD(camera->isLookingAtMagnet(), Mouse::isLeftMouseButtonPressed(), Mouse::isRightMouseButtonPressed());
        }
    }
}

void GameManager::drawScene(const shared_ptr<MatrixStack> &P,
                            const shared_ptr<MatrixStack> &V, const vec3 &light,
                            bool shadow) {
    if (shadow) {
        shadowManager->getShadowDepthProgram()->bind();
        // Get ship part
        spaceShipPart->draw(shadowManager->getShadowDepthProgram());
        
        vfc->extractVFPlanes(P->topMatrix(), V->topMatrix());
        for (unsigned int i = 0; i < objects.size(); i++) {
            if (objects.at(i)->isCuboid()) {
                std::shared_ptr<Cuboid> cub = dynamic_pointer_cast<Cuboid>(objects.at(i));
                std::vector<vec3> *temp = cub->getAabbMinsMaxs();
                
                if (!vfc->viewFrustCull(temp)) {
                    cub->draw(shadowManager->getShadowDepthProgram());
                }
                
                delete temp;
            }
        }
        
        // Render magnet gun
        magnetGun->draw(shadowManager->getShadowDepthProgram());
        
        shadowManager->getShadowDepthProgram()->unbind();
    }
    else {
        // Draw ship part
        shipPartProgram->bind();
        shadowManager->addShadowsToScene(shipPartProgram);
        shipPartColorTexture->bind(shipPartProgram->getUniform("diffuseTex"));
        shipPartSpecularTexture->bind(shipPartProgram->getUniform("specularTex"));
        glUniformMatrix4fv(shipPartProgram->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(shipPartProgram->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
        glUniform3fv(shipPartProgram->getUniform("lightPos"), 1, value_ptr(light));
        spaceShipPart->draw(shipPartProgram);
        shipPartSpecularTexture->unbind();
        shipPartColorTexture->unbind();
        shipPartProgram->unbind();
        
        // Render skyscrapers
        //        skyscraperProgram->bind();
        //        glUniformMatrix4fv(skyscraperProgram->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        //        glUniformMatrix4fv(skyscraperProgram->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
        //        glUniform3fv(skyscraperProgram->getUniform("lightPos"), 1, value_ptr(vec3(l)));
        //        glUniform1f(skyscraperProgram->getUniform("lightIntensity"), lightIntensity);
        
        vfc->extractVFPlanes(P->topMatrix(), V->topMatrix());
        for (unsigned int i = 0; i < objects.size(); i++) {
            if (objects.at(i)->isCuboid()) {
                std::shared_ptr<Cuboid> cub = dynamic_pointer_cast<Cuboid>(objects.at(i));
                std::vector<vec3> *temp = cub->getAabbMinsMaxs();
                
                if (!vfc->viewFrustCull(temp)) {
                    //objectsDrawn++;
                    if (cub->isMagnetic()) {
                        program->bind();
                        shadowManager->addShadowsToScene(program);
                        glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
                        glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
                        glUniform3fv(program->getUniform("lightPos"), 1, value_ptr(light));
                        glUniform1f(program->getUniform("lightIntensity"), lightIntensity);
                        cub->draw(program);
                        program->unbind();
                    }
                    else {
                        skyscraperProgram->bind();
                        glUniformMatrix4fv(skyscraperProgram->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
                        glUniformMatrix4fv(skyscraperProgram->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
                        glUniform3fv(skyscraperProgram->getUniform("lightPos"), 1, value_ptr(light));
                        glUniform1f(skyscraperProgram->getUniform("lightIntensity"), lightIntensity);
                        glUniform3fv(skyscraperProgram->getUniform("scalingFactor"), 1, value_ptr(cub->getScale()));
                        cub->draw(skyscraperProgram);
                        skyscraperProgram->unbind();
                    }
                }
                
                delete temp;
            }
        }
        
        skyscraperProgram->unbind();
        
        // Render magnet gun
        program->bind();
        shadowManager->addShadowsToScene(program);
        GLSL::checkError(GET_FILE_LINE);
        glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        GLSL::checkError(GET_FILE_LINE);
        glUniform3fv(program->getUniform("lightPos"), 1, value_ptr(light));
        GLSL::checkError(GET_FILE_LINE);
        glUniform1f(program->getUniform("lightIntensity"), lightIntensity);
        GLSL::checkError(GET_FILE_LINE);
        V->pushMatrix();
        V->loadIdentity();
        glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
        GLSL::checkError(GET_FILE_LINE);
        glClear(GL_DEPTH_BUFFER_BIT);
        GLSL::checkError(GET_FILE_LINE);
        magnetGun->draw(program);
        V->popMatrix();
        
        magnetBeam->draw(program);
        glEnable(GL_DEPTH_TEST);
        program->unbind();
        
        psystem->draw(V->topMatrix() , P->topMatrix(), 0);
        
        if (bullet->getDebugFlag()) {
            bullet->renderDebug(P->topMatrix(), V->topMatrix());
        }
        
        V->popMatrix();
        P->popMatrix();
    }
}

// If the window is resized, capture the new size and reset the viewport

void GameManager::resize_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void GameManager::resolveMagneticInteractions() {
    vec3 startLoc = camera->getPosition();
    vec3 endLoc = startLoc + camera->getDirection() * MAGNET_RANGE;

    //vector<shared_ptr<GameObject>> nearObjs = kdtree->findObjectsIntersectedByRay(startLoc, endLoc);
    //shared_ptr<GameObject> obj = RayTrace::rayTrace(startLoc, endLoc, objects);
    shared_ptr<GameObject> obj = bvh->findClosestHitObject(startLoc, endLoc);
    if (obj && obj->isMagnetic()) {
        camera->setLookingAtMagnet(true);
        if (Mouse::isLeftMouseButtonPressed()) {
            if (!fmod->isPlaying("magnet")) {
                fmod->playSound("magnet", false, 1);
            }
            vec3 dir = normalize(endLoc - startLoc);
            btVector3 bulletDir = btVector3(dir.x, dir.y, dir.z);
            bullet->getBulletObject("cam")->getRigidBody()->setLinearVelocity(bulletDir * MAGNET_STRENGTH);
        } else if (Mouse::isRightMouseButtonPressed()) {
            if (!fmod->isPlaying("magnet")) {
                fmod->playSound("magnet", false, 1);
            }
            vec3 dir = normalize(startLoc - endLoc);
            btVector3 bulletDir = btVector3(dir.x, dir.y, dir.z);
            bullet->getBulletObject("cam")->getRigidBody()->setLinearVelocity(bulletDir * MAGNET_STRENGTH);
        }

    } else {
        camera->setLookingAtMagnet(false);
        if (Mouse::isLeftMouseButtonPressed() || Mouse::isRightMouseButtonPressed()) {
            if (!fmod->isPlaying("click"))
                fmod->playSound("click", false, 1);
        }
    }
}

// Returns a random float between the provided low and high floats.

float GameManager::randFloat(float l, float h) {
    float r = rand() / (float) RAND_MAX;
    return (1.0f - r) * l + r * h;
}
