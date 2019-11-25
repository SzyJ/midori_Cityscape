// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: CameraTrack.h
// Date: 24/11/2019

#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace City {

    class CameraTrack {
    public:
        CameraTrack();
        ~CameraTrack() = default;

        bool IsComplete() { return m_IsComplete; }

        void SetTime(float time);

        glm::vec3 GetPosition();
        float GetYaw();
        float GetPitch();

    private:
        struct TrackCheckpoint {

            TrackCheckpoint(float time, glm::vec3 position, float yaw, float pitch)
                :Time(time), Position(position), Yaw(yaw), Pitch(pitch) {}

            float Time;
            glm::vec3 Position;
            float Yaw;
            float Pitch;
        };

        std::vector<TrackCheckpoint> m_Checkpoints;

        uint32_t m_CurrentCheckpoint = 0;
        float m_TotalTime = 0.0f;
        bool m_IsComplete = false;
    };
}
