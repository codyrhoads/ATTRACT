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
#include <glm/gtc/type_ptr.hpp>

class MatrixStack;
class BoundingSphere;
class GameObject;
class BulletManager;
class BulletSphere;

class Camera
{
public:
    Camera();
    Camera(int gridSize);
    virtual ~Camera();
    
    void mouseMoved(double x, double y);
    void interpretPressedKeys(const std::vector<char> &pressedKeys, BulletManager *bullet);
    void applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const;
    void applyViewMatrix(std::shared_ptr<MatrixStack> MV) const;
    bool checkForCollision(const std::shared_ptr<GameObject> &otherObj);
    void resolveCollision();
    
    void setAspect(float a) {aspect = a;}
    void setBoundingSphere(const std::shared_ptr<BoundingSphere> &sphere) {boundingSphere = sphere;}
    
    void setPosition(glm::vec3 inPos);
    void setLookingAtMagnet(bool newLookingAtMagnet) {lookingAtMagnet = newLookingAtMagnet;}
    
    glm::vec3 getPosition() const {return position;}
    glm::vec3 getDirection() const {return glm::normalize(glm::vec3(cos(pitch) * cos(yaw), sin(pitch), cos(pitch) * cos((3.14/2) - yaw)));}
    std::shared_ptr<BoundingSphere> getPlayerSphere() const {return boundingSphere;}
    bool isLookingAtMagnet() const {return lookingAtMagnet;}
    
private:
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
    bool lookingAtMagnet;
};


#endif /* Camera_hpp */
