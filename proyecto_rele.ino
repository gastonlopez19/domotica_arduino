#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>





 
// Sustituir con datos de vuestra red
const char* ssid     = "TeleCentro-63d9";
const char* password = "24361556";
const int rele = 5;
ESP8266WiFiMulti WiFiMulti;

 
void setup()
{
    pinMode(rele,OUTPUT);

  Serial.begin(9600);
  Serial.print("Iniciamos el proceso: \n");

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a:\t");
  Serial.println(ssid); 
 
  // Esperar a que nos conectemos
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);
   Serial.print('.');
  }
 
  // Mostrar mensaje de exito y dirección IP asignada
  Serial.println();
  Serial.print("Conectado a:\t");
  Serial.println(WiFi.SSID()); 
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

}
 
void loop() 
{
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;
    StaticJsonDocument<300> sjd;
    String lamparaURL =  "http://192.168.0.68:8080/api/Sensores_Arduino_Habitaciones/?id_habitacion=1&id_arduino=1&id_sensor=1";
    
    if (http.begin(client, lamparaURL)) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("Codigo de Respuesta: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String respHTTP = http.getString();
          DeserializationError errorJson = deserializeJson(sjd, respHTTP);
          if (errorJson) { 
            Serial.println("Error al desarmar el objeto JSON");
            return; 
          }

          String valor = sjd[0]["valor_sensor"];
          Serial.println("Estado del sensor: " + valor);


          if (valor == "1"){
            digitalWrite(rele,HIGH);
          }else if(valor == "0"){
            digitalWrite(rele,LOW);
          }
        }
      } else {
        Serial.printf("Fallo al obtener la informacion, Error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("No se puede conectar al dispositivo indicado\n");
    }
  }

  delay(500);

  
}
