//
//  KDTree.hpp
//  ATTRACT
//
//  Created by Cody Rhoads on 2/9/17.
//
//

#ifndef KDTree_hpp
#define KDTree_hpp

#include <vector>
#include <memory>

#include <glm/gtc/type_ptr.hpp>

class GameObject;

struct KDTree
{
public:
    KDTree(const std::vector<std::shared_ptr<GameObject>> &objects);
    virtual ~KDTree();
    
    std::vector<std::shared_ptr<GameObject>> findObjectsNearPoint(glm::vec3 point);
    std::vector<std::shared_ptr<GameObject>> findObjectsIntersectedByRay(glm::vec3 start,
                                                                         glm::vec3 end);
    void printTree();
private:
    struct KDNode;
    
    std::shared_ptr<KDNode> createBranch(int axis,
                                         std::vector<std::shared_ptr<GameObject>> &objects);
    static bool compareByXAxis(const std::shared_ptr<GameObject> &a,
                               const std::shared_ptr<GameObject> &b);
    static bool compareByYAxis(const std::shared_ptr<GameObject> &a,
                               const std::shared_ptr<GameObject> &b);
    static bool compareByZAxis(const std::shared_ptr<GameObject> &a,
                               const std::shared_ptr<GameObject> &b);
    
    std::vector<std::shared_ptr<GameObject>> searchTree(std::shared_ptr<KDNode> node,
                                                        glm::vec3 point);
    std::vector<std::shared_ptr<GameObject>> searchTreeWithRay(std::shared_ptr<KDNode> node,
                                                               glm::vec3 start,
                                                               glm::vec3 end);
    
    std::shared_ptr<KDNode> root;
};

#endif /* KDTree_hpp */
