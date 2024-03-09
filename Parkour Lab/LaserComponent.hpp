//
//  LaserComponent.hpp
//  Lab10
//
//  Created by David Marinas on 11/9/22.
//

#ifndef LaserComponent_hpp
#define LaserComponent_hpp

#include <stdio.h>
#include "MeshComponent.h"
#include "Math.h"
#include "SegmentCast.h"

class LaserComponent : public MeshComponent{
    public:
        LaserComponent(class Actor* owner, bool usesAlpha = false);
        void Draw(class Shader* shader) override;
        void Update(float deltaTime) override;
        Matrix4 LStoM(LineSegment segment);
    private:
        std::vector<LineSegment> ls;
};
#endif /* LaserComponent_hpp */
