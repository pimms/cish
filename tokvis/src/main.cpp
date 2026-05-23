#include <fstream>
#include <memory>

#include "TreeRenderer.h"
#include "parse/Parser.h"
#include "tok/Tokenizer.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

struct SDLContext
{
    SDL_Window *window{};
    SDL_GLContext glContext{};
    float windowScale{};
};

std::unique_ptr<cish::parse::ParseTree> loadParseTree(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <my-sweet-file.c>\n", argv[0]);
        return nullptr;
    }

    auto path = std::filesystem::current_path();
    printf("CURRENT PATH: %s\n", path.c_str());

    std::ifstream ifs(argv[1]);
    if (!ifs.is_open()) {
        fprintf(stderr, "Failed to open file '%s'", argv[1]);
        return nullptr;
    }

    ifs.seekg(0, std::ios::end);
    const std::streamsize size = ifs.tellg();
    ifs.seekg(0);
    std::string sourceBuffer(size, '\0');
    ifs.read(&sourceBuffer[0], size);

    try {
        cish::tok::Tokenizer tokenizer(sourceBuffer);
        auto tokens = tokenizer.tokenize();
        if (tokens.empty()) {
            fprintf(stderr, "File contains zero tokens. Let's assume this is an error in cish.");
            return nullptr;
        }

        cish::parse::Parser parser(tokens);
        return parser.parse();
    } catch (const std::exception &e) {
        fprintf(stderr, "Failed to parse '%s':\n%s\n", argv[1], e.what());
    }
    return nullptr;
}

std::unique_ptr<SDLContext> setupSDL()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return nullptr;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    const float windowScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("Cish Token Visualizer", (int)(1280 * windowScale), (int)(800 * windowScale), window_flags);
    if (!window) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return nullptr;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return nullptr;
    }

    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    return std::make_unique<SDLContext>(window, glContext, windowScale);
}

void setupImGui(const SDLContext* context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(context->windowScale);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(context->window, context->glContext);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void render()
{
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    const auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y });
    ImGui::SetNextWindowPos({ viewport->WorkPos.x, viewport->WorkPos.y });

    {
        ImGui::Begin("Tokvis", nullptr, flags);

        if (ImGui::BeginTable("table", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody)) {
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Value");
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::TreeNodeEx("root", ImGuiTreeNodeFlags_DrawLinesFull, "Root")) {

                // CHILD A
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TreeNodeEx("ca", ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen, "Child");
                ImGui::TableNextColumn();
                ImGui::Text("1");

                // CHILD B
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TreeNodeEx("CB", ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                ImGui::TableNextColumn();
                ImGui::Text("2");

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::TreeNodeEx("childc", ImGuiTreeNodeFlags_DrawLinesFull, "Child C")) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TreeNodeEx("CD", ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    ImGui::TableNextColumn();
                    ImGui::Text("4");

                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
            ImGui::EndTable();
        }

        ImGui::ShowDemoWindow();
        ImGui::End();
    }
}

int main(int argc, char **argv)
{
    auto parseTree = loadParseTree(argc, argv);
    if (!parseTree) return 1;
    cish::tokvis::TreeRenderer treeRenderer(std::move(parseTree));

    auto sdlContext = setupSDL();
    if (!sdlContext) return 1;
    setupImGui(sdlContext.get());

    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT)
                running = false;
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y });
        ImGui::SetNextWindowPos({ viewport->WorkPos.x, viewport->WorkPos.y });
        ImGui::Begin("Tokvis", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        treeRenderer.render();
        ImGui::End();

        ImGui::EndFrame();
        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(sdlContext->window);
    }

    return 0;
}
