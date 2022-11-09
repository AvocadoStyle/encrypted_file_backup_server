import socket

with socket.socket() as s:
    s.connect(("127.0.0.1", 8085))
    input("press enter to start: ")
    s.sendall(b'\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x01\x04\x4C\x00\x00\x00\x04EDEN')
    s.sendall(b'0000000311137434122009999')
    data = s.recv(1024)
    print(f"data is: {data}")










