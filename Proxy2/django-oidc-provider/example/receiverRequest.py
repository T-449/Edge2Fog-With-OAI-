from socket import *
import os
import django
import random
import string

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'app.settings')
django.setup()

from django.contrib.auth.models import User

# EDGE_IP = '127.0.0.1'
# EDGE_IP = '192.168.0.108'
EDGE_IP = '192.168.61.3'
vUser_CLIENT_PORT_IMSI = 9999  # this port is the vUser client port for forwarding IMSI to home  MME

vUser_IP = '127.0.0.1'
vUser_SERVER_PORT = 11000  # this port is the vUser server port for receiving IMSI from OIDC client

FOG_IP = '127.0.0.1'
vUser_CLIENT_PORT = 20000  # this is the client port for forwarding auth Challenge to fog


def random_char(y):
    return ''.join(random.choice(string.ascii_letters) for x in range(y))


def deleteUser():
    userFile = open('myUser.txt', 'r')
    line = userFile.readline()
    line = line.split(' ')
    if User.objects.filter(username=line[0]).exists():
        user = User.objects.get(username=line[0])
        user.delete()
    userFile.close()


with socket(AF_INET, SOCK_STREAM) as vUser_serverSocket:
    vUser_serverSocket.bind((vUser_IP, vUser_SERVER_PORT))
    vUser_serverSocket.listen()

    # Waiting for connection from Fog/OIDC client
    while True:
        print('Waiting to receive IMSI from fog...')
        conn, addr = vUser_serverSocket.accept()
        with conn:
            print('Connected by', addr)
            while True:
                data = conn.recv(200)
                if not data:
                    break
                else:
                    data = data.decode('utf-8')
                    print('Received data: ' + data)
                    # Connect to edge MME
                    with socket(AF_INET, SOCK_STREAM) as vUser_clientSocket:
                        if len(data) == 15 and data.isdigit():
                            vUser_clientSocket.connect((EDGE_IP, vUser_CLIENT_PORT_IMSI))
                            vUser_clientSocket.sendall(data.encode())
                            vUser_clientSocket.close()
                        else:
                            vectorFile = open('authVectors.txt', 'r')
                            line = vectorFile.readline()
                            line = line.split(' ')
                            compareResponse = '404 Error'
                            if data == line[0]:
                                deleteUser()
                                userName = random_char(3)
                                email = random_char(3) + "@" + random_char(5) + ".com"
                                password = random.randint(10000, 20000)
                                newUser = User.objects.create_superuser(userName, email, password)
                                newUser.save()
                                userFile = open('myUser.txt', 'w')
                                userFile.write(userName + ' ' + email + ' ' + str(password))
                                userFile.close()
                                compareResponse = '200 OK'
                            with socket(AF_INET, SOCK_STREAM) as vUser_clientSocket_reply:
                                vUser_clientSocket_reply.connect((FOG_IP, vUser_CLIENT_PORT))
                                vUser_clientSocket_reply.sendall(compareResponse.encode())
                                vUser_clientSocket_reply.close()