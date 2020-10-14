#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

namespace Eternal {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
    {
    }

    void EditorLayer::OnAttach()
    {
        FrameBufferSpecification frameBufferSpec;
        frameBufferSpec.Width = 1280;
        frameBufferSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);
        m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
        m_DoomTexture = Texture2D::Create("assets/textures/EternalLogo.png");
    
        m_ActiveScene = CreateRef<Scene>();

        auto chessSquare = m_ActiveScene->CreateEntity("Chess Square");
        chessSquare.AddComponent<TransformComponent>(glm::vec3{ 3.0f, 1.0f, 0.0f }, glm::vec2{ 1.0f, 1.0f}, 0.0f);
        chessSquare.AddComponent<SpriteRendererComponent>(m_CheckerboardTexture, 1, m_TintColor);
        m_ChessSquareEntity = chessSquare;

        auto colorSquare = m_ActiveScene->CreateEntity("One Color Square");
        colorSquare.AddComponent<TransformComponent>(glm::vec3{ 0.0f });
        colorSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 0.8f, 0.8f, 1 });

        auto DoomSquare = m_ActiveScene->CreateEntity("Doom Square");
        DoomSquare.AddComponent<TransformComponent>(glm::vec3{-1.0f, -1.0f, -0.1f }, glm::vec2{ 5.0f, 5.0f }, 30.0f);
        DoomSquare.AddComponent<SpriteRendererComponent>(m_DoomTexture);

        auto ChernoSquare = m_ActiveScene->CreateEntity("Cherno Square");
        ChernoSquare.AddComponent<TransformComponent>(glm::vec3{ -10.0f, -10.0f, 0.0f }, glm::vec2{ 3.0f, 3.0f }, 40.0f);
        ChernoSquare.AddComponent<SpriteRendererComponent>(Texture2D::Create("assets/textures/Logo.png"));
    }

    void EditorLayer::OnDetach()
    {
        
    }

    void EditorLayer::OnUpdate(Timestep ts) // Max Frame Rate auf 60 cappen maybe!!
    {
        //Update
        if (m_ViewportFocused)
            m_CameraController.OnUpdate(ts);

        //Renderer Stats
        Renderer2D::ResetStats();

        m_FrameBuffer->Bind();

        //Render
        RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.7f, 1 });
        RenderCommand::Clear();

        Renderer2D::BeginScene(m_CameraController.GetCamera());

        //Update Scene
        m_ActiveScene->OnUpdate(ts);

        Renderer2D::EndScene();
        m_FrameBuffer->Unbind();
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

        ImGui::Begin("Settings");

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Batch Renderer Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quad Count: %d", stats.QuadCount);
        ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

        ImGui::End();

        ImGui::Begin("Property Panel");

        ImGui::Text(m_ChessSquareEntity.GetComponent<TagComponent>().Tag.c_str());
        ImGui::SliderFloat3("Checkerboard Position", glm::value_ptr(m_ChessSquareEntity.GetComponent<TransformComponent>().Position), -10.0f, 10.0f);
        ImGui::SliderFloat2("Checkerboard Quad Size", glm::value_ptr(m_ChessSquareEntity.GetComponent<TransformComponent>().Size), 0.0f, 10.0f);
        ImGui::SliderInt("Checkerboard UV Scaling", &m_ChessSquareEntity.GetComponent<SpriteRendererComponent>().TextureScale, 1, 10);
        ImGui::SliderFloat("Checkerboard Rotation", &m_ChessSquareEntity.GetComponent<TransformComponent>().Rotation, 0.0f, 360.0f);
        ImGui::Image((void*)m_ChessSquareEntity.GetComponent<SpriteRendererComponent>().Texture->GetRendererID(), ImVec2(256.0f, 256.0f));
        ImGui::ColorEdit4("Tint Color of Checkerboard", glm::value_ptr(m_ChessSquareEntity.GetComponent<SpriteRendererComponent>().Color));

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

        ImGui::Begin("Scene Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        //ET_INFO("ViewportPanelSize: {0}, {1}: ", viewportPanelSize.x, viewportPanelSize.y);
        if (m_ViewportPanelSize != *((glm::vec2*)&viewportPanelSize))
        {
            m_ViewportPanelSize = { viewportPanelSize.x, viewportPanelSize.y };
            m_FrameBuffer->Resize((uint32_t)m_ViewportPanelSize.x, (uint32_t)m_ViewportPanelSize.y);
        }
        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportPanelSize.x, m_ViewportPanelSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
        
        ImGui::End();

        ImGui::PopStyleVar();

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