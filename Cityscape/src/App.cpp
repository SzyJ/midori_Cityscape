// Author: Szymon Jackiewicz
// 
// Project: sandbox
// File: SandApp.cpp
// Date: 09/10/2019

#include <Midori.h>

#include "imgui.h"
#include <midori/renderer/MeshLoader.h>

#include <glm/gtx/string_cast.hpp>


class CityLayer : public midori::Layer {
public:
    CityLayer() : Layer("Cityscape") {

        midori::RenderCommand::Init();
    }

    ~CityLayer() {
        
    }

    void OnAttach() override {
        MD_INFO("Example Layer Attached");
    }

    void OnDetach() override {
        MD_INFO("Example Layer Detached");
    }

    void OnUpdate(midori::DeltaTime delta) override {
        
    }

    void OnImGuiRender() override {
        
    }

    void OnEvent(midori::Event& event) override {
        
    }

};

class Cityscape : public midori::Application {
public:
    Cityscape() {
        PushLayer(new CityLayer());
    }

    ~Cityscape() {}
};

midori::Application* midori::CreateApplication() { return new Cityscape(); }
