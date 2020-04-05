/*
 * Smart Parking System 
 * Copyright Soedirman Moeda Project 2018
 */

#include <stdlib.h>
#include <ESP8266WiFi.h>

WiFiClient client;
//WIFI credentials go here
const char* ssid     = "nodemcu";
const char* password = "123456789";
//
const char* server="smartparkingv1.herokuapp.com"; //ip server
String path ="/api/sensor/";
String id="1";
int pin_digital = 4;

void setup() {
  pinMode(pin_digital, INPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");Serial.println("WiFi connected");
  Serial.print("IP address: ");Serial.println(WiFi.localIP());
}

void loop() {
  //Baca data sensor
  int data = digitalRead(pin_digital);
  //Buat koneksi ke server
  if(client.connect(server, 80)){
    Serial.println("Connected to server");
    //atur parameter
    String parameter = String("field=") + String(data);
    Serial.print("Parameter : ");Serial.println(parameter);
    //buat request header
    client.print("PUT "+path+id+" HTTP/1.1\r\n");
    client.print("Host: "+String(server)+"\r\n");
    client.print("Accept: application/json\r\n");
    client.print("Connection: close\r\n");
    //buat request body
    client.print("Content-Length: ");
    client.print(parameter.length());
    client.print("\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n\r\n");
    client.print(parameter);
    client.print("\r\n\r\n");
    //baca keseluruhan respon
    bool a=false;
    String status_code = "";
    while(client.available()){
      //Print keseluruhan respon
      //String response= client.readStringUntil('\r');
      //Serial.print(response);

      //Print http kode aja
      char c=client.read();
      if(a==true){
        if(c==' '){a=false; break;}
        status_code+=c;
      }else if(c==' ' && a==false && status_code==""){a=true;}      
    }
    //jika data berhasil terkirim, respon kode http dari server adalah 200
    Serial.print("Http kode : ");Serial.println(status_code);
    if(status_code=="200"){Serial.println("Berhasil terkirim");}
    else{Serial.println("Gagal terkirim");}
    Serial.println();
  }
  client.stop();
  delay(1000);
}
