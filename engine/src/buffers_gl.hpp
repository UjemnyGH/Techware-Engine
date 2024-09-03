#pragma once
#ifndef _TE_BUFFERS_GL_
#define _TE_BUFFERS_GL_

#include "core.hpp"
#include <vector>

#include <fstream>

namespace te {
    typedef struct GLShader {
        uint32_t mId;

        /**
         * @brief Load shader from memory
         * 
         * @param src 
         * @param type 
         */
        void LoadShader(const char* src, uint32_t type) {
            mId = glCreateShader(type);
            glShaderSource(mId, 1, &src, nullptr);
            glCompileShader(mId);
        }

        
        void LoadShader(std::string file) {
            uint32_t type;
            
            if(file.find(".vert") != std::string::npos || file.find(".vs") != std::string::npos) {
                type = GL_VERTEX_SHADER;
            }
            else if(file.find(".frag") != std::string::npos || file.find(".fs") != std::string::npos) {
                type = GL_FRAGMENT_SHADER;
            }
            else if(file.find(".geom") != std::string::npos || file.find(".gs") != std::string::npos) {
                type = GL_GEOMETRY_SHADER;
            } 
            else if(file.find(".comp") != std::string::npos || file.find(".cs") != std::string::npos) {
                type = GL_COMPUTE_SHADER;
            }
            else if(file.find(".tesc") != std::string::npos || file.find(".tcs") != std::string::npos) {
                type = GL_TESS_CONTROL_SHADER;
            }
            else if(file.find(".tese") != std::string::npos || file.find(".tes") != std::string::npos) {
                type = GL_TESS_EVALUATION_SHADER;
            }
            else {
                type = GL_VERTEX_SHADER;
            }

            std::ifstream f(file, std::ios::binary | std::ios::ate);

            size_t len = f.tellg();
            f.seekg(0, std::ios::beg);

            std::string src;
            src.resize(len);

            f.read(src.data(), src.size());

            f.close();

            LoadShader(src.c_str(), type);
        }

        ~GLShader() {
            if(mId != 0) {
                glDeleteShader(mId);

                mId = 0;
            } 
        }
    } GLShader;

    typedef struct GLProgram {
        uint32_t mId;
        bool mCreated = false;

        void Init() {
            if(!mCreated) {
                mId = glCreateProgram();

                mCreated = true;
            }
        }

        void Use() {
            Init();

            glUseProgram(mId);
        }

        void Unuse() {
            glUseProgram(0);
        }

        void Attach(GLShader sh) {
            Init();

            glAttachShader(mId, sh.mId);
        }

        void Link() {
            Init();

            glLinkProgram(mId);
        }
        
        ~GLProgram() {
            if(mCreated) {
                glDeleteProgram(mId);

                mCreated = false;
            }
        }
    } GLProgram;

    typedef struct GLArray {
        uint32_t mId;
        bool mCreated = false;

        void Init() {
            if(!mCreated) {
                glGenVertexArrays(1, &mId);

                mCreated = true;
            }
        }

        void Bind() {
            Init();

            glBindVertexArray(mId);
        }

        void Unbind() {
            glBindVertexArray(0);
        }

        ~GLArray() {
            if(mCreated) {
                glDeleteVertexArrays(1, &mId);

                mCreated = false;
            }
        }
    } GLArray;

    typedef struct GLBufferPtrData {
        size_t mIndex, mDimmension, mSize, mOffset;
        uint32_t mType = GL_FLOAT;

        GLBufferPtrData(size_t index, size_t dimm, size_t size, size_t offset) : mIndex(index), mDimmension(dimm), mSize(size), mOffset(offset) {}
    } GLBufferPtrData;

    typedef struct GLBuffer {
        uint32_t mId;
        bool mCreated = false;

        void Init() {
            if(!mCreated) {
                glGenBuffers(1, &mId);

                mCreated = true;
            }
        }

        void Bind() {
            Init();

            glBindBuffer(GL_ARRAY_BUFFER, mId);
        }

        void BindPlace(uint32_t index, uint32_t dimm) {
            Bind();

            glVertexAttribPointer(index, dimm, GL_FLOAT, 0, 0, nullptr);
            glEnableVertexAttribArray(index);
        }

        void BindPtr(std::vector<GLBufferPtrData> data) {
            Bind();

            for(GLBufferPtrData & d : data) {
                glVertexAttribPointer(d.mIndex, d.mDimmension, d.mType, GL_FALSE, d.mSize, (void*)d.mOffset);
                glEnableVertexAttribArray(d.mIndex);
            }
        }

        void BindData(std::vector<float> data) {
            Bind();

            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);
        }

        ~GLBuffer() {
            if(mCreated) {
                glDeleteBuffers(1, &mId);

                mCreated = false;
            }
        }
    } GLBuffer;

    typedef struct GLUniformBuffer {
        uint32_t mId;
        bool mCreated = false;

        void Init() {
            if(!mCreated) {
                glGenBuffers(1, &mId);

                mCreated = true;
            }
        }

        void Bind() {
            Init();

            glBindBuffer(GL_UNIFORM_BUFFER, mId);
        }

        void Unbind() {
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        void Allocate(uint32_t bytes) {
            Bind();

            glBufferData(GL_UNIFORM_BUFFER, bytes, nullptr, GL_DYNAMIC_DRAW);
            
            Unbind();
        }

        void UniformBinding(uint32_t binding) {
            Bind();
            
            glBindBufferBase(GL_UNIFORM_BUFFER, binding, mId);

            Unbind();
        }

        void BindData(uint32_t offset, uint32_t size, void* ptr) {
            Bind();

            glBufferSubData(GL_UNIFORM_BUFFER, offset, size, ptr);

            Unbind();
        }

        ~GLUniformBuffer() {
            if(mCreated) {
                glDeleteBuffers(1, &mId);

                mCreated = false;
            }
        }
    } GLUniformBuffer;

    typedef struct GLTexture {
        uint32_t mId;
        bool mCreated = false;

        void Init() {
            if(!mCreated) {
                glGenTextures(1, &mId);

                mCreated = true;
            }
        }

        void Bind() {
            Init();

            glBindTexture(GL_TEXTURE_2D_ARRAY, mId);
        }

        void BindUnit(uint32_t unit) {
            Init();

            glBindTextureUnit(unit, mId);
        }

        void BindData(std::vector<uint8_t> data, uint32_t width, uint32_t height, uint32_t layers) {
            Bind();

            glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, layers);
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layers, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 32);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        ~GLTexture() {
            if(mCreated) {
                glGenTextures(1, &mId);

                mCreated = false;
            }
        }
    } GLTexture;

    typedef struct GLFramebuffer {
        uint32_t mId, mTId;
        bool mCreated = false, mTCreated = false;

        void Init() {
            if(!mCreated) {
                glGenFramebuffers(1, &mId);

                mCreated = true;
            }
        }

        void Bind() {
            Init();

            glBindFramebuffer(GL_FRAMEBUFFER, mId);
        }

        void Unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void GetDepthStencil(int width, int height) {
            Bind();

            if(!mTCreated) {
                glGenTextures(1, &mTId);

                mTCreated = true;
            }

            glBindTexture(GL_TEXTURE_2D, mTId);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mTId, 0);

            Unbind();
        }

        void GetDepth(int width, int height) {
            Bind();

            if(!mTCreated) {
                glGenTextures(1, &mTId);

                mTCreated = true;
            }

            glBindTexture(GL_TEXTURE_2D, mTId);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTId, 0);

            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            Unbind();
        }

        void GetColor(int width, int height) {
            Bind();

            if(!mTCreated) {
                glGenTextures(1, &mTId);

                mTCreated = true;
            }

            glBindTexture(GL_TEXTURE_2D, mTId);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTId, 0);

            Unbind();
        }

        ~GLFramebuffer() {
            if(mCreated) {
                glDeleteFramebuffers(1, &mId);
            
                mCreated = false;
            }

            if(mTCreated) {
                glDeleteTextures(1, &mTId);

                mTCreated = false;
            }
        }
    } GLFramebuffer;
}

#endif