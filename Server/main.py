import sqlite3
import data
import data.logic as logic


if __name__ == '__main__':
    print("test1")
    client_lg = logic.ClientLogic(1, "Eden", "1234567", "26/10/2022", "ABCDEF")
    file_lg = logic.file_logic.FileLogic()
    clients_db = data.Clients()
    clients_db.create_table()
    clients_db.insert_table(client_lg)
    print(clients_db.get_client_by_id(1)[0][1]) # will return Name ==>> Eden

    print("test2")


