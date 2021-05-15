#include "String.h" 
#include <ESP8266WiFi.h>

union int_b {
  int bval;
  byte  b[8];
} ;

class Mod_String{
private:
	String entrada;
	String orientacionX;
	String orientacionY;
	String orientacionZ;
    String RSSI_CADENA;
    String ent_X;
  String ent_Y;
  String ent_Z;
    String ent_R;
	int OX;
	int OY;
	int OZ;
  int RSSI_D;

public:
	Mod_String();		//Constructor de la clase
	

void obtener_enteros();
void obtener_orientacion();
	void set_entrada(String value);
	String get_entrada();
	void set_orientacionX(String value);
	String get_orientacionX();
	void set_orientacionY(String value);
	String get_orientacionY();
	void set_orientacionZ(String value);
	String get_orientacionZ();
      void set_RSSI_CADENA(String value);
  String get_RSSI_CADENA();
	void set_OX(int value);
	int get_OX();
	void set_OY(int value);
	int get_OY();
	void set_OZ(int value);
	int get_OZ();
   void set_RSSI_D(int value);
  int get_RSSI_D();
   void set_ent_X(String value); 
  String get_ent_X();
  void set_ent_Y(String value);
  String get_ent_Y();
  void set_ent_Z(String value);
  String get_ent_Z();
  void set_ent_R(String value);
  String get_ent_R();
	

};
