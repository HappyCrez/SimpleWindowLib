#ifndef PCH_H
#define _CRT_SECURE_NO_WARNINGS
#define PCH_H

#ifdef SIMPLEWINDOW_EXPORTS
#define simple_window_api __declspec(dllexport)
#else
#define simple_window_api __declspec(dllimport)
#endif

#include "framework.h"
#include "MathSWM.h"
#include <string>
#include <iostream>
#include <queue>
#include <stdint.h>


#endif //PCH_H
