#pragma once
#ifndef _TE_LOAD_PNG_
#define _TE_LOAD_PNG_

#include "core.hpp"
#include <vector>
#include <fstream>

namespace te {
    /**
     * @brief Copy memory (bytes) by given size specified by T, by T it means it can operate on arbitrary sizes
     * 
     * @tparam T type of returned data (mainly for size)
     * @param ptr pointer to bytes to convert
     * @return T our data returned by desired type
     */
    template<class T>
    T PNGCopyMemoryToSizeOf(void* ptr) {
        return *(T*)ptr;
    }

    std::vector<uint8_t> PNGLoadPNGFile(std::string path, uint32_t* pWidth, uint32_t* pHeight) {
        std::vector<uint8_t> result;

        std::ifstream fpng(path, std::ios::binary | std::ios::ate);

        size_t fpng_length = fpng.tellg();
        fpng.seekg(0, std::ios::beg);

        std::string fpng_data;
        fpng_data.resize(fpng_length);
        fpng.read(fpng_data.data(), fpng_data.size());
        fpng.close();

        if(PNGCopyMemoryToSizeOf<uint64_t>((void*)&fpng_data[0]) == 0x89504E470D0A1A0AUL)

        for(size_t i = 0; i < fpng_data.size(); i++) {
            
        }
    }
}

#endif