#pragma once
#ifndef _TE_CORE_
#define _TE_CORE_

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace te {
    #define TE_ERR(msg) std::cout << "[ERROR]: " << __FILE__ << ":" << __LINE__ << " @ " << __FUNCTION__ << "() > " << msg << std::endl; 
    #define TE_INFO(msg) std::cout << "[INFO]: " << __FILE__ << ":" << __LINE__ << " @ " << __FUNCTION__ << "() > " << msg << std::endl; 
    #define TE_WARN(msg) std::cout << "[WARN]: " << __FILE__ << ":" << __LINE__ << " @ " << __FUNCTION__ << "() > " << msg << std::endl; 

    uint64_t gNextNumber = 0;

    uint64_t GetNextGlobalNumber() { return gNextNumber++; }
}

#endif