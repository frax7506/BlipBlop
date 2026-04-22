#pragma once

#include <cstring>

#define HD_SafeDelete(a) { delete a; a = 0; }
#define HD_SafeDeleteArray(a) { delete[] a; a = 0; }

#define HD_Move(a) static_cast<decltype(a)&&>(a)
#define HD_Unused(a) a;
