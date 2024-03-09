//
//  Coin.hpp
//  Lab11
//
//  Created by David Marinas on 11/16/22.
//

#ifndef Coin_hpp
#define Coin_hpp

#include <stdio.h>
#include "Actor.h"

class Coin : public Actor {
    public:
        Coin(class Game* game, Actor* parent);
        void OnUpdate(float deltaTime) override;
    private:
        class MeshComponent* mc;
        class CollisionComponent* collide;
    
};
#endif /* Coin_hpp */
