from socket import *
import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'app.settings')
django.setup()

# vUser_IP = '192.168.0.103'
vUser_IP = '192.168.0.102'
# vUser_IP = '192.168.0.107'
# vUser_IP = '127.0.0.1'
vUser_SERVER_PORT = 10400  # this is the server port for receiving auth Challenge from home edge MME

FOG_IP = '127.0.0.1'
vUser_CLIENT_PORT = 20000  # this is the client port for forwarding auth Challenge to fog

with socket(AF_INET, SOCK_STREAM) as vUser_serverSocket:
    vUser_serverSocket.bind((vUser_IP, vUser_SERVER_PORT))
    vUser_serverSocket.listen()
    while True:
        print('Waiting for connection from home edge MME...')
        conn, addr = vUser_serverSocket.accept()
        with conn:
            print('Connected to home edge MME by', addr)
            while True:
                authChallenge = conn.recv(2048)
                if not authChallenge:
                    break
                else:
                    authChallenge = authChallenge.decode('utf-8')
                    print("Received data: ", authChallenge)
                    authChallenge = authChallenge.split()
                    authChallenge_str = authChallenge[0] + ' ' + authChallenge[1] + ' ' + authChallenge[2] + ' ' + authChallenge[3]
                    vectorFile = open('authVectors.txt', 'w')
                    vectorFile.write(authChallenge_str)
                    vectorFile.close()
                    authChallenge = authChallenge[3] + ' ' + authChallenge[1]
                    with socket(AF_INET, SOCK_STREAM) as vUser_clientSocket:
                        vUser_clientSocket.connect((FOG_IP, vUser_CLIENT_PORT))
                        vUser_clientSocket.sendall(bytes(authChallenge, 'utf-8'))
                        vUser_clientSocket.close()
