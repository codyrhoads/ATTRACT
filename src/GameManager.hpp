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

class InputManager;
class Camera;
class Program;
class Shape;
class GameObject;
class Texture;

class GameManager
{
public:
    GameManager(GLFWwindow *window, const std::string &resourceDir, const std::string &levelDir, const std::string &level);
    virtual ~GameManager();
    
    void initScene();
    void processInputs();
    void updateGame(double dt);
    void renderGame(int fps);
    static void resize_callback(GLFWwindow *window, int width, int height);
    
    int getNumObjCollected() const {return numObjCollected;}
    int getNumObj() const {return objects.size();}
private:
    std::shared_ptr<GameObject> createObject(bool magnetic, bool deadly, bool spawnPoint, bool collectable, bool light, bool moving, int shape);
    std::shared_ptr<GameObject> createObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, bool magnetic, bool deadly, bool spawnPoint, bool collectable, bool light, bool moving, int shape);
    void importLevel(std::string level);
    std::shared_ptr<GameObject> parseObject(std::string objectString);
    void printStringToScreen(float x, float y, const std::string &text, float r, float g, float b);
    float randFloat(float l, float h);
    
    GLFWwindow *window;
    std::string RESOURCE_DIR, LEVEL_DIR, level;
    
    std::vector<std::shared_ptr<GameObject>> objects;
    std::shared_ptr<GameObject> playerSpawn, spaceshipPart, light, tempObject;
    double objIntervalCounter;
    int numObjCollected;
    int gameWon;
    
    std::shared_ptr<InputManager> inputManager;
    std::shared_ptr<Camera> camera;
    
    std::shared_ptr<Program> program, ground;
    std::vector<std::shared_ptr<Shape>> shapes;
    glm::vec4 lightPos;
    float lightIntensity;
    std::shared_ptr<Texture> grass;
    std::vector<float> grassPosBuf;
    std::vector<float> grassTexBuf;
    std::vector<unsigned int> grassIndBuf;
    std::map<std::string,GLuint> grassBufIDs;
    int grassIndCount;
    bool objectPlacement, setSpawn, setCollectable, setLight;
    unsigned int currentObject;
    int currentAxis;
    bool night;
};

#endif /* GameManager_hpp */
