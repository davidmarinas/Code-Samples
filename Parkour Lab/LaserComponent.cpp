//
//  LaserComponent.cpp
//  Lab10
//
//  Created by David Marinas on 11/9/22.
//

#include "LaserComponent.hpp"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "CollisionComponent.h"
#include "Block.hpp"

LaserComponent::LaserComponent(Actor* owner, bool usesAlpha)
    :MeshComponent(owner,true)
{
    SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Draw(Shader* shader)
{
    if (mMesh)
    {
        for(auto it: ls)
        {
            // Set the world transform
            shader->SetMatrixUniform("uWorldTransform",
                LStoM(it));
            // Set the active texture
            Texture* t = mMesh->GetTexture(mTextureIndex);
            if (t)
            {
                t->SetActive();
            }
            // Set the mesh's vertex array as active
            VertexArray* va = mMesh->GetVertexArray();
            va->SetActive();
            // Draw
            glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
        }
    }
}
void LaserComponent::Update(float deltaTime)
{
    Player* player = mOwner->GetGame()->GetPlayer();
    ls.clear();
    LineSegment first;
    first.mStart = mOwner->GetWorldPosition();
    first.mEnd = mOwner->GetWorldPosition() + mOwner->GetWorldForward() * 500.0f;
    CastInfo info;
    if(SegmentCast(player,first,info))
    {
        first.mEnd = info.mPoint;
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
        player->SetPosition(player->GetRespawnPos());
    }
    else if(SegmentCast(mOwner->GetGame()->GetBlocks(),first,info, mOwner->GetParent())){
        
        first.mEnd = info.mPoint;
        Block* block = static_cast<Block*>(info.mActor);
        LineSegment prev = first;
        while(block->GetMirror())
        {
            Vector3 oldDir = prev.mEnd - prev.mStart;
            Vector3 newDir = Vector3::Reflect(oldDir, info.mNormal);
            LineSegment second;
            second.mStart = prev.mEnd;
            second.mEnd = second.mStart + newDir * 500.0f;
            if(SegmentCast(mOwner->GetGame()->GetBlocks(),second,info,block))
            {
                second.mEnd = info.mPoint;
                block = static_cast<Block*>(info.mActor);
                ls.push_back(second);
                prev = second;
            }
            else{
                break;
            }
        }
    }
    ls.push_back(first);
}

Matrix4 LaserComponent::LStoM(LineSegment segment)
{
    Matrix4 scale = Matrix4::CreateScale(segment.Length(),1.0f,1.0f);
    
    Vector3 newDir = segment.mEnd - segment.mStart;
    Quaternion quarter;
    newDir.Normalize();
    if(Vector3::Dot(Vector3::UnitX,newDir) == 1)
    {
        quarter = Quaternion::Identity;
    }
    else if(Vector3::Dot(Vector3::UnitX,newDir) == -1)
    {
        quarter = Quaternion(Vector3::UnitZ,Math::Pi);
    }
    else{
        Vector3 axis = Vector3::Normalize(Vector3::Cross(Vector3::UnitX,newDir));
        float angle = Math::Acos(Vector3::Dot(Vector3::UnitX,newDir));
        quarter = Quaternion(axis,angle);
    }
    
    Matrix4 rotation = Matrix4::CreateFromQuaternion(quarter);
    Matrix4 translation = Matrix4::CreateTranslation(segment.PointOnSegment(0.5f));
    return scale * rotation * translation;
}
