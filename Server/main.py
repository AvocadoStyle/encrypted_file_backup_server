import sqlite3

from data.clients_data import Clients



if __name__ == '__main__':
    print("test1")
    clients_db = Clients()
    clients_db.create_table()
    clients_db.insert_table(1, "Eden", "123456", "26/10/2022", "ABCDEF")
    print(clients_db.client_by_id(1)[0][1])
    print("test2")


