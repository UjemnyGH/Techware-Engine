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
        bool mInitializeWithWindow = false;
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
            SetFlag(LF_Update | LF_LateUpdate | LF_FixedUpdate | LF_End | LF_Awake | LF_Start);
         
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
            TE_INFO("Switching scene to " << pScene->GetName() << " @ " << pScene)
            std::vector<Scene*>::iterator iter = std::find(mScenePtr.begin(), mScenePtr.end(), pScene);

            if(!(*iter)->IsInitialized() && !(*iter)->mInitializeWithWindow) {
                TE_INFO("Scene ain`t initialized, trying to initialize scene!")

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

            TE_INFO("Switching scene to " << mScenePtr[i]->GetName() << " @ " << mScenePtr[i])

            if(!mScenePtr[i]->IsInitialized() && !mScenePtr[i]->mInitializeWithWindow) {
                TE_INFO("Scene ain`t initialized, trying to initialize scene!")

                mScenePtr[i]->Awake();
                mScenePtr[i]->Start();
                mScenePtr[i]->__DONT_TOUCH_Initialize();
            }

            mCurrentScene = nullptr;
            mCurrentScene = mScenePtr[i];
        }

        virtual void Awake() override {
            if(mCurrentScene && mCurrentScene->mInitializeWithWindow) mCurrentScene->Awake();
        }

        virtual void Start() override {
            if(mCurrentScene && mCurrentScene->mInitializeWithWindow) mCurrentScene->Start();
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