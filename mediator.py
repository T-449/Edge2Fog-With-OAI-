from socket import *

MEDIATOR_IP = '192.168.0.108'   # Ip of device running mediator.py
MEDIATOR_SERVER_PORT = 10400

EDGE_IP = '192.168.61.3'
EDGE_SERVER_PORT = 9999

PROXY_IP = '192.168.0.106'
PROXY_SERVER_PORT = 11998

while(True):

    # Open server socket for proxy

    with socket(AF_INET, SOCK_STREAM) as server_for_proxy:
        server_for_proxy.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        server_for_proxy.bind((MEDIATOR_IP, MEDIATOR_SERVER_PORT))
        server_for_proxy.listen()

        print('Waiting for proxy connection at port ',  MEDIATOR_SERVER_PORT)

        conn_proxy, addr_proxy = server_for_proxy.accept()
        with conn_proxy:
            print('Connected to proxy by ', addr_proxy)
            data = conn_proxy.recv(200)
            conn_proxy.close()
            server_for_proxy.close()

            if not data:
                continue
            else:
                data = data.decode('utf-8')
                print('Received data from proxy: ' + data)

                # Open client socket for edge

                with socket(AF_INET, SOCK_STREAM) as client_for_edge:
                    client_for_edge.connect((EDGE_IP, EDGE_SERVER_PORT))
                    client_for_edge.sendall(data.encode())
                    client_for_edge.close()
                
                # Open server socket for edge

                with socket(AF_INET, SOCK_STREAM) as server_for_edge:
                    server_for_edge.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
                    server_for_edge.bind((MEDIATOR_IP, MEDIATOR_SERVER_PORT))
                    server_for_edge.listen()

                    print('Waiting for edge connection at port ', MEDIATOR_SERVER_PORT)

                    conn_edge, addr_edge = server_for_edge.accept()
                    with conn_edge:
                        print('Connected to edge by ', addr_edge)
                        data = conn_edge.recv(200)
                        server_for_edge.close()

                        if not data:
                            continue
                        else:
                            data = data.decode('utf-8')
                            print('Received data from edge: ' + data)

                            # Open client socket for edge

                            with socket(AF_INET, SOCK_STREAM) as client_for_proxy:
                                client_for_proxy.connect((PROXY_IP, PROXY_SERVER_PORT))
                                client_for_proxy.sendall(data.encode())
                                client_for_proxy.close()