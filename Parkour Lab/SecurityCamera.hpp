//
//  SecurityCamera.hpp
//  Lab11
//
//  Created by David Marinas on 11/21/22.
//

#ifndef SecurityCamera_hpp
#define SecurityCamera_hpp

#include <stdio.h>
#include "Actor.h"
#include "Math.h"

class SecurityCamera : public Actor {
    public:
        SecurityCamera(class Game* game, Actor* parent);
        ~SecurityCamera();
        void OnUpdate(float deltaTime) override;
        void SetStartQ(Quaternion set) {startQ = set; }
        void SetEndQ(Quaternion set) {endQ = set; }
        void SetInterp(float time) {interpTime = time; }
        void SetPause(float time) {pauseTime = time; }
        void PosSound(int channel);
        
    private:
        class MeshComponent* mc;
        class SecurityCone* cone_;
        Quaternion startQ;
        Quaternion endQ;
        float interpTime;
        float pauseTime;
        float timer = 0.0f;
        float pTimer = 0.0f;
        int rotateChannel = -1;
        int stopChannel = -1;
};
#endif /* SecurityCamera_hpp */
