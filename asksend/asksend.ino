#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
#include <xxtea-iot-crypt.h>

int counter = 1;

RH_ASK driver(4800, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2 4800bps is best so far
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

void setup()
{
  xxtea.setKey("MY_P@ssw0rd_16_b"); //TX Password to encrypt the message (must be the same as RX) max 16 bytes.

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

    String cmd = ("TestCMD-");
    cmd += String(counter); // adds an increment number at the end of cmd to randomize each encrypted code.
    String msg = xxtea.encrypt(cmd); //encrypt the plaintext with incremental number.


    driver.send((uint8_t *)msg.c_str(), msg.length()); // send that encrypted msg.
    driver.waitPacketSent();
    
    delay(3000); //wait 3 seconds to transmit next unique msg.


    counter++; // Increment counter
}

