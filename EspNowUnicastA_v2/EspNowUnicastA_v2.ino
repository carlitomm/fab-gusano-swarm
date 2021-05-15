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

//Informacion necesaria para la Tx y Rx de datos entre ESP
String cadena, S_valorX,S_valorY,S_valorZ;
// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xDA, 0xBF, 0xC0, 0xF9, 0xEA, 0x25};
char datoRX[6];
Mod_String Orientaciones;

int inicio;
int modo =1; //Modo de trabajo del robot: 1 Coreografia, 2 Seguimiento, 3 Mezcla
int opcion =1; //Opcion de seguimiento : 1 Lider, 2 Seguidor
int valx = 3000;
int valy = 4000;
int valz = 5000;
int RSSI_;
int d_RSSI, dist_RSSI;

//Variables para la obtencion de las orientaciones
const int mpuAddress = 0x68;
MPU6050 mpu(mpuAddress);
float accel_ang_x, accel_ang_y;
int16_t ax, ay, az;
int16_t gx, gy, gz;
long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y, girosc_ang_z;
float girosc_ang_x_prev, girosc_ang_y_prev, girosc_ang_z_prev;
//****

void updateGiro(){
  dt = (millis() - tiempo_prev) / 1000.0;
  //dt = millis() - tiempo_prev;
  tiempo_prev = millis();

accel_ang_x = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0/ 3.14);
accel_ang_y = atan(-ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0/ 3.14);

 // girosc_ang_x = 0.98*(girosc_ang_x_prev + (gx / 131)*dt) + 0.02*accel_ang_x;
 // girosc_ang_y = 0.98*(girosc_ang_y_prev + (gy / 131)*dt) + 0.02*accel_ang_y;
   
//girosc_ang_x = (gx / 131)*dt / 1000.0 + girosc_ang_x_prev;
//girosc_ang_y = (gy / 131)*dt / 1000.0 + girosc_ang_y_prev;
 //girosc_ang_z = (gz / 131)*dt/ 1000.0 + girosc_ang_z_prev;

  girosc_ang_x_prev = girosc_ang_x;
    girosc_ang_y_prev = girosc_ang_y;
      girosc_ang_z_prev = girosc_ang_z;
  
}

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
    inicio = 1;
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
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);
  
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

  inicio = 0;

  /*while(inicio == 0){ //Sincronizacion cambiarla y pasarla al setup
    char msg[60];
  int ini = snprintf(msg, sizeof(msg), "I %d",1);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), ini);
  Serial.println("Pareja desconectada");
  delay(50);
    //*****Fin Sincronizacion Falla por ahora
  }*/
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
  Serial.println("WiFi connected");
}

void
loop()
{
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
  Serial.println("WiFi connected");
 
//Leer los datos modo y opcion de la web

//Obtencion de las orientaciones
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);
  updateGiro();
// Detalle: hasta ahora los valores validos son girosc_ang_x y girosc_ang_y

//Procedimiento para TX las Orientaciones y RSSI
  char msg[60];
  int lenx = snprintf(msg, sizeof(msg), "X %d",valx);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenx);
  int leny = snprintf(msg, sizeof(msg), "Y %d",valy);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), leny);
  int lenz = snprintf(msg, sizeof(msg), "Z %d",valz);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenz);
    int lenrssi = snprintf(msg, sizeof(msg), "R %d",RSSI_);
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), lenrssi);
  //Fin TX
  
switch(modo){
  case 1: //Coreografia
  //Incorporar movimientos iniciales
  break;
  case 2: //Seguimiento
      //Obtener Opcion
    RSSI_= WiFi.RSSI();
    dist_RSSI = distancia_RSSI(RSSI_);  //Distancia obtenida con el RSSI
    Orientaciones.set_ent_X(S_valorX);
    Orientaciones.set_ent_Y(S_valorY);
    Orientaciones.set_ent_Z(S_valorZ);
    Orientaciones.obtener_orientacion();
    Serial.println("Valor en eje X");
    Serial.println(Orientaciones.get_OX());
    Serial.println("Valor en eje Y");
    Serial.println(Orientaciones.get_OY());
    Serial.println("Valor en eje Z");
    Serial.println(Orientaciones.get_OZ());
        Serial.print("Distancia ");
    Serial.println(dist_RSSI);

    //Logica para el seguimiento
  
  break;
  case 3: //Mezcla
  //Mezclar los modos anteriores
  break;
}



 
  delay(1000);
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
