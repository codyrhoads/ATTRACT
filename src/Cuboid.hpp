//
//  Cuboid.hpp
//  ATTRACT
//
//  Created by Cody Rhoads on 2/6/17.
//
//

#ifndef Cuboid_hpp
#define Cuboid_hpp

#include "GameObject.hpp"
#include "AABoundingBox.hpp"

#define WAIT_TIME (2.0f)

class Cuboid : public GameObject
{
public:
    Cuboid();
    Cuboid(const glm::vec3 &position, const glm::vec3 &position2, const glm::vec3 &direction,
           const glm::vec3 &halfExtents, const glm::vec3 &scale, float velocity,
           const std::shared_ptr<Shape> &shape,
           const std::shared_ptr<Material> &material, bool magnetic, bool door);
    virtual ~Cuboid();
    
    bool isCollidingWithBoundingSphere(const std::shared_ptr<BoundingSphere> &sphere);
    bool isCollidingWithBoundingBox(const std::shared_ptr<AABoundingBox> &box);
    bool isCollidingWithOtherObject(const std::shared_ptr<GameObject> &otherObj);
    
    bool isIntersectingWithLineSegment(const glm::vec3 &start,
                                       const glm::vec3 &end,
                                       glm::vec3 *closestPointOfIntersection);
    
    bool isCuboid() const {return true;}
    bool isMagnetic() const {return magnetic;}
    bool isDoor() const {return door;}
    bool getCanMove() const {return canMove;}
    void setCanMove(bool b) {canMove = b; waiting = 0;}
    
    void update(float dt);
    glm::vec3 getMins() const {return boundingBox->getMins();}
    glm::vec3 getMaxes() const {return boundingBox->getMaxes();}
    std::vector<glm::vec3> *getAabbMinsMaxs();
protected:
    glm::vec3 position1, position2, dir;
    std::shared_ptr<AABoundingBox> boundingBox;
    bool magnetic, door, canMove;
    float waiting;
};

#endif /* Cuboid_hpp */
