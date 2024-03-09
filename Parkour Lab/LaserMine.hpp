//
//  LaserMine.hpp
//  Lab10
//
//  Created by David Marinas on 11/9/22.
//

#ifndef LaserMine_hpp
#define LaserMine_hpp

#include <stdio.h>
#include "Actor.h"

class LaserMine : public Actor {
    public:
    LaserMine(class Game* game, Actor* parent);
    private:
        class MeshComponent* mc;
};
#endif /* LaserMine_hpp */
