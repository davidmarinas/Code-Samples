#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
    ,mParent(parent)
{
    if(mParent == nullptr)
    {
        mGame->AddActor(this);
    }
    else{
        mParent->AddChild(this);
    }
}

Actor::~Actor()
{
    while (!mChildren.empty())
    {
        delete mChildren.back();
    }
    
    if(mParent == nullptr)
    {
        mGame->RemoveActor(this);
    }
    else{
        mParent->RemoveChild(this);
    }
    for(auto it : mComponents)
    {
        delete it;
    }
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    CalcWorldTransform();
    if(mState == ActorState::Active)
    {
        for(auto it : mComponents)
        {
            it->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
    CalcWorldTransform();
    for(auto it : mChildren)
    {
        it->Update(deltaTime);
    }
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
    if(mState == ActorState::Active)
    {
        for(auto it : mComponents)
        {
            it->ProcessInput(keyState);
        }
        OnProcessInput(keyState);
    }
}

void Actor::OnProcessInput(const Uint8* keyState)
{
    
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

Vector3 Actor::GetQuatForward()
{
    Vector3 rotated = Vector3::Transform(Vector3::UnitX,mQuarter);
    return Vector3::Normalize(rotated);
}

void Actor::CalcWorldTransform()
{
    Matrix4 scale =  Matrix4::CreateScale(mScale);
    Matrix4 rotateZ = Matrix4::CreateRotationZ(mRotation);
    Matrix4 translate = Matrix4::CreateTranslation(mPosition);
    Matrix4 quarter = Matrix4::CreateFromQuaternion(mQuarter);
    mWorldTransform = scale * rotateZ * quarter * translate;
    
    if(mParent != nullptr)
    {
        if(mInheritScale)
        {
            mWorldTransform *= mParent->GetWorldTransform();
        }
        else{
            mWorldTransform *= mParent->GetWorldRotTrans();
        }
    }
}

Matrix4 Actor::GetWorldRotTrans()
{
    Matrix4 rotateZ = Matrix4::CreateRotationZ(mRotation);
    Matrix4 translate = Matrix4::CreateTranslation(mPosition);
    Matrix4 quarter = Matrix4::CreateFromQuaternion(mQuarter);
    Matrix4 temp = rotateZ * quarter * translate;
    if(mParent != nullptr)
    {
        temp *= mParent->GetWorldRotTrans();
    }
    return temp;
}

void Actor::RemoveChild(Actor* child)
{
    auto iter = std::find(mChildren.begin(), mChildren.end(), child);
    if (iter != mChildren.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        auto iter2 = mChildren.end() - 1;
        std::iter_swap(iter, iter2);
        mChildren.pop_back();
    }
}
