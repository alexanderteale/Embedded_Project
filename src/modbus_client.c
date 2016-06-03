// Standalone Modbus Request
// References
// http://libmodbus.org/docs/v3.0.6/modbus_read_registers.html
// http://libmodbus.org/docs/v3.0.6/modbus_new_tcp.html

    //Libraries to Include
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <modbus-tcp.h>
#include <arpa/inet.h>

    //Definitions
// Modbus server location, Home IP
#define ModbusAddressServer "127.0.0.1"
// Modbus Port for Comms
#define ModbusPortServer 502
// Modbus Device as Provided by Kim (12,34)
#define ModbusID 34
// Modbus Device Instances as Provided by Kim
#define ModbusInstance 3


int main(void){
    int mbdata; //Data reads into this variable
    int i; // Counter for loops etc.
    uint16_t  mbdatastore[256];   //16bit unsigned int for data storage
    modbus_t *ctx; 
    ctx = modbus_new_tcp(ModbusAddressServer, ModbusPortServer);// Create libmodbus content (First Input uses IPv4 Address)
    
    //if the system didnt fill in the modbus content
    //from http://libmodbus.org/docs/v3.0.6/modbus_new_tcp.html
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    printf("Modbus Created\n");
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    //Connection Established
    printf("Connection has been Established\n");
    
    //Store the Data
    mbdata=modbus_read_registers(ctx, 34, 4, mbdatastore);

    // Check for errors (negative stored Values)
    if (mbdata < 0) {
        printf("Error getting data from server\n");
        modbus_free(ctx);// free the used space
        return -1;//error
    }

    for (i = 0; i < mbdata; i++) {
        printf("reg[%d]=%d (0x%X)\n", i, mbdatastore[i], mbdatastore[i]);
    }
    //Disconnect
    modbus_close(ctx);
    //Free the used space
    modbus_free(ctx);

}
