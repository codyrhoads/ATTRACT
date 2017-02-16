//
//  GameManager.hpp
//  Lab1
//
//  Created by Cody Rhoads on 1/17/17.
//
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <string>
#include <memory>
#include <vector>
#include <map>

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.hpp"

class InputManager;
class Camera;
class Program;
class Shape;
class GameObject;
class Texture;
class BulletManager;
class VfcManager;
class FmodManager;
class SpaceShipPart;

class GuiManager;

enum State {GAME, MENU, PAUSE};

struct KDTree;

class GameManager
{
public:
    GameManager(GLFWwindow *window, const std::string &resourceDir);
    virtual ~GameManager();
    
    void initScene();
    State processInputs();
    void updateGame(double dt);
    void renderGame(int fps);
    static void resize_callback(GLFWwindow *window, int width, int height);
    
    State getState() const {return gameState;}
    int getNumObjCollected() const {return numObjCollected;}
    int getNumObj() const {return objects.size();}
private:
    void createLevel();
    void resolveMagneticInteractions();
    void printStringToScreen(float x, float y, const std::string &text, float r, float g, float b);
    float randFloat(float l, float h);
    
    GLFWwindow *window;
    std::string RESOURCE_DIR;
    State gameState;
    
    BulletManager *bullet;
    VfcManager *vfc;
    FmodManager *fmod;

    GuiManager *gui;
        
    std::shared_ptr<KDTree> kdtree;
    

    std::vector<std::shared_ptr<GameObject>> objects;

    double objIntervalCounter;
    int numObjCollected;
    int gameWon;
    
    std::shared_ptr<InputManager> inputManager;
    std::shared_ptr<Camera> camera;
    
    std::shared_ptr<Program> program;
    std::vector<std::shared_ptr<Shape>> shapes;
    
    std::shared_ptr<SpaceShipPart> spaceShipPart;

    glm::vec4 lightPos;
    float lightIntensity;

};

#endif /* GameManager_hpp */
