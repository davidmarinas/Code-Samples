//
//  Checkpoint.hpp
//  Lab11
//
//  Created by David Marinas on 11/16/22.
//

#ifndef Checkpoint_hpp
#define Checkpoint_hpp

#include <stdio.h>
#include <string>
#include "Actor.h"

class Checkpoint : public Actor{
    public:
        Checkpoint(class Game* game, Actor* parent);
        void OnUpdate(float deltaTime) override;
        void SetActive(bool result) {active = result;}
        bool IsActive() {return active; }
        void SetLevelString(std::string level) {mLevelString = level; }
        void SetText(std::string text) {mText = text; }
        
    private:
        class MeshComponent* mc;
        class CollisionComponent* collide;
        bool active = false;
        std::string mLevelString;
        std::string mText;
};
#endif /* Checkpoint_hpp */
