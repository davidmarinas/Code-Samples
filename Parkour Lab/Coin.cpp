//
//  Coin.cpp
//  Lab11
//
//  Created by David Marinas on 11/16/22.
//

#include "Coin.hpp"
#include "Game.h"
#include "Player.hpp"
#include "Checkpoint.hpp"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "HUD.hpp"

Coin::Coin(Game* game, Actor* parent)
: Actor(game, parent) {
    
    MeshComponent* mesh = new MeshComponent(this);
    mc = mesh;
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
    
    CollisionComponent* coll = new CollisionComponent(this);
    collide = coll;
    collide->SetSize(100.0f,100.0f,100.0f);
}

void Coin::OnUpdate(float deltaTIme)
{
    SetRotation(mRotation + Math::Pi * deltaTIme);
    CollisionComponent* player = mGame->GetPlayer()->GetComponent<CollisionComponent>();
    
    if(collide->Intersect(player))
    {
        Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Coin.wav"), 0);
        mGame->GetPlayer()->GetHUD()->CollectedCoin();
        SetState(ActorState::Destroy);
    }
}
