//
//  SecurityCamera.cpp
//  Lab11
//
//  Created by David Marinas on 11/21/22.
//

#include "SecurityCamera.hpp"
#include "SecurityCone.hpp"
#include "Game.h"
#include "Player.hpp"
#include "Checkpoint.hpp"
#include "Renderer.h"
#include "MeshComponent.h"

SecurityCamera::SecurityCamera(Game* game, Actor* parent)
: Actor(game, parent) {
    MeshComponent* mesh = new MeshComponent(this);
    mc = mesh;
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
    SecurityCone* cone = new SecurityCone(mGame,this);
    cone_ = cone;
    mGame->AddCamera(this);
}

SecurityCamera::~SecurityCamera()
{
    mGame->RemoveCamera(this);
    if(rotateChannel != -1)
    {
        Mix_HaltChannel(rotateChannel);
        rotateChannel = -1;
    }
}

void SecurityCamera::OnUpdate(float deltaTime)
{
    
    if(!cone_->GetInside())
    {
        timer+=deltaTime;
        float currPos = Math::Clamp(timer/interpTime,0.0f,1.0f);
        Quaternion pos = Quaternion::Slerp(startQ,endQ,currPos);
        SetQuarternion(pos);
        if(currPos == 1.0f)
        {
            if(rotateChannel != -1)
            {
                Mix_HaltChannel(rotateChannel);
                rotateChannel = -1;
                stopChannel = Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotorStop.wav"),0);
                PosSound(stopChannel);
            }
            pTimer+= deltaTime;
            if(pTimer >= pauseTime)
            {
                timer = 0.0f;
                Quaternion temp = startQ;
                startQ = endQ;
                endQ = temp;
                pTimer = 0.0f;
            }
        }
        else{
            if(rotateChannel == -1)
            {
                rotateChannel = Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotor.wav"),0);
                PosSound(rotateChannel);
            }
            else{
                PosSound(rotateChannel);
            }
        }
    }
    else{
        if(rotateChannel != -1)
        {
            Mix_HaltChannel(rotateChannel);
            rotateChannel = -1;
            Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotorStop.wav"),0);
            PosSound(stopChannel);
        }
    }
}

void SecurityCamera::PosSound(int channel)
{
    Vector3 playerPos = mGame->GetPlayer()->GetPosition();
    Vector3 cameraPos = GetPosition();
    
    if(Vector3::Distance(playerPos, cameraPos) > 1500.0f)
    {
        Mix_Volume(channel, 0);
    }
    else if(Vector3::Distance(playerPos, cameraPos) > 500.0f)
    {
        float vol = Vector3::Distance(playerPos, cameraPos) - 500;
        vol = vol / 1000;
        vol = Math::Lerp(0, 128, 1 - vol);
        Mix_Volume(channel, (int)(vol));
        
    }
    else{
        Mix_Volume(channel, 128);
    }
}
