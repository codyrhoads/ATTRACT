//
//  Mouse.cpp
//  Lab1
//
//  Created by Cody Rhoads on 1/19/17.
//
//

#include "Mouse.hpp"
#include <iostream>

double Mouse::mouseX = 0;
double Mouse::mouseY = 0;
bool Mouse::mouseToggles[2] = {false, false};

// This function is called when the mouse moves
void Mouse::cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
    mouseX = xmouse;
    mouseY = ymouse;
    //std::cout << "(" + std::to_string(xmouse) + ", " + std::to_string(ymouse) + ")" << std::endl;
}

// This function resets the mouse position
void Mouse::resetMouse(GLFWwindow* window, double xmouse, double ymouse)
{
    mouseX = xmouse;
    mouseY = ymouse;
    glfwSetCursorPos(window, mouseX, mouseY);
}

// This function is called when the mouse buttons are pressed.
void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        mouseToggles[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        mouseToggles[button] = false;
    }
}
