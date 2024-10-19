#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
#include <xxtea-iot-crypt.h>
// RH_ASK driver;
RH_ASK driver(4800, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

void setup()
{
  xxtea.setKey("MY_P@ssw0rd_16_b"); // RX Password to decrypt the received message (must be the same as TX)
  
#ifdef RH_HAVE_SERIAL
    Serial.begin(115200);	  // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
	;
#endif
}

void loop()
{
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        char message[buflen + 1];
        memcpy(message, buf, buflen);
        message[buflen] = '\0'; // Null-terminate

        String msg = message;
        Serial.print("Got: ");
        Serial.println(msg);

        Serial.print("Decrypted msg: "); 
        Serial.println(xxtea.decrypt(msg)); // Decrypt the received msg to extract actual cmd

    }
}
