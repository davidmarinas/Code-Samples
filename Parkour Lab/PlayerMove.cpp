//
//  PlayerMove.cpp
//  Lab07
//
//  Created by David Marinas on 10/18/22.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include <SDL2/SDL.h>
#include "Random.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "Player.hpp"
#include "CameraComponent.hpp"
#include "Block.hpp"
#include "Checkpoint.hpp"

const float scale = 700.0f;

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
{
    runningChannel = Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
    Mix_Pause(runningChannel);
    ChangeState(Falling);
}

PlayerMove::~PlayerMove()
{
    Mix_HaltChannel(runningChannel);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    if(keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S])
    {
        SetForwardSpeed(0.0f);
    }
    else if(keyState[SDL_SCANCODE_W])
    {
        AddForce(mOwner->GetForward() * scale);
    }
    else if(keyState[SDL_SCANCODE_S])
    {
        AddForce(mOwner->GetForward() * -scale);
    }
    else if(!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S])
    {
        SetForwardSpeed(0.0f);
    }
    
    if(keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A])
    {
        SetStrafeSpeed(0.0f);
    }
    else if(keyState[SDL_SCANCODE_D])
    {
        AddForce(mOwner->GetRight() * scale);
    }
    else if(keyState[SDL_SCANCODE_A])
    {
        AddForce(mOwner->GetRight() * -scale);
    }
    else if(!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A])
    {
        SetStrafeSpeed(0.0f);
    }
    if(keyState[SDL_SCANCODE_SPACE] && mCurrentState == OnGround)
    {
        if(!Pressed)
        {
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
            AddForce(mJumpForce);
            ChangeState(Jump);
        }
    }
    else{
        Pressed = false;
    }
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    float angularSpeed = x / 500.0f * Math::Pi * 10.0f;
    SetAngularSpeed(angularSpeed);
    float pitchSpeed = y / 500.0f * Math::Pi * 10.0f;
    CameraComponent* camera = mOwner->GetGame()->GetPlayer()->GetCamera();
    camera->SetPitchSpeed(pitchSpeed);
}

void PlayerMove::Update(float deltaTime)
{
    if(mCurrentState == OnGround)
    {
        UpdateOnGround(deltaTime);
    }
    else if(mCurrentState == Jump)
    {
        UpdateJump(deltaTime);
    }
    else if(mCurrentState == Falling)
    {
        UpdateFalling(deltaTime);
    }
    else if(mCurrentState == WallClimb)
    {
        UpdateWallClimb(deltaTime);
    }
    else if(mCurrentState == WallRun)
    {
        UpdateWallRun(deltaTime);
    }
    
    Player* player = mOwner->GetGame()->GetPlayer();
    if(mOwner->GetPosition().z < -750.0f)
    {
        mOwner->SetPosition(player->GetRespawnPos());
        mOwner->SetRotation(0.0f);
        mVelocity = Vector3::Zero;
        mPendingForces = Vector3::Zero;
        mCurrentState = Falling;
    }
    
    if((mCurrentState == OnGround && mVelocity.Length() > 50.0f) || mCurrentState == WallClimb || mCurrentState == WallRun)
    {
        Mix_Resume(runningChannel);
    }
    else{
        Mix_Pause(runningChannel);
    }
}

void PlayerMove::UpdateOnGround(float deltaTime)
{
    PhysicsUpdate(deltaTime);
    bool falling = true;
    for(auto i : mOwner->GetGame()->GetBlocks())
    {
        CollisionComponent* block = i->GetComponent<CollisionComponent>();
        CollisionComponent* self = mOwner->GetComponent<CollisionComponent>();
        CollSide result = FixCollision(self, block);
        if(result == CollSide::Top)
        {
            falling = false;
        }
        if(result == CollSide::Left || result == CollSide::Right || result == CollSide::Front || result == CollSide::Back)
        {
            if(CanWallClimb(result))
            {
                mCurrentState = WallClimb;
                mWallClimbTimer = 0.0f;
                break;
            }
        }
    }
    
    if(falling)
    {
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateJump(float deltaTime)
{
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    for(auto i : mOwner->GetGame()->GetBlocks())
    {
        CollisionComponent* block = i->GetComponent<CollisionComponent>();
        CollisionComponent* self = mOwner->GetComponent<CollisionComponent>();
        CollSide result = FixCollision(self, block);
        if(result == CollSide::Bottom)
        {
            mVelocity.z = 0.0f;
        }
        if(result == CollSide::Left || result == CollSide::Right || result == CollSide::Front || result == CollSide::Back)
        {
            if(CanWallClimb(result))
            {
                mCurrentState = WallClimb;
                mWallClimbTimer = 0.0f;
                break;
            }
            else if(CanWallRun(result))
            {
                mCurrentState = WallRun;
                CameraComponent* camera = mOwner->GetGame()->GetPlayer()->GetCamera();
                if(result == CollSide::Right)
                {
                    camera->SetRunSpeed(Math::Pi);
                    frontback = false;
                    camera->isWallRunning(true,false);
                }
                else if(result == CollSide::Left) {
                    camera->SetRunSpeed(-Math::Pi);
                    frontback = false;
                    camera->isWallRunning(true,false);
                }
                else if(result == CollSide::Front)
                {
                    camera->SetRunSpeed(-Math::Pi);
                    frontback = true;
                    camera->isWallRunning(true,true);
                }
                else{
                    camera->SetRunSpeed(Math::Pi);
                    frontback = true;
                    camera->isWallRunning(true,true);
                }
                mWallRunTimer = 0.0f;
                break;
            }
        }
    }
    if(mVelocity.z <= 0.0f)
    {
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateFalling(float deltaTime)
{
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    for(auto i : mOwner->GetGame()->GetBlocks())
    {
        CollisionComponent* block = i->GetComponent<CollisionComponent>();
        CollisionComponent* self = mOwner->GetComponent<CollisionComponent>();
        CollSide result = FixCollision(self, block);
        if(result == CollSide::Top)
        {
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav"), 0);
            mVelocity.z = 0.0f;
            ChangeState(OnGround);
        }
    }
}
CollSide PlayerMove::FixCollision(class CollisionComponent* self, class CollisionComponent* block)
{
    Vector3 offset;
    CollSide result = self->GetMinOverlap(block, offset);
    float normalForce = 700.0f;
    
    if(result != CollSide::None)
    {
        mOwner->SetPosition(mOwner->GetPosition() + offset);
    }
    if(result == CollSide::Left || result == CollSide::Right || result == CollSide::Front || result == CollSide::Back)
    {
        if(result == CollSide::Front)
        {
            AddForce(Vector3(-normalForce,0.0f,0.0f));
        }
        else if(result == CollSide::Back)
        {
            AddForce(Vector3(normalForce,0.0f,0.0f));
        }
        else if(result == CollSide::Left)
        {
            AddForce(Vector3(0.0f,normalForce,0.0f));
        }
        else
        {
            AddForce(Vector3(0.0f,-normalForce,0.0f));
        }
    }
    
    return result;
}

void PlayerMove::PhysicsUpdate(float deltaTime)
{
    mAcceleration = mPendingForces * (1.0f / mMass);
    mVelocity += mAcceleration * deltaTime;
    FixXYVelocity();
    mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
    mOwner->SetRotation(mOwner->GetRotation() + GetAngularSpeed() * deltaTime);
    mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity()
{
    float max_speed = 400.0f;
    float break_factor = 0.9f;
    Vector2 xyVelocity = Vector2(mVelocity.x,mVelocity.y);
    
    if(xyVelocity.Length() > max_speed)
    {
        xyVelocity = Vector2::Normalize(xyVelocity);
        xyVelocity *= max_speed;
    }
    if(mCurrentState == OnGround || mCurrentState == WallClimb)
    {
        if(Math::Abs(mAcceleration.x) < 0.0005f)
        {
            xyVelocity.x *= break_factor;
        }
        if(Math::Abs(mAcceleration.y) < 0.0005f)
        {
            xyVelocity.y *= break_factor;
        }
        if(mAcceleration.x > 0 && xyVelocity.x < 0)
        {
            xyVelocity.x *= break_factor;
        }
        else if(mAcceleration.x < 0 && xyVelocity.x > 0)
        {
            xyVelocity.x *= break_factor;
        }
        if(mAcceleration.y > 0 && xyVelocity.y < 0)
        {
            xyVelocity.y *= break_factor;
        }
        else if(mAcceleration.y < 0 && xyVelocity.y > 0)
        {
            xyVelocity.y *= break_factor;
        }
    }
    mVelocity.x = xyVelocity.x;
    mVelocity.y = xyVelocity.y;
}

void PlayerMove::UpdateWallClimb(float deltaTime)
{
    AddForce(mGravity);
    mWallClimbTimer += deltaTime;
    if(mWallClimbTimer < 0.4f)
    {
        AddForce(mWallForce);
    }
    PhysicsUpdate(deltaTime);
    bool isClimbing = false;
    for(auto i : mOwner->GetGame()->GetBlocks())
    {
        CollisionComponent* block = i->GetComponent<CollisionComponent>();
        CollisionComponent* self = mOwner->GetComponent<CollisionComponent>();
        CollSide result = FixCollision(self, block);
        if(result == CollSide::Left || result == CollSide::Right || result == CollSide::Front || result == CollSide::Back)
        {
            isClimbing = true;
        }
    }
    
    if(!isClimbing || mVelocity.z <= 0.0f)
    {
        mVelocity.z = 0.0f;
        ChangeState(Falling);
    }
}
bool PlayerMove::CanWallClimb(CollSide side)
{
    Vector3 wallNormal3;
    Vector2 wallNormal2;
    Vector2 xyVelocity = Vector2(mVelocity.x,mVelocity.y);
    
    if(side == CollSide::Front)
    {
        wallNormal3.Set(-1.0f,0.0f,0.0f);
        wallNormal2.Set(1.0f,0.0f);
    }
    else if(side == CollSide::Back)
    {
        wallNormal3.Set(1.0f,0.0f,0.0f);
        wallNormal2.Set(-1.0f,0.0f);
    }
    else if(side == CollSide::Left)
    {
        wallNormal3.Set(0.0f,1.0f,0.0f);
        wallNormal2.Set(0.0f,-1.0f);
    }
    else
    {
        wallNormal3.Set(0.0f,-1.0f,0.0f);
        wallNormal2.Set(0.0f,1.0f);
    }
    
    if(Vector3::Dot(wallNormal3, mOwner->GetForward()) < -0.90f)
    {
        if(Vector2::Dot(wallNormal2,xyVelocity) > 0.75f)
        {
            if(Math::Abs(xyVelocity.Length()) > 350.0f)
            {
                return true;
            }
        }
    }
    
    return false;
}

void PlayerMove::UpdateWallRun(float deltaTime)
{
    AddForce(mGravity);
    mWallRunTimer += deltaTime;
    if(mWallRunTimer < 0.4f)
    {
        AddForce(mWallRunForce);
    }
    PhysicsUpdate(deltaTime);
    for(auto i : mOwner->GetGame()->GetBlocks())
    {
        CollisionComponent* block = i->GetComponent<CollisionComponent>();
        CollisionComponent* self = mOwner->GetComponent<CollisionComponent>();
        FixCollision(self, block);
    }
    if(mVelocity.z <= 0.0f)
    {
        mVelocity.z = 0.0f;
        ChangeState(Falling);
        mOwner->GetGame()->GetPlayer()->GetCamera()->isWallRunning(false,frontback);
    }
}

bool PlayerMove::CanWallRun(CollSide side)
{
    Vector3 wallNormal3;
    Vector2 wallNormal2;
    Vector2 xyVelocity = Vector2(mVelocity.x,mVelocity.y);
    
    if(side == CollSide::Front)
    {
        wallNormal3.Set(-1.0f,0.0f,0.0f);
        wallNormal2.Set(1.0f,0.0f);
    }
    else if(side == CollSide::Back)
    {
        wallNormal3.Set(1.0f,0.0f,0.0f);
        wallNormal2.Set(-1.0f,0.0f);
    }
    else if(side == CollSide::Left)
    {
        wallNormal3.Set(0.0f,1.0f,0.0f);
        wallNormal2.Set(0.0f,1.0f);
    }
    else
    {
        wallNormal3.Set(0.0f,-1.0f,0.0f);
        wallNormal2.Set(0.0f,-1.0f);
    }
    
    if(Math::Abs(Vector3::Dot(wallNormal3, mOwner->GetForward())) < 0.7f)
    {
        if(Vector2::Dot(Vector2(mOwner->GetForward().x,mOwner->GetForward().y),xyVelocity) > 0.75f)
        {
            if(Math::Abs(xyVelocity.Length()) > 350.0f)
            {
                return true;
            }
        }
    }
    
    return false;
}
