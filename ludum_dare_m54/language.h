#pragma once

#include <cstdint>

#define ARRAY_LENGTH(ar) ((sizeof((ar)))/(sizeof((ar[0])))

#define local_persist		static
#define global				static
#define internal			static

