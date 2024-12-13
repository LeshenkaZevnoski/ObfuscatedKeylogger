"""
ObfuscatedKeylogger.py
Author: Alex Zevnoski (Leshenka)
Copyright (c) 2024 Alex Zevnoski (Leshenka)
This file is part of the ObfuscatedKeylogger project, an educational
and experimental project demonstrating obfuscation and data transmission techniques.

DISCLAIMER: This software is intended solely for educational purposes.
The author does not condone or support illegal or unethical use of this software.
"""

import ipaddress

def deobfuscate_ipv4(server_logs):
    deobfuscated_data = bytearray()
    
    for ip_str in server_logs:
        try:
            # Convert IP address to its integer octets
            ip = ipaddress.ip_address(ip_str)
            octets = ip.packed
            
            # Add each octet to the deobfuscated data
            deobfuscated_data.extend(octets)
        
        except ValueError as e:
            print(f"Invalid IP address {ip_str}: {e}")
    
    return bytes(deobfuscated_data)

def main():
    with open("ipv4_keystrokes.txt", "r") as file:
        server_logs = file.read().splitlines()
    
    keylog_data = deobfuscate_ipv4(server_logs)
    
    print("Deobfuscated Data (Hex):", keylog_data.hex())
    
    # Optional: Try to decode as text if applicable
    try:
        print("Deobfuscated Data (Text):", keylog_data.decode('utf-8', errors='replace'))
    except UnicodeDecodeError:
        print("Unable to decode data as UTF-8 text.")

if __name__ == "__main__":
    main()