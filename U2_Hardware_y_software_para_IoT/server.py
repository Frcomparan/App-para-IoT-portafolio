from http.server import BaseHTTPRequestHandler, HTTPServer
import json

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

    # Metodo GET, que manda la cadena indicada con cada petición
    def do_GET(self):
        self._set_response()
        response = f"The counter is: {counter}"
        self.wfile.write(response.encode()) 

    # Función que procesa las peticiones POST del servidor
    def do_POST(self):
        global counter
        content_length = int(self.headers["Content-Length"])
        post_data = self.rfile.read(content_length)

        # Print the complete HTTP request
        print("\n----- Incoming POST Request -----")
        print(f"Requestline: {self.requestline}")
        print(f"Headers:\n{self.headers}")
        print(f"Body:\n{post_data.decode()}")
        print("-------------------------------")

        try:
            data = json.loads(post_data.decode())
        except:
            self._set_response("application/json", 404)
            self.wfile.write(json.dumps({"message": "Invalid JSON"}).encode())
            return

        # Code to handle the counter changes
        # if data have an "action" attribute with the content "inc", 
        # the counter increment accoding with the specified step
        # otherwise if data have an "action" attribute with the content "dec, 
        # the counter decrement accoding with the specified step
        for key in data:
            if key == 'action':
                quantity = data['quantity']
                if (data['action'] == 'asc'):
                    counter += quantity
                    print(f'Counter incremented to {counter}')
                if (data['action'] == 'desc'):
                    counter -= quantity
                    print(f'Counter decremented to {counter}')

        # Respond to the client
        response = f"The counter change to {counter}"
        response_data = json.dumps({"message": "Received POST data", "result": response})
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