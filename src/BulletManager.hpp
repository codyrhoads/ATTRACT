/* 
 * File:   BulletManager.h
 * Author: Darryl
 *
 * Created on January 27, 2017, 4:08 PM
 */

#ifndef BULLETMANAGER_HPP
#define BULLETMANAGER_HPP

#include <iostream>
#include <vector>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BulletDebugDraw.hpp"
#include <map>

class BulletObject;

class BulletManager {
public:
    BulletManager();
    virtual ~BulletManager();
    void createPlane(std::string name, double x, double y, double z);
    void createSphere(std::string name, double x, double y, double z, double radius);
    void createBox(std::string name, glm::vec3 loc, glm::vec3 dims,
                   glm::vec3 scale, float mass);
    void createMagneticBox(std::string name, glm::vec3 loc, glm::vec3 dims,
                           glm::vec3 scale, float mass);
    void step(float dt);
    void rayTrace(glm::vec3 startLoc, glm::vec3 endLoc);
    BulletObject* getBulletObject(std::string name);
    glm::vec3 getBulletObjectState(std::string name);
    btDiscreteDynamicsWorld* getDynamicsWorld() {return dynamicsWorld;};
    std::map<std::string, BulletObject*> getBulletObjects() {return bulletObjects;};
    void renderDebug(mat4 p, mat4 v);
    bool getDebugFlag(); 
    void setDebugFlag(bool f);
    void setDebugMode(int mode);
private:
    /*Bullet stuff*/
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    
    std::map<std::string, BulletObject*> bulletObjects;
    std::vector<btRigidBody*> magneticObjects;
    
    btIDebugDraw *debug;
    bool debugFlag;
};

#endif /* BULLETMANAGER_HPP */

