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
#include <tree/AbstractParseTreeVisitor.h>

#include "FileBrowser.h"

using namespace cish::tokvis;

struct SDLContext
{
    SDL_Window *window{};
    SDL_GLContext glContext{};
    float windowScale{};
};

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

int main(int argc, char **argv)
{
    auto sdlContext = setupSDL();
    if (!sdlContext) return 1;
    setupImGui(sdlContext.get());

    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    bool running = true;

    FileBrowser fileBrowser;
    std::unique_ptr<TreeRenderer> treeRenderer;
    if (argc == 2) {
        treeRenderer = std::make_unique<TreeRenderer>();
        treeRenderer->loadFile(argv[1]);
    }

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

        auto clickedFile = fileBrowser.render();
        if (clickedFile.has_value()) {
            treeRenderer = std::make_unique<TreeRenderer>();
            treeRenderer->loadFile(clickedFile.value());
        }

        if (treeRenderer != nullptr) {
            if (treeRenderer->render()) {
                treeRenderer = nullptr;
            }
        }

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
