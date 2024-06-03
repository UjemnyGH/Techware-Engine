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
            
            if(file.find(".vert") != -1 || file.find(".vs") != -1) {
                type = GL_VERTEX_SHADER;
            }
            else if(file.find(".frag") != -1 || file.find(".fs") != -1) {
                type = GL_FRAGMENT_SHADER;
            }
            else if(file.find(".geom") != -1 || file.find(".gs") != -1) {
                type = GL_GEOMETRY_SHADER;
            } 
            else if(file.find(".comp") != -1 || file.find(".cs") != -1) {
                type = GL_COMPUTE_SHADER;
            }
            else if(file.find(".tesc") != -1 || file.find(".tcs") != -1) {
                type = GL_TESS_CONTROL_SHADER;
            }
            else if(file.find(".tese") != -1 || file.find(".tes") != -1) {
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
}

#endif