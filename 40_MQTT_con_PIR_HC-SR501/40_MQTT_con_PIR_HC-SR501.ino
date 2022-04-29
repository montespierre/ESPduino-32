#include <WiFi.h> //ok
#include <PubSubClient.h>

WiFiClient espClient; 
PubSubClient client(espClient); // con las caracteristicas del wifiClient

// Credenciales para conecatarse al wifi
const char* ssid = "CI_ICHL";
const char* password = "df97xBTSeCKy";

const char* mqtt_server = "192.168.1.21";
//const char* mqtt_server = "mono.local";
int port = 1883;

// Variables
String humedad;
String mensajesT;
const byte sensorPir = 14; // sensor en el lugar 14
int estado = LOW; // no hay movimiento por defecto entonces hay nivel bajo
int valor = 0; //guardar el valor entregado por el sensor 1

///////////// Realiza conexion al wifi (inicio) //////////////
void setup_wifi(); // 
///////////// Realiza conexion al wifi (fin) //////////////

/////////////////// callback MQTT (inicio) /////////////////////////
// Realizando conexion MQTT
void callback(char* topic, byte* message, unsigned int length);
/////////////////// callback MQTT (fin) /////////////////////////

/////////////////// Reconexion (inicio) /////////////////////////
// Realiza la reconexion en caso de fallo
void reconnect();
/////////////////// Reconexion (fin) /////////////////////////

void setup() {
  Serial.begin(115200);
  delay(10);
  // conectarse a wifi
  setup_wifi();
  // Configurar el MQTT client
  client.setServer(mqtt_server, 1883);
  // callback es una funcion que se ejecuta luego de un evento determinadocomo la pulsacion de un boton una interfaz que manda un mensaje MQTT
  client.setCallback(callback);

}

int contador = 0;
void loop() {
  contador = contador + 1;
  Serial.println(contador);
  
  if(!client.connected()){
    reconnect();
  }
  client.loop(); // Sirve para esperar los mensajes entrantes de los topicos subscritos

  Serial.println(mensajesT);

  ////////////////////// Enviando un mensaje//////////////////////////
  // Señal que queremos enviar al borker}
  // leer cada iteracion
  valor = digitalRead(sensorPir); //leer valor del sensor
  
  if(valor == HIGH){ // si hay movimiento
    //digitalWrite(led, HIGH); //encender led
    humedad = "on";
    delay(100);
    
    if(estado == LOW){ //viene por defecto
      Serial.println("Movimiento detectado");
      Serial.println(sensorPir);
      estado = HIGH; // cambiamos estado a alto     
    }
     
  } else { // si no hay movimiento
    //digitalWrite(led, LOW); // encender led
    humedad = "off";
    delay(250); // mantener encendido

    if(estado == HIGH){
        Serial.println("Sin movimiento");
        Serial.println(sensorPir);
        estado = LOW; // cambiar a LOW
    }
    
  }
  ////////////////////// Enviando un mensaje (fin)//////////////////////////
  // Convertir el valor a char array
  char humString[8];
  //dtostrf(humedad, 1, 2, humString);
  //Serial.print("Humedad: ");
  //Serial.println(humString);
  humedad.toCharArray(humString, 8);
  client.publish("ESPduinoTopic/input1", humString); // Topic: 'ESPduinoTopic/input1'
  delay(5000);

  //char buffer[10]=" ";
  //char* formato="Bucle %i";
  //sprintf(buffer, formato, i);
  //i++;
  //Serial.println(buffer);
}
