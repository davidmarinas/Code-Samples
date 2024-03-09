//
//  SecurityCone.hpp
//  Lab11
//
//  Created by David Marinas on 11/21/22.
//

#ifndef SecurityCone_hpp
#define SecurityCone_hpp

#include <stdio.h>
#include "Actor.h"

class SecurityCone : public Actor {
    public:
        SecurityCone(class Game* game, Actor* parent);
        void OnUpdate(float deltaTime) override;
        bool GetInside() {return inside;}
        
    private:
        class MeshComponent* mc;
        bool inside = false;
        float insideTimer = 0.0f;
        float scaledHeight;
        float deathTimer = 0.0f;
        bool kill = false;
        int securityChannel = -1;
};
#endif /* SecurityCone_hpp */
