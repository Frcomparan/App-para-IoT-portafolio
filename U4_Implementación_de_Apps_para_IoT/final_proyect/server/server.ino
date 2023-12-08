#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "miredUwU";      // Cambia por tu SSID
const char *password = "Itcolima6"; // Cambia por tu contraseña

// Utilizamos la libreria ESPAsyncWebServer para crear el servidor
AsyncWebServer server(80);

// ------------------------ Carrito configuración -------------------------------

/**
 17 |||   |||  18
 05 |||   |||  19


 04 |||   |||  15
 16 |||   |||  16
**/

// Llanta trasceras
#define din1 15 // cafe **der
#define din2 2  // rojo ** der
#define din3 4  // blanco ** izq
#define din4 16 // negro ** izq

// Llanta delanteras
#define tin1 17 // azul ** izq
#define tin2 5  // verde ** izq
#define tin3 18 // morado ** der
#define tin4 19 // blanco ** der
// ------------------------ Carrito configuración -------------------------------

void setup()
{
  // Inicializa la comunicación serial
  Serial.begin(115200);

  // ======================================================================================================

  // ------------------------ Carrito configuración -------------------------------
  // Inicializamos los pines del carrito como pines de salida
  pinMode(tin1, OUTPUT);
  pinMode(tin2, OUTPUT);
  pinMode(tin3, OUTPUT);
  pinMode(tin4, OUTPUT);

  pinMode(din1, OUTPUT);
  pinMode(din2, OUTPUT);
  pinMode(din3, OUTPUT);
  pinMode(din4, OUTPUT);

  // Apagamos ruedas despues de inicializarlas (por si acaso)
  ruedasApagadas();
  // ------------------------ Carrito configuración -------------------------------

  // ======================================================================================================

  // ------------------------ Conexión a Wifi configuración -------------------------------
  // Conéctate a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conexión exitosa");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  // ------------------------ Conexión a WiFi configuración -------------------------------

  // ======================================================================================================
  // ------------------------ Servidor configuración -------------------------------
  server.on("/luminity", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    int luminity = analogRead(34);
    
    if (!isnan(luminity)) {
      String response = String(luminity);
      Serial.println(luminity);
      request->send(200, "text/plain", response);
    } else {
      request->send(400, "text/plain", "Grados no validos");
    } });

  /*
    Seteo de la ruta del servidor que maneja el movimiento del carrito baso en tiempos
  */
  server.on("/carro", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String movimiento = request->arg("mov");
    String timeString = request->arg("time");
    int time = timeString.toInt();

    if (movimiento == "adelante") {
      moverAdelante(time);
      String response = "Carrito avanza hacia: " + movimiento;
      request->send(200, "text/plain", response);
    } else if (movimiento == "atras") {
      moverAtras(time);
      String response = "Carrito avanza hacia: " + movimiento;
      request->send(200, "text/plain", response);
    } else if (movimiento == "derecha") {
      moverDerecha(time);
      String response = "Carrito avanza hacia: " + movimiento;
      request->send(200, "text/plain", response);
    } else if (movimiento == "izquierda") {
      moverIzquierda(time);
      String response = "Carrito avanza hacia: " + movimiento;
      request->send(200, "text/plain", response);
    } else {
      request->send(400, "text/plain", "Movimiento no validos");
    } });

  // Iniciar el servidor
  server.begin();
  // ------------------------ Servidor configuración -------------------------------
}

void loop()
{
}

/*
  Metodo que desactiva los pines
  (coloca en LOW) de las ruedas
  para detener el carro
*/
void ruedasApagadas()
{
  // Rueda delantera izquierda
  digitalWrite(tin1, LOW);
  digitalWrite(tin2, LOW);

  // Rueda delantera derecha
  digitalWrite(tin3, LOW);
  digitalWrite(tin4, LOW);

  // Rueda traseara derecha
  digitalWrite(din1, LOW);
  digitalWrite(din2, LOW);

  // Rueda trasera izquierda
  digitalWrite(din3, LOW);
  digitalWrite(din4, LOW);
}

/*
  Metodo que activa los pines de las ruedas
  para mover el carro hacia delante
*/
void ruedasAdelante()
{
  // Rueda delantera izquierda
  digitalWrite(tin1, LOW);
  digitalWrite(tin2, HIGH);

  // Rueda delantera derecha
  digitalWrite(tin3, LOW);
  digitalWrite(tin4, HIGH);

  // Rueda traseara derecha
  digitalWrite(din1, LOW);
  digitalWrite(din2, HIGH);

  // Rueda trasera izquierda
  digitalWrite(din3, LOW);
  digitalWrite(din4, HIGH);
}

/*
  Metodo que activa los pines de las ruedas
  para mover el carro hacia atras
*/
void reudasDetras()
{
  // Rueda delantera izquierda
  digitalWrite(tin1, HIGH);
  digitalWrite(tin2, LOW);

  // Rueda delantera derecha
  digitalWrite(tin3, HIGH);
  digitalWrite(tin4, LOW);

  // Rueda traseara derecha
  digitalWrite(din1, HIGH);
  digitalWrite(din2, LOW);

  // Rueda trasera izquierda
  digitalWrite(din3, HIGH);
  digitalWrite(din4, LOW);
}

/*
  Metodo que activa los pines de las ruedas
  para mover el carro hacia la izquerda
*/
void reudasIzquierda()
{
  // Rueda delantera izquierda
  digitalWrite(tin1, HIGH);
  digitalWrite(tin2, LOW);

  // Rueda delantera derecha
  digitalWrite(tin3, LOW);
  digitalWrite(tin4, HIGH);

  // Rueda traseara derecha
  digitalWrite(din1, LOW);
  digitalWrite(din2, HIGH);

  // Rueda trasera izquierda
  digitalWrite(din3, HIGH);
  digitalWrite(din4, LOW);
}

/*
  Metodo que activa los pines de las ruedas
  para mover el carro hacia la deracha
*/
void reudasDerecha()
{
  // Rueda delantera izquierda
  digitalWrite(tin1, LOW);
  digitalWrite(tin2, HIGH);

  // Rueda delantera derecha
  digitalWrite(tin3, HIGH);
  digitalWrite(tin4, LOW);

  // Rueda traseara derecha
  digitalWrite(din1, HIGH);
  digitalWrite(din2, LOW);

  // Rueda trasera izquierda
  digitalWrite(din3, LOW);
  digitalWrite(din4, HIGH);
}

/*
  Función para mover el carro hacia delante
  "x" cantidad de tiempo
*/
void moverAdelante(int time)
{
  ruedasAdelante();

  delay(time);

  ruedasApagadas();
}

/*
  Función para mover el carro hacia atras
  "x" cantidad de tiempo
*/
void moverAtras(int time)
{
  reudasDetras();

  delay(time);

  ruedasApagadas();
}

/*
  Función para mover el carro hacia la derecha
  "x" cantidad de tiempo
*/
void moverDerecha(int time)
{
  reudasDerecha();

  delay(time);

  ruedasApagadas();
}

/*
  Función para mover el carro hacia la izquierda
  "x" cantidad de tiempo
*/
void moverIzquierda(int time)
{

  reudasIzquierda();

  delay(time);

  ruedasApagadas();
}
