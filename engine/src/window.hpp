#pragma once
#ifndef _TE_WINDOW_
#define _TE_WINDOW_

#include "core.hpp"
#include <thread>
#include <chrono>
#include "scene.hpp"

namespace te {
    class Window {
    private:
        GLFWwindow* mWindowPtr;
        LayerHandler mLayerHandler;
        SceneHandler mSceneHandler;

        bool mWindowClosed = false;

        uint32_t mFixedUpdatesPerSecond = 128;

        /**
         * @brief Handle fixed update function
         * 
         */
        void FixedUpdateHandler() {
            while(!mWindowClosed) {
                std::chrono::time_point start = std::chrono::high_resolution_clock::now();

                LayerHandler::pGlobal->LayersFixedUpdate();

                // Needed for stable mFixedUpdatesPerSecond (FixedUpdate sort of FPS)
                std::this_thread::sleep_for(std::chrono::duration<long double, std::milli>(std::chrono::duration<long double, std::milli>(1.0 / (long double)mFixedUpdatesPerSecond).count() - std::chrono::duration<long double, std::milli>(std::chrono::high_resolution_clock::now() - start).count()));
            }
        }

    public:
        static Window* pGlobal;

        Window() {
            if(!pGlobal) {
                pGlobal = this;

                TE_INFO("Created global Window")
            }
            else {
                TE_WARN("Cannot create another global Window!")
            }
        }

        /**
         * @brief Run main window
         * 
         * @param title 
         * @param width 
         * @param height 
         */
        void Run(std::string title, uint32_t width, uint32_t height) {            
            LayerHandler::pGlobal->AddLayer(&mSceneHandler);
            
            LayerHandler::pGlobal->LayersAwake();

            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, 4);

            mWindowPtr = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

            if(!mWindowPtr) {
                TE_ERR("Cannot create GLFW window!")

                return;
            }

            glfwMakeContextCurrent(mWindowPtr);

            if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
                TE_ERR("Cannot load OpenGL 4.5 context!")

                return;
            }

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_MULTISAMPLE);

            LayerHandler::pGlobal->LayersStart();

            std::jthread fixed_update_thread(&Window::FixedUpdateHandler, this);

            fixed_update_thread.detach();

            while(!(mWindowClosed = glfwWindowShouldClose(mWindowPtr))) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

                LayerHandler::pGlobal->LayersUpdate();

                glfwSwapBuffers(mWindowPtr);

                LayerHandler::pGlobal->LayersLateUpdate();

                glfwPollEvents();
            }

            LayerHandler::pGlobal->LayersEnd();

            fixed_update_thread.request_stop();

            glfwTerminate();
        }
    };

    Window* Window::pGlobal = nullptr;
}

#endif