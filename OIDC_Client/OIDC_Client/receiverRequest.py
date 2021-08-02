import socket

PROXY_IP = '127.0.0.1'
PROXY_SERVER_PORT = 11000  # the client_for_proxy will connect at this port

FOG_IP = '127.0.0.1'
FOG_SERVER_PORT = 10000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_for_UE:
    server_for_UE.bind((FOG_IP, FOG_SERVER_PORT))
    server_for_UE.listen()

    # Waiting for connection from UE
    while True:
        print('Waiting to receive IMSI from UE...')
        conn_UE, addr_UE = server_for_UE.accept()
        with conn_UE:
            print('Connected to UE  by', addr_UE)
            while True:
                data = conn_UE.recv(200)
                if not data:
                    break
                else:
                    data = data.decode()
                    print('Received data from UE: ' + data)
                    # Connect to vIdP of proxy2
                    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_for_proxy:
                        client_for_proxy.connect((PROXY_IP, PROXY_SERVER_PORT))
                        client_for_proxy.sendall(data.encode())
                        client_for_proxy.close()
