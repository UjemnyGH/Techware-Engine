#pragma once
#ifndef _TE_SCENE_
#define _TE_SCENE_

#include "layer.hpp"

namespace te {
    class Scene {
    protected:
        std::string mName;
        bool mInitializedDONTTOUCH = false;

    public:
        bool IsInitialized() { return mInitializedDONTTOUCH; }
        void __DONT_TOUCH_Initialize() { mInitializedDONTTOUCH = true; }

        void SetName(std::string name) { mName = name; }
        std::string GetName() { return mName; }

        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void FixedUpdate() {}
        virtual void End() {}
    };

    class SceneHandler : public Layer {
    private:
        std::vector<Scene*> mScenePtr;
        Scene* mCurrentScene = nullptr;

    public:
        static SceneHandler* pGlobal;

        SceneHandler() {
            SetFlag(LF_Update | LF_LateUpdate | LF_FixedUpdate | LF_End);
         
            mName = "SceneHandler" + std::to_string(GetNextGlobalNumber());
            
            if(!pGlobal) {
                mName = "SceneHandlerGlobal";
                mTag = "SceneHandlerGlobalTag";

                pGlobal = this;
                
                TE_INFO("Created global SceneHandler")
            }
            
            mType = "SCENE_HANDLER";
        }

        void AddScene(Scene* pScene) {
            mScenePtr.push_back(pScene);
        }

        void SwitchScene(Scene* pScene) {
            std::vector<Scene*>::iterator iter = std::find(mScenePtr.begin(), mScenePtr.end(), pScene);

            if(!(*iter)->IsInitialized()) {
                (*iter)->Awake();
                (*iter)->Start();
                (*iter)->__DONT_TOUCH_Initialize();
            }

            mCurrentScene = nullptr;
            mCurrentScene = *iter;
        }

        void SwitchScene(std::string name) {
            uint32_t i = 0;

            for(Scene* s : mScenePtr) {
                if(s->GetName() == name) {
                    break;
                }

                i++;
            }

            if(!mScenePtr[i]->IsInitialized()) {
                mScenePtr[i]->Awake();
                mScenePtr[i]->Start();
                mScenePtr[i]->__DONT_TOUCH_Initialize();
            }

            mCurrentScene = nullptr;
            mCurrentScene = mScenePtr[i];
        }

        virtual void Update() override {
            if(mCurrentScene) mCurrentScene->Update();
        }

        virtual void LateUpdate() override {
            if(mCurrentScene) mCurrentScene->LateUpdate();
        }

        virtual void FixedUpdate() override {
            if(mCurrentScene) mCurrentScene->FixedUpdate();
        }

        virtual void End() override {
            if(mCurrentScene) mCurrentScene->End();
        }
    };

    SceneHandler* SceneHandler::pGlobal = nullptr;
}

#endif