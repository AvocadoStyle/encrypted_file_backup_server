import socket

with socket.socket() as s:
    s.connect(("127.0.0.1", 8085))
    input("press enter to start: ")
    s.sendall(b'papaya is tasty')
    data = s.recv(1024)
    print(f"data is: {data}")
