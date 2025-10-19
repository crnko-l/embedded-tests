#include "command_handler.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

extern float lower_limit;
extern float upper_limit;
extern float scaling_factor;
extern ADC_HandleTypeDef hadc1;

static void SoftwareReset(void)
{
    NVIC_SystemReset();
}

void process_command(hdlc_t *hdlc_rx)
{
    CommandPacket pkt;
    pkt.code = hdlc_rx->rx_buffer[0];
    pkt.parameters_length = hdlc_rx->rx_index - 3;
    memcpy(pkt.parameters, &hdlc_rx->rx_buffer[1], pkt.parameters_length);
    handle_command(&pkt, hdlc_rx);
}

void handle_command(const CommandPacket *pkt, hdlc_t *hdlc_rx)
{
    switch (pkt->code)
    {
        case Soft_Reset:
            SoftwareReset();
            break;

        case Uc_Detect:
            hdlc_rx->tx_buffer[0] = Uc_Detect;
            hdlc_rx->tx_buffer[1] = 1;
            hdlc_tx_raw_frame_oneshot(hdlc_rx->tx_buffer, 2);
            break;

        case Get_Limits:
            hdlc_rx->tx_buffer[0] = Get_Limits;
            memcpy(&hdlc_rx->tx_buffer[1], &lower_limit, sizeof(float));
            memcpy(&hdlc_rx->tx_buffer[1 + sizeof(float)], &upper_limit, sizeof(float));
            hdlc_tx_raw_frame_oneshot(hdlc_rx->tx_buffer, 1 + 2 * sizeof(float));
            break;


        case Set_Lower_Limit:
            if (pkt->parameters_length >= 2)
                lower_limit = (float)((pkt->parameters[0] << 8) | pkt->parameters[1]);
            break;

        case Set_Upper_Limit:
            if (pkt->parameters_length >= 2)
                upper_limit = (float)((pkt->parameters[0] << 8) | pkt->parameters[1]);
            break;

        case Get_Scaling_Factor:
        {
            uint16_t sf = (uint16_t)(scaling_factor * 10000);
            hdlc_rx->tx_buffer[0] = Get_Scaling_Factor;
            hdlc_rx->tx_buffer[1] = sf >> 8;
            hdlc_rx->tx_buffer[2] = sf;
            hdlc_tx_raw_frame_oneshot(hdlc_rx->tx_buffer, 3);
            break;
        }

        case Get_Distance:
        {
            HAL_ADC_Start(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
            uint32_t val = HAL_ADC_GetValue(&hadc1);
            HAL_ADC_Stop(&hadc1);

            hdlc_rx->tx_buffer[0] = Get_Distance;
            hdlc_rx->tx_buffer[1] = val >> 8;
            hdlc_rx->tx_buffer[2] = val;
            hdlc_tx_raw_frame_oneshot(hdlc_rx->tx_buffer, 3);
            break;
        }

        default:
            printf("Unknown cmd: 0x%02X\r\n", pkt->code);
            break;
    }
}
