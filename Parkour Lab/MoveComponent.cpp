#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
    
    mOwner->SetRotation(mOwner->GetRotation() + GetAngularSpeed() * deltaTime);
    
    Vector3 velocity = mOwner->GetForward() * GetForwardSpeed();
    velocity *= deltaTime;
    mOwner->SetPosition( mOwner->GetPosition() + velocity);
    
    mOwner->SetPosition(mOwner->GetPosition() + GetStrafeSpeed() * mOwner->GetRight() * deltaTime);
    
}
