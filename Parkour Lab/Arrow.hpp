//
//  Arrow.hpp
//  Lab11
//
//  Created by David Marinas on 11/16/22.
//

#ifndef Arrow_hpp
#define Arrow_hpp

#include <stdio.h>
#include "Actor.h"

class Arrow : public Actor {
    public:
        Arrow(class Game* game, Actor* parent);
        void OnUpdate(float deltaTime) override;
        
    private:
        class MeshComponent* mc;
};
#endif /* Arrow_hpp */
