//
//  HUD.hpp
//  Lab12
//
//  Created by David Marinas on 11/28/22.
//

#ifndef HUD_hpp
#define HUD_hpp

#include <stdio.h>
#include "UIComponent.h"
#include <string>

class HUD : public UIComponent {
    public:
        HUD(class Actor* owner);
        ~HUD();
        void Update(float deltaTime) override;
        void Draw(class Shader* shader) override;
        void CollectedCoin();
        void UpdateCheckpoint(std::string text);
    private:
        class Font* mFont;
        class Texture* mTimerText;
        class Texture* mCoinText;
        class Texture* mCheckpointText;
        class Texture* mRadar;
        class Texture* mRadarArrow;
        class Texture* mBlip;
        bool checkpointSet = false;
};
#endif /* HUD_hpp */
