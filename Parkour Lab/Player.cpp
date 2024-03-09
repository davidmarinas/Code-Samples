//
//  Player.cpp
//  Lab07
//
//  Created by David Marinas on 10/18/22.
//

#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "PlayerMove.hpp"
#include "CameraComponent.hpp"
#include "HUD.hpp"

Player::Player(class Game* game, Actor* parent)
:Actor(game, parent)
{
    
    PlayerMove* playerMove = new PlayerMove(this);
    pmove = playerMove;
    
    camera = new CameraComponent(this);
    
    CollisionComponent* coll = new CollisionComponent(this);
    collide = coll;
    collide->SetSize(50.0f, 175.0f, 50.0f);
    hud = new HUD(this);
}
