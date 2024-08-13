from socket import *
from hashlib import sha256
from random import randint
from pprint import pprint


ip = '127.0.0.1'
port = 8090
client = socket(AF_INET, SOCK_STREAM)
client.connect((ip, port))
pprint(f'connected to {ip}:{port}')
client.send('Auth'.encode())
order = client.recv(1024).decode()
final = 'Failure'
for i in range(32):
    guess = ip + str(i)
    guess_hash = sha256(guess.encode()).hexdigest()
    client.send(guess_hash.encode())
    result = client.recv(1024).decode()
    pprint(f'attempt {i} || hash: {guess_hash} || result: {result}')
    if result == 'Hash Found':
        client.close()
        final = result
        break
pprint(f'result: {final}')



