#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"
#include "Eternal/Scene/SceneSerializer.h"
#include "Eternal/Utils/PlatformUtils.h"
#include "Eternal/NativeScripts/ScriptHandler.h"

namespace Eternal {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
    }

    void EditorLayer::OnAttach()
    {
        m_SceneViewportPanel = CreateRef<SceneViewportPanel>();
        m_ActiveScene = CreateRef<Scene>();
        m_SceneHierachyPanel = CreateRef<SceneHierarchyPanel>();
        m_PropertiesPanel = CreateRef<PropertiesPanel>();
        m_SettingsPanel = CreateRef<SettingsPanel>();

#if 0
        auto chessSquare = m_ActiveScene->CreateEntity("Chess Square");
        chessSquare.GetComponent<TransformComponent>().Position = { glm::vec3{0.0f, 4.0f, 0.0f} };
        chessSquare.AddComponent<SpriteRendererComponent>("assets/textures/Checkerboard.png");
        chessSquare.AddComponent<PhysicsComponent>(true);

        auto colorsquare = m_ActiveScene->CreateEntity("One Color Square");
        colorsquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 0.8f, 0.8f, 1 });
        colorsquare.AddComponent<PhysicsComponent>();

        auto camera = m_ActiveScene->CreateEntity("Camera Entity");
        camera.AddComponent<CameraComponent>();
        camera.GetComponent<CameraComponent>().Primary = true;

#endif

#if 0
        //Entitys
        auto colorSquare = m_ActiveScene->CreateEntity("One Color Square");
        colorSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 0.8f, 0.8f, 1 });
        
        auto chessSquare = m_ActiveScene->CreateEntity("Chess Square");
        chessSquare.GetComponent<TransformComponent>().Position = { glm::vec3{ 1.0f, 1.0f, 0.1f } };
        chessSquare.AddComponent<SpriteRendererComponent>("assets/textures/Checkerboard.png");
        
        auto ChernoSquare = m_ActiveScene->CreateEntity("Cherno Square");
        ChernoSquare.GetComponent<TransformComponent>().Position = { glm::vec3{ -10.0f, -10.0f, 0.0f } };
        ChernoSquare.AddComponent<SpriteRendererComponent>("assets/textures/Logo.png");
        
        auto DoomSquare = m_ActiveScene->CreateEntity("Doom Square");
        DoomSquare.GetComponent<TransformComponent>().Position = { glm::vec3{ -1.0f, -1.0f, -0.1f } };
        DoomSquare.GetComponent<TransformComponent>().Size = {glm::vec2{ 5.0f, 5.0f }};
        DoomSquare.AddComponent<SpriteRendererComponent>("assets/textures/EternalLogo.png");
        DoomSquare.AddComponent<NativeScriptComponent>().SetScript(ScriptHandler::GetScript(Scripts::ERotater));
        
        auto camera = m_ActiveScene->CreateEntity("Camera Entity");
        camera.AddComponent<CameraComponent>();
        
        camera.AddComponent<NativeScriptComponent>().SetScript(ScriptHandler::GetScript(Scripts::EController));
#endif
        m_SceneViewportPanel->SetContext(m_ActiveScene);
        m_SceneHierachyPanel->SetContext(m_ActiveScene);
        m_PropertiesPanel->SetContext(m_SceneHierachyPanel);
    }

    void EditorLayer::OnDetach()
    {
        
    }

    void EditorLayer::OnUpdate(Timestep ts) // Max Frame Rate auf 60 cappen maybe!!
    {
        m_SceneViewportPanel->OnUpdate(ts);
        m_SettingsPanel->SetTimestep(ts);
    }

    void EditorLayer::OnImGuiRender()
    {
        ChangeSytle(false);

        //ImGui::ShowDemoWindow();

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
            ImGuiStyle& style = ImGui::GetStyle();
            float minWinSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = 370.0f;
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            style.WindowMinSize.x = minWinSizeX;

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("New", "Ctrl+N"))
                    {
                        NewScene();
                    }

                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    {
                        OpenScene();
                    }

                    if (ImGui::MenuItem("Safe As...", "Ctrl+S"))
                    {
                        SaveSceneAs();
                    }

                    ImGui::Separator();
                    if (ImGui::MenuItem("Exit", "Ctrl+Q"))
                        Exit();

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }

        //Panels
        m_SceneViewportPanel->OnImGuiRender();
        m_SceneHierachyPanel->OnImGuiRender();
        m_PropertiesPanel->OnImGuiRender();
        m_SettingsPanel->OnImGuiRender();

        if (dockingEnabled)
            ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_SceneViewportPanel->OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(ET_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    void EditorLayer::ChangeSytle(bool show)
    {
        if (show) {
            ImGui::Begin("Dear ImGui Style Editor");
            ImGui::ShowStyleEditor();
            ImGui::End();
        }
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(ET_KEY_LEFT_CONTROL) || Input::IsKeyPressed(ET_KEY_RIGHT_CONTROL);
        switch (e.GetKeyCode())
        {
        case ET_KEY_N:
        {
            if (control)
                NewScene();

            break;
        }
        case ET_KEY_O:
        {
            if (control)
                OpenScene();

            break;
        }
        case ET_KEY_S:
        {
            if (control)
                SaveSceneAs();

            break;
        }
        case ET_KEY_Q:
        {
            if (control)
                Exit();

            break;
        }
        }
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_SceneViewportPanel->SetContext(m_ActiveScene);
        m_SceneViewportPanel->OnViewportResize(m_SceneViewportPanel->GetViewportSizeX(), m_SceneViewportPanel->GetViewportSizeY());
        m_SceneHierachyPanel->SetContext(m_ActiveScene);
        m_PropertiesPanel->SetContext(m_SceneHierachyPanel);
    }

    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Eternal Scene (*.eternal)\0*.eternal\0");
        if (!filepath.empty())
        {
            NewScene();

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Eternal Scene (*.eternal)\0*.eternal\0");
        if (!filepath.empty())
        {
            m_ActiveScene->SetName(FileDialogs::GetFileName(filepath));
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }

    void EditorLayer::Exit()
    {
        Application::Get().Close();
    }

    

}