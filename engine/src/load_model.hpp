#pragma once
#ifndef _TE_LOAD_MODEL_
#define _TE_LOAD_MODEL_

#include "core.hpp"
#include "ul_mesh.hpp"
#include "buffers_gl.hpp"

namespace te {
    struct ModelData {
        std::vector<float> mVertices, mTextureCoordinates, mNormals;
    };

    ModelData MLoadModel(std::string path) {
        ul_mesh_t mesh;
        uint32_t type = ULMtype_END_DONT_USE;

        if(path.find(".ply") != -1) type = ULMtype_ply;
        else if(path.find(".obj") != -1) type = ULMtype_obj;
        else if(path.find(".stl") != -1) type = ULMtype_stl;
        else {
            TE_WARN("Cannot find desired model extension type in \"" << path << "\", returning nothing!")

            return ModelData();
        }

        ulMeshLoad(&mesh, path.c_str(), type);

        ModelData result;

        std::copy(mesh.vertices.begin(), mesh.vertices.end(), std::back_inserter(result.mVertices));
        std::copy(mesh.normals.begin(), mesh.normals.end(), std::back_inserter(result.mNormals));
        std::copy(mesh.textureCoordinates.begin(), mesh.textureCoordinates.end(), std::back_inserter(result.mTextureCoordinates));

        return result;
    }

    struct RenderData {
        std::vector<ModelData> mOriginalModelData;
        std::vector<float> mTextureId;
        std::vector<float> mTextureLayer;
        std::vector<float> mColors;
        std::vector<float> mJoinedData;
        size_t mVerticesSize, mTextureCoordsSize, mNormalsSize, mTextureIdSize, mTextureLayerSize, mColorSize;
        size_t mVerticesOffset, mTextureCoordsOffset, mNormalsOffset, mTextureIdOffset, mTextureLayerOffset, mColorOffset;

        GLArray mArray;
        GLBuffer mBuffer;

        void Rebind() {
            mArray.Bind();

            mBuffer.BindData(mJoinedData);

            mBuffer.BindPtr(std::vector<GLBufferPtrData>({
                (GLBufferPtrData){0, 3, mVerticesSize, mVerticesOffset},
                (GLBufferPtrData){1, 2, mTextureCoordsSize, mTextureCoordsOffset},
                (GLBufferPtrData){2, 3, mNormalsSize, mNormalsOffset},
                (GLBufferPtrData){3, 1, mTextureIdSize, mTextureIdOffset},
                (GLBufferPtrData){4, 1, mTextureLayerSize, mTextureLayerOffset},
                (GLBufferPtrData){5, 4, mColorSize, mColorOffset}
            }));

            mArray.Unbind();
        }

        void AddModel(ModelData md) {
            mOriginalModelData.push_back(md);

            for(size_t i = 0; i < md.mVertices.size() / 3; i++) {
                mTextureId.push_back(32.0f);
                mTextureLayer.push_back(0.0f);
                mColors.push_back(1.0f);
                mColors.push_back(1.0f);
                mColors.push_back(1.0f);
                mColors.push_back(1.0f);
            }
        }

        void Join() {
            mJoinedData.clear();

            mVerticesSize = 3 * sizeof(float);
            mTextureCoordsSize = 2 * sizeof(float);
            mNormalsSize = 3 * sizeof(float);
            mTextureIdSize = sizeof(float);
            mTextureLayerSize = sizeof(float);
            mColorSize = 4 * sizeof(float);

            mVerticesOffset = mJoinedData.size() * sizeof(float);

            for(size_t i = 0; i < mOriginalModelData.size(); i++) {
                std::copy(mOriginalModelData[i].mVertices.begin(), mOriginalModelData[i].mVertices.end(), std::back_inserter(mJoinedData));
            }

            mTextureCoordsOffset = mJoinedData.size() * sizeof(float);

            for(size_t i = 0; i < mOriginalModelData.size(); i++) {
                std::copy(mOriginalModelData[i].mTextureCoordinates.begin(), mOriginalModelData[i].mTextureCoordinates.end(), std::back_inserter(mJoinedData));
            }

            mNormalsOffset = mJoinedData.size() * sizeof(float);

            for(size_t i = 0; i < mOriginalModelData.size(); i++) {
                std::copy(mOriginalModelData[i].mNormals.begin(), mOriginalModelData[i].mNormals.end(), std::back_inserter(mJoinedData));
            }

            mTextureIdOffset = mJoinedData.size() * sizeof(float);

            std::copy(mTextureId.begin(), mTextureId.end(), std::back_inserter(mJoinedData));

            mTextureLayerOffset = mJoinedData.size() * sizeof(float);

            std::copy(mTextureLayer.begin(), mTextureLayer.end(), std::back_inserter(mJoinedData));

            mColorOffset = mJoinedData.size() * sizeof(float);

            std::copy(mColors.begin(), mColors.end(), std::back_inserter(mJoinedData));
        }

        void Delete() {
            mArray.~GLArray();
            mBuffer.~GLBuffer();
        }

        ~RenderData() {
            Delete();
        }
    };
}

#endif