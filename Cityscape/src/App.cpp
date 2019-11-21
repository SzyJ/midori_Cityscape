// Author: Szymon Jackiewicz
// 
// Project: sandbox
// File: SandApp.cpp
// Date: 09/10/2019

#include "Resporces.h"


#include <Midori.h>
#include <imgui.h>

class CityLayer : public midori::Layer {
public:
    CityLayer() : Layer("Cityscape") {
        midori::Application::Get().GetWindow().SetCursorEnabled(m_CursorEnabled);
        unsigned int screenWidth = midori::Application::Get().GetWindow().GetWindowWidth();
        unsigned int screenHeight = midori::Application::Get().GetWindow().GetWindowHeight();

        m_Camera = new midori::PerspectiveCamera((float)screenWidth / (float)screenHeight, glm::vec3(0.0f, 0.0f, 3.0f));

        m_CityScene.SetCamera(m_Camera);
        m_Skybox = new midori::Skybox(TEXTURE_SKYBOX);
        m_CityScene.SetSkybox(m_Skybox);


        midori::RenderCommand::Init();
    }

    ~CityLayer() {
        delete m_Skybox;
        delete m_Camera;
    }

    void OnAttach() override {
        MD_INFO("Example Layer Attached");
    }

    void OnDetach() override {
        MD_INFO("Example Layer Detached");
    }

    void OnUpdate(midori::DeltaTime delta) override {
        m_DeltaAverage = (m_DeltaAverage * CONF_FPS_SMOOTHING) + (delta * (1.0f - CONF_FPS_SMOOTHING));

        // Draw
        midori::RenderCommand::SetClearColor({ 0.26f, 0.26f, 0.26f, 1.0f });
        midori::RenderCommand::Clear();
        m_CityScene.Draw();
    }

    void OnImGuiRender() override {
        ImGui::Begin("FPS");
        ImGui::Text(std::to_string((1.0f / m_DeltaAverage)).c_str());
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
            }
            break;
        }
    }

private:
    void OnWindowResize(midori::WindowResizeEvent& resizeEvent) {
        const auto newWidth = resizeEvent.GetWidth();
        const auto newHeight = resizeEvent.GetHeight();

        midori::RenderCommand::SetViewport(0, 0, newWidth, newHeight);
        m_Camera->OnWindowResize(newWidth, newHeight);
    }

    // Debug
    midori::DeltaTime m_DeltaAverage = 0.0f;

    // Camera
    midori::PerspectiveCamera* m_Camera;
    float m_MoveSpeed = 10.0f;
    float m_LookSens = 0.1f;
    bool m_CursorEnabled = false;

    // Scene
    midori::Scene m_CityScene;
    midori::Skybox* m_Skybox;

};

class Cityscape : public midori::Application {
public:
    Cityscape() {
        PushLayer(new CityLayer());
    }

    ~Cityscape() {}
};

midori::Application* midori::CreateApplication() { return new Cityscape(); }
