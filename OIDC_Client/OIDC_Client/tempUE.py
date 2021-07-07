from socket import *

clientIP = '127.0.0.1'
clientPort = 10000

serverIP = '127.0.0.1'
serverPort = 39999

with socket(AF_INET, SOCK_STREAM) as serverSocket:
    serverSocket.bind((serverIP, serverPort))
    serverSocket.listen()
    while True:
        if input("Press s to start server:") == 's':
            with socket(AF_INET, SOCK_STREAM) as clientSocket:
                clientSocket.connect((clientIP, clientPort))
                imsi = '320230100000025'
                clientSocket.sendall(imsi.encode())
                clientSocket.close()
            conn, addr = serverSocket.accept()
            with conn:
                data = conn.recv(2048)
                if not data:
                    break
                else:
                    data = data.decode('utf-8')
                    print(data)
                    if data == '200 OK':
                        print("User authenticated")
                    else:
                        with socket(AF_INET, SOCK_STREAM) as clientSocket:
                            clientSocket.connect((clientIP, clientPort))
                            res = 'a54211d5e3ba50bf'
                            clientSocket.sendall(res.encode())
                            clientSocket.close()