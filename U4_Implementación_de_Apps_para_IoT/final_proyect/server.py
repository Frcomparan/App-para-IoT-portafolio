import requests
from db_storage import DBStorage
import flask
import flask_cors
import threading
import time
import datetime

# URL del servidor
url = "http://192.168.185.110"

actual_luminity = 0

# Función para obtener la luminosidad
def save_luminity():
    global actual_luminity 
    # Se obtiene la respuesta del servidor
    response = requests.get(url + "/luminosity")

    luminity = response.text

    print("luminity: " + luminity)

    actual_luminity = luminity 

    db = DBStorage()
    db.connect()
    db.insert(luminity)
    db.disconnect()

def update_luminity():
    global actual_luminity 
    # Se obtiene la respuesta del servidor
    response = requests.get(url + "/luminosity")

    luminity = response.text

    print("luminity: " + luminity)

    actual_luminity = luminity 
    
def timer_save():
    while True:
        save_luminity()
        time.sleep(30)

def timer_update():
    while True:
        update_luminity()
        time.sleep(1)

# Iniciar el servidor
app = flask.Flask(__name__)

# Configurar CORS para permitir acceso por otros dominios a todas las rutas

cors = flask_cors.CORS(app, resources={r"/*": {"origins": "*"}})

# Ruta para retornar la luminosidad
@app.route('/luminity', methods=['GET'])
def get_luminity():
    global actual_luminity
    return {'luminity': actual_luminity}

# Ruta para obtener la luminosidad de la ultima hora
@app.route('/luminity_last_hour', methods=['GET'])
def get_luminity_last_hour():
    db = DBStorage()
    db.connect()
    end_date = datetime.datetime.now()
    start_date = end_date - datetime.timedelta(hours=1)
    luminity = db.get_measurements_by_time(start_date, end_date)
    print(luminity)
    db.disconnect()
    return luminity

if __name__ == '__main__':
    # Obtner la luminosidad cada 3 segundos
    t = threading.Thread(target=timer_save)
    t2 = threading.Thread(target=timer_update)
    t.start()
    t2.start()
    # Iniciar el servidor
    app.run(host='0.0.0.0', port=5000)

