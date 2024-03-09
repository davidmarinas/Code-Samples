#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include "Math.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
#ifdef __EMSCRIPTEN__
    void EmRunIteration()
    {
        if (!mIsRunning)
        {
            emscripten_cancel_main_loop();
        }
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
#endif
    void Shutdown();
    
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    
    Mix_Chunk* GetSound(const std::string& fileName);
    
    class Renderer* GetRenderer() {	return mRenderer; }
    const float WINDOW_WIDTH = 1024.0f;
    const float WINDOW_HEIGHT = 768.0f;
    
    void SetPlayer(class Player* player) {mPlayer = player;}
    class Player* GetPlayer() {return mPlayer;}
    void AddBlock(class Actor* actor);
    void RemoveBlock(class Actor* actor);
    std::vector<class Actor*> GetBlocks() {return mBlocks;}
    void AddCheckpoint(class Checkpoint* point) {mCheckpoints.push(point); }
    void RemoveCheckpoint() {mCheckpoints.pop();}
    class Checkpoint* GetActiveCheckpoint() {return mCheckpoints.front();}
    bool MoreCheckpoints() {return !mCheckpoints.empty(); }
    void SetLevel(std::string level) { mNextLevel = level; }
    float& Timer() {return timer;}
    float& CheckTimer() {return checkTimer;}
    bool& DrawCheckTimer() {return drawCheckText;}
    int& Coin() {return coin;}
    std::string& LastCheckText() {return lastCheckText;}
    bool& NotFirstLevel() {return notFirstLevel;}
    void AddCamera(class SecurityCamera* camera);
    void RemoveCamera(class SecurityCamera* camera);
    std::vector<class SecurityCamera*> GetCameras() {return mSCameras;}

    
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();
    void LoadNextLevel();
    
    std::unordered_map<std::string, Mix_Chunk*> mSounds;
    
    // All the actors in the game
    std::vector<class Actor*> mActors;
    
    class Renderer* mRenderer = nullptr;
    
    Uint32 mTicksCount = 0;
    bool mIsRunning;
    class Player* mPlayer;
    std::vector<class Actor*> mBlocks;
    std::queue<class Checkpoint*> mCheckpoints;
    std::string mNextLevel;
    float timer = 0;
    int coin = 0;
    float checkTimer = 0;
    bool drawCheckText = false;
    std::string lastCheckText;
    bool notFirstLevel = false;
    std::vector<class SecurityCamera*> mSCameras;
};
