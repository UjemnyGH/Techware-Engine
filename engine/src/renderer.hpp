#pragma once
#ifndef _TE_RENDERER_
#define _TE_RENDERER_

#include "window.hpp"
#include "load_model.hpp"
#include <string>

namespace te {
    class SimpleRenderer : public Layer {
    private:
        std::vector<RenderedModel*> mModelPtr;
        GLProgram mShaderProgram;

    public:
        SimpleRenderer(std::string name) {
            SetFlag(LF_Update | LF_Start);
            SetName(name);
            SetTag("default");
            mType = "SIMPLE_RENDERER";
            LayerHandler::pGlobal->AddLayer(this);
        }

        GLProgram* operator()() { return &mShaderProgram; }
        void AddModel(RenderedModel* pModel) {
            mModelPtr.push_back(pModel);
        }

        void RemoveModel(RenderedModel* pModel) {
            std::vector<RenderedModel*>::iterator iter = std::find(mModelPtr.begin(), mModelPtr.end(), pModel);

            if(iter.base()) {
                mModelPtr.erase(iter);
            }
        }

        void AttachShaders(std::vector<GLShader> shaders) {
            mShaderProgram.~GLProgram();

            for(GLShader & shader : shaders) {
                mShaderProgram.Attach(shader);
            }

            mShaderProgram.Link();
        }

        virtual void Update() override {
            if(mShaderProgram.mCreated) {
                TE_INFO("Rendering " << 1.0 / Window::pGlobal->mDeltaTime)
                for(RenderedModel* model : mModelPtr) {
                    mShaderProgram.Use();
                    model->mArray.Bind();

                    model->mUsedTexturePtr->Bind();
                    model->UniformShaderProgram(&mShaderProgram);
                    model->BindBuffer();

                    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram.mId, "uTransform"), 1, GL_FALSE, model->mTransform.GetTransform().m);

                    glDrawArrays(GL_TRIANGLES, 0, model->mTextureCoordsOffset / sizeof(float));

                    model->mArray.Unbind();
                    mShaderProgram.Unuse();
                }
            }
        }
    };

    class Renderer : public Layer {
    private:
        RenderData* mRenderDataPtr;

    public:
        GLProgram mShaderProgram;

        void SetRenderData(RenderData* pRD) {
            mRenderDataPtr = pRD;
        }

        void AttachShaders(std::vector<GLShader> shaders) {
            mShaderProgram.~GLProgram();

            for(GLShader & shader : shaders) {
                mShaderProgram.Attach(shader);
            }

            mShaderProgram.Link();
        }

        virtual void Update() override {
            if(mShaderProgram.mCreated) {
                mShaderProgram.Use();
                mRenderDataPtr->mArray.Bind();

                glDrawArrays(GL_TRIANGLES, 0, mRenderDataPtr->mTextureCoordsOffset / sizeof(float));

                mRenderDataPtr->mArray.Unbind();
                mShaderProgram.Unuse();
            }
        }
    };
}

#endif