#include<SoftwareSerial.h>
#include<XBee.h> 


union int_b {
  int bval;
  byte  b[8];
} ;

class Comunicacion_XBee{
private:
  //Crear objetos tipo Xbee para trabajar en la clase
	XBee xbee = XBee(); 
	XBeeResponse response = XBeeResponse();
   SoftwareSerial nss;
	XBeeAddress64 addr64_R;
	XBeeAddress64 addr64_C;
	ZBTxRequest C_tx;   	//Coordinador es quien transmite
	ZBTxRequest R_tx;	//Router es quien transmite
	ZBTxStatusResponse txStatus = ZBTxStatusResponse();
     ZBRxResponse C_rx = ZBRxResponse();
	//Coordinador es quien recibe
  	ZBRxResponse R_rx = ZBRxResponse();
	//Router es quien recibe
	ModemStatusResponse C_msr = ModemStatusResponse();
	ModemStatusResponse R_msr = ModemStatusResponse();

//Comandos AT
// Turn on I/O sampling
uint8_t irCmd[2];
uint8_t irValue[2];                                  
uint8_t d1Cmd[2];
uint8_t d1Value[1];
RemoteAtCommandRequest remoteAtRequest;
RemoteAtCommandResponse remoteAtResponse = RemoteAtCommandResponse();

  int_b desechable_Tx;
  int_b desechable_Rx;
	int_b iniciar_AP;   //Sx de inicio mandada desde AP al St para ver si esta encendido
	int Inicio_Mov_C ;  // Variable para iniciar los mov una vez encendidos los dos gusanos
	int Inicio_Mov_R ;  // Variable para iniciar los mov una vez encendidos los dos 
  int_b dato;
	//Modo de trabajo 1: Coreografia  2: Seguir  3:Mezcla
int_b modo_C;
int_b modo_R;
	//Opcion de seguimiento 1:Lider 2:Seguidor
int_b opcion_C;
int_b opcion_R;
int_b opcion_Temp;
int fallo_Tx;
int fallo_Rx;
	
	int_b  C_orientacion_X;
	int_b  C_orientacion_Y;
	int_b  C_orientacion_Z;
	int_b  R_orientacion_X;
	int_b  R_orientacion_Y;
	int_b  R_orientacion_Z;


	int_b RSSI_;

	uint8_t payload[8];   //Variable para  la Tx de datos Modo API
public:
  void Inicializacion();
	Comunicacion_XBee();		//Constructor de la clase
	void IniciarCom();
	void RecibirInicio();
 void ConfInicio();
 void RecibirConfirmacion();
	//void ConfirmCom();
	//void RecibirConfirm();
  void RxDato();
  
	void TxModo();
	void RxModo();

	void TxOpcion();
	void RxOpcion();

	void C_TxOrientacionX();
	void C_RxOrientacionX();
	void C_TxOrientacionY();
	void C_RxOrientacionY();
	void C_TxOrientacionZ();
	void C_RxOrientacionZ();

	void R_TxOrientacionX();
	void R_RxOrientacionX();
	void R_TxOrientacionY();
	void R_RxOrientacionY();
	void R_TxOrientacionZ();
	void R_RxOrientacionZ();

	void TxRSSI();
	void RxRSSI();
	void ComanATOFFIT();	//Enviar Comando AT para generar IT
	void ComanATOnIT();
	void sendRemoteAtCommand(); 

	void set_addr64_R (uint32_t msb, uint32_t lsb);  	
	XBeeAddress64 get_addr64_R ();
	void set_addr64_C (uint32_t msb, uint32_t lsb);
	XBeeAddress64 get_addr64_C ();	//falta entrada
	
	//void set_d1Value (uint8_t value);
	//uint8_t  get_d1Value ();


	void set_iniciar_AP (int value);
	int get_iniciar_AP  ();

	void set_Inicio_Mov_C (int value);
	int get_Inicio_Mov_C ();

	void set_Inicio_Mov_R (int value);
	int get_Inicio_Mov_R ();

  void set_dato(int_b value);
  int_b get_dato();
  
	void set_modo_C(int value);
	int get_modo_C();

	void set_modo_R(int value);
	int get_modo_R();

	void set_opcion_C(int value);
	int get_opcion_C();

	void set_opcion_R(int value);
	int get_opcion_R();

	void set_fallo_Tx(int value);
	int get_fallo_Tx();

	void set_fallo_Rx(int value);
	int get_fallo_Rx();

	void set_C_orientacion_X(int value);
	int get_C_orientacion_X();

	void set_C_orientacion_Y(int value);
	int get_C_orientacion_Y();

	void set_C_orientacion_Z(int value);
	int get_C_orientacion_Z();

	void set_R_orientacion_X(int value);
	int get_R_orientacion_X();

	void set_R_orientacion_Y(int value);
	int get_R_orientacion_Y();

	void set_R_orientacion_Z(int value);
	int get_R_orientacion_Z();

	void set_RSSI(int value);
	int get_RSSI();

};
