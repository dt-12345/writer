#pragma once

#include <util.h>

#define PRINT(...)                                         \
	{                                                      \
		int len = nn::util::SNPrintf(buf, sizeof(buf), __VA_ARGS__); \
		svcOutputDebugString(buf, len);                    \
	}

#define EXPORT __attribute__((visibility("default")))