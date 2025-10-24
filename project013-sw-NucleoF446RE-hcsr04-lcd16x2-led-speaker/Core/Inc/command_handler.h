#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include "hdlc.h"

#define Soft_Reset         0x01
#define Uc_Detect          0x02
#define Get_Limits         0x10
#define Set_Lower_Limit    0x11
#define Set_Upper_Limit    0x12
#define Get_Scaling_Factor 0x13
#define Get_Distance       0x14
#define Get_History_Sample   0x15

typedef struct {
    uint8_t code;
    uint8_t parameters_length;
    uint8_t parameters[16];
} CommandPacket;

void process_command(hdlc_t* hdlc_rx);
void handle_command(const CommandPacket* pkt, hdlc_t* hdlc_rx);

#endif
