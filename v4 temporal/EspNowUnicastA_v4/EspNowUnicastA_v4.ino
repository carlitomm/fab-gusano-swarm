#include <WifiEspNow.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif
#include "Mod_String.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

//INICIO PARTE DE CARLOS
#include <ESP8266WebServer.h>
#include "Servo.h"
#include "./thread/Thread.h"
#include "./layout/html.h"
#include "behaviors.h"

Thread movment = Thread();
int first = 5;
int second = 10;
int third = 15;

int change_flag = 0;
int init_flag = 1;

const char* ssid = "WiFi-Meat";
const char* password = "12345678";

int mov;
int tiempo_delay = 15;
uint8_t gusano_state = 1;
int i, j;
int pos = 0;
int counter = 0;

Servo servo1;
Servo servo2;
Servo servo3;

//Informacion necesaria para la Tx y Rx de datos entre ESP
String cadena, S_valorX,S_valorY,S_valorZ, S_modo, S_opc, S_mov, S_tiempo_delay;
// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xDA, 0xBF, 0xC0, 0xF9, 0xEA, 0x25};
char datoRX[6];
Mod_String cambios;

WifiEspNowSendStatus status_A; 

int inicio, cont;
int modo; //Modo de trabajo del robot: 1 Coreografia, 2 Seguimiento, 3 Mezcla
int opcion; //Opcion de seguimiento : 1 Lider, 2 Seguidor
int valx = 3000;
int valy = 4000;
int valz = 5000;
float RSSI_;
float d_RSSI, dist_RSSI;

//Variables para la obtencion de las orientaciones
const int mpuAddress = 0x68;
MPU6050 mpu(mpuAddress);
float accel_ang_x, accel_ang_y;
int16_t ax, ay, az;
int16_t gx, gy, gz;
long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y, girosc_ang_z;
float girosc_ang_x_prev, girosc_ang_y_prev, girosc_ang_z_prev;
float orient_X_B, orient_X_A,orient_Y_B, orient_Y_A, orient_Z_B, orient_Z_A;
float Rssi_A, Rssi_B;
//****

/*void updateGiro(){
  dt = (millis() - tiempo_prev) / 1000.0;
  //dt = millis() - tiempo_prev;
  tiempo_prev = millis();

accel_ang_x = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0/ 3.14);
accel_ang_y = atan(-ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0/ 3.14);

  girosc_ang_x_prev = girosc_ang_x;
    girosc_ang_y_prev = girosc_ang_y;
      girosc_ang_z_prev = girosc_ang_z;
  
}*/

void
printReceivedMessage(const uint8_t mac[6], const uint8_t* buf, size_t count, void* cbarg)
{
  //Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3],
              //  mac[4], mac[5]);
  for (int i = 0; i < count; ++i) {
    //Serial.print(static_cast<char>(buf[i]));
    datoRX[i]= static_cast<char>(buf[i]);
    
  }
  
  for(int j=0; j<6; j++){
  cadena=String(cadena + datoRX[j]);
  }
  switch (datoRX[0]){
    case 'X':
    S_valorX = cadena;
    break;
    case 'Y':
    S_valorY= cadena;
    break;
    case 'Z':
    S_valorZ= cadena;
    break;
    case 'I':
    //inicio = 1;
    break;
    case 'M':
    S_modo= cadena;
    break;
    case 'V':
    S_mov= cadena;
    break; 
    case 'O':
    S_opc= cadena;
    break;  
        case 'D':
    S_tiempo_delay= cadena;
    break;  

  }
  
  cadena="";
}

void
setup()
{
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  Serial.println();

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, password);
  WiFi.softAPdisconnect(false);

  mov = 1;

  servo1.attach(4); //5-14 D5
  servo2.attach(5); //6-12 D6
  servo3.attach(1);  //16-15 D8

  movment.onRun(on_movment);
  movment.setInterval(tiempo_delay);


  
  Serial.print("MAC address of this node is ");
  Serial.println(WiFi.softAPmacAddress());

  uint8_t mac[6];
  WiFi.softAPmacAddress(mac);
  Serial.println();
  Serial.println("You can paste the following into the program for the other device:");
  Serial.printf("static uint8_t PEER[]{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", mac[0],
                mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println();

  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }

  WifiEspNow.onReceive(printReceivedMessage, nullptr);

  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }
  S_valorX="";
  S_valorY="";
  S_valorZ="";
  S_opc="";
  S_modo="";
  S_mov="";
   cont =0;
  inicio = 0;
  while(inicio != 1){
      char msg[60];
  int lenm = snprintf(msg, sizeof(msg), "Inicio");
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenm);
  delay(20);
      status_A = WifiEspNow.getSendStatus();
      if(status_A == WifiEspNowSendStatus::OK)
    { inicio = 1;}
    Serial.print(".");
    }
    Serial.println();
}

void
loop()
{
  //#####Desconexion
  while(cont>10)
  {
      char msg[60];
  int lenm = snprintf(msg, sizeof(msg), "Inicio");
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenm);
  delay(20);
      status_A = WifiEspNow.getSendStatus();
    if(status_A == WifiEspNowSendStatus::OK)
    { cont=0;}
    //Serial.println("Desconectado");
  }
  //#####Desconexion
//Obtencion de las orientaciones
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);
//  updateGiro();
// Detalle: hasta ahora los valores validos son girosc_ang_x y girosc_ang_y

    int n=WiFi.scanNetworks();
    if(n==0)
    Serial.println("No networks found");
    else{
      for(int i=0;i<n;++i){
       if(WiFi.SSID(i)!="WiFi-Meat")
        continue;
     RSSI_=WiFi.RSSI(i);
      
        }}


//Procedimiento para TX las Orientaciones y RSSI
  char msg[60];
  int lenx = snprintf(msg, sizeof(msg), "X %d",valx);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenx);
  int leny = snprintf(msg, sizeof(msg), "Y %d",valy);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), leny);
  int lenz = snprintf(msg, sizeof(msg), "Z %d",valz);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenz);
    int lenrssi = snprintf(msg, sizeof(msg), "R %f",RSSI_);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenrssi);
 delay(50);
  //Fin TX

//!!!Estado de conexion
    status_A = WifiEspNow.getSendStatus();
    if(status_A == WifiEspNowSendStatus::OK)
   { cont=0;}
     else if(status_A == WifiEspNowSendStatus::FAIL)
     { cont++;}
//!!!!!!!!!!!!!!!!!!!!!
  cambios.obtener_int(S_modo);
  modo=cambios.get_entero();
  cambios.set_entero(0);
  cambios.obtener_int(S_mov);
  mov=cambios.get_entero();
  cambios.set_entero(0);
  cambios.obtener_int(S_opc);
  opcion=cambios.get_entero();
  cambios.set_entero(0);
    cambios.obtener_int(S_tiempo_delay);
  tiempo_delay=cambios.get_entero();
  cambios.set_entero(0);
//Datos procesados

switch(modo){
  case 1: //Coreografia
   if (gusano_state == 1) {
    if (movment.shouldRun()) {
      movment.run();
    }
  }
  break;
  case 2: //Seguimiento
      //Obtener Opcion

   // dist_RSSI = distancia_RSSI(RSSI_);  //Distancia obtenida con el RSSI
    //Serial.print("Distancia RSSI ");
    //Serial.println(dist_RSSI);
    cambios.set_ent_X(S_valorX);
    cambios.set_ent_Y(S_valorY);
    cambios.set_ent_Z(S_valorZ);
    cambios.obtener_orientacion();
    //Serial.println("Valor en eje X");
   // Serial.println(Orientaciones.get_OX());
   // Serial.println("Valor en eje Y");
   // Serial.println(Orientaciones.get_OY());
   // Serial.println("Valor en eje Z");
   // Serial.println(Orientaciones.get_OZ());
       

    //Logica para el seguimiento
  
  break;
  case 3: //Mezcla
  //Mezclar los modos anteriores
  Serial.println("Mezcla");
  break;
}



 
 
}

float distancia_RSSI (float rssi)
{
  float expo=(-1*rssi-58.0)/26.0;
  //float Fm = rssi+75; //-98,-91,-93,-75,-72
 // float logar = 27.0*log10(5);
  //float expo=((-18-Fm-rssi-logar+48.56)/27.0);
  //Serial.println(expo);
  d_RSSI= pow(10,expo);
  return d_RSSI;
}

void on_movment() {
  switch (mov) {
    case 1:   // MOVMENT COMBINATED
      if (change_flag == 1) {
        change_flag == 0;
        for (pos = servo1.read(); pos < 90; pos += 1) {
          servo1.write(pos);
          delay(15);
        }
        for (pos = servo2.read(); pos < 90; pos += 1) {
          servo2.write(pos);
          delay(15);
        }
        for (pos = servo3.read(); pos > 45; pos -= 1) {
          servo3.write(pos);
          delay(15);
        }
      }
      if (counter < first) //LINEAR Forward
      {
        for (pos = 0; pos <= 179; pos += 1)
        {
          servo1.write(ar1[pos]);
          servo2.write(90);
          servo3.write(ar2[pos]);
          delay(tiempo_delay);
        }
        counter ++;
      }

      if (counter == first) { //PREPARE FOR CHANGE
        for (pos = 90; pos > 45; pos -= 1) {
          servo1.write(pos);
          delay(15);
        }
        for (pos = 45; pos < 90; pos += 1) {
          servo3.write(pos);
          delay(15);
        }
        counter++;
        break;
      }

      if (counter > first && counter < second) //LINEAR BACKWARD
      {
        for (pos = 0; pos <= 179; pos += 1)
        {
          servo1.write(ar2[pos]);
          servo2.write(90);
          servo3.write(ar1[pos]);
          delay(tiempo_delay);
        }
        counter ++;
      }

      if (counter == second) { //PREPARE FOR CHANGE
        for (pos = 90; pos > 45; pos -= 1) {
          servo3.write(pos);
          delay(15);
        }
        for (pos = 45; pos < 90; pos += 1) {
          servo1.write(pos);
          delay(15);
        }
        counter++;
        break;
      }

      if (counter > second && counter < third)
      {
        for (pos = 0; pos <= 179; pos += 1) {
          servo1.write(ar1[pos]);
          servo2.write(ar1[pos]);
          servo3.write(ar2[pos]);
          delay(tiempo_delay);
        }
        counter ++;
      }

      if (counter == third) { // repeat the process
        counter = 0;
      }
      break;
    case 2:
      LateralRight(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 3:
      LinearBack(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 4:
      LateralLeft(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 5:
      LinearFront(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 6:
      RotationOnFrontEdge(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 7:
      newLateral(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
  }
}
