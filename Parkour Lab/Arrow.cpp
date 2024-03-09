//
//  Arrow.cpp
//  Lab11
//
//  Created by David Marinas on 11/16/22.
//

#include "Arrow.hpp"
#include "Game.h"
#include "Player.hpp"
#include "Checkpoint.hpp"
#include "Renderer.h"
#include "MeshComponent.h"

Arrow::Arrow(Game* game, Actor* parent)
: Actor(game, parent) {
    SetScale(0.15f);
    MeshComponent* mesh = new MeshComponent(this);
    mc = mesh;
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
}

void Arrow::OnUpdate(float deltaTIme)
{
    if(!mGame->MoreCheckpoints())
    {
        SetQuarternion(Quaternion::Identity);
    }
    else{
        Vector3 playerPos = mGame->GetPlayer()->GetPosition();
        Vector3 checkPos = mGame->GetActiveCheckpoint()->GetPosition();
        Vector3 result = checkPos - playerPos;
        result.Normalize();
        Quaternion quarter;
        if(Vector3::Dot(Vector3::UnitX,result) == 1)
        {
            quarter = Quaternion::Identity;
        }
        else if(Vector3::Dot(Vector3::UnitX,result) == -1)
        {
            quarter = Quaternion(Vector3::UnitZ,Math::Pi);
        }
        else{
            Vector3 axis = Vector3::Normalize(Vector3::Cross(Vector3::UnitX,result));
            float angle = Math::Acos(Vector3::Dot(Vector3::UnitX,result));
            quarter = Quaternion(axis,angle);
        }
        SetQuarternion(quarter);
    }
    SetPosition(mGame->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f)));
    
}
