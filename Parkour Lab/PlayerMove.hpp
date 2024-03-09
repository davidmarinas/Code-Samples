//
//  PlayerMove.hpp
//  Lab07
//
//  Created by David Marinas on 10/18/22.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Math.h"
#include <vector>


class PlayerMove: public MoveComponent
{
    public:
        enum MoveState {
            OnGround,
            Jump,
            Falling,
            WallClimb,
            WallRun
        };
        PlayerMove(class Actor* actor);
        ~PlayerMove();
        void ProcessInput(const Uint8* keyState) override;
        void Update(float deltaTime) override;
        void ChangeState(MoveState state) {mCurrentState = state; }
        void UpdateOnGround(float deltaTime);
        void UpdateJump(float deltaTime);
        void UpdateFalling(float deltaTime);
        void UpdateWallClimb(float deltaTime);
        void UpdateWallRun(float deltaTime);
        void PhysicsUpdate(float deltaTime);
        void AddForce(const Vector3& force) {mPendingForces += force; }
        void FixXYVelocity();
        bool CanWallClimb(CollSide side);
        bool CanWallRun(CollSide side);
    
    private:
        MoveState mCurrentState;
        Vector3 mVelocity;
        Vector3 mAcceleration;
        Vector3 mPendingForces;
        float mMass = 1.0f;
        Vector3 mGravity = Vector3(0.0f,0.0f,-980.0f);
        Vector3 mJumpForce = Vector3(0.0f,0.0f,35000.0f);
        Vector3 mWallForce = Vector3(0.0f, 0.0f, 1800.0f);
        Vector3 mWallRunForce = Vector3(0.0f, 0.0f, 1200.0f);
        bool Pressed = false;
        float mWallClimbTimer = 0.0f;
        float mWallRunTimer = 0.0f;
        bool frontback = false;
        int runningChannel;
    protected:
        CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
};

#endif /* PlayerMove_hpp */
