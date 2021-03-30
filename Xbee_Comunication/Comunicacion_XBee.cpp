#include "Comunicacion_XBee.h"

void Comunicacion_XBee::Inicializacion(){

     nss.begin(115200);
     xbee.begin(nss);
}
Comunicacion_XBee :: Comunicacion_XBee(): nss( 2, 13) {

for (int i=0; i < 8 ; i++){
	payload[i]= 0;
     }
addr64_R = XBeeAddress64(0x0013A200,0x41C32576);
addr64_C = XBeeAddress64(0x0013A200,0x41C19D7D);
C_tx=ZBTxRequest(addr64_R,payload,sizeof(payload));
  R_tx=ZBTxRequest(addr64_C,payload,sizeof(payload));
   iniciar_AP.bval = 1;		//Inicializo las variables
Inicio_Mov_C = 0;
Inicio_Mov_R = 0;
dato.bval=0;
modo_C.bval=1;
modo_R.bval=1;
opcion_C.bval=1;
opcion_R.bval=2;
opcion_Temp.bval=0;
C_orientacion_X.bval=0;
C_orientacion_Y.bval=0;
C_orientacion_Z.bval=0;
R_orientacion_X.bval=0;
R_orientacion_Y.bval=0;
R_orientacion_Z.bval=0;

RSSI_.bval = 0;
fallo_Tx=0;
fallo_Rx=0;

irCmd[0] = {'I'};
irCmd[1] = {'R'};
irValue[0] = { 0xff};    
irValue[1] = { 0xff };                                    
d1Cmd[0] = { 'D'};
d1Cmd[1] = { '1' };   
d1Value[0] = { 0x04 };
remoteAtRequest = RemoteAtCommandRequest(addr64_R, irCmd, irValue, sizeof(irValue));  //Direccion comando AT

}
void Comunicacion_XBee::IniciarCom(){
     for (int i=0; i < 8 ; i++){
  payload[i]= desechable_Tx.b[i];
     }
     xbee.send(C_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
       Inicio_Mov_C = 1;
      } else {
        // the remote XBee did not receive our packet. is it powered on?
      Inicio_Mov_C = 0;
      }
    }
  } 
  delay(1000);
}

void Comunicacion_XBee::ConfInicio(){
     for (int i=0; i < 8 ; i++){
  payload[i]= desechable_Tx.b[i];
     }
     xbee.send(R_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
       Inicio_Mov_R = 1;
      } else {
        // the remote XBee did not receive our packet. is it powered on?
      Inicio_Mov_R = 0;
      }
    }
  } 
  //delay(1000);
}
void Comunicacion_XBee::RecibirInicio(){
    if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(R_rx);
         for (int i = 0; i < R_rx.getDataLength(); i++) {
          desechable_Rx.b[i] = R_rx.getData()[i];
        }
      }
    } 
        xbee.readPacket();
      
}
//DUPLICAR LAS DOS PRIMERAS FUNCIONES COMO CONFIRMACIONES
void Comunicacion_XBee::RecibirConfirmacion(){
    if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(C_rx);
         for (int i = 0; i < C_rx.getDataLength(); i++) {
          desechable_Rx.b[i] = C_rx.getData()[i];
        }
      }
    } 
        xbee.readPacket();
      
}

void Comunicacion_XBee::TxModo(){

for (int i=0; i < 8 ; i++){
	payload[i]= modo_C.b[i];  //Esto es lo que cambia 
     }
   payload[7]= 'm'; 
     xbee.send(C_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;
      } else {
        fallo_Tx=1;
      }
    }
  } 
  //Serial.println(fallo_Tx);
  delay(1000);


}
void Comunicacion_XBee::RxDato(){

    if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(C_rx);
         for (int i = 0; i < C_rx.getDataLength(); i++) {
          dato.b[i] = C_rx.getData()[i];
        }
      }
    } 
        xbee.readPacket();

}



void Comunicacion_XBee::RxModo(){

    if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(R_rx);
         for (int i = 0; i < R_rx.getDataLength(); i++) {
          modo_R.b[i] = R_rx.getData()[i];
        }
      }
    } else {
     // modo_R.bval = 1;
    }
        xbee.readPacket();

}

void Comunicacion_XBee::TxOpcion(){

for (int i=0; i < 8 ; i++){
	payload[i]= opcion_C.b[i];  //Esto es lo que cambia 
     }
     xbee.send(C_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;

      } else {
        fallo_Tx=1;

      }
    }
  } 
  //delay(1000);



}

void Comunicacion_XBee::RxOpcion(){
 
if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(R_rx);
         for (int i = 0; i < R_rx.getDataLength(); i++) {
          opcion_Temp.b[i] = R_rx.getData()[i];
        }
		
      }
    } else {
     // opcion_R.bval = 2;
    }
if(opcion_Temp.bval==1){
	opcion_R.bval=2;
} else if (opcion_Temp.bval==2){
	opcion_R.bval=1;
}
else
{ opcion_R.bval=2;}

        xbee.readPacket();



}

void Comunicacion_XBee::C_TxOrientacionX(){
for (int i=0; i < 8 ; i++){
	payload[i]= C_orientacion_X.b[i];  //Esto es lo que cambia 
     }
     xbee.send(C_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;

      } else {
        fallo_Tx=1;

      }
    }
  } 
  //delay(1000);

}

void Comunicacion_XBee::C_TxOrientacionY(){
for (int i=0; i < 8 ; i++){
	payload[i]= C_orientacion_Y.b[i];  //Esto es lo que cambia 
     }
     xbee.send(C_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;

      } else {
        fallo_Tx=1;

      }
    }
  } 
  //delay(1000);

}

void Comunicacion_XBee::C_TxOrientacionZ(){
for (int i=0; i < 8 ; i++){
	payload[i]= C_orientacion_Z.b[i];  //Esto es lo que cambia 
     }
     xbee.send(C_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;

      } else {
        fallo_Tx=1;

      }
    }
  } 
  //delay(1000);

}

void Comunicacion_XBee::C_RxOrientacionX(){
	if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(C_rx);
         for (int i = 0; i < C_rx.getDataLength(); i++) {
          R_orientacion_X.b[i] = C_rx.getData()[i];
        }
		fallo_Rx=0;
      }
    } else {
      fallo_Rx=1;
    }


        xbee.readPacket();

}


void Comunicacion_XBee::C_RxOrientacionY(){
	if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(C_rx);
         for (int i = 0; i < C_rx.getDataLength(); i++) {
          R_orientacion_Y.b[i] = C_rx.getData()[i];
        }
		fallo_Rx=0;
      }
    } else {
      fallo_Rx=1;
    }


        xbee.readPacket();

}

void Comunicacion_XBee::C_RxOrientacionZ(){
	if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(C_rx);
         for (int i = 0; i < C_rx.getDataLength(); i++) {
          R_orientacion_Z.b[i] = C_rx.getData()[i];
        }
		fallo_Rx=0;
      }
    } else {
      fallo_Rx=1;
    }


        xbee.readPacket();

}

void Comunicacion_XBee::R_TxOrientacionX(){
for (int i=0; i < 8 ; i++){
	payload[i]= R_orientacion_X.b[i];  //Esto es lo que cambia 
     }
     xbee.send(R_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;

      } else {
        fallo_Tx=1;

      }
    }
  } 
  //delay(1000);

}

void Comunicacion_XBee::R_TxOrientacionY(){
for (int i=0; i < 8 ; i++){
	payload[i]= R_orientacion_Y.b[i];  //Esto es lo que cambia 
     }
     xbee.send(R_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;

      } else {
        fallo_Tx=1;

      }
    }
  } 
  //delay(1000);

}

void Comunicacion_XBee::R_TxOrientacionZ(){
for (int i=0; i < 8 ; i++){
	payload[i]= R_orientacion_Z.b[i];  //Esto es lo que cambia 
     }
     xbee.send(R_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;

      } else {
        fallo_Tx=1;

      }
    }
  } 
  //delay(1000);

}

void Comunicacion_XBee::R_RxOrientacionX(){
	if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(R_rx);
         for (int i = 0; i < R_rx.getDataLength(); i++) {
          C_orientacion_X.b[i] = R_rx.getData()[i];
        }
		fallo_Rx=0;
      }
    } else {
      fallo_Rx=1;
    }


        xbee.readPacket();

}


void Comunicacion_XBee::R_RxOrientacionY(){
	if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(R_rx);
         for (int i = 0; i < R_rx.getDataLength(); i++) {
          C_orientacion_Y.b[i] = R_rx.getData()[i];
        }
		fallo_Rx=0;
      }
    } else {
      fallo_Rx=1;
    }


        xbee.readPacket();

}

void Comunicacion_XBee::R_RxOrientacionZ(){
	if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(R_rx);
         for (int i = 0; i < R_rx.getDataLength(); i++) {
          C_orientacion_Z.b[i] = R_rx.getData()[i];
        }
		fallo_Rx=0;
      }
    } else {
      fallo_Rx=1;
    }


        xbee.readPacket();

}



void Comunicacion_XBee::TxRSSI(){
for (int i=0; i < 8 ; i++){
	payload[i]= RSSI_.b[i];  //Esto es lo que cambia 
     }
     xbee.send(R_tx);
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        fallo_Tx=0;
      } else {
        fallo_Tx=1;
      }
    }
  } 
  //delay(1000);


}

void Comunicacion_XBee::RxRSSI(){
if (xbee.getResponse().isAvailable()) {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(C_rx);
         for (int i = 0; i < C_rx.getDataLength(); i++) {
          RSSI_.b[i] = C_rx.getData()[i];
        }
		fallo_Rx=0;
      }
    } else {
		fallo_Rx=1;
    }
        xbee.readPacket();

}

void Comunicacion_XBee::ComanATOnIT(){

d1Value[0] = { 0x04 };
sendRemoteAtCommand();
  
  // now reuse same object for DIO1 command
  remoteAtRequest.setCommand(d1Cmd);
  remoteAtRequest.setCommandValue(d1Value);
  remoteAtRequest.setCommandValueLength(sizeof(d1Value));

  sendRemoteAtCommand();
  
 remoteAtRequest.clearCommandValue();
}

void Comunicacion_XBee::ComanATOFFIT(){

d1Value[0] = { 0x05 };

sendRemoteAtCommand();
  
  // now reuse same object for DIO1 command
  remoteAtRequest.setCommand(d1Cmd);
  remoteAtRequest.setCommandValue(d1Value);
  remoteAtRequest.setCommandValueLength(sizeof(d1Value));

  sendRemoteAtCommand();
  
 remoteAtRequest.clearCommandValue();
}



void Comunicacion_XBee :: sendRemoteAtCommand() {
  nss.println("Sending command to the XBee");

  // send the command
  xbee.send(remoteAtRequest);

  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {
    // got a response!

    // should be an AT command response
    if (xbee.getResponse().getApiId() == REMOTE_AT_COMMAND_RESPONSE) {
      xbee.getResponse().getRemoteAtCommandResponse(remoteAtResponse);

      if (remoteAtResponse.isOk()) {
        nss.print("Command [");
        nss.print(remoteAtResponse.getCommand()[0]);
        nss.print(remoteAtResponse.getCommand()[1]);
        nss.println("] was successful!");

        if (remoteAtResponse.getValueLength() > 0) {
          nss.print("Command value length is ");
          nss.println(remoteAtResponse.getValueLength(), DEC);

          nss.print("Command value: ");
          
          for (int i = 0; i < remoteAtResponse.getValueLength(); i++) {
            nss.print(remoteAtResponse.getValue()[i], HEX);
            nss.print(" ");
          }

          nss.println("");
        }
      } else {
        nss.print("Command returned error code: ");
        nss.println(remoteAtResponse.getStatus(), HEX);
      }
    } else {
      nss.print("Expected Remote AT response but got ");
      nss.print(xbee.getResponse().getApiId(), HEX);
    }    
  } else if (xbee.getResponse().isError()) {
    nss.print("Error reading packet.  Error code: ");  
    nss.println(xbee.getResponse().getErrorCode());
  } else {
    nss.print("No response from radio");  
  }
}

void Comunicacion_XBee :: set_addr64_R (uint32_t msb, uint32_t lsb){
	addr64_R = XBeeAddress64(msb,lsb);

}
 	
XBeeAddress64 Comunicacion_XBee :: get_addr64_R (){
	return addr64_R;
}

void Comunicacion_XBee :: set_addr64_C (uint32_t msb, uint32_t lsb){
	addr64_C = XBeeAddress64(msb,lsb);

}

 	
XBeeAddress64 Comunicacion_XBee :: get_addr64_C (){
	return addr64_C;
}

/*void Comunicacion_XBee :: set_d1Value (uint8_t value){
	d1Value[] = value;

}
uint8_t  Comunicacion_XBee :: get_d1Value (){
	return d1Value;
}*/

void Comunicacion_XBee :: set_iniciar_AP (int value){
	iniciar_AP.bval = value;
}
int Comunicacion_XBee :: get_iniciar_AP  (){
	return iniciar_AP.bval;
}

void Comunicacion_XBee :: set_Inicio_Mov_C (int value){
	Inicio_Mov_C = value;
}
int Comunicacion_XBee :: get_Inicio_Mov_C (){
	return Inicio_Mov_C;
}

void Comunicacion_XBee :: set_Inicio_Mov_R (int value){
	Inicio_Mov_R = value;
}
int Comunicacion_XBee :: get_Inicio_Mov_R (){
	return Inicio_Mov_R;
}
void Comunicacion_XBee :: set_dato(int_b value){
  dato = value;
}
int_b Comunicacion_XBee :: get_dato(){
  return dato;
}



void Comunicacion_XBee :: set_modo_C(int value){
	modo_C.bval = value;
}
int Comunicacion_XBee :: get_modo_C(){
	return modo_C.bval;
}

void Comunicacion_XBee :: set_modo_R(int value){
	modo_R.bval = value;
}
int Comunicacion_XBee :: get_modo_R(){
	return modo_R.bval;
}

void Comunicacion_XBee :: set_opcion_C(int value){
	opcion_C.bval = value;
}
int Comunicacion_XBee :: get_opcion_C(){
	return opcion_C.bval;
}

void Comunicacion_XBee :: set_opcion_R(int value){
	opcion_R.bval = value;
}
int Comunicacion_XBee :: get_opcion_R(){
	return opcion_R.bval;
}

void Comunicacion_XBee :: set_fallo_Tx(int value){
	fallo_Tx = value;
}
int Comunicacion_XBee :: get_fallo_Tx(){
	return fallo_Tx;
}

void Comunicacion_XBee :: set_C_orientacion_X(int value){
	C_orientacion_X.bval = value;
}
int Comunicacion_XBee :: get_C_orientacion_X(){
	return C_orientacion_X.bval;
}

void Comunicacion_XBee :: set_C_orientacion_Y(int value){
	C_orientacion_Y.bval = value;
}
int Comunicacion_XBee :: get_C_orientacion_Y(){
	return C_orientacion_Y.bval;
}

void Comunicacion_XBee :: set_C_orientacion_Z(int value){
	C_orientacion_Z.bval = value;
}
int Comunicacion_XBee :: get_C_orientacion_Z(){
	return C_orientacion_Z.bval;
}


void Comunicacion_XBee :: set_R_orientacion_X(int value){
	R_orientacion_X.bval = value;
}
int Comunicacion_XBee :: get_R_orientacion_X(){
	return R_orientacion_X.bval;
}

void Comunicacion_XBee :: set_R_orientacion_Y(int value){
	R_orientacion_Y.bval = value;
}
int Comunicacion_XBee :: get_R_orientacion_Y(){
	return R_orientacion_Y.bval;
}

void Comunicacion_XBee :: set_R_orientacion_Z(int value){
	R_orientacion_Z.bval = value;
}
int Comunicacion_XBee :: get_R_orientacion_Z(){
	return R_orientacion_Z.bval;
}

void Comunicacion_XBee :: set_RSSI(int value){
	RSSI_.bval = value;
}
int Comunicacion_XBee :: get_RSSI(){
	return RSSI_.bval;
}
