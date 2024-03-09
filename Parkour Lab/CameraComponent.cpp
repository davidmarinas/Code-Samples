//
//  CameraComponent.cpp
//  Lab08
//
//  Created by David Marinas on 10/21/22.
//

#include "CameraComponent.hpp"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"

CameraComponent::CameraComponent(Actor* owner)
: Component(owner)
{
    
}

void CameraComponent::Update(float deltaTime)
{
    Vector3 ownerPos = mOwner->GetPosition();
    Vector3 ownerUp = Vector3::UnitZ;
    
    mPitchAngle += mPitchSpeed * deltaTime;
    mPitchAngle = Math::Clamp(mPitchAngle, -Math::Pi / 4.0f, Math::Pi / 4.0f);
    Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
    Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
    Matrix4 combined  = pitch * yaw;
    Vector3 result = Vector3::Transform(Vector3::UnitX, combined);
    
    Matrix4 run = Matrix4::Identity;
    if(wallRun)
    {
        mRunAngle += mRunSpeed * deltaTime;
        mRunAngle = Math::Clamp(mRunAngle, -Math::Pi / 4.0f, Math::Pi / 4.0f);
        if(frontBack)
        {
            run = Matrix4::CreateRotationY(mRunAngle);
        }
        else{
            run = Matrix4::CreateRotationX(mRunAngle);
        }
    }
    else if(mRunAngle != 0.0f)
    {
        mRunAngle -= mRunSpeed * deltaTime;
        if(Math::Abs(mRunAngle) < 0.2)
        {
            mRunAngle = 0.0f;
        }
        if(frontBack)
        {
            run = Matrix4::CreateRotationY(mRunAngle);
        }
        else{
            run = Matrix4::CreateRotationX(mRunAngle);
        }
    }
    Vector3 runVec = Vector3::Transform(Vector3::UnitZ, run);
    
    Matrix4 viewMatrix = Matrix4::CreateLookAt(ownerPos,ownerPos + result,ownerUp + runVec);
    Game* game = mOwner->GetGame();
    game->GetRenderer()->SetViewMatrix(viewMatrix);
    
}
