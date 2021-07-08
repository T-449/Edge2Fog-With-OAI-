import socket

FOG_IP = '127.0.0.1'
FOG_SERVER_PORT = 20000

vUE_IP = '127.0.0.1'
FOG_CLIENT_PORT = 39999

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as fog_serverSocket:
    fog_serverSocket.bind((FOG_IP, FOG_SERVER_PORT))
    fog_serverSocket.listen()
    while True:
        print('Waiting for connection from vIdp of proxy2...')
        conn, addr = fog_serverSocket.accept()
        with conn:
            print('Connected to vIdp by', addr)
            while True:
                authChallenge = conn.recv(200)
                if not authChallenge:
                    break
                else:
                    authChallenge = authChallenge.decode()
                    print(authChallenge)
                    if authChallenge == '200 OK':
                        stateFile = open('../state.txt', 'w')
                        stateFile.write('')
                        stateFile.close()
                    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as fog_clientSocket:
                        fog_clientSocket.connect((vUE_IP, FOG_CLIENT_PORT))
                        fog_clientSocket.sendall(authChallenge.encode())
                        fog_clientSocket.close()
