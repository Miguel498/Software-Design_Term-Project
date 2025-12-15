// Cross-platform stdafx.h (precompiled header alternative)

#pragma once

// Standard libraries
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>

// If compiling on Windows
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

using namespace std;
