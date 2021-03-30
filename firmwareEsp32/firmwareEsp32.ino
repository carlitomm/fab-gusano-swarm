#include <stdio.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>

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

  server.on("/swarm_behavior", handle_swarm_behavior);
  server.on("/swarm_role", handle_swarm_role);
  server.begin();

  //server.send(200, "text/html", SendHTML(gusano_state, mov, tiempo_delay));

  servo1.attach(4); //5-14 D5
  servo2.attach(5); //6-12 D6
  servo3.attach(1);  //16-15 D8

  mov = 1;

  movment.onRun(on_movment);
  movment.setInterval(tiempo_delay);

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
