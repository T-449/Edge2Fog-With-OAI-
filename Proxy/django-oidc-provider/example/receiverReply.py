from socket import *
import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'app.settings')
django.setup()

PROXY_IP = '192.168.0.106'
PROXY_SERVER_PORT = 11998  # this is the server port for receiving auth Challenge from mediator

FOG_IP = '127.0.0.1'
FOG_SERVER_PORT = 20000  # this is the client port for forwarding auth Challenge to fog

with socket(AF_INET, SOCK_STREAM) as server_for_mediator:
    server_for_mediator.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    server_for_mediator.bind((PROXY_IP, PROXY_SERVER_PORT))
    server_for_mediator.listen()

    while True:
        print('Waiting for connection from mediator...')
        conn_mediator, addr_mediator = server_for_mediator.accept()
        with conn_mediator:
            print('Connected to mediator by ', addr_mediator)
            while True:
                authChallenge = conn_mediator.recv(200)
                if not authChallenge:
                    break
                else:
                    authChallenge = authChallenge.decode('utf-8')
                    print("Received data from mediator: ", authChallenge)
                    authChallenge = authChallenge.split()
                    authChallenge_str = authChallenge[0] + ' ' + authChallenge[1] + ' ' + authChallenge[2] + ' ' + authChallenge[3]
                    vectorFile = open('authVectors.txt', 'w')
                    vectorFile.write(authChallenge_str)
                    vectorFile.close()
                    authChallenge = authChallenge[3] + ' ' + authChallenge[1]
                    with socket(AF_INET, SOCK_STREAM) as client_for_fog:
                        client_for_fog.connect((FOG_IP, FOG_SERVER_PORT))
                        client_for_fog.sendall(bytes(authChallenge, 'utf-8'))
                        client_for_fog.close()
