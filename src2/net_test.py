import socket

def main():
    host = '127.0.0.1'
    port = 5555

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind((host, port))
        server_socket.listen()

        print(f"Server is listening on {host}:{port}")

        while True:
            client_socket, addr = server_socket.accept()
            with client_socket:
                data = b"B"
                client_socket.sendall(data)
                data = b"A"
                client_socket.sendall(data)

                print(f"Connected by {addr}")
                data = client_socket.recv(1024)
                if data:
                    print(f"Raw data: {data}")
                    print(f"Received: {data.decode('utf-8')}")
                else:
                    print("No data received.")
                print("Closing client connection.")

if __name__ == "__main__":
    main()

