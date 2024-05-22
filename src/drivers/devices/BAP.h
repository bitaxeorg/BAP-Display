#ifndef __BAP_H__
#define __BAP_H__

#define PIN_ENABLE5V 15

#define BAP_TX_PIN 1
#define BAP_RX_PIN 2

#define BAP_HEADER_LENGTH 4
#define CRC_LENGTH 1

void BAP_test(void * task_id);

#endif // __BAP_H__