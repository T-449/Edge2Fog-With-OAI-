import time
from socket import *

clientIP = '127.0.0.1'
clientPort = 10000

serverIP = '127.0.0.1'
serverPort = 39999

authServer_IP = '127.0.0.1'
authServer_Port = 10500

authResponse_IP = '127.0.0.1'
authResponse_Port = 10600

giveIMSI = 1

with socket(AF_INET, SOCK_STREAM) as serverSocket:
    serverSocket.bind((serverIP, serverPort))
    serverSocket.listen()
    while True:
        if giveIMSI == 1:
            input('Press any key to send IMSI : ')
            with socket(AF_INET, SOCK_STREAM) as clientSocket:
                clientSocket.connect((clientIP, clientPort))
                imsi = '320230100000025'
                giveIMSI = 0
                clientSocket.sendall(imsi.encode())
                start = time.time()
                clientSocket.close()
        conn, addr = serverSocket.accept()
        with conn:
            data = conn.recv(200)
            if not data:
                break
            else:
                data = data.decode()
                print(data)
                if data == '200 OK':
                    end = time.time()
                    print("User authenticated")
                    print("Time required : " + str(end-start) + ' seconds')
                    giveIMSI = 1
                else:
                    with socket(AF_INET, SOCK_STREAM) as clientSocket:
                        data = (data.split())[1]
                        clientSocket.connect((authServer_IP, authServer_Port))
                        clientSocket.sendall(data.encode())
                        clientSocket.close()
                        with socket(AF_INET, SOCK_STREAM) as responseSocket:
                            responseSocket.bind((authResponse_IP, authResponse_Port))
                            responseSocket.listen()
                            print('Waiting for client')
                            connResponse, addrResponse = responseSocket.accept()
                            with connResponse:
                                response = connResponse.recv(200)
                                response = response.decode()
                                print("RES: " + response)
                                responseSocket.close()
                                with socket(AF_INET, SOCK_STREAM) as clientResponse:
                                    clientResponse.connect((clientIP, clientPort))
                                    clientResponse.sendall(response.encode())
                                    clientResponse.close()
