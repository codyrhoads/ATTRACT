//
//  GameObject.hpp
//  Lab1
//
//  Created by Cody Rhoads on 5/19/16.
//
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <memory>

class Shape;
class Program;
class Material;

class GameObject
{
public:
    GameObject();
    GameObject(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation,
               const std::shared_ptr<Shape> &shape,
               const bool magnetic, const bool deadly, const bool spawnPoint, const bool collectable, const bool light, const bool moving, const bool door);
    virtual ~GameObject();

    std::shared_ptr<Material> createMaterial(bool magnetic, bool deadly, bool spawnPoint, bool collectable, bool light, bool moving, bool door);
    void draw(const std::shared_ptr<Program> &prog);

    glm::vec3 getPosition() const {return position;}
    void setPosition(glm::vec3 pos) {position = pos;}
    glm::vec3 getPosition2() const {return position2;}
    void setPosition2(glm::vec3 pos) {position2 = pos;}
    glm::vec3 getScale() const {return scale;}
    void setScale(glm::vec3 s) {scale = s;}
    glm::vec3 getRotation() const {return rotation;}
    void setRotation(glm::vec3 r) {rotation = r;}
    std::string toString();
    void setSelected(bool s) {selected = s;}
    bool getMagnetic() const {return magnetic;}
    void setMagnetic(bool mag);
    bool getDeadly() const {return deadly;}
    void setDeadly(bool dead);
    bool getDoor() const {return door;}
    void setDoor(bool isDoor);
    bool getMoving() const {return moving;}
    void setMoving(bool move);
    float getSpeed() const {return speed;}
    void setSpeed(float v);
private:
    glm::vec3 position, scale, rotation, position2;
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    bool magnetic, deadly, spawnPoint, collectable, light, selected, moving, door;
    float speed;
};

#endif /* GameObject_hpp */
