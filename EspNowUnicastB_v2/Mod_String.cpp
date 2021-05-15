#include "Mod_String.h"
#include "String.h"

Mod_String :: Mod_String(){

entrada="";
orientacionX="";
orientacionY="";
orientacionZ="";
RSSI_CADENA="";
ent_X="";
ent_Y="";
ent_Z="";
ent_R="";
OX=0;
OY=0;
OZ=0;
RSSI_D=0;
}

void Mod_String::obtener_orientacion(){
    
  ent_X.remove(0,1);
  ent_Y.remove(0,1);
  ent_Z.remove(0,1);
  ent_R.remove(0,1);

OX = ent_X.toInt();
OY = ent_Y.toInt();
OZ = ent_Z.toInt();
RSSI_D = ent_R.toInt();
}


void Mod_String::obtener_enteros(){
    
 entrada.remove(0,8);
orientacionX = entrada;
orientacionY = entrada;
orientacionZ = entrada;

orientacionX.remove(4,16);
OX = orientacionX.toInt();

orientacionY.remove(0,8);
orientacionY.remove(4,8);
OY = orientacionY.toInt();

orientacionZ.remove(0,16);
OZ = orientacionZ.toInt();



}



 	

void Mod_String:: set_entrada(String value){
	entrada = value;
}
String Mod_String:: get_entrada(){
	return entrada;
}

void Mod_String:: set_orientacionX(String value){
	orientacionX = value;
}
String Mod_String:: get_orientacionX(){
	return orientacionX;
}
void Mod_String:: set_orientacionY(String value){
	orientacionY = value;
}
String Mod_String:: get_orientacionY(){
	return orientacionY;
}

void Mod_String:: set_orientacionZ(String value){
	orientacionZ = value;
}
String Mod_String :: get_orientacionZ(){
	return orientacionZ;
}






void Mod_String:: set_OX(int value){
	OX = value;
}
int Mod_String:: get_OX(){
	return OX;
}

void Mod_String:: set_OY(int value){
	OY = value;
}
int Mod_String:: get_OY(){
	return OY;
}
void Mod_String:: set_OZ(int value){
	OZ = value;
}
int Mod_String:: get_OZ(){
	return OZ;
}

//_________

void Mod_String:: set_ent_X(String value){
  ent_X = value;
}
String Mod_String:: get_ent_X(){
  return ent_X;
}
void Mod_String:: set_ent_Y(String value){
  ent_Y = value;
}
String Mod_String:: get_ent_Y(){
  return ent_Y;
}

void Mod_String:: set_ent_Z(String value){
  ent_Z = value;
}
String Mod_String :: get_ent_Z(){
  return ent_Z;
}
void Mod_String:: set_ent_R(String value){
  ent_R = value;
}
String Mod_String :: get_ent_R(){
  return ent_R;
}

void Mod_String:: set_RSSI_D(int value){
 RSSI_D = value;
}
int Mod_String:: get_RSSI_D(){
  return RSSI_D;
}
void Mod_String:: set_RSSI_CADENA(String value){
  RSSI_CADENA = value;
}
String Mod_String :: get_RSSI_CADENA(){
  return RSSI_CADENA;
}
