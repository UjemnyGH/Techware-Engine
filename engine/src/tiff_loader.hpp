#pragma once
#ifndef _TE_TIFF_LOADER_
#define _TE_TIFF_LOADER_

#include <vector>
#include <string>
#include <cstdint>
#include "core.hpp"
#include <fstream>

namespace te {
    typedef struct TIFF_Ifd {
        uint16_t mDirEntries;
        std::vector<uint16_t> mTagsId;
        std::vector<uint16_t> mTagsDataType;
        std::vector<uint32_t> mTagsDataCount;
        std::vector<uint32_t> mTagsDataOffset;
        uint32_t mNextOffset;
    } TIFF_Ifd;

    typedef struct TIFF {
        bool mLittleEndian;
        uint32_t mOffset;
        uint32_t mWidth, mHeight;

        std::vector<TIFF_Ifd> mIfds;
        
        void Load(std::string path) {
            std::ifstream file(path, std::ios::binary | std::ios::ate);

            size_t len = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<uint8_t> source;
            source.resize(len);

            file.read((char*)source.data(), source.size());

            file.close();

            if(source[0] == 0x49) {
                mLittleEndian = true;
                mOffset = source[4] | source[5] << 8 | source[6] << 16 | source[7] << 24;
            }
            else {
                mLittleEndian = false;
                mOffset = source[7] | source[6] << 8 | source[5] << 16 | source[4] << 24;
            }
        }

    } TIFF;
}

#endif