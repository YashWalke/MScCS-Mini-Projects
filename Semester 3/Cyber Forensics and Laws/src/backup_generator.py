import mysql.connector as connector
from sys import exit

if __name__ == "__main__":
    hostname = input("Enter host name [localhost]>")
    hostname = hostname if hostname != "" else "localhost"
    username = input("Enter your username > ")
    if username == "":
        exit("Please input the correct username")
    print("If password is not set, just press [Enter] on the following prompt")
    password = input("Enter your password > ")
    database_name = input("Enter the name of the database you want to backup > ")
    if database_name == "":
        exit("Please input the correct database name")
    
    print("Trying connection...")
    try:
        connection = connector.connect(host = hostname, user = username, password = password, database = database_name)
        cursor = connection.cursor()
        print("Connection successful")
        
        cursor.execute("show tables;")
        table_names : list[str] = []
        for record in cursor.fetchall():
            table_names.append(record[0])
        
        backup_database_name = database_name + "_backup"
        cursor.execute(f"create database {backup_database_name};")
        cursor.execute(f"use {backup_database_name};")
        
        for table_name in table_names:
            cursor.execute(f"create table {table_name} select * from {database_name}.{table_name}")
        print("Backup successful")
    except:
        exit("Connection unsuccessful")