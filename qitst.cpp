#define GLAD_GL_IMPLEMENTATION
#include "engine/vendor/linux/include/glad/gl.h"
#include <GLFW/glfw3.h>
#include <iostream>

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

struct V2 {
    float x, y;
};

struct V4 {
    float x, y, z, w;
};

int main() {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, 0, sizeof(float) * 2, (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, 0, sizeof(float) * 4, (void*)48);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // glVertexAttribDivisor(0, 1);
    // glVertexAttribDivisor(1, 1);

    while(!glfwWindowShouldClose(wnd)) {
        glClear(0x4100);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glUseProgram(sp);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(wnd);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}