//
//  SecurityCone.cpp
//  Lab11
//
//  Created by David Marinas on 11/21/22.
//

#include "SecurityCone.hpp"
#include "Game.h"
#include "Player.hpp"
#include "Checkpoint.hpp"
#include "Renderer.h"
#include "MeshComponent.h"
#include <iostream>

const float camAngle = 30.0f;
const float camHeight = 100.0f;


SecurityCone::SecurityCone(Game* game, Actor* parent)
: Actor(game, parent) {
    MeshComponent* mesh = new MeshComponent(this,true);
    mc = mesh;
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));
    SetPosition(Vector3(30.0f,0.0f,0.0f));
    SetScale(3.0f);
    scaledHeight = mScale * camHeight;
    
}

void SecurityCone::OnUpdate(float deltaTime)
{
    Player* player = mGame->GetPlayer();
    Vector3 dist = player->GetPosition() - GetWorldPosition();
    float distDot = Vector3::Dot(dist,GetWorldForward());
    dist.Normalize();
    float angleDot = Vector3::Dot(dist,GetWorldForward());
    float angle = Math::Acos(angleDot);
    
    if(Math::ToDegrees(angle) <= camAngle && distDot <= scaledHeight && distDot >= 0.0f)
    {
        inside = true;
        insideTimer += deltaTime;
    }
    else{
        inside = false;
        insideTimer = 0.0f;
    }
    if(kill)
    {
        deathTimer += deltaTime;
        if(deathTimer >= 0.5f)
        {
            kill = false;
            deathTimer = 0.0f;
            player->SetPosition(player->GetRespawnPos());
        }
    }
    else if(inside)
    {
        if(insideTimer >= 2.0f)
        {
            mc->SetTextureIndex(2);
            kill = true;
        }
        else{
            if(securityChannel == -1)
            {
                securityChannel = Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/SecurityDetected.wav"),0);
            }
            mc->SetTextureIndex(1);
        }
    }
    else{
        if(securityChannel != -1)
        {
            Mix_HaltChannel(securityChannel);
            securityChannel = -1;
        }
        mc->SetTextureIndex(0);
    }
}
