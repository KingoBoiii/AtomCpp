#pragma once

#include "Atom/Core/PlatformDetection.h"
#include "Atom/Core/Base.h"
#include "Atom/Core/Log.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <map>
#include <chrono>

#ifdef AT_PLATFORM_WINDOWS
	#include <Windows.h>
#endif