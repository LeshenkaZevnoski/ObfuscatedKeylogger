# ObfuscatedKeylogger

## Project Overview
There are numerous keyloggers written in C++ that are available within the Open Source community, as to strengthen more and take a deeper dive into what capabilities of a keylogger can benefit a Red Teamer and potentially has been used by other APT groups (Advanced Persistent Threat groups). This implementation incorporates advanced memory manipulation, obfuscation techniques, and secure data transmisison, pushing the regular boundaries of what such tools can achieve.
## Disclaimer
**Important:**  
This project is intended for educational and research purposes only. The author does not condone or support illegal or unethical use of this software.

## Features
- Logging keystrokes into a pre-defined memory region instead of writing them to a file, enabling the program to function solely within memory.
* ￼Reading keystrokes from memory and disguising them as IPv4 addresses.
* ￼Periodically sending keystrokes from the written memory region back to the attacker’s server within a specific time frame to divert suspicion. 

## Getting Started
### Prerequisites
- `CMake`, `libcurl`.

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/AlexZevnoski4060/ObfuscatedKeylogger.git
   cd ObfuscatedKeylogger
2. Create a build directory
   ```bash
   mkdir build
   cd build
   ```
3. Build the project
   ```bash
   cmake --build
   ```
The compiled executable will be located in:
`/Release/x64/Release/ObfuscatedKeylogger.exe`
