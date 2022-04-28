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
float humedad = 0;
String mensajesT;

///////////// Realiza conexion al wifi (inicio) //////////////
void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi conectado");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
}
///////////// Realiza conexion al wifi (fin) //////////////

/////////////////// callback MQTT (inicio) /////////////////////////
// Realizando conexion MQTT
void callback(char* topic, byte* message, unsigned int length){
  Serial.print("Mensaje recibido en topic: ");
  Serial.print(topic);
  Serial.print(", Message: ");
  String messageTemp;
  for(int i = 0; i < length; i++){
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  String mensajesT = messageTemp;
  Serial.println();
  
}
/////////////////// callback MQTT (fin) /////////////////////////

/////////////////// Reconexion (inicio) /////////////////////////
// Realiza la reconexion en caso de fallo
void reconnect(){
  // Bucle hasta que reconecte
  while(!client.connected()){
      Serial.print("Intentado conexion MQTT");
      if(client.connect("ESPduino01")){
        Serial.println("conectado");
        client.subscribe("ESPduinoTopic/output1"); // Topic : 'ESPduinoTopic/output1'
        
      }else{
        Serial.print("Fallo, rc =");
        Serial.print(client.state());
        Serial.println(" Intentelo de nuevo en 5s");
        delay(5000);
      }
  }
}
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

void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop(); // Sirve para esperar los mensajes entrantes de los topicos subscritos

  Serial.println(mensajesT);

  // Enviando un mensaje
  // Señal que queremos enviar al borker
  humedad = 289.78;
  // Convertir el valor a char array
  char humString[8];
  dtostrf(humedad, 1, 2, humString);
  Serial.print("Humedad: ");
  Serial.println(humString);
  client.publish("ESPduinoTopic/input1", humString); // Topic: 'ESPduinoTopic/input1'
  delay(5000);

  //char buffer[10]=" ";
  //char* formato="Bucle %i";
  //sprintf(buffer, formato, i);
  //i++;
  //Serial.println(buffer);
}
