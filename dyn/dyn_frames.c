/*
 * hal_uart.c
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 *
 * Dynamixel framing layer
 */

#include "dyn_frames.h"
#include <stdio.h>

#ifndef __MSP432P401R__

#include "hal_dyn_uart/hal_dyn_uart_emu.h"
#include "fake_msp.h"

#define f_TxUAC2 TxUAC2_emu
#define f_Sentit_Dades_Tx Sentit_Dades_Tx_emu
#define f_Sentit_Dades_Rx Sentit_Dades_Rx_emu
#define f_rx_uart_byte rx_uart_byte_emu

#else
#include "hal_dyn_uart/hal_dyn_uart.h"
#include "msp.h"

#define f_TxUAC2 TxUAC2
#define f_Sentit_Dades_Tx Sentit_Dades_Tx
#define f_Sentit_Dades_Rx Sentit_Dades_Rx
#define f_Activa_Timer_TimeOut Activa_Timer_TimeOut
#define f_rx_uart_byte rx_uart_byte

#endif

/**
 * enum de los distintos bits de error
 */
typedef enum _err_bits {
    ERR_INSTR = 0x40,
    ERR_OVRLD = 0x20,
    ERR_CHKSUM = 0x10,
    ERR_RANGE = 0x08,
    ERR_OVRHT = 0x04,
    ERR_ANGLMT = 0x02,
    ERR_VOLTG = 0x01,
} ERR_BITS_t;

//TxPacket()  3 par�metres: ID del Dynamixel, Mida dels par�metres, Instruction byte. torna la mida del "Return packet"
byte TxPacket(byte bID, byte bParameterLength, byte bInstruction,
              const byte *Parametros) {
    byte bCount, bCheckSum, bPacketLength;
    byte TxBuffer[32];
    f_Sentit_Dades_Tx();  //El pin P3.0 (DIRECTION_PORT) el posem a 1 (Transmetre)
    TxBuffer[0] = 0xff;    //Primers 2 bytes que indiquen inici de trama FF, FF.
    TxBuffer[1] = 0xff;
    TxBuffer[2] = bID;         //ID del mòdul al que volem enviar el missatge
    TxBuffer[3] = bParameterLength + 2; //Length(Parameter,Instruction,Checksum)
    TxBuffer[4] = bInstruction;    //Instrucció que enviem al Mòdul

    if (Parametros[0] < 6) {
        printf("Error: la instrucció no pot modificar les cinc primeres");
        return -1;
    }

    for (bCount = 0; bCount < bParameterLength; bCount++) //Comencem a generar la trama que hem d�enviar
    {
        TxBuffer[bCount + 5] = Parametros[bCount];
    }
    bCheckSum = 0;
    bPacketLength = bParameterLength + 4 + 2;
    for (bCount = 2; bCount < bPacketLength - 1; bCount++) //C�lcul del checksum
    {
        bCheckSum += TxBuffer[bCount];
    }
    TxBuffer[bCount] = ~bCheckSum;         //Escriu el Checksum (complement a 1)
    for (bCount = 0; bCount < bPacketLength; bCount++) //Aquest bucle �s el que envia la trama al M�dul Robot
    {
        while (!(UCA2IFG & UCTXIFG));
        f_TxUAC2(TxBuffer[bCount]);
    }
    while ((UCA2STATW & UCBUSY)) {
    };   //Espera fins que s�ha transm�s el �ltim byte
    f_Sentit_Dades_Rx(); //Posem la l�nia de dades en Rx perqu� el m�dul Dynamixel envia resposta
    return (bPacketLength);
}

struct RxReturn RxPacket(void) {
    struct RxReturn respuesta;
    byte bCount, bCheckSum, bPacketLength;

    respuesta.time_out = false;
    respuesta.idx = 0;
    //f_Sentit_Dades_Rx();   //Ponemos la linea half duplex en Rx
    //f_Activa_Timer_TimeOut();
    for (bCount = 0; bCount < 4; bCount++) {
        f_rx_uart_byte(&respuesta);
    } //fin del for
    if (!respuesta.time_out) {
        for (bCount = 0; bCount < respuesta.StatusPacket[3]; bCount++) {
            f_rx_uart_byte(&respuesta);
        } //fin del for
    }
    bCheckSum = 0;

    //Calculamos la longitud del paquete
    bPacketLength = respuesta.StatusPacket[3] + 4;

    //empezamos a contar despu�s de las primeras dos variables
    for(bCount = 2; bCount < bPacketLength - 1; bCount++) {
        bCheckSum += respuesta.StatusPacket[bCount];
    }
    //Hacemos el complemento a uno del resultado
    bCheckSum = ~bCheckSum;

    //Comprobamos si coincide con el original y ponemos el bit de error de checksum a 1 en caso contrario
    if(bCheckSum != respuesta.StatusPacket[bPacketLength - 1]){
        respuesta.StatusPacket[4] |= ERR_CHKSUM;
    }

    return respuesta;
}


/**
 * Perform a full read/write transaction
 *
 * This function will send an instruction frame to the dynamixel module
 * and the following status frame
 *
 * @param[in] bID Id of the dynamixel module
 * @param[in] bParameterLength Number of parameters to send
 * @param[in] bInstruction Instruction type of the frame
 * @param[in] Parametros Parameters of the TX frame
 * @return Returns a RxReturn struct with the information of the reply
 */
struct RxReturn RxTxPacket(byte bID, byte bParameterLength, byte bInstruction,
                           const byte *Parametros) {
    struct RxReturn respuesta;
    TxPacket(bID, bParameterLength, bInstruction, Parametros);
    respuesta = RxPacket();
    return respuesta;
}
