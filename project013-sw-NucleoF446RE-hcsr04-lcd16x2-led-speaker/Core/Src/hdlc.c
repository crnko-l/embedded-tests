#include "hdlc.h"
#include "command_handler.h"
#include "main.h"

static hdlc_t hdlc;

static void hdlc_tx_byte(uint8_t byte)
{
    HAL_UART_Transmit(&huart2, &byte, 1, HAL_MAX_DELAY);
}

static void hdlc_esc_tx_byte(uint8_t byte)
{
    if (byte == HDLC_FLAG_SOF || byte == HDLC_CONTROL_ESCAPE)
    {
        hdlc_tx_byte(HDLC_CONTROL_ESCAPE);
        byte ^= HDLC_ESCAPE_BIT;
    }
    hdlc_tx_byte(byte);
}

void hdlc_init(void)
{
    memset(&hdlc, 0, sizeof(hdlc_t));
    hdlc.state = HDLC_SOF_WAIT;
}

void hdlc_process_rx_byte(uint8_t byte, uint8_t src)
{
    switch (hdlc.state)
    {
        case HDLC_SOF_WAIT:
            if (byte == HDLC_FLAG_SOF)
            {
                hdlc.rx_index = 0;
                hdlc.state = HDLC_GET_DATA;
            }
            break;

        case HDLC_GET_DATA:
            if (byte == HDLC_FLAG_SOF)
            {
                if (hdlc.rx_index >= 3)
                {
                    uint16_t crc_calc = crc16(hdlc.rx_buffer, hdlc.rx_index - 2);
                    uint16_t crc_recv = (hdlc.rx_buffer[hdlc.rx_index - 1] << 8) |
                                         hdlc.rx_buffer[hdlc.rx_index - 2];
                    if (crc_calc == crc_recv)
                        process_command(&hdlc);
                }
                hdlc.rx_index = 0;
            }
            else if (byte == HDLC_CONTROL_ESCAPE)
            {
                hdlc.state = HDLC_ESCAPE;
            }
            else
            {
                if (hdlc.rx_index < HDLC_MAX_FRAME_SIZE)
                    hdlc.rx_buffer[hdlc.rx_index++] = byte;
            }
            break;

        case HDLC_ESCAPE:
            byte ^= HDLC_ESCAPE_BIT;
            if (hdlc.rx_index < HDLC_MAX_FRAME_SIZE)
                hdlc.rx_buffer[hdlc.rx_index++] = byte;
            hdlc.state = HDLC_GET_DATA;
            break;
    }
}

void hdlc_tx_raw_frame_oneshot(uint8_t *payload, uint8_t len)
{
    uint16_t crc = crc16(payload, len);

    hdlc_tx_byte(HDLC_FLAG_SOF);
    for (uint8_t i = 0; i < len; i++)
        hdlc_esc_tx_byte(payload[i]);

    hdlc_esc_tx_byte((uint8_t)(crc & 0xFF));
    hdlc_esc_tx_byte((uint8_t)(crc >> 8));
    hdlc_tx_byte(HDLC_FLAG_SOF);
}
