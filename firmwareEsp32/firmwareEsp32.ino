#include <Servo.h>
#include <stdio.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Thread.h>
#include <WiFiClient.h>

Thread movment = Thread();
int first = 5;
int second = 10;
int third = 15;

int change_flag = 0;
int init_flag = 1;

WebServer server(80);
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

/*************************************************************************
   FUNCTIONS PROTOTIPES
  /************************************************************************/

void on_movment();
String SendHTML(uint8_t gusano_state, int movment, int Tdelay);
void LateralRight();
void LinearBack();
void LateralLeft();

/*************************************************************************
   ARRAY DEFINITION FOR SERVOS
  /************************************************************************/
int ar1[] = {90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90};
int ar2[] = {45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45};
int ar3[] = {45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 132, 134, 136, 138, 140, 138, 136, 134, 132, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45};

int mov2_1[] = {90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135};
int mov2_2[] = {135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90};
int mov2_3[] = {90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135};  //136

int mov3_1[] = {90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180};
int mov3_2[] = {180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90};
int mov3_3[] = {90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180};
//mov 3 Rotacion completa

int mov4_1[] = {45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45};
int mov4_2[] = {90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90};
int mov4_3[] = {135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135}; //181
//mov 4 Rotacion en el lugar


/************************************************************************************************
  SET UP
*************************************************************************************************/

void setup() {

  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

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
  server.begin();

  //server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));

  servo1.attach(4); //5-14 D5
  servo2.attach(5); //6-12 D6
  servo3.attach(1);  //16-15 D8

  mov = 1;

  movment.onRun(on_movment);
  movment.setInterval(tiempo_delay);

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

/*************************************************************
  LOOP*
***************************************************************/

void loop() {

  server.handleClient();

  if (gusano_state == 1) {
    if (movment.shouldRun()) {
      movment.run();
    }
  }
}

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
      LateralRight();
      break;
    case 3:
      LinearBack();
      break;
    case 4:
      LateralLeft();
      break;
    case 5:
      LinearFront();
      break;
    case 6:
      RotationOnFrontEdge();
      break;
    case 7:
      newLateral();
      break;
  }
}

/************************************************************************************************
  HTML TEXT
*************************************************************************************************/

String SendHTML(uint8_t gusano_state, int movment, int Tdelay) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name =\"veiwport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>MEAT Control</title>\n";

  ptr += "<style>html { font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #9c4444; margin: 50px auto 30px; } h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #b43333;margin: 50px;} h2 {color: #444444; padding: 30px 0px; margin: 50px; background-color: #5c8297c0;}\n";
  ptr += ".button {display: block; width: 80px; background-color: #3498db; border: none; color: white; padding: 13px 30px; text-decoration: none; font-size: 35px; margin: 0px auto 30px; cursor: pointer; border-radius: 4px}\n";

  ptr += ".button-on {background-color: #65e70f; width: 100px; height: 45px; font-size: 45px;}\n";
  ptr += ".button-on:active {background-color: #65e70f; width: 100px;}\n";
  ptr += ".button-off {background-color: #ee0808; width: 120px; height: 60px; font-size: 45px;}\n";
  ptr += ".button-off:active {background-color: #ee0808; width: 100px;}\n";

  ptr += ".button-spd {background-color: #3498db; margin: auto}\n";
  ptr += ".button-mov {background-color: #3498db; width: 170px; text-align: center;}\n";

  ptr += "p {font-size: 35px;}";
  ptr += "</style>\n";

  ptr += "</head>\n";

  ptr += "<body>\n";
  ptr += "<h1>MEAT WEB SERVER</h1>\n";

  if (gusano_state) {
    ptr += "<p>Status:Turn ON</p> <a class=\"button button-off\" href = \"/On-Off\"> OFF </a> \n";
  } else {
    ptr += "<p>Status:Turn OFF</p><a class=\"button button-on\" href = \"/On-Off\"> ON </a>\n";
    if (Tdelay == 15)
    {
      ptr += "<h2>Speed: NORMAL</h2>\n";
      ptr += "<div style=\"display:flex\">\n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_less\"> --- </a> \n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_more\" > +++ </a> \n";
      ptr += "</div>\n";
    }
    if (Tdelay == 10)
    {
      ptr += "<h2>Speed: FAST</h2>\n";
      ptr += "<div style=\"display:flex\">\n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_less\"> --- </a> \n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_more\"> +++ </a> \n";
      ptr += "</div>\n";
    }
    if (Tdelay == 20)
    {
      ptr += "<h2>Speed: SLOW</h2>\n";
      ptr += "<div style=\"display:flex\">\n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_less\"> --- </a> \n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_more\" > +++ </a> \n";
      ptr += "</div>\n";
    }
    if (Tdelay == 25)
    {
      ptr += "<h2>Speed: SUPER SLOW</h2>\n";
      ptr += "<div style=\"display:flex\">\n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_less\" > --- </a> \n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_more\" > +++ </a> \n";
      ptr += "</div>\n";
    }

    if (mov == 1)
    {
      ptr += "<h2>Movement Type: COMBINED</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LINEAR FRONT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";

      /*if (first == 5) {
        ptr += "<h2>Movement Range: SHORT</h2>";
        }
        if (first == 10) {
        ptr += "<h2>Movement Range: NORMAL</h2>";
        }
        if (first == 15) {
        ptr += "<h2>Movement Range: LARGE</h2>";
        }
        if (first == 20) {
        ptr += "<h2>Movement Range: LARGER</h2>";
        }
        ptr += "<div style=\"display:flex\">\n";
        ptr += "<a class=\"button button-spd\" href = \"/range_mod_less\" > --- </a> \n";
        ptr += "<a class=\"button button-spd\" href = \"/range_mod_more\" > +++ </a> \n";
        ptr += "</div>\n";*/
    }
    if (mov == 2)
    {
      ptr += "<h2>Movement Type: LATERAL RIGHT Rotating</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LINEAR FRONT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (mov == 3)
    {
      ptr += "<h2>Movement Type: LINEAR BACK</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LINEAR FRONT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (mov == 4)
    {
      ptr += "<h2>Movement Type: LATERAL lEFT</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LINEAR FRONT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (mov == 5)
    {
      ptr += "<h2>Movement Type: LINEAR FRONT</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (mov == 6)
    {
      ptr += "<h2>Movement Type: 6 ROTATION</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (mov == 7)
    {
      ptr += "<h2>Movement Type: 7 Lateral Left Rotating</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
    }
  }

  ptr += " </body>\n";
  ptr += " </html>\n";
  return ptr;
}


/************************************************************************************************************************
  BEHAIVOIOURS
*************************************************************************************************************************/
void LateralRight() {
  if (change_flag == 1)
  {
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
  for (pos = 0; pos <= 179; pos += 1) {
    servo1.write(ar1[pos]);
    servo2.write(ar1[pos]);
    servo3.write(ar2[pos]);
    delay(tiempo_delay);
  }
}

void LinearBack() {
  if (change_flag == 1)
  {
    change_flag == 0;
    for (pos = servo1.read(); pos > 45; pos -= 1) {
      servo1.write(pos);
      delay(15);
    }
    for (pos = servo2.read(); pos < 90; pos += 1) {
      servo2.write(pos);
      delay(15);
    }
    for (pos = servo3.read(); pos < 90; pos += 1) {
      servo3.write(pos);
      delay(15);
    }
  }
  for (pos = 0; pos <= 179; pos += 1) {
    servo1.write(ar2[pos]);
    servo2.write(90);
    servo3.write(ar1[pos]);
    delay(tiempo_delay);
  }
}

void LateralLeft() {
  if (change_flag == 1)
  {
    change_flag == 0;
    for (pos = servo1.read(); pos < 90; pos += 1) {
      servo1.write(pos);
      delay(15);
    }
    for (pos = servo2.read(); pos > 45; pos -= 1) {
      servo2.write(pos);
      delay(15);
    }
    for (pos = servo3.read(); pos > 45; pos -= 1) {
      servo3.write(pos);
      delay(15);
    }
  }
  for (pos = 0; pos <= 179; pos += 1) {
    servo1.write(ar1[pos]);
    servo2.write(ar2[pos]);
    servo3.write(ar2[pos]);
    delay(tiempo_delay);
  }
}

void LinearFront() {
  if (change_flag == 1)
  {
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
  for (pos = 0; pos <= 179; pos += 1) {
    servo1.write(ar1[pos]);
    servo2.write(90);
    servo3.write(ar2[pos]);
    delay(tiempo_delay);
  }
}

void RotationOnFrontEdge() {
  if (change_flag == 1)
  {
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
  for (pos = 0; pos <= 179; pos += 1) {
    servo1.write(90);
    servo2.write(ar1[pos]);
    servo3.write(ar2[pos]);
    delay(tiempo_delay);
  }
}

void newLateral() {
  if (change_flag == 1)
  {
    change_flag == 0;
    for (pos = servo1.read(); pos < 90; pos += 1) {
      servo1.write(pos);
      delay(15);
    }
    for (pos = servo2.read(); pos > 45; pos -= 1) {
      servo2.write(pos);
      delay(15);
    }
    for (pos = servo3.read(); pos < 90; pos += 1) {
      servo3.write(pos);
      delay(15);
    }
  }
  for (pos = 0; pos <= 179; pos += 1) {
    servo1.write(ar1[pos]);
    servo2.write(ar2[pos]);
    servo3.write(ar1[pos]);
    delay(tiempo_delay);
  }
}

void compbined1() {
  if (counter < first){
    LinearFront();
  }
  
}