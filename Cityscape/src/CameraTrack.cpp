// Author: Szymon Jackiewicz
// 
// Project: Cityscape
// File: CameraTrack.cpp
// Date: 24/11/2019

#include "CameraTrack.h"

namespace City {

    CameraTrack::CameraTrack() {
        m_Checkpoints.emplace_back(0.0f, glm::vec3(0.0f, 4.0f, 60.0f), -90.0f, 0.0f);
        m_Checkpoints.emplace_back(14.0f, glm::vec3(0.0f, 4.0f, 0.0f), -90.0f, 0.0f);
        m_Checkpoints.emplace_back(18.0f, glm::vec3(0.0f, 10.0f, -25.0f), -108.0f, 42.0f);
        m_Checkpoints.emplace_back(23.0f, glm::vec3(5.0f, 40.0f, -25.0f), -210.0f, -20.0f);
        m_Checkpoints.emplace_back(28.0f, glm::vec3(26.0f, 56.0f, -6.0f), -210.0f, -27.0f);
        m_Checkpoints.emplace_back(37.0f, glm::vec3(18.0f, 52.0f, -2.0f), -297.0f, -33.0f);
        m_Checkpoints.emplace_back(40.0f, glm::vec3(25.0f, 50.0f, 32.0f), -434.0f, -36.0f);
        m_Checkpoints.emplace_back(50.0f, glm::vec3(53.0f, 64.0f, 65.0f), -489.0f, -34.0f);
    }

    void CameraTrack::SetTime(float time) {
        m_TotalTime = time;

        if (m_IsComplete) {
            return;
        }

        if (m_Checkpoints.at(m_CurrentCheckpoint + 1).Time <= m_TotalTime) {
            ++m_CurrentCheckpoint;
        }

        if (m_Checkpoints.size() - 1 <= m_CurrentCheckpoint) {
            m_IsComplete = true;
        }
    }

    glm::vec3 CameraTrack::GetPosition() {
        if (m_IsComplete) {
            return m_Checkpoints.at(m_Checkpoints.size() - 1).Position;
        }

        TrackCheckpoint lastCheckpoint = m_Checkpoints.at(m_CurrentCheckpoint);
        TrackCheckpoint nextCheckpoint = m_Checkpoints.at(m_CurrentCheckpoint + 1);

        float interpolateValue = (m_TotalTime - lastCheckpoint.Time) / (nextCheckpoint.Time - lastCheckpoint.Time);
        return glm::mix(lastCheckpoint.Position, nextCheckpoint.Position, interpolateValue);
    }

    float CameraTrack::GetYaw() {
        if (m_IsComplete) {
            return m_Checkpoints.at(m_Checkpoints.size() - 1).Yaw;
        }

        TrackCheckpoint lastCheckpoint = m_Checkpoints.at(m_CurrentCheckpoint);
        TrackCheckpoint nextCheckpoint = m_Checkpoints.at(m_CurrentCheckpoint + 1);

        float interpolateValue = (m_TotalTime - lastCheckpoint.Time) / (nextCheckpoint.Time - lastCheckpoint.Time);
        return glm::mix(lastCheckpoint.Yaw, nextCheckpoint.Yaw, interpolateValue);
    }

    float CameraTrack::GetPitch() {
        if (m_IsComplete) {
            return m_Checkpoints.at(m_Checkpoints.size() - 1).Pitch;
        }

        TrackCheckpoint lastCheckpoint = m_Checkpoints.at(m_CurrentCheckpoint);
        TrackCheckpoint nextCheckpoint = m_Checkpoints.at(m_CurrentCheckpoint + 1);

        float interpolateValue = (m_TotalTime - lastCheckpoint.Time) / (nextCheckpoint.Time - lastCheckpoint.Time);
        return glm::mix(lastCheckpoint.Pitch, nextCheckpoint.Pitch, interpolateValue);
    }

}
