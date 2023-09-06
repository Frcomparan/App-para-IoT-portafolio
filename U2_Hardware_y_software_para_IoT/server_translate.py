from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import os
import requests
from dotenv import load_dotenv
load_dotenv()

API_KEY = os.getenv("API_KEY")

# Contador del programa
counter = 0

class MyHTTPRequestHandler(BaseHTTPRequestHandler):
    # Establece la configuración inicial de una respuesta
    # Estatus OK (200)
    # Cabecera, por defecto "text/plain", pero puede ser modificada
    def _set_response(self, content_type="text/plain", code=200):
        self.send_response(code)
        self.send_header("Content-type", content_type)
        self.end_headers()

    def throw_custom_error(self, message, code = 400):
        self._set_response("application/json", code)
        self.wfile.write(json.dumps({"message": message}).encode())

    # Metodo GET, que manda la cadena indicada con cada petición
    def do_GET(self):
        self._set_response()
        response = f"The counter is: {counter}"
        self.wfile.write(response.encode()) 

    # Función que procesa las peticiones POST del servidor
    def do_POST(self):
        global API_KEY
        content_length = int(self.headers["Content-Length"])
        post_data = self.rfile.read(content_length)

        try:
            data = json.loads(post_data.decode())
        except:
            self.throw_custom_error("Invalid JSON")
            return
    
        # Check if the text is provided
        if (data.get("text") is None):
            self.throw_custom_error("Text wasnt provided")
            return

        text = data['text']

        # Obtain translation
        
        url = "https://google-translate1.p.rapidapi.com/language/translate/v2"

        payload = {
            "q": text,
            "target": "en",
            "source": "es"
        }
        headers = {
            "content-type": "application/x-www-form-urlencoded",
            "Accept-Encoding": "application/gzip",
            "X-RapidAPI-Key": API_KEY,
            "X-RapidAPI-Host": "google-translate1.p.rapidapi.com"
        }

        response = requests.post(url, data=payload, headers=headers)

        translation = response.json()["data"]["translations"][0]["translatedText"]

        # Respond to the client
        response_data = json.dumps({"message": "Successful request", "translation": translation,})
        self._set_response("application/json")
        self.wfile.write(response_data.encode())

# Establecemos la creacion del servidor, del tipo HTTP, 
# y establecemos el puerto por el que escuchara
def run_server(server_class=HTTPServer, handler_class=MyHTTPRequestHandler, port=7800):
    server_address = ("", port)
    httpd = server_class(server_address, handler_class)
    print(f"Starting server on port {port}...")
    httpd.serve_forever()

if __name__ == "__main__":
    run_server()