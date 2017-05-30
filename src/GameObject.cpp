//
//  GameObject.cpp
//  Lab1
//
//  Created by Cody Rhoads on 5/19/16.
//
//
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>

#include "GameObject.hpp"
#include "Program.h"
#include "Shape.h"
#include "Material.hpp"
#include "BoundingSphere.hpp"
#include "MatrixStack.h"

using namespace std;
using namespace glm;

GameObject::GameObject() :
position(vec3(0.0f, 0.0f, 0.0f)),
position2(vec3(0.0f, 0.0f, 0.0f)),
moving(false),
speed(0.0f),
scale(vec3(1.0f, 1.0f, 1.0f)),
rotation(vec3(1.0f, 1.0f, 1.0f)),
shape(make_shared<Shape>()),
material(make_shared<Material>())
{

}

GameObject::GameObject(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation,
                       const std::shared_ptr<Shape> &shape,
                       const bool magnetic, const bool deadly, const bool spawnPoint, const bool collectable, const bool light, const bool moving) :
position(position),
position2(vec3(0.0f, 0.0f, 0.0f)),
scale(scale),
rotation(rotation),
shape(shape),
magnetic(magnetic),
deadly(deadly),
spawnPoint(spawnPoint),
collectable(collectable),
light(light),
moving(moving),
speed(0.0f),
selected(false)
{
    material = createMaterial(magnetic, deadly, spawnPoint, collectable, light, moving);
}

GameObject::~GameObject()
{

}

shared_ptr<Material> GameObject::createMaterial(bool magnetic, bool deadly, bool spawnPoint, bool collectable, bool light, bool moving)
{
    if (magnetic && deadly) {
        return make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(1.0f, 0.5f, 1.0f), vec3(1.0f, 0.9f, 0.8f), 200.0f);
    } else if (magnetic) {
        return make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.9f, 0.8f), 200.0f);
    } else if (deadly) {
        return make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(0.0f, 0.5f, 1.0f), vec3(1.0f, 0.9f, 0.8f), 200.0f);
    } else if (spawnPoint) {
        return make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.9f, 0.8f), 200.0f);
    } else if (collectable) {
        return make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.9f, 0.8f), 200.0f);
    } else if (light) {
        return make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.9f, 0.8f), 200.0f);
    } else if (moving) {
        return make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(0.0f, 1.0f, 0.3f), vec3(1.0f, 0.9f, 0.8f), 200.0f);
    } else {
        return make_shared<Material>(vec3(0.9f, 0.9f, 0.9f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), 200.0f);
    }
}

void GameObject::setMagnetic(bool mag) {
    magnetic = mag;
    material = createMaterial(magnetic, deadly, spawnPoint, collectable, light, moving);
}

void GameObject::setDeadly(bool dead) {
    deadly = dead;
    material = createMaterial(magnetic, deadly, spawnPoint, collectable, light, moving);
}

void GameObject::setMoving(bool move) {
    moving = move;
    position2 = vec3(position.x, position.y, position.z);
    material = createMaterial(magnetic, deadly, spawnPoint, collectable, light, moving);
}

void GameObject::setSpeed(float v) {
    speed = v;
}

void GameObject::draw(const shared_ptr<Program> &prog)
{
    material->setUniforms(prog);
    
    auto transMat = make_shared<MatrixStack>();
    transMat->translate(position);
    transMat->rotate(rotation.z, vec3(1, 0, 0));
    transMat->rotate(rotation.y, vec3(0, 1, 0));
    transMat->rotate(rotation.x, vec3(0, 0, 1));
    transMat->scale(scale);
    glUniformMatrix4fv(prog->getUniform("objTransMatrix"), 1, GL_FALSE, value_ptr(transMat->topMatrix()));
    glUniform1i(prog->getUniform("selected"), selected ? 1 : 0);

    shape->draw(prog);

    if (moving) {
        material = make_shared<Material>(vec3(0.2f, 0.2f, 0.2f), vec3(1.0f, 0.8f, 0.8f), vec3(1.0f, 0.9f, 0.8f), 200.0f);

        material->setUniforms(prog);
    
        auto mat = make_shared<MatrixStack>();
        mat->translate(position2);
        mat->rotate(rotation.z, vec3(1, 0, 0));
        mat->rotate(rotation.y, vec3(0, 1, 0));
        mat->rotate(rotation.x, vec3(0, 0, 1));
        mat->scale(scale);
        glUniformMatrix4fv(prog->getUniform("objTransMatrix"), 1, GL_FALSE, value_ptr(mat->topMatrix()));
        glUniform1i(prog->getUniform("selected"), selected ? 1 : 0);

        shape->draw(prog);

        material = createMaterial(magnetic, deadly, spawnPoint, collectable, light, moving);
    }
}

string vecString(vec3 v)
{
    return to_string(v.x) + "," + to_string(v.y) + "," + to_string(v.z);
}

string GameObject::toString()
{
    return vecString(position) + "," + vecString(scale) + "," + vecString(rotation) + "," + to_string(magnetic)
        + "," + to_string(deadly) + "," + to_string(spawnPoint) + "," + to_string(collectable) + "," + to_string(light)
        + "," + to_string(moving) + "," + to_string(speed) + "," + vecString(position2);
}
