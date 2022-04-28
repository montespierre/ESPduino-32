//const byte led = 13; //led del circuito
const byte sensorPir = 14; // sensor en el lugar 14
int estado = LOW; // no hay movimiento por defecto entonces hay nivel bajo
int valor = 0; //guardar el valor entregado por el sensor 1

void setup() {
  //pinMode(led, OUTPUT);
  Serial.begin(115200);

}

void loop() {
  // leer cada iteracion
  valor = digitalRead(sensorPir); //leer valor del sensor
  
  if(valor == HIGH){ // si hay movimiento
    //digitalWrite(led, HIGH); //encender led
    delay(100);

    if(estado == LOW){ //viene por defecto
      Serial.println("Movimiento detectado");
      Serial.println(sensorPir);
      estado = HIGH; // cambiamos estado a alto
    }
     
  } else { // si no hay movimiento
    //digitalWrite(led, LOW); // encender led
    delay(250); // mantener encendido

    if(estado == HIGH){
        Serial.println("Sin movimiento");
        Serial.println(sensorPir);
        estado = LOW; // cambiar a LOW
    }
    
  }

}
