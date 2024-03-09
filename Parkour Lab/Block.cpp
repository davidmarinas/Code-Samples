//
//  Block.cpp
//  Lab09
//
//  Created by David Marinas on 10/27/22.
//

#include "Block.hpp"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"

Block::Block(Game* game, Actor* parent)
: Actor(game, parent) {
    
    SetScale(64.0f);
    MeshComponent* mesh = new MeshComponent(this);
    mc = mesh;
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    CollisionComponent* coll = new CollisionComponent(this);
    collide = coll;
    collide->SetSize(1.0f,1.0f,1.0f);
    
    GetGame()->AddBlock(this);
}

Block::~Block()
{
    GetGame()->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime)
{
    if(rotates)
    {
        SetRotation(mRotation + Math::Pi/4 * deltaTime);
    }
}
