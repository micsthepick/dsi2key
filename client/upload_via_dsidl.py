import netifaces
import http.server
import qrcode
import shutil
import sys
import os

PORT = 8000
SERVED = False

if len(sys.argv) != 2:
    print("Usage: python script_name.py <file_path>")
    sys.exit(1)

FILE_PATH = sys.argv[1]


def get_local_ip():
    for interface in netifaces.interfaces():
        # Get the IPv4 configuration of the interface
        ipv4_addresses = netifaces.ifaddresses(interface).get(netifaces.AF_INET, [])

        for address in ipv4_addresses:
            ip = address['addr']

            # Check if the IP address belongs to one of the common subnets
            if ip.startswith('192.168.') or ip.startswith('10.') or ('172.' in ip and 16 <= int(ip.split('.')[1]) <= 31):
                return ip

    print("Could not determine local IP address within the most common subnets.")
    return None

import time

CHUNK_SIZE = 256
SEND_DELAY = 0.005  # delay in seconds

class CustomHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
         # If requested path doesn't match the FILE_PATH
        if self.path.strip('/') != FILE_PATH.strip('/'):
            self.send_error(404, "File not found")
            return
        path = self.translate_path(self.path)
        try:
            with open(path, 'rb') as f:
                self.send_response(200)
                self.send_header('Content-Type', 'application/octet-stream')  # Send as binary data
                self.send_header('Content-Length', os.path.getsize(path))  # Explicitly set content length
                self.end_headers()

                # Serve the file in chunks with a delay
                while True:
                    chunk = f.read(CHUNK_SIZE)
                    if not chunk:
                        break
                    self.wfile.write(chunk)
                    time.sleep(SEND_DELAY)
        except Exception as e:
            print("Error while serving the file:", str(e))

    # def end_headers(self):
    #     self.send_header('Content-Length', str(self.content_length))
    #     super().end_headers()

    # def send_response_only(self, code, message=None):
    #     self.content_length = int(self.headers.get('Content-Length', 0))
    #     super().send_response_only(code, message)

    # def do_GET(self):
    #     if self.path == f"/{FILE_PATH}":
    #         return super().do_GET()
    #     else:
    #         self.send_error(404, "File Not Found")

def display_qr_code(url):
    qr = qrcode.QRCode(version=1, error_correction=qrcode.constants.ERROR_CORRECT_L, box_size=1, border=4)
    qr.add_data(url)
    qr.make(fit=True)

    # Generate the QR code as a list of lists (2D list) where True represents filled blocks and False represents empty ones
    matrix = qr.get_matrix()

    ascii_art = ""
    SCALE_FACTOR = 1  # Change this to adjust the size

    for row in matrix:
        line = ''.join([' ' if cell else '█' for cell in row])  # '█' represents filled blocks, ' ' represents empty blocks
        # Stretch each line horizontally
        stretched_line = ''.join([char * SCALE_FACTOR*2 for char in line])
        # Duplicate each line vertically
        for _ in range(SCALE_FACTOR):
            ascii_art += stretched_line + "\n"

    print(ascii_art)


def main():
    local_ip = get_local_ip()
    url = f"http://{local_ip}:{PORT}/{FILE_PATH}"
    print(f"Serving {FILE_PATH} at {url}")

    display_qr_code(url)

    with http.server.HTTPServer((local_ip, PORT), CustomHTTPRequestHandler) as httpd:
        httpd.serve_forever()


if __name__ == "__main__":
    main()
