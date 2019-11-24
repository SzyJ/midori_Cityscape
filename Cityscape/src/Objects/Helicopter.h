// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: Helicopter.h
// Date: 23/11/2019

#pragma once

#include <midori/scene/SceneObject.h>
#include <midori/scene/lighting/Lights.h>

namespace City {

    class Helicopter {
    public:
        Helicopter();
        ~Helicopter();

        void Update(float delta);

        midori::ref<midori::SceneObject> GetSceneObject() { return m_Chassis; }
        midori::ref<midori::SpotLight> GetSpotLight() { return m_SpotLight; }
    private:
        float m_AngularPosition = 0.0f;

        // Propeller
        float m_PropellerRotation = 0.0f;
        const float c_RotationSpeed = 0.8f;

        // Helicopter
        float c_FlightHeight = 37.0f;
        float c_FlightSpeed = 0.2f;
        float c_FlightArc = 30.0f;

        midori::ref<midori::SceneObject> m_Chassis;
        midori::ref<midori::SceneObject> m_Propeller;

        midori::ref<midori::SpotLight> m_SpotLight;

        midori::ref<midori::Shader> m_HelicopterShader;
    };

}
