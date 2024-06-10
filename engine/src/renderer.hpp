#pragma once
#ifndef _TE_RENDERER_
#define _TE_RENDERER_

#include "window.hpp"
#include "load_model.hpp"

namespace te {
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