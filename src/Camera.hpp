//
//  Camera.hpp
//  Lab1
//
//  Created by Cody Rhoads on 5/24/16.
//
//

#ifndef Camera_hpp
#define Camera_hpp

#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

class MatrixStack;
class BoundingSphere;
class GameObject;

class Camera
{
public:
    Camera();
    Camera(int gridSize, GLFWwindow *window);
    virtual ~Camera();
    
    void mouseMoved(double x, double y);
    void interpretPressedKeys(const std::vector<char> &pressedKeys);
    void applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const;
    void applyViewMatrix(std::shared_ptr<MatrixStack> MV) const;
    bool checkForCollision(const std::shared_ptr<GameObject> &otherObj);
    void resolveCollision();
    
    void setAspect(float a) {aspect = a;}
    void setBoundingSphere(const std::shared_ptr<BoundingSphere> &sphere) {boundingSphere = sphere;}
    
    glm::vec3 getPosition() const {return position;}
    glm::vec3 getForward();
    std::shared_ptr<BoundingSphere> getPlayerSphere() const {return boundingSphere;}
private:
    GLFWwindow *window;
    glm::vec3 position;
    glm::vec3 oldPosition;
    float yaw;
    float pitch;
    float aspect;
    float fovy;
    float znear;
    float zfar;
    glm::vec2 mousePrev;
    std::shared_ptr<BoundingSphere> boundingSphere;
};


#endif /* Camera_hpp */
