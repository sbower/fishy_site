
// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>

#define MAXTIMINGS 100
#define DHT_PIN 17

int readDHT();

int main(int argc, char **argv)
{
  if (!bcm2835_init())
        return 1;

  readDHT();
  return 0;
} // main


int bits[250], data[100];
int bitidx = 0;

int readDHT() {
  int counter = 0;
  int laststate = HIGH;
  int j=0;

  // Set GPIO DHT_PIN to output
  bcm2835_gpio_fsel(DHT_PIN, BCM2835_GPIO_FSEL_OUTP);

  bcm2835_gpio_write(DHT_PIN, HIGH);
  usleep(500000);  // 500 ms
  bcm2835_gpio_write(DHT_PIN, LOW);
  usleep(20000);

  bcm2835_gpio_fsel(DHT_PIN, BCM2835_GPIO_FSEL_INPT);

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

  // wait for DHT_PIN to drop?
  while (bcm2835_gpio_lev(DHT_PIN) == 1) {
    usleep(1);
  }

  // read data!
  for (int i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while ( bcm2835_gpio_lev(DHT_PIN) == laststate) {
	counter++;
	//nanosleep(1);		// overclocking might change this?
        if (counter == 1000)
	  break;
    }
    laststate = bcm2835_gpio_lev(DHT_PIN);
    if (counter == 1000) break;
    bits[bitidx++] = counter;

    if ((i>3) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      data[j/8] <<= 1;
      if (counter > 200)
        data[j/8] |= 1;
      j++;
    }
  }

  float f, h;
  h = data[0] * 256 + data[1];
  h /= 10;

  f = (data[2] & 0x7F)* 256 + data[3];
  f /= 10.0;
  if (data[2] & 0x80)  f *= -1;
  printf("t=%.1f crccheck=%d\n", f, data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF));
  
  return 1;

}
