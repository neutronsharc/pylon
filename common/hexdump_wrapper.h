//
// Created by Shawn on 12/13/16.
//

#ifndef PYLON_HEXDUMP_WRAPPER_H
#define PYLON_HEXDUMP_WRAPPER_H

#include "hexdump.h"

using namespace std;

// hexdump a buffer into a string.
void HexDumpBuffer(void* buffer, int size, string* output);


#endif //PYLON_HEXDUMP_WRAPPER_H
