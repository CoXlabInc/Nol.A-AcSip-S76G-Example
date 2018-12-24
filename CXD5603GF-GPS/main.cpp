#include <cox.h>

static void printMenu() {
  Serial.printf(
    " 1. Enter idle mode\n"
    " 2. Hot start\n"
  );
}

void setup() {
  Serial.begin(115200);
  Serial.printf("\n*** [AcSip S76G] CXD5603GF GPS/GNSS Test ***\n");
  printMenu();

  Serial.onReceive([](SerialPort &p) {
    /* Note that printf() and SerialPort::printf() functions convert "\n" to "\r\n" automatically. */
    while (p.available() > 0) {
      char c = p.read();
      switch(c) {
        case '1': gps.turnOn(); break;
        case '2': gps.turnOff(); break;
        default: printMenu(); break;
      }
    }
  });
  Serial.listen();

  gps.begin();
  gps.onReadDone([](
    uint8_t fixQuality,
    uint8_t hour,
    uint8_t minute,
    uint8_t sec,
    uint16_t subsec,
    int32_t latitude,
    int32_t longitude,
    int32_t altitude,
    uint8_t numSatellites
  ) {
    printf(
      "* [GPS] fixQ:%u, %02u:%02u:%02u.%02u, %c %ld.%ld, %c %ld.%ld, %ld.%ld, # of sat:%u\n",
      fixQuality,
      hour, minute, sec, subsec,
      latitude >= 0 ? 'N' : 'S', latitude / 100000, latitude % 100000,
      longitude >= 0 ? 'E' : 'W', longitude / 100000, longitude % 100000,
      altitude / 10, altitude % 10,
      numSatellites
    );
  });

}
