//
//  Player.hpp
//  Lab07
//
//  Created by David Marinas on 10/18/22.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Actor.h"
class Player : public Actor
{
    public:
        Player(class Game* game, Actor* parent);
        class PlayerMove* GetPMove() {return pmove;}
        class CameraComponent* GetCamera() {return camera; }
        void SetRespawnPos(Vector3 pos) {respawn = pos;}
        Vector3 GetRespawnPos() {return respawn;}
        class HUD* GetHUD() {return hud;}
    
    private:
        class PlayerMove* pmove;
        class CameraComponent* camera;
        class CollisionComponent* collide;
        class HUD* hud;
        Vector3 respawn;
};
#endif /* Player_hpp */
