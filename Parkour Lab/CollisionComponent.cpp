#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	offset = Vector3::Zero;
    if(!this->Intersect(other))
    {
        return CollSide::None;
    }
    float rightDist = Math::Abs(other->GetMin().y - this->GetMax().y);
    float leftDist = Math::Abs(other->GetMax().y - this->GetMin().y);
    float frontDist = Math::Abs(other->GetMin().x -this->GetMax().x);
    float backDist = Math::Abs(other->GetMax().x - this->GetMin().x);
    float bottomDist = Math::Abs(other->GetMin().z - this->GetMax().z);
    float topDist = Math::Abs(other->GetMax().z - this->GetMin().z);
    
    float min = Math::Min(Math::Min(Math::Min(Math::Min(Math::Min(frontDist,backDist),topDist),bottomDist),leftDist),rightDist);
    
    if(min == topDist)
    {
        offset.z = other->GetMax().z - this->GetMin().z;
        return CollSide::Top;
    }
    else if(min == bottomDist)
    {
        offset.z = other->GetMin().z - this->GetMax().z;
        return CollSide::Bottom;
    }
    else if(min == leftDist)
    {
        offset.y = other->GetMax().y - this->GetMin().y;
        return CollSide::Left;
    }
    else if(min == rightDist){
        offset.y = other->GetMin().y - this->GetMax().y;
        return CollSide::Right;
    }
    else if(min == frontDist){
        offset.x = other->GetMin().x -this->GetMax().x;
        return CollSide::Front;
    }
    else{
        offset.x = other->GetMax().x - this->GetMin().x;
        return CollSide::Back;
    }
}
