from http.server import BaseHTTPRequestHandler, HTTPServer
import json

class MyHTTPRequestHandler(BaseHTTPRequestHandler):
    global counter
    counter = 0

    # Establece la configuración inicial de una respuesta
    # Estatus OK (200)
    # Cabecera, por defecto "text/plain", pero puede ser modificada
    def _set_response(self, content_type="text/plain"):
        self.send_response(200)
        self.send_header("Content-type", content_type)
        self.end_headers()

    # Metodo GET, que manda la cadena indicada con cada petición
    def do_GET(self):
        self._set_response()
        self.wfile.write(f"Hello from the server!\nThe counter is:{counter}".encode()) 

    # Función que procesa las peticiones POST del servidor
    def do_POST(self):
        content_length = int(self.headers["Content-Length"])
        post_data = self.rfile.read(content_length)

        # Print the complete HTTP request
        print("\n----- Incoming POST Request -----")
        print(f"Requestline: {self.requestline}")
        print(f"Headers:\n{self.headers}")
        print(f"Body:\n{post_data.decode()}")
        print("-------------------------------")

        # Respond to the client
        response_data = json.dumps({"message": "Received POST data", "data": post_data.decode()})
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