import socket

class CHIFServer:
    def __init__(self):
        #Server Settings
        self.host = "0.0.0.0"
        self.port = 9999
        #Init
        self.socket = socket.socket()
        self.socket.bind((self.host, self.port))
        print(f"HOSTED FROM HOSTNAME '{self.host}'")
    def run(self):
        print(f"SERVER *{self.host}* WAITING FOR CONNECTION")
        self.socket.listen(2)
        conn, address = self.socket.accept()
        print("Connection from: " + str(address))
        while True:
            data = conn.recv(1024).decode()
            if not data:
                break
            print("from connected user: " + str(data))
            data = input(' -> ')
            conn.send(data.encode())
        conn.close()



if __name__ == '__main__':
    server = CHIFServer()
    server.run()