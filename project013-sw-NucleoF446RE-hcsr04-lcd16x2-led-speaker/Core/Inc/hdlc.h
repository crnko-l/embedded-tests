#ifndef HDLC_H_
#define HDLC_H_

#include "main.h"
#include "crc.h"
#include <stdint.h>
#include <string.h>

#define HDLC_FLAG_SOF          0x7E
#define HDLC_CONTROL_ESCAPE    0x7D
#define HDLC_ESCAPE_BIT        0x20
#define HDLC_MAX_FRAME_SIZE    64

typedef enum {
    HDLC_SOF_WAIT,
    HDLC_GET_DATA,
    HDLC_ESCAPE
} hdlc_state_t;

typedef struct {
    hdlc_state_t state;
    uint8_t rx_buffer[HDLC_MAX_FRAME_SIZE];
    uint8_t rx_index;
    uint8_t tx_buffer[HDLC_MAX_FRAME_SIZE];
} hdlc_t;

void hdlc_init(void);
void hdlc_process_rx_byte(uint8_t byte, uint8_t src);
void hdlc_tx_raw_frame_oneshot(uint8_t *payload, uint8_t len);

#endif /* HDLC_H_ */
