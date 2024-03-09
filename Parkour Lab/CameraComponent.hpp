//
//  CameraComponent.hpp
//  Lab08
//
//  Created by David Marinas on 10/21/22.
//

#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include <stdio.h>
#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
    public:
        CameraComponent(class Actor* owner);
        void Update(float deltaTime) override;
        float GetPitchSpeed() {return mPitchSpeed; }
        void SetPitchSpeed(float speed) {mPitchSpeed = speed; }
        void isWallRunning(bool run, bool side) { wallRun = run; frontBack = side;}
        void SetRunSpeed(float speed) {mRunSpeed = speed; }
    
    private:
        float mPitchAngle = 0.0f;
        float mPitchSpeed = 0.0f;
        float mRunAngle = 0.0f;
        float mRunSpeed = 0.0f;
        bool wallRun = false;
        bool frontBack = false;
};
#endif /* CameraComponent_hpp */
