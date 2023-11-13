#include "ano.h"

/* The version of Ano Assistant is V7 */

/* Define the data frame */
int8_t ANO_Send_Data[32] = {0xAA, 0xFF};
uint8_t Data_len, sumcheck, addcheck, data_cnt;

void FANO_Send_Data(int8_t *data)
{
    ANO_Send_Data[2] = data[0];
    ANO_Send_Data[3] = data[1];
    for (uint8_t i = 0; i < ANO_Send_Data[3]; i++)
    {
        ANO_Send_Data[i + 4] = data[i + 1];
    }
    uint8_t i;
    sumcheck = 0;
    addcheck = 0;
    for (i = 0; i < ANO_Send_Data[3] + 4; i++)
    {
        sumcheck += ANO_Send_Data[i];
        addcheck += sumcheck;
    }
    ANO_Send_Data[4 + ANO_Send_Data[3]] = sumcheck;
    ANO_Send_Data[5 + ANO_Send_Data[3]] = addcheck;
    for (i = 0; i < ANO_Send_Data[3] + 6; i++)
    {
        SendByte(ANO_Send_Data[i]);
    }
}

void FANO_Send_MAG(int8_t *data)
{
    // int16 MAG_X; int16 MAG_Y; int16 MAG_Z;
    int8_t tmp_data[14 + 2];
    tmp_data[0] = 0x02; // function code
    tmp_data[1] = 14;   // data length
    tmp_data[2] = data[0];
    tmp_data[3] = data[1]; // MAG_X
    tmp_data[4] = data[2];
    tmp_data[5] = data[3]; // MAG_Y
    tmp_data[6] = data[4];
    tmp_data[7] = data[5]; // MAG_Z
    tmp_data[8] = 0;
    tmp_data[9] = 0;
    tmp_data[10] = 0;
    tmp_data[11] = 0; // ALT_BAR
    tmp_data[12] = 0;
    tmp_data[13] = 0; // TMP
    tmp_data[14] = 0; // BAR_STA
    tmp_data[15] = 0; // MAG_STA
    FANO_Send_Data(tmp_data);
}

void FANO_Send_ACC_GRY(int8_t *data)
{
    // int16 ACC_X; int16 ACC_Y; int16 ACC_Z; int16 GRY_X; int16 GRY_Y; int16 GRY_Z;
    int8_t tmp_data[13 + 2];
    tmp_data[0] = 0x01; // function code
    tmp_data[1] = 13;   // data length
    tmp_data[2] = data[0];
    tmp_data[3] = data[1]; // ACC_X
    tmp_data[4] = data[2];
    tmp_data[5] = data[3]; // ACC_Y
    tmp_data[6] = data[4];
    tmp_data[7] = data[5]; // ACC_Z
    tmp_data[8] = data[6];
    tmp_data[9] = data[7]; // GRY_X
    tmp_data[10] = data[8];
    tmp_data[11] = data[9]; // GRY_Y
    tmp_data[12] = data[10];
    tmp_data[13] = data[11]; // GRY_Z
    tmp_data[14] = 0;        // SHOCK_STA
    FANO_Send_Data(tmp_data);
}
