#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "Scripts/CameraController.h"

#include <glm/gtc/type_ptr.hpp>

namespace Eternal {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
    {
    }

    void EditorLayer::OnAttach()
    {
        auto checkerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
        auto doomTexture = Texture2D::Create("assets/textures/EternalLogo.png");
    
        m_ActiveScene = CreateRef<Scene>();
        m_SceneHierachyPanel = CreateRef<SceneHierarchyPanel>();
        m_PropertiesPanel = CreateRef<PropertiesPanel>();
        m_SettingsPanel = CreateRef<SettingsPanel>();
        m_SceneViewportPanel = CreateRef<SceneViewportPanel>();

        //Entitys
        auto chessSquare = m_ActiveScene->CreateEntity("Chess Square");
        chessSquare.AddComponent<TransformComponent>() = { glm::vec3{ 1.0f, 1.0f, 0.1f } };
        chessSquare.AddComponent<SpriteRendererComponent>(checkerboardTexture);
        m_ChessSquareEntity = chessSquare;

        auto colorSquare = m_ActiveScene->CreateEntity("One Color Square");
        colorSquare.AddComponent<TransformComponent>();
        colorSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 0.8f, 0.8f, 1 });

        auto ChernoSquare = m_ActiveScene->CreateEntity("Cherno Square");
        ChernoSquare.AddComponent<TransformComponent>() = { glm::vec3{ -10.0f, -10.0f, 0.0f } };
        ChernoSquare.AddComponent<SpriteRendererComponent>(Texture2D::Create("assets/textures/Logo.png"));

        auto DoomSquare = m_ActiveScene->CreateEntity("Doom Square");
        DoomSquare.AddComponent<TransformComponent>() = { glm::vec3{ -1.0f, -1.0f, -0.1f }, glm::vec2{ 5.0f, 5.0f }, 30.0f };
        DoomSquare.AddComponent<SpriteRendererComponent>(doomTexture);
   
        auto camera = m_ActiveScene->CreateEntity("Camera Entity");
        camera.AddComponent<TransformComponent>();
        camera.AddComponent<CameraComponent>();

        camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        m_CameraEntity = camera;
        m_SceneHierachyPanel->SetContext(m_ActiveScene);
        m_PropertiesPanel->SetContext(m_SceneHierachyPanel);
        m_SceneViewportPanel->SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
        
    }

    void EditorLayer::OnUpdate(Timestep ts) // Max Frame Rate auf 60 cappen maybe!!
    {
        m_SceneViewportPanel->OnUpdate(ts);
        m_SettingsPanel->SetTimestep(ts);
        //Update
        //if (m_ViewportFocused)
        //Old TODO remove
        m_CameraController.OnUpdate(ts);

    }

    void EditorLayer::OnImGuiRender()
    {
        ChangeSytle(false);

        static bool dockingEnabled = true;
        if (dockingEnabled)
        {

            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    //ImGui::Separator();
                    //if (ImGui::MenuItem("Close DockSpace", NULL, false, &dockspaceOpen != NULL))
                       // dockspaceOpen = false;
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }

        //Panels
        m_SceneHierachyPanel->OnImGuiRender();
        m_PropertiesPanel->OnImGuiRender();
        m_SettingsPanel->OnImGuiRender();
        m_SceneViewportPanel->OnImGuiRender();

        if (dockingEnabled)
            ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }

    void EditorLayer::ChangeSytle(bool show)
    {
        if (show) {
            ImGui::Begin("Dear ImGui Style Editor");
            ImGui::ShowStyleEditor();
            ImGui::End();
        }
    }

    

}