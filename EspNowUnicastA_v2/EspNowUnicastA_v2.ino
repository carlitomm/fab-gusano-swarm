#include <WifiEspNow.h> //Router
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif
#include "Mod_String.h"
#include "Wire.h"

//INICIO PARTE DE CARLOS
#include <ESP8266WebServer.h>
#include "Servo.h"
#include "Thread.h"
#include "./layout/html.h"
#include "behaviors.h"
#include <SimpleKalmanFilter.h>
#include "Kalman.h" 

Mod_String cambios;
Thread movment = Thread();
int first = 5;
int second = 10;
int third = 15;

int change_flag = 0;
int init_flag = 1;

const char* ssid = "WiFi-Meat";
const char* password = "12345678";

int mov;
int tiempo_delay = 20;
uint8_t gusano_state = 1;
int i, j;
int pos = 0;
int counter = 0;

Servo servo1;
Servo servo2;
Servo servo3;

//Informacion necesaria para la Tx y Rx de datos entre ESP
String cadena, S_gus_state, S_valorX,S_valorY,S_valorZ, S_modo, S_opc, S_mov, S_tiempo_delay;
// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xDA, 0xBF, 0xC0, 0xF9, 0xEA, 0x25};
char datoRX[6];
SimpleKalmanFilter simpleKalmanFilter(4, 4, 0.01);
WifiEspNowSendStatus status_A; 

int inicio, cont, estado;
int modo; //Modo de trabajo del robot: 1 Coreografia, 2 Seguimiento, 3 Mezcla
int opcion;
float RSSI_; int n;
float d_RSSI, dist_RSSI;
int cont_rssi;

//===Variables Seguimiento===
int iteracion, inter, parte, etapa;
bool movim;
float dist_prev;
//===========================

float Rssi_A, Rssi_B, RSSI_prev;


//=======Funcion para la RX de datos entre ESP=======
void printReceivedMessage(const uint8_t mac[6], const uint8_t* buf, size_t count, void* cbarg) {
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
    inicio = 1;
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
    case 'G':
    S_gus_state= cadena;
    break;  

  }
  
  cadena="";
}
//==================================================

void
setup()
{
  Serial.begin(115200);
  Wire.begin();
  Serial.println();

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, password);
   
  mov = 1;

  servo1.attach(14); //5-14 D5
  servo2.attach(12); //6-12 D6
  servo3.attach(15);  //16-15 D8

  movment.onRun(on_movment);
  movment.setInterval(tiempo_delay);
  
  uint8_t mac[6];
  WiFi.softAPmacAddress(mac);
  
  bool ok = WifiEspNow.begin();
  if (!ok) {
    //Serial.println("WifiEspNow.begin() failed");
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
  dist_RSSI=1;
   cont =0;
   modo =2;
   opcion=1;
   iteracion=0;
   movim=0;
   etapa = 1;
   parte = 0;
 WiFi.scanNetworks(true,false);
   estado = 0;
  inicio = 0;

//=======Sincronia inicial=======
  while(estado != 1){ //TX
    
      char msg[60];
  int lenm = snprintf(msg, sizeof(msg), "Inicio");
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenm);
  delay(20);
      status_A = WifiEspNow.getSendStatus();
      if(status_A == WifiEspNowSendStatus::OK)
    { estado = 1; }
    else {estado = 0; }
    Serial.print(".");
    }

  while(inicio != 1){ //Confirmacion
    Serial.println("Esperando Confirmacion");
     delay(20);
  }  
      Serial.println();
//======================================
  
}

void
loop()
{
//=======Desconexion=======
 /* while(cont>5)
  {
      char msg[60];
  int lenm = snprintf(msg, sizeof(msg), "Inicio");
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenm);
  delay(20);
      status_A = WifiEspNow.getSendStatus();
    if(status_A == WifiEspNowSendStatus::OK)
    { cont=0;}
  }*/
//======================================
 
//=======Obtencion del RSSI=======
//Actualizacion del valor cada 6 ciclos
//De no hacerlo asi se interrumpe el movimiento
if(cont_rssi == 5){
  dist_prev = dist_RSSI;
  RSSI_prev = Rssi_A;
  WiFi.scanNetworks(true,false);
  cont_rssi=0;
}
else if(cont_rssi > 1) 
{
        int n=WiFi.scanComplete();
        if(n > -1)
   {
    for(int i=0;i<n;++i){
      if(WiFi.SSID(i)!="WiFi-Meat")
       continue;
      RSSI_=WiFi.RSSI(i);
      Rssi_A=simpleKalmanFilter.updateEstimate(RSSI_);
      }
    }
}
  cont_rssi ++;
  dist_RSSI = distancia_RSSI(Rssi_A);
//======================================

//=======Procedimiento para TX=======
  char msg[60];
  int lenrssi = snprintf(msg, sizeof(msg), "R %f",RSSI_);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenrssi);
 int lenrssi_prev = snprintf(msg, sizeof(msg), "P %f",RSSI_prev);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenrssi_prev);
  delay(50);
//======================================  

//=======Estado de conexion=======
 /*   status_A = WifiEspNow.getSendStatus();
    if(status_A == WifiEspNowSendStatus::OK)
   { cont=0;}
     else if(status_A == WifiEspNowSendStatus::FAIL)
     { cont++;}*/
//======================================
  
//=======Procesar datos recibidos=======
  cambios.obtener_int(S_modo);
  modo=cambios.get_entero();
  cambios.set_entero(0);
  cambios.obtener_int(S_mov);
  mov=cambios.get_entero();
  cambios.set_entero(0);
  cambios.obtener_int(S_opc);
  opcion=cambios.get_entero();
  cambios.set_entero(0);
  cambios.obtener_int_delay(S_tiempo_delay);
  tiempo_delay=cambios.get_entero();
  cambios.set_entero(0);
  cambios.obtener_int(S_gus_state);
  gusano_state=cambios.get_entero();
  cambios.set_entero(0);
//======================================

switch(modo){
  case 1: //Coreografia
   if (gusano_state == 1) {
    if (movment.shouldRun()) {
      movment.run();
    }
  }
  break;
  case 2: //Seguimiento
     if(opcion == 1)  //Seguidor
     {
      if(dist_RSSI > 0.40){
      seguimiento(); 
      
      }
  else {
    etapa = 1;
    parte = 0;
    if (gusano_state == 1) {
      if (movment.shouldRun()) {
       movment.run();
      }
    }
  }
     }
     else if (opcion == 2)  //Lider
     {
       if (gusano_state == 1) {
          if (movment.shouldRun()) {
          movment.run();
          }
        }
     }
 break;
  case 3: //Mezcla
  //Mezclar los modos anteriores
  //Creo que en el anterior se mezclan ya
  break;
   
}
}


void seguimiento()  //Esta funciona bien, caotica, pero bien 
{
  
    if(dist_RSSI > 0.50){
      if(iteracion < 7){ //LateralRight
        switch(movim){
          case 0:
          LateralLeft(change_flag, servo1, servo2, servo3, tiempo_delay);
          break;
          case 1:
          LateralRight(change_flag, servo1, servo2, servo3, tiempo_delay);
          break;
        }
        iteracion++; 
    }
    if(iteracion == 7){
        if(dist_RSSI > dist_prev){  //LateralLeft
      movim = !movim;   //VER SI ESTE COMANDO SE PUEDE USAR
      iteracion=0;
    } else if(dist_RSSI < dist_prev)
    {
      movim = movim;
      iteracion=0;
    }
    }
    }
    else  {
      //Definir movimientos para cuando estan cerca el uno del otro
      iteracion=0;
      movim = 0;
      if (gusano_state == 1) {
    if (movment.shouldRun()) {
      movment.run();
    }
  }
    }
   

  

}



//Funciona, se acerca
//PROBLEMA: despues de un tiempo se detiene
/*void seguimiento()
{
         switch (etapa){
          case 1: 
         
            //Serial.println("Etapa 1");
            if(parte < 4){
              //Serial.println("Parte < 4");
              LateralLeft(change_flag, servo1, servo2, servo3, tiempo_delay);
              parte ++;
            } else if(parte > 3){
              //Serial.println("Parte > 3");
              LinearFront(change_flag, servo1, servo2, servo3, tiempo_delay);
              parte ++;
            }
        
        break;
        case 2: 
        //Serial.println("Etapa 2");
          if(parte < 2){ //Serial.println("Parte < 2");
            LinearBack(change_flag, servo1, servo2, servo3, tiempo_delay);
            parte ++;
          } else if(parte > 1){ //Serial.println("Parte > 1");
            LateralLeft(change_flag, servo1, servo2, servo3, tiempo_delay);
            parte ++;
          }
      
        break;
        case 3: 
        //Serial.println("Etapa 3");
          if(parte < 4){ //Serial.println("Parte < 4");
            LateralRight(change_flag, servo1, servo2, servo3, tiempo_delay);
            parte ++;
          } else if(parte > 3){ //Serial.println("Parte > 3");
            LinearFront(change_flag, servo1, servo2, servo3, tiempo_delay);
            parte ++;
          }
        
        break;
        case 4: 
        //Serial.println("Etapa 4");
          if(parte < 2){ //Serial.println("Parte < 2");
            LinearBack(change_flag, servo1, servo2, servo3, tiempo_delay);
            parte ++;
          } else if(parte > 1){ //Serial.println("Parte > 1");
            LateralRight(change_flag, servo1, servo2, servo3, tiempo_delay);
            parte ++;
          }
          
        break;
    }
//=======================================================
 if (parte == 6){
              parte = 0;
              if (dist_prev < dist_RSSI){
                //Mov erroneo
                //Serial.println("Mov erroneo");
                etapa ++;
                if(etapa == 5){
                  etapa = 1;
                }
              } else if (dist_prev > dist_RSSI){
                 //Serial.println("Mov correcto");
                etapa = etapa;
              }
            }
//========================================================
}*/


float distancia_RSSI (float rssi)
{
  float expo=(-1*rssi-59.0)/27.0;
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
    //Serial.println("Mov 1");
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
     // Serial.println("Mov 2");
      LateralRight(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 3:
    //Serial.println("Mov 3");
      LinearBack(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 4:
    //Serial.println("Mov 4");
      LateralLeft(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 5:
    //Serial.println("Mov 5");
      LinearFront(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 6:
    //Serial.println("Mov 6");
      RotationOnFrontEdge(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
    case 7:
      //Serial.println("Mov 7");
      newLateral(change_flag, servo1, servo2, servo3, tiempo_delay);
      break;
  }
}
