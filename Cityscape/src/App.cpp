// Author: Szymon Jackiewicz
// 
// Project: sandbox
// File: SandApp.cpp
// Date: 09/10/2019

#include "Resources.h"
#include "Objects/Scene.h"
#include "CameraTrack.h"

#include <Midori.h>
#include <midori/renderer/post-processing/PostProcessingPipeline.h>

#include <imgui.h>

class CityLayer : public midori::Layer {
public:
    CityLayer() : Layer("Cityscape") {
        m_CursorEnabled = true;
        midori::Application::Get().GetWindow().SetCursorEnabled(m_CursorEnabled);
        m_ScreenWidth = midori::Application::Get().GetWindow().GetWindowWidth();
        m_ScreenHeight = midori::Application::Get().GetWindow().GetWindowHeight();

        midori::Application::Get().GetWindow().SetVSync(true);

        m_Camera = new midori::PerspectiveCamera((float)m_ScreenWidth / (float)m_ScreenHeight, glm::vec3(0.0f, 60.0f, 10.0f));
        m_Camera->SetFarZ(200.0f);
        m_CityScene.SetCamera(m_Camera);

        m_CityScene.SetScreenDimensions(m_ScreenWidth, m_ScreenHeight);

        m_PostProcess.AddStage(SHADER_PP_FXAA);
        m_PostProcess.AddStage(SHADER_PP_COLOR_AMP);
        m_PostProcess.AddStage(SHADER_PP_RAINDROP);
        m_PostProcess.UpdateScreenSize(m_ScreenWidth, m_ScreenHeight);

        float minimapQuadVertices[] = {
        // positions     // texCoords
        -1.0f,  1.0f,    0.0f, 1.0f,
        -1.0f, -1.0f,    0.0f, 0.0f,
         1.0f, -1.0f,    1.0f, 0.0f,

        -1.0f,  1.0f,    0.0f, 1.0f,
         1.0f, -1.0f,    1.0f, 0.0f,
         1.0f,  1.0f,    1.0f, 1.0f
        };
        auto vertexBuffer = midori::VertexBuffer::Create(minimapQuadVertices, (2 + 2) * 3 * 2);
        vertexBuffer->SetLayout({
            { midori::ShaderDataType::Float2, "a_Position" },
            { midori::ShaderDataType::Float2, "a_TexCoord" }
            });
        m_Minimap = midori::VertexArray::Create();
        m_Minimap->AddVertexBuffer(vertexBuffer);

        m_MinimapShader = midori::Shader::Load(GAME_SHADERS"Minimap");

        midori::RenderCommand::Init();
    }

    ~CityLayer() {
        delete m_Camera;
    }

    void OnUpdate(midori::DeltaTime delta) override {
        m_DeltaAverage = (m_DeltaAverage * CONF_FPS_SMOOTHING) + (delta * (1.0f - CONF_FPS_SMOOTHING));

        m_TotalTime += delta;

        m_Track.SetTime(m_TotalTime);

        if (!m_Track.IsComplete() && m_OnTrack) {
            m_Camera->SetPosition(m_Track.GetPosition());
            m_Camera->SetDirection(m_Track.GetYaw(), m_Track.GetPitch());
        }

        if (m_Track.IsComplete() && m_OnTrack) {
            m_OnTrack = false;
            midori::Application::Get().GetWindow().SetCursorEnabled(m_CursorEnabled = false);
        }

        m_CityScene.Update(delta);

        // Handle input
        if (!m_OnTrack) {
            if (midori::Input::IsKeyPressed(MD_KEY_W)) {
                m_Camera->Move(midori::MovementDirection::forward, delta * m_MoveSpeed);
            }
            if (midori::Input::IsKeyPressed(MD_KEY_A)) {
                m_Camera->Move(midori::MovementDirection::left, delta * m_MoveSpeed);
            }
            if (midori::Input::IsKeyPressed(MD_KEY_S)) {
                m_Camera->Move(midori::MovementDirection::backward, delta * m_MoveSpeed);
            }
            if (midori::Input::IsKeyPressed(MD_KEY_D)) {
                m_Camera->Move(midori::MovementDirection::right, delta * m_MoveSpeed);
            }
            if (midori::Input::IsKeyPressed(MD_KEY_SPACE)) {
                m_Camera->Move(midori::MovementDirection::up, delta * m_MoveSpeed);
            }
            if (midori::Input::IsKeyPressed(MD_KEY_LEFT_SHIFT)) {
                m_Camera->Move(midori::MovementDirection::down, delta * m_MoveSpeed);
            }
        }

        // Draw
        m_CityScene.CalculateShadows();
        m_PostProcess.BeginPostProcess();

        m_CityScene.Draw();

        m_PostProcess.FinishPostProcess(m_TotalTime);

        // Draw Minimap
        if (m_ShowMinimap) {
            midori::RenderCommand::SetViewport(5, 5, m_ScreenWidth / 4.0f, m_ScreenHeight / 4.0f);
            m_MinimapShader->Bind();
            m_MinimapShader->UploadUniformInt("u_InputTexture", 10);
            midori::Renderer::Submit(m_MinimapShader, m_Minimap);

            midori::RenderCommand::SetViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
        }
    }

    void OnImGuiRender() override {
        ImGui::Begin("FPS");
        ImGui::Text(std::to_string((1.0f / m_DeltaAverage)).c_str());
        ImGui::Text(std::string("Cam Pos: (").append(std::to_string(m_Camera->GetPosition().x)).append(", ").append(std::to_string(m_Camera->GetPosition().y)).append(", ").append(std::to_string(m_Camera->GetPosition().z)).append(")").c_str());
        ImGui::Text(std::string("Cam Dir: (Yaw: ").append(std::to_string(m_Camera->GetYaw())).append(", Pitch: ").append(std::to_string(m_Camera->GetPitch())).append(")").c_str());
        ImGui::End();
    }

    void OnEvent(midori::Event& event) override {
        switch (event.GetEventType()) {
        case midori::EventType::WindowResize:
            OnWindowResize((midori::WindowResizeEvent&) event);
            break;

        case midori::EventType::MouseMoved:
            if (!m_CursorEnabled) {
                auto& moveEvent = (midori::MouseMovedEvent&) event;
                m_Camera->Rotate(m_LookSens * moveEvent.GetX(), m_LookSens * moveEvent.GetY());
            }
            break;

        case midori::EventType::KeyPressed:
            auto& keyPressedEvent = (midori::KeyPressedEvent&) event;
            if (keyPressedEvent.GetKeyCode() == MD_KEY_ESCAPE) {
                midori::Application::Get().GetWindow().SetCursorEnabled(m_CursorEnabled = !m_CursorEnabled);
                m_OnTrack = false;
            } else if (keyPressedEvent.GetKeyCode() == MD_KEY_0) {
                m_TotalTime = 0.0f;
                m_OnTrack = true;
                m_Camera->SetPosition(glm::vec3(0.0f, 60.0f, 10.0f));
                m_Camera->SetDirection(-90.0f, 0.0f);
            } else if (keyPressedEvent.GetKeyCode() == MD_KEY_M) {
                m_ShowMinimap = !m_ShowMinimap;
            }
            break;
        }
    }

private:
    void OnWindowResize(midori::WindowResizeEvent& resizeEvent) {
        const auto newWidth = resizeEvent.GetWidth();
        const auto newHeight = resizeEvent.GetHeight();
        m_ScreenWidth = newWidth;
        m_ScreenHeight = newHeight;

        midori::RenderCommand::SetViewport(0, 0, newWidth, newHeight);
        m_Camera->OnWindowResize(newWidth, newHeight);
        m_CityScene.SetScreenDimensions(newWidth, newHeight);
        m_PostProcess.UpdateScreenSize(newWidth, newHeight);
    }

    // Debug
    midori::DeltaTime m_DeltaAverage = 0.0f;
    float m_TotalTime = 0.0f;
    uint32_t m_ScreenWidth;
    uint32_t m_ScreenHeight;
    midori::ref<midori::VertexArray> m_Minimap;
    midori::ref<midori::Shader> m_MinimapShader;
    bool m_ShowMinimap = false;

    // Camera
    midori::PerspectiveCamera* m_Camera;
    City::CameraTrack m_Track;
    float m_MoveSpeed = 15.0f;
    float m_LookSens = 0.1f;
    bool m_CursorEnabled = false;
    bool m_OnTrack = true;

    // Scene
    City::Scene m_CityScene;

    midori::PostProcessingPipeline m_PostProcess;
};

class Cityscape : public midori::Application {
public:
    Cityscape() {
        PushLayer(new CityLayer());
    }

    ~Cityscape() {}
};

midori::Application* midori::CreateApplication() { return new Cityscape(); }
