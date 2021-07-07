import socket

CLIENT_PORT = 14000
SERVER_PORT = 13000

def client(IMSI='0'):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('127.0.0.1', CLIENT_PORT))
        s.sendall(IMSI.encode())
        print('Sent data: ' + IMSI)

def server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(('127.0.0.1', SERVER_PORT))
        s.listen()
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                print('Received data: ' + data.decode())
