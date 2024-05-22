#include "BAP.h"
#include <Arduino.h>
#include <timeconst.h>
#include "utils.h"
#include "../displays/display.h"

void BAP_test(void * task_id)
{

    HardwareSerial Serial2(2); // Use UART2

    // Begin Serial2 on GPIO16 (RX) and GPIO17 (TX)
    Serial2.begin(115200, SERIAL_8N1, BAP_RX_PIN, BAP_TX_PIN);
    Serial.begin(115200); // Start the regular Serial port for debugging

    while (true)
    {
        char preamble[1];
        char info[2];
        char crc[1];

        if (!Serial2.available())
        {
            delay(100);
            continue;
        }

        Serial2.readBytes(preamble, 1);
        if (preamble[0] != (char)0xFF || !Serial2.available())
        {
            continue;
        }
        Serial2.readBytes(preamble, 1);
        if (preamble[0] != (char)0xAA || !(Serial2.available() >= 2))
        {
            continue;
        }

        Serial2.readBytes(info, 2);
        size_t data_len = info[0];

        unsigned char *data = new unsigned char[data_len];
        Serial2.readBytes(data, data_len);
        double result;
        memcpy(&result, data, 8);
        Serial.println(result);

        Serial2.readBytes(crc, 1);
        uint8_t crc_msg = crc[0];

        uint8_t message_len = data_len + BAP_HEADER_LENGTH;
        unsigned char *message = new unsigned char[data_len + 4];
        message[0] = 0xFF;
        message[1] = 0xAA;
        message[2] = data_len;
        message[3] = info[1];
        memcpy(message + 4, data, data_len);

        uint8_t crc_check = crc5(message, message_len);

        if(crc_msg == crc_check){
            Serial.println("CRC PASS");
        }else{
            Serial.println("CRC FAIL");
        }

        tDisplay_UpdateHashrate(result);
        // size_t data_len = carrier_buf[2];
        // char * buf;
        // Serial2.readBytes(buf, data_len);
        // double result;
        // memcpy(&result, buf, 8);
        // Serial.println(result);
    }
}