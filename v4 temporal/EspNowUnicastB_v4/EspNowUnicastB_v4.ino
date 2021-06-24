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
#include "Thread.h"

#include "./layout/html.h"
#include "behaviors.h"

Thread movment = Thread();
int first = 5;
int second = 10;
int third = 15;

int change_flag = 0;
int init_flag = 1;

ESP8266WebServer server(80);
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

//FIN PARTE DE CARLOS

//Informacion necesaria para la Tx y Rx de datos entre ESP
String cadena, S_valorX,S_valorY,RSSI_cad, S_valorZ;
// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xFE, 0xF5, 0xC4, 0x8B, 0xE7, 0x4C};
char datoRX[6];
Mod_String Orientaciones;

WifiEspNowSendStatus status_B; 

int inicio, cont;
int modo; //Modo de trabajo del robot: 1 Coreografia, 2 Seguimiento, 3 Mezcla
int opcion; //Opcion de seguimiento : 1 Lider, 2 Seguidor
float d_RSSI, dist_RSSI;
  int val =1200;
  int valy = 9800;
   int valz = 5600;

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


void
printReceivedMessage(const uint8_t mac[6], const uint8_t* buf, size_t count, void* cbarg)
{
  //Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3],
            //    mac[4], mac[5]);
  for (int i = 0; i < count; ++i) {
   // Serial.print(static_cast<char>(buf[i]));
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
   // inicio = 1;
    break;
        case 'R':
    RSSI_cad = cadena;
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
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  //WiFi.softAPdisconnect(false);

  //iNICIO DE PARTE DE CARLOS
  server.on("/", handleRoot);
  server.on("/On-Off", handle_On_Off);
  server.on("/speed_mod_less", handle_SPD_less);
  server.on("/speed_mod_more", handle_SPD_more);

  server.on("/change_mov_1", handle_mov1);
  server.on("/change_mov_2", handle_mov2);
  server.on("/change_mov_3", handle_mov3);
  server.on("/change_mov_4", handle_mov4);
  server.on("/change_mov_5", handle_mov5);
  server.on("/change_mov_6", handle_mov6);
  server.on("/change_mov_7", handle_mov7);

  server.on("/range_mod_less", handle_range_less);
  server.on("/range_mod_more", handle_range_more);

  server.on("/swarm_behavior", handle_swarm_behavior);
  server.on("/swarm_role", handle_swarm_role);
  server.on("/coleography", handle_coleography);
  server.on("/following", handle_following);
  server.on("/mixed", handle_mixed);
  server.on("/leader", handle_leader);
  server.on("/follower", handle_follower);
  server.begin();

  mov = 1;

  servo1.attach(14); //5-14 D5
  servo2.attach(12); //6-12 D6
  servo3.attach(15);  //16-15 D8

  movment.onRun(on_movment);
  movment.setInterval(tiempo_delay);
  //FIN DE PARTE DE CARLOS

  //Serial.print("MAC address of this node is ");
  //Serial.println(WiFi.softAPmacAddress());

  uint8_t mac[6];
  WiFi.softAPmacAddress(mac);
  //Serial.println();
 // Serial.println("You can paste the following into the program for the other device:");
 // Serial.printf("static uint8_t PEER[]{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", mac[0],
               // mac[1], mac[2], mac[3], mac[4], mac[5]);
  //Serial.println();
modo =1;
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

  cont =0;
    inicio =0;
    //Sincronia
    while(inicio != 1){
      char msg[60];
  int lenm = snprintf(msg, sizeof(msg), "Inicio");
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenm);
  delay(20);
      status_B = WifiEspNow.getSendStatus();
      if(status_B == WifiEspNowSendStatus::OK)
    { inicio = 1;}
     Serial.print(".");
    }
    Serial.println();
    //Fin Sincronia
}


void
loop()
{

  server.handleClient();

  //####Desconexion
while(cont>5)
  {
      char msg[60];
  int lenm = snprintf(msg, sizeof(msg), "Inicio");
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenm);
  delay(20);
      status_B = WifiEspNow.getSendStatus();
    if(status_B == WifiEspNowSendStatus::OK)
    { cont=0;}
    Serial.println("Desconectado");
  }
  //####
 
//Procedimiento para TX las Orientaciones
 char msg[60];
  int len = snprintf(msg, sizeof(msg), "X %d", val);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len);
  int leny = snprintf(msg, sizeof(msg), "Y %d", valy);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), leny);
  int lenz = snprintf(msg, sizeof(msg), "Z %d", valz);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenz);
  int modo_w = snprintf(msg, sizeof(msg), "M %d", modo);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), modo_w);
  int mov_w = snprintf(msg, sizeof(msg), "V %d", mov);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), mov_w);
  int opc_w = snprintf(msg, sizeof(msg), "O %d", opcion);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), opc_w);
  int time_delay_w = snprintf(msg, sizeof(msg), "D %d", tiempo_delay);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), time_delay_w);
  int gus_state_w = snprintf(msg, sizeof(msg), "G %d", gusano_state);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), gus_state_w);
//Fin TX
delay(50);
//!!!!!!!!!!!!!!!!!!!!!
    status_B = WifiEspNow.getSendStatus();
    if(status_B == WifiEspNowSendStatus::OK)
   { cont=0;}
    else if(status_B == WifiEspNowSendStatus::FAIL)
     { cont++;}
 
   Serial.print("Modo "); Serial.println(modo);
   Serial.print("Opcion"); Serial.println(opcion);
//!!!!!!!!!!!!!!!!!!!!!

//~~~~~Rx de Orientacion A y RSSI
     /* Orientaciones.set_ent_X(S_valorX);
      Orientaciones.set_ent_Y(S_valorY);
      Orientaciones.set_ent_Z(S_valorZ);
      Orientaciones.set_ent_R(RSSI_cad);
      Orientaciones.obtener_orientacion();
      orient_X_A=Orientaciones.get_OX();
      orient_Y_A=Orientaciones.get_OY();
      orient_Z_A=Orientaciones.get_OZ();
      Rssi_B=Orientaciones.get_RSSI_D();*/

    //Serial.print("RSSI ");
    //Serial.println(Rssi_B);

//~~~~~Rx de Orientacion y RSSI
  
switch(modo){
  case 1: //Coreografia
   if (gusano_state == 1) {
    if (movment.shouldRun()) {
      movment.run();
    }
  }
  break;
  case 2: //Seguimiento
      //if(opcion == 1)//Lider
     // {
      //  if (gusano_state == 1) {
    //if (movment.shouldRun()) {
    //  movment.run();
   // }
 // }
     // }
      
      
      
      //Obtener Opcion
          
   
   // Orientaciones.set_ent_X(S_valorX);
   // Orientaciones.set_ent_Y(S_valorY);
   // Orientaciones.set_ent_Z(S_valorZ);
   // Orientaciones.set_ent_R(RSSI_cad);
   // Orientaciones.obtener_orientacion();
   // Serial.print("RSSI ");
   // Serial.println(Orientaciones.get_RSSI_D());
     /*dist_RSSI = distancia_RSSI(Orientaciones.get_RSSI_D());  //Distancia obtenida con el RSSI
    Serial.println("Valor en eje X");
    Serial.println(Orientaciones.get_OX());
    Serial.println("Valor en eje Y");
    Serial.println(Orientaciones.get_OY());
    Serial.println("Valor en eje Z");
    Serial.println(Orientaciones.get_OZ());
    Serial.print("Distancia ");
    Serial.println(dist_RSSI);
    //Logica para el seguimiento
  */
  break;
  case 3: //Mezcla
  //Mezclar los modos anteriores
 
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

//INICIO DE PARTE DE CARLOS
/*************************************************************
  FUNCTION DEFINITION
***************************************************************/

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

/************************************************************************************************
  HandleR Functions
*************************************************************************************************/

void handle_On_Off() {
  if (gusano_state == 0) {
    gusano_state = 1;
  } else {
    gusano_state = 0;
  }
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

void handle_SPD_more() {
  if (tiempo_delay <= 10) {
    tiempo_delay = 10;
  } else {
    tiempo_delay = tiempo_delay - 5;
  }
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

void handle_SPD_less() {
  if (tiempo_delay >= 25) {
    tiempo_delay = 25;
  } else {
    tiempo_delay = tiempo_delay + 5;
  }
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

void handleRoot() {
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

/************************************************************************************************
  Handle Movments Functions
*************************************************************************************************/

void handle_mov1() { // MOVMENT COMBINATED
  mov = 1;
  change_flag = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}
void handle_mov2() { //LATERAL Right
  mov = 2;
  change_flag = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}
void handle_mov3() { //LINEAR fORWARD
  mov = 3;
  change_flag = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}
void handle_mov4() { //LINEAR fORWARD
  mov = 4;
  change_flag = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}
void handle_mov5() { //LINEAR fORWARD
  mov = 5;
  change_flag = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}
void handle_mov6() { //Rotation
  mov = 6;
  change_flag = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}
void handle_mov7() { //new Lateral
  mov = 7;
  change_flag = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

/************************************************************************************************
  Handle Range Functions
*************************************************************************************************/

void handle_range_less() {
  if (first <= 5) {
    first = 5;
    second = 10;
    third = 15;
  } else {
    first -= 5;
    second -= 5;
    third -= 5;
  }
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

void handle_range_more() {
  if (first >= 20) {
    first = 20;
    second = 25;
    third = 30;
  } else {
    first += 5;
    second += 5;
    third += 5;
  }
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

/************************************************************************************************
  Handle swarm functions
*************************************************************************************************/
void handle_swarm_behavior(){
  
  server.send(200, "text/html", SendSwarmBehavior());
  
}

void handle_swarm_role(){
  
  server.send(200, "text/html", SendSwarmRole());
  
}

void handle_coleography(){
  modo = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

void handle_following(){
  modo = 2;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

void handle_mixed(){
  modo = 3;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

void handle_leader(){
  opcion = 1;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

void handle_follower(){
  opcion = 2;
  server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));
}

//FIN DE PARTE DE CARLOS
