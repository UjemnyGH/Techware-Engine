#define GLAD_GL_IMPLEMENTATION
#include "engine/vendor/linux/include/glad/gl.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <vector>

uint32_t LoadShader(const char* src, uint32_t type) {
    uint32_t sh = glCreateShader(type);
    glShaderSource(sh, 1, &src, nullptr);
    glCompileShader(sh);

    return sh;
}

const char* gVShader =
"#version 450 core\n"
"layout(location = 0) in vec4 iPos;\n"
"layout(location = 1) in vec4 iCol;\n"
"out vec4 vCol;\n"
"void main() {\n"
"   gl_Position = iPos;\n"
"   vCol = iCol;\n}\n";

const char* gFShader = 
"#version 450 core\n"
"in vec4 vCol;\n"
"out vec4 oCol;\n"
"void main() {\n"
"   oCol = vCol;\n}\n";

#define MAX_DATA 2000

float data[] = {
    0.5f, 0.5f,
        // 0.9f, 0.0f, 0.0f, 0.9f,

    -0.5f, 0.5f,
        // 0.0f, 0.9f, 0.0f, 0.9f,

    0.5f, -0.5f,
        // 0.0f, 0.0f, 0.9f, 0.9f,

    -0.5f, 0.5f,
        // 0.9f, 0.0f, 0.9f, 0.9f,

    0.5f, -0.5f,
        // 0.0f, 0.9f, 0.9f, 0.9f,

    -0.5f, -0.5f,
        // 0.0f, 0.0f, 0.9f, 0.9f


    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

std::vector<float> dataV;

int main() {
    for(size_t i = 0; i < MAX_DATA; i++) {
        dataV.push_back(0.5f);
        dataV.push_back(0.5f);
        dataV.push_back(-0.5f);
        dataV.push_back(0.5f);
        dataV.push_back(0.5f);
        dataV.push_back(-0.5f);
        dataV.push_back(-0.5f);
        dataV.push_back(0.5f);
        dataV.push_back(0.5f);
        dataV.push_back(-0.5f);
        dataV.push_back(-0.5f);
        dataV.push_back(-0.5f);
    }

    for(size_t i = 0; i < MAX_DATA; i++) {
        dataV.push_back(1.0f);
        dataV.push_back(0.0f);
        dataV.push_back(0.0f);
        dataV.push_back(1.0f);
        dataV.push_back(0.0f);
        dataV.push_back(1.0f);
        dataV.push_back(0.0f);
        dataV.push_back(1.0f);
        dataV.push_back(0.0f);
        dataV.push_back(0.0f);
        dataV.push_back(1.0f);
        dataV.push_back(1.0f);
        dataV.push_back(0.0f);
        dataV.push_back(1.0f);
        dataV.push_back(0.0f);
        dataV.push_back(1.0f);
        dataV.push_back(0.0f);
        dataV.push_back(0.0f);
        dataV.push_back(1.0f);
        dataV.push_back(1.0f);
        dataV.push_back(0.0f);
        dataV.push_back(0.0f);
        dataV.push_back(0.0f);
        dataV.push_back(1.0f);
    }

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* wnd = glfwCreateWindow(800, 600, "Window", nullptr, nullptr);

    if(!wnd) {
        return -1;
    }

    glfwMakeContextCurrent(wnd);

    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        return -2;
    }

    uint32_t 
    vs = LoadShader(gVShader, GL_VERTEX_SHADER),
    fs = LoadShader(gFShader, GL_FRAGMENT_SHADER);

    uint32_t sp = glCreateProgram();
    uint32_t vao, vbo;

    glAttachShader(sp, vs);
    glAttachShader(sp, fs);
    glLinkProgram(sp);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dataV[0]) * dataV.size(), dataV.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, 0, sizeof(float) * 2, (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, 0, sizeof(float) * 4, (void*)(MAX_DATA * sizeof(float) * 12));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // glVertexAttribDivisor(0, 1);
    // glVertexAttribDivisor(1, 1);

    double l, c, d;
    size_t elements = 1;

    while(!glfwWindowShouldClose(wnd)) {
        c = glfwGetTime();
        d = c - l;
        l = c;

        printf("%f FPS\n", 1.0 / d);

        // if(1.0 / d < 60.0 && elements > 10) {
        //     printf("%ld\n", elements);
            
        //     break;
        // }
        // else {
        //     elements++;
        // }

        glClear(0x4100);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glUseProgram(sp);
        glBindVertexArray(vao);

        // for(size_t i = 0; i < elements; i++) {
            glDrawArrays(GL_TRIANGLES, 0, MAX_DATA * 12);
        // }

        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(wnd);

        glfwPollEvents();

        glfwSwapInterval(0);
    }

    glfwTerminate();

    return 0;
}