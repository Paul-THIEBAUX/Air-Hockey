#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../../lib/cpulib/lib/cpu-core-debug.lib")
#pragma comment(lib, "../../lib/cpulib/lib/cpu-render-debug.lib")
#pragma comment(lib, "../../lib/cpulib/lib/cpu-engine-debug.lib")
#else
#pragma comment(lib, "../../lib/cpulib/lib/cpu-core.lib")
#pragma comment(lib, "../../lib/cpulib/lib/cpu-render.lib")
#pragma comment(lib, "../../lib/cpulib/lib/cpu-engine.lib")
#endif

#include <cpu-engine/cpu-engine.h>

#include "App.h"
