// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: Water.h
// Date: 26/11/2019

#pragma once

#include <midori/scene/SceneObject.h>
#include <midori/renderer/Shader.h>
#include <midori/renderer/Texture.h>

namespace City {

    class Water {
    public:
        Water();
        ~Water() = default;

        midori::ref<midori::SceneObject> GetObject() { return m_WaterPlane; }

        void Update(float delta) {
            m_TotalTime += delta;

            m_Shader->Bind();
            m_Shader->UploadUniformFloat("u_TotalTime", m_TotalTime);
            m_Shader->UploadUniformInt("u_AttachmentPoint", m_AttachmentPoint);
        }

        void SetAttachmentPoint(uint32_t attachment) { m_AttachmentPoint = attachment; }
        void SetSize(float size) { m_WaterPlane->SetScale(size); }
        void SetPosition(glm::vec3 position) { m_WaterPlane->SetPosition(position); }

    private:
        midori::ref<midori::SceneObject> m_WaterPlane;
        midori::ref<midori::Shader> m_Shader;
        midori::ref<midori::Texture2D> m_WaterTex;
        midori::ref<midori::Texture2D> m_WaterNormal;

        float m_TotalTime = 0.0f;
        uint32_t m_AttachmentPoint = 0;
    };

}
