import json
import os

def load_data_size():
    TABLE_ITEMS_SIZE = "sql_table_field_size_configuration.json"
    js_file = os.path.join(__package__, TABLE_ITEMS_SIZE)
    with open(js_file, "r") as sql_size_data:
        data_size = json.load(sql_size_data)
        return data_size
