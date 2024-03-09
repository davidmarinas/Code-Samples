//
//  Block.hpp
//  Lab09
//
//  Created by David Marinas on 10/27/22.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"

class Block : public Actor {
    
public:
    Block(class Game* game, Actor* parent);
    ~Block();
    void SetMirror(bool set) {mirror = set; }
    bool GetMirror() {return mirror; }
    bool GetRotate() {return rotates; }
    void SetRotate(bool result) { rotates = result; }
    void OnUpdate(float deltaTime) override;
    
private:
    class MeshComponent* mc;
    class CollisionComponent* collide;
    bool mirror = false;
    bool rotates = false;
};
#endif /* Block_hpp */
