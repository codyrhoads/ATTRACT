//
//  Skybox.hpp
//  ATTRACT
//
//  Created by Cody Rhoads on 3/6/17.
//
//

#ifndef Skybox_hpp
#define Skybox_hpp

#include <string>
#include <memory>

class MatrixStack;
class TextureCube;
class Program;
class Shape;
class DepthExtractor;

class Skybox
{
public:
    Skybox(const std::string &resourceDir,
           const std::shared_ptr<Shape> &skyShape);
    virtual ~Skybox();
    
    void render(std::shared_ptr<MatrixStack> &P,
                std::shared_ptr<MatrixStack> &V,
                std::shared_ptr<MatrixStack> &prevVP,
                std::shared_ptr<DepthExtractor> &cameraDepth);
private:
    std::shared_ptr<TextureCube> tc;
    std::shared_ptr<Program> prog;
    std::shared_ptr<Shape> shape;
};

#endif /* Skybox_hpp */
