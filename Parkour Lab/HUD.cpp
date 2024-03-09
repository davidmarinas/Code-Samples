//
//  HUD.cpp
//  Lab12
//
//  Created by David Marinas on 11/28/22.
//

#include "HUD.hpp"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Font.h"
#include "SecurityCamera.hpp"
#include "Player.hpp"

float radarRange = 1500.0f;
float radarRadius = 92.0f;

HUD::HUD(Actor* owner)
: UIComponent(owner) {
    mFont = new Font();
    mFont->Load("Assets/Inconsolata-Regular.ttf");
    mTimerText = mFont->RenderText("00:00.00");
    mCoinText = mFont->RenderText("00/55");
    if(mOwner->GetGame()->Coin() != 0)
    {
        mOwner->GetGame()->Coin()--;
        CollectedCoin();
    }
    if(mOwner->GetGame()->NotFirstLevel())
    {
        UpdateCheckpoint(mOwner->GetGame()->LastCheckText());
    }
    mRadar = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
    mRadarArrow = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
    mBlip = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");
}

HUD::~HUD()
{
    mFont->Unload();
    delete mFont;
}

void HUD::Draw(class Shader* shader)
{
    DrawTexture(shader, mTimerText, Vector2(-420.0f, -325.0f));
    DrawTexture(shader, mCoinText, Vector2(-442.0f, -295.0f));
    if(checkpointSet)
    {
        DrawTexture(shader, mCheckpointText, Vector2::Zero);
    }
    DrawTexture(shader, mRadar, Vector2(400.0f, -280.0f));
    DrawTexture(shader, mRadarArrow, Vector2(400.0f, -280.0f));
    
    Player* player = mOwner->GetGame()->GetPlayer();
    Vector2 playerPos = Vector2(player->GetPosition().y,player->GetPosition().x);
    Vector2 playerForward = Vector2(player->GetForward().x,player->GetForward().y);
    
    float angle = Math::Atan2(playerForward.y,playerForward.x);
    Matrix3 rotation = Matrix3::CreateRotation(angle);
    
    
    for(auto it: mOwner->GetGame()->GetCameras())
    {
        Vector2 objectPos = Vector2(it->GetPosition().y,it->GetPosition().x);
        Vector2 objectForward = Vector2(it->GetWorldForward().y,it->GetWorldForward().x);
        Vector2 result = objectPos - playerPos;
        
        if(Vector2::Distance(objectPos,playerPos) < radarRange)
        {
            result = result  * (1/radarRange);
            result = result * radarRadius;
            result = Vector2::Transform(result, rotation);
            objectForward = Vector2::Transform(objectForward, rotation);
            float angleFix = Math::Atan2(objectForward.y, objectForward.x);
            DrawTexture(shader, mBlip, Vector2(400.0f + result.x, -280.0f + result.y),1.0f,angleFix);
        }
    }
}

void HUD::Update(float deltaTime)
{
    float& timer = mOwner->GetGame()->Timer();
    timer += deltaTime;
    mTimerText->Unload();
    delete mTimerText;
    int minutes = (int)(timer) / 60;
    std::string min;
    if(minutes < 10)
    {
        min = "0" + std::to_string(minutes);
    }
    else{
        min = std::to_string(minutes);
    }
    
    
    int seconds = (int)(timer) % 60;
    std::string secs;
    if(seconds < 10)
    {
        secs = "0" + std::to_string(seconds);
    }
    else{
        secs = std::to_string(seconds);
    }
    int milliseconds = (int)(timer * 100) % 100;
    std::string milli;
    if(milliseconds < 10)
    {
        milli = "0" + std::to_string(milliseconds);
    }
    else{
        milli = std::to_string(milliseconds);
    }
    
    std::string results = min + ":" + secs + "." + milli;
    mTimerText = mFont->RenderText(results);
    
    if(mOwner->GetGame()->DrawCheckTimer())
    {
        mOwner->GetGame()->CheckTimer()+= deltaTime;
    }
    if(mOwner->GetGame()->CheckTimer() > 5.0f)
    {
        mOwner->GetGame()->DrawCheckTimer() = false;
        mOwner->GetGame()->CheckTimer() = 0.0f;
        mCheckpointText->Unload();
        delete mCheckpointText;
        checkpointSet = false;
    }
    
}

void HUD::CollectedCoin()
{
    mCoinText->Unload();
    delete mCoinText;
    int& coin = mOwner->GetGame()->Coin();
    coin++;
    std::string coin_;
    if(coin < 10)
    {
        coin_ = "0" + std::to_string(coin);
    }
    else{
        coin_ = std::to_string(coin);
    }
    
    mCoinText = mFont->RenderText(coin_ + "/55");
}

void HUD::UpdateCheckpoint(std::string text)
{
    checkpointSet = true;
    mCheckpointText = mFont->RenderText(text);
}
