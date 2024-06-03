#pragma once
#ifndef _TE_LAYER_
#define _TE_LAYER_

#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include "core.hpp"

namespace te {
    enum LayerFlags {
        LF_Awakend = 0x1,
        LF_Started = 0x2,
        LF_Awake = 0x4,
        LF_Start = 0x8,
        LF_Update = 0x10,
        LF_LateUpdate = 0x20,
        LF_FixedUpdate = 0x40,
        LF_End = 0x80
    };

    class Layer {
    protected:
        std::string mName = "N_DEF";
        std::string mTag = "T_DEF";
        std::string mType = "T_LAYER";

        uint8_t mFlags;

    public:
        /**
         * @brief Set the Flags
         * 
         * @param flags 
         */
        void SetFlag(uint8_t flags) { mFlags = flags; }

        /**
         * @brief Get the Flags
         * 
         * @return uint8_t 
         */
        uint8_t GetFlags() { return mFlags; }

        void SetName(std::string name) { mName = name; }
        void SetTag(std::string tag) { mTag = tag; }

        std::string GetName() { return mName; }
        std::string GetTag() { return mTag; }
        std::string GetType() { return mType; }

        /**
         * @brief Awake layer
         * 
         */
        virtual void Awake() {}

        /**
         * @brief Start layer
         * 
         */
        virtual void Start() {}

        /**
         * @brief Update layer
         * 
         */
        virtual void Update() {}

        /**
         * @brief Late update layer
         * 
         */
        virtual void LateUpdate() {}

        /**
         * @brief Update layer but on another thread
         * 
         */
        virtual void FixedUpdate() {}

        /**
         * @brief Ends layer
         * 
         */
        virtual void End() {}
    };

    class LayerHandler {
    private:
        std::vector<Layer*> mLayerPtr;

    public:
        /**
         * @brief Global static pointer to main LayerHandler
         * 
         */
        static LayerHandler* pGlobal;

        LayerHandler() {
            if(!pGlobal) {
                pGlobal = this;

                TE_INFO("Created global LayerHandler")
            }
            else {
                TE_WARN("Cannot create another global LayerHandler!")
            }
        }

        void AddLayer(Layer* pLayer) {
            mLayerPtr.push_back(pLayer);
        }

        void RemoveLayer(Layer* pLayer) {
            std::vector<Layer*>::iterator iter = std::find(mLayerPtr.begin(), mLayerPtr.end(), pLayer);

            mLayerPtr.erase(iter);
        }

        void RemoveLayer(std::string name) {
            size_t i = 0;
            
            for(Layer* l : mLayerPtr) {
                if(l->GetName() == name) {
                    break;
                }

                i++;
            }

            mLayerPtr.erase(mLayerPtr.begin() + i);
        }

        void RemoveLayerByTag(std::string tag) {
            size_t i = 0;
            
            for(Layer* l : mLayerPtr) {
                if(l->GetTag() == tag) {
                    break;
                }

                i++;
            }

            mLayerPtr.erase(mLayerPtr.begin() + i);
        }

        /**
         * @brief Awakes all added layers
         * 
         */
        void LayersAwake() {
            for(Layer* l : mLayerPtr) {
                if(l->GetFlags() & LF_Awake && !(l->GetFlags() & LF_Awakend)) {
                    l->Awake();

                    l->SetFlag(l->GetFlags() | LF_Awakend);
                }
            }
        }

        /**
         * @brief Start all added layers
         * 
         */
        void LayersStart() {
            for(Layer* l : mLayerPtr) {
                if(l->GetFlags() & LF_Start && !(l->GetFlags() & LF_Start)) {
                    l->Start();

                    l->SetFlag(l->GetFlags() | LF_Start);
                }
            }
        }

        /**
         * @brief Update all added layers
         * 
         */
        void LayersUpdate() {
            for(Layer* l : mLayerPtr) {
                if(l->GetFlags() & LF_Update) {
                    l->Update();
                }
            }
        }

        /**
         * @brief Late update all added layers
         * 
         */
        void LayersLateUpdate() {
            for(Layer* l : mLayerPtr) {
                if(l->GetFlags() & LF_LateUpdate) {
                    l->LateUpdate();
                }
            }
        }

        /**
         * @brief Fixed update all added layers
         * 
         */
        void LayersFixedUpdate() {
            for(Layer* l : mLayerPtr) {
                if(l->GetFlags() & LF_FixedUpdate) {
                    l->FixedUpdate();
                }
            }
        }

        /**
         * @brief End all added layers
         * 
         */
        void LayersEnd() {
            for(Layer* l : mLayerPtr) {
                if(l->GetFlags() & LF_End) {
                    l->End();
                }
            }
        }
    };

    LayerHandler* LayerHandler::pGlobal = nullptr;
}

#endif