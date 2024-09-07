#pragma once

#include "nn.hpp"

#define PRINT(...)                                         \
	{                                                      \
		int len = nn::util::SNPrintf(buf, sizeof(buf), __VA_ARGS__); \
		svcOutputDebugString(buf, len);                    \
	}        
    