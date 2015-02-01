#pragma once

#include <iostream>
#include <cstdint>

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ "<><.> ======= " __FILE__ "("__STR1__(__LINE__)") : WARNING: "

#define ARRAY_LENGTH(ar) ((sizeof((ar)))/(sizeof((ar[0])))

#define local_persist		static
#define global				static
#define bglinternal            static

