#pragma once

#define HD_SafeDelete(a) { delete a; a = 0; }
#define HD_SafeDeleteArray(a) { delete[] a; a = 0; }
