from socket import *
from hashlib import sha256
from random import randint
from pprint import pprint


ip = '127.0.0.1'
port = 8090
server = socket(AF_INET, SOCK_STREAM)
server.bind((ip, port))
server.listen(1)
pprint(f'listening on: {ip}:{port}')
while True:
    connection, address = server.accept()
    authentication = connection.recv(1024).decode()
    pprint(f'get authentication message from {address}')
    random_number = randint(0, 31)
    pprint(f'server number: {random_number}')
    answer = ip + str(random_number)
    answer_hash = sha256(answer.encode()).hexdigest()
    cannot_guess = True
    while cannot_guess:
        connection.send('Send Hash!'.encode())
        client_guess = connection.recv(1024).decode()
        pprint(f'guess: {client_guess}  || answer: {answer_hash}')
        if answer_hash == client_guess:
            cannot_guess = False
    connection.send('Hash Found'.encode())
    connection.close()
    pprint('connection closed!')
    print()
