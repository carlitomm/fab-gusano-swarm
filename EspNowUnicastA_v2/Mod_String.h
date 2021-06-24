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
	float OX;
	float OY;
	float OZ;
  int RSSI_D;
    int entero;

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
	void set_OX(float value);
	float get_OX();
	void set_OY(float value);
	float get_OY();
	void set_OZ(float value);
	float get_OZ();
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
	void obtener_int(String cadena);
    void obtener_int_delay(String cadena);
  void set_entero(int value);
  int get_entero();
};
