//
//  Checkpoint.cpp
//  Lab11
//
//  Created by David Marinas on 11/16/22.
//

#include "Checkpoint.hpp"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Player.hpp"
#include "HUD.hpp"

Checkpoint::Checkpoint(Game* game, Actor* parent)
: Actor(game, parent) {
    
    MeshComponent* mesh = new MeshComponent(this);
    mc = mesh;
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
    CollisionComponent* coll = new CollisionComponent(this);
    collide = coll;
    collide->SetSize(25,25,25);
}

void Checkpoint::OnUpdate(float deltaTIme)
{
    if(active)
    {
        mc->SetTextureIndex(0);
        if(mGame->MoreCheckpoints())
        {
            Player* player = mGame->GetPlayer();
            CollisionComponent* playerColl = player->GetComponent<CollisionComponent>();
            if(collide->Intersect(playerColl))
            {
                Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
                player->SetRespawnPos(GetPosition());
                player->GetHUD()->UpdateCheckpoint(mText);
                mGame->DrawCheckTimer() = true;
                mGame->CheckTimer() = 0.0f;
                if(!mLevelString.empty())
                {
                    mGame->LastCheckText() = mText;
                    mGame->SetLevel(mLevelString);
                }
                mGame->RemoveCheckpoint();
                SetState(ActorState::Destroy);
            }
        }
    }
    else{
        mc->SetTextureIndex(1);
    }
}
