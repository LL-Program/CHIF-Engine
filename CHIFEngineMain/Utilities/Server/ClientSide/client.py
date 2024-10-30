import socket

class ServerClient:
    def __init__(self):
        self.host = "Lukas-Laptop"
        self.port = 9999
        self.socket = socket.socket()
    def connect(self,host,port):
        self.socket.connect((host, port))
    def run(self):
        self.connect(self.host, self.port)
        message = input(" -> ")
        while message.lower().strip() != 'bye':
            self.socket.send(message.encode())
            data = self.socket.recv(1024).decode()
            print('Received from server: ' + data)
            message = input(" -> ")
        self.socket.close()
    




if __name__ == '__main__':
    client = ServerClient()
    client.run()