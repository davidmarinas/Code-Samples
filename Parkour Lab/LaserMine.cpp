//
//  LaserMine.cpp
//  Lab10
//
//  Created by David Marinas on 11/9/22.
//

#include "LaserMine.hpp"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "LaserComponent.hpp"

LaserMine::LaserMine(Game* game, Actor* parent)
: Actor(game, parent) {
    
    MeshComponent* mesh = new MeshComponent(this);
    mc = mesh;
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
    
    new LaserComponent(this);
}
