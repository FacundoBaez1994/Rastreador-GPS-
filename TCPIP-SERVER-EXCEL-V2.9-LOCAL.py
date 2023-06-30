# -*- coding: utf-8 -*-
"""
Created on Mon Jun 19 09:54:12 2023
"""

import socket
import sys
import os
import folium
import time
from selenium import webdriver
import urllib
import xlsxwriter
from datetime import datetime
import keyboard
import signal
from sys import exit

##################### SETUP #####################################

driver = webdriver.Chrome(executable_path=r'D:\PNA\DIDI\Rastreador\Backup\Python\chromedriver.exe')
driver.get("file:///D:\PNA\DIDI\Rastreador\Backup\Python\seguimiento.html")

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#################################################

def folium_plot_locations(coord_list):
    m = folium.Map(location=coord_list[0][:2], zoom_start=15)

    # Agregar un marcador en cada ubicación
    for i, coord in enumerate(coord_list):
        latitude, longitude = coord[:2]
        timestamp = coord[2]
        tooltip = f'Hora: {timestamp}'
        icon = folium.features.CustomIcon(icon_image='car_icon.png', icon_size=(30, 30))
        folium.Marker(
            location=[latitude, longitude],
            popup=f'<strong>Dispositivo {i+1}</strong><br>Hora: {timestamp}',
            tooltip=tooltip,
            icon=icon
        ).add_to(m)

    # Unir las ubicaciones con flechas
    for i in range(len(coord_list) - 1):
        folium.PolyLine([coord_list[i][:2], coord_list[i+1][:2]], color="red", weight=2.5, opacity=1).add_to(m)

    m.save('seguimiento.html')
    driver.refresh()

def main():
    print('Bienvenido')
    print('Prefectura Naval Argentina')
    print('Departamento de Apoyo Tecnologico para el Analisis Criminal')
    print('Servidor TCP IP - Dispositivo Rastreador GPS')
    
    index = 0
    latency = 0
    max_timediff = 3600
    timeout = 20  # Tiempo límite sin recibir datos en segundos

    ############### ENCABEZADO EXCEL ###################
    namefile = ''
    now = datetime.now()
    dt_string = now.strftime("%d-%m-%Y %H-%M-%S")
    namefile = dt_string + 'GPS_LOG_DEVICE1.xls'
        
    # Create a workbook and add a worksheet.
    workbook = xlsxwriter.Workbook(namefile)
    worksheet = workbook.add_worksheet()
    row = 0
    col = 0
    # First row.
    worksheet.write(row, col, 'Latitud')
    worksheet.write(row, col + 1, 'Longitud')
    worksheet.write(row, col + 2, 'Fecha / Hora')
    worksheet.write(row, col + 5, 'URL')
    ###########################################

    ######### INICIANDO SERVIDOR TCP/IP##########
    begin_datetime = datetime.now()
    server_address = ('192.168.0.246', 123)
    print('Iniciando en {} Puerto {}'.format(*server_address))
    sock.bind(server_address)
    sock.listen(1)
    begin_datetime = datetime.now()
    
    coord_list = []  # Lista para almacenar las coordenadas recibidas
    
    while True:
        print('Esperando envio de datos')
        connection, client_address = sock.accept()
        connection.settimeout(timeout)  # Establecer el tiempo límite para recibir datos
        
        try:
            print('Conexion desde rastreador:', client_address)
            
            while True:
                # RECIBE DATOS
                try:
                    data = connection.recv(64)  # Ajusta el tamaño del búfer según tus necesidades
                    if not data:
                        print('No se pudo encontrar más datos desde:', client_address)
                        break
                    data = data.decode()  # Decodifica los datos recibidos
                    device_id, latitude, longitude = data.split('|')  # Divide los datos en las variables correspondientes
                    latitude = float(latitude)
                    longitude = float(longitude)
                    print('ID de Dispositivo:', device_id)
                    print('Latitud:', latitude)
                    print('Longitud:', longitude)

                    # PLOTEO DE COORDENADAS
                    timestamp = datetime.now().strftime("%H:%M:%S")
                    coord_list.append((latitude, longitude, timestamp))
                    folium_plot_locations(coord_list)

                    index += 1
                    row += 1 
                    actual_datetime = datetime.now()
                    dt_string = actual_datetime.strftime("%d/%m/%Y %H:%M:%S")
                    print(dt_string)
                    string_latitude = str(latitude)
                    string_longitude = str(longitude)
                    googlemaps_url_string = "www.google.com/maps/place/" + string_latitude + "," + string_longitude
                    worksheet.write(row, col, latitude)
                    worksheet.write(row, col + 1, longitude)
                    worksheet.write(row, col + 2, dt_string)
                    worksheet.write(row, col + 5, googlemaps_url_string)

                    timediff = (actual_datetime - begin_datetime).total_seconds()

                    if timediff >= max_timediff:
                        begin_datetime = actual_datetime
                        print('Generando registro historico')
                        workbook.close()

                        namefile = ''
                        now = datetime.now()
                        dt_string = now.strftime("%d-%m-%Y %H-%M-%S")
                        namefile = dt_string + 'GPS_LOG_DEVICE1.xls'

                        workbook = xlsxwriter.Workbook(namefile)
                        worksheet = workbook.add_worksheet()
                        row = 0
                        col = 0
                        worksheet.write(row, col, 'Latitud')
                        worksheet.write(row, col + 1, 'Longitud')
                        worksheet.write(row, col + 2, 'Fecha / Hora')
                        worksheet.write(row, col + 5, 'URL')
                        index = 0

                except socket.timeout:
                    print('No se recibieron datos durante {} segundos. Cerrando conexión.'.format(timeout))
                    break

        finally:
            print('Cerrando conexion TCP/IP')
            connection.close()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Interrupted')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)