import json
import os
from sys import stderr


def get_port_from_file(file):
    """
    gets the port from the file
    :param file: file that contains the port
    :return: port number in string format
    """
    default_port = 1234
    try:
        with open(file, "r") as fport:
            return int(fport.readline())
    except:
        print(f"the file {file} is not available, will use default port {default_port}", stderr)
        return default_port

def get_server_configuration():
    SERVER_CONFIGURATION = "server_configuration.json"
    js_file = os.path.join(__package__, SERVER_CONFIGURATION)
    with open(js_file, "r") as sql_size_data:
        data_size = json.load(sql_size_data)
        return data_size