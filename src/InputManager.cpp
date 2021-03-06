//
//  InputManager.cpp
//  Lab1
//
//  Created by Cody Rhoads on 1/18/17.
//
//

#include <vector>
#include <iostream>

#include "InputManager.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "FmodManager.hpp"

using namespace std;

InputManager::InputManager(shared_ptr<Camera> &camera) :
camera(camera) {

}

InputManager::~InputManager() {

}

State InputManager::processGameInputs(shared_ptr<BulletManager> &bullet,
        shared_ptr<FmodManager> &fmod) {
    // Move mouse.
    camera->mouseMoved(Mouse::getMouseX(), Mouse::getMouseY(), false);

    // Create a vector to hold the keyboard inputs
    vector<char> pressedKeys;

    if (Keyboard::isPressed(GLFW_KEY_W) || Keyboard::isPressed(GLFW_KEY_UP)) {
        pressedKeys.push_back('w');
    }
    if (Keyboard::isPressed(GLFW_KEY_S) || Keyboard::isPressed(GLFW_KEY_DOWN)) {
        pressedKeys.push_back('s');
    }
    if (Keyboard::isPressed(GLFW_KEY_A) || Keyboard::isPressed(GLFW_KEY_LEFT)) {
        pressedKeys.push_back('a');
    }
    if (Keyboard::isPressed(GLFW_KEY_D) || Keyboard::isPressed(GLFW_KEY_RIGHT)) {
        pressedKeys.push_back('d');
    }
    if (Keyboard::isPressed(GLFW_KEY_SPACE)) {
        pressedKeys.push_back(' ');
    }
    if (Keyboard::isPressed(GLFW_KEY_P)) {
        pressedKeys.push_back('p');
    }
    if (Keyboard::isPressed(GLFW_KEY_I)) {
        pressedKeys.push_back('i');
    }
    if (Keyboard::isPressed(GLFW_KEY_O)) {
        pressedKeys.push_back('o');
    }
    if (Keyboard::isPressed(GLFW_KEY_ESCAPE)) {
        if (!fmod->isPlaying("select")) {
            fmod->playSound("select", false, 0.7);
        }
        return PAUSE;
    }

    camera->interpretPressedKeys(pressedKeys, bullet, fmod);
    return GAME;
}

State InputManager::processCutsceneInputs(shared_ptr<BulletManager> &bullet,
        shared_ptr<FmodManager> &fmod, shared_ptr<GameObject> obj, State gameState, shared_ptr<GuiManager> &gui) {
    //camera->mouseMoved(Mouse::getMouseX(), Mouse::getMouseY(), true);

    vec3 old = obj->getPosition();
    float movement = 0.01f;
    if (Keyboard::isPressed(GLFW_KEY_Y)) {
        old.x += movement;
    }
    if (Keyboard::isPressed(GLFW_KEY_U)) {
        old.x -= movement;
    }
    if (Keyboard::isPressed(GLFW_KEY_H)) {
        old.y +=movement;
    }
    if (Keyboard::isPressed(GLFW_KEY_J)) {
        old.y -= movement;
    }
    if (Keyboard::isPressed(GLFW_KEY_B)) {
        old.z += movement;
    }
    if (Keyboard::isPressed(GLFW_KEY_N)) {
        old.z -= movement;
    }
    
    if (Keyboard::isPressed(GLFW_KEY_SPACE)) {
        if (gameState == CUTSCENE_START)
            return GAME;
        else
            return MENU;
    }
    //cout << old.x << " " << old.y << " " << old.z << endl;
    //obj->setPosition(old);
    return gameState;
}

State InputManager::processMenuInputs(shared_ptr<GuiManager> &gui,
        shared_ptr<FmodManager> &fmod) {

    vector<char> pressedKeys;

    if (Keyboard::isPressed(GLFW_KEY_W) || Keyboard::isPressed(GLFW_KEY_UP)) {
        pressedKeys.push_back('w');
        if (!fmod->isPlaying("select")) {
            fmod->playSound("select", false, 0.7);
        }

    }
    if (Keyboard::isPressed(GLFW_KEY_S) || Keyboard::isPressed(GLFW_KEY_DOWN)) {
        pressedKeys.push_back('s');
        if (!fmod->isPlaying("select")) {
            fmod->playSound("select", false, 0.7);
        }
    }

    if (Keyboard::isPressed(GLFW_KEY_ENTER)) {
        pressedKeys.push_back('\n');
        if (!fmod->isPlaying("choose")) {
            fmod->playSound("choose", false, 1);
        }
    }
    return gui->interpretMenuPressedKeys(pressedKeys);

}

State InputManager::processPauseInputs(shared_ptr<GuiManager> &gui,
        shared_ptr<FmodManager> &fmod) {
    vector<char> pressedKeys;
    if (!Keyboard::isPressed(GLFW_KEY_ESCAPE)) {
        pressedKeys.push_back('<');
        if (!fmod->isPlaying("choose")) {
            fmod->playSound("choose", false, 1);
        }
    }
    if (Keyboard::isPressed(GLFW_KEY_ENTER)) {
        pressedKeys.push_back('\n');
        if (!fmod->isPlaying("choose")) {
            fmod->playSound("choose", false, 1);
        }
    }
    return gui->interpretPausePressedKeys(pressedKeys);
}

State InputManager::processDeathInputs(shared_ptr<GuiManager> &gui,
        shared_ptr<FmodManager> &fmod) {

    vector<char> pressedKeys;

    if (Keyboard::isPressed(GLFW_KEY_W) || Keyboard::isPressed(GLFW_KEY_UP)) {
        pressedKeys.push_back('w');
        if (!fmod->isPlaying("select")) {
            fmod->playSound("select", false, 0.7);
        }
    }
    if (Keyboard::isPressed(GLFW_KEY_S) || Keyboard::isPressed(GLFW_KEY_DOWN)) {
        pressedKeys.push_back('s');
        if (!fmod->isPlaying("select")) {
            fmod->playSound("select", false, 0.7);
        }
    }

    if (Keyboard::isPressed(GLFW_KEY_ENTER)) {
        pressedKeys.push_back('\n');
        if (!fmod->isPlaying("choose")) {
            fmod->playSound("choose", false, 1);
        }
    }
    return gui->interpretDeathPressedKeys(pressedKeys);

}

State InputManager::processWinInputs(shared_ptr<GuiManager> &gui,
        shared_ptr<FmodManager> &fmod) {

    vector<char> pressedKeys;

    if (Keyboard::isPressed(GLFW_KEY_W) || Keyboard::isPressed(GLFW_KEY_UP)) {
        pressedKeys.push_back('w');
        if (!fmod->isPlaying("select")) {
            fmod->playSound("select", false, 0.7);
        }

    }
    if (Keyboard::isPressed(GLFW_KEY_S) || Keyboard::isPressed(GLFW_KEY_DOWN)) {
        pressedKeys.push_back('s');
        if (!fmod->isPlaying("select")) {
            fmod->playSound("select", false, 0.7);
        }
    }

    if (Keyboard::isPressed(GLFW_KEY_ENTER)) {
        pressedKeys.push_back('\n');
        if (!fmod->isPlaying("choose")) {
            fmod->playSound("choose", false, 0.7);
        }
    }
    return gui->interpretWinPressedKeys(pressedKeys);

}
