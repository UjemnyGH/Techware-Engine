#pragma once
#ifndef _TS_ENTRY_POINT_
#define _TS_ENTRY_POINT_

#include "window.hpp"

te::Window gWindow;

int main() {
    gWindow.Run("Techware Engine", 800, 600);

    return 0;
}

#endif