#include <cox.h>
#include <nmea.h>
#include <nmea/gpgga.h>
#include <nmea/gprmc.h>
#include <nmea/gpgsv.h>

static void printMenu() {
  Serial.printf(
    " 1. Enter idle mode\n"
    " 2. Hot start\n"
  );
}

void setup() {
  Serial.begin(115200);
  Serial.printf("\n*** [AcSip S76G] CXD5603GF GNSS Test ***\n");
  printMenu();

  Serial.onReceive([](SerialPort &p) {
    /* Note that printf() and SerialPort::printf() functions convert "\n" to "\r\n" automatically. */
    while (p.available() > 0) {
      char c = p.read();
      switch(c) {
        case '1': gps.turnOff(); break;
        case '2': gps.turnOn(); break;
        default: printMenu(); break;
      }
    }
  });
  Serial.listen();

  gps.begin();
  gps.onNMEAReceived = [](Gps&) {
    char *data;
    nmea_s *parsed = nullptr;
    if (gps.getLatestGGA() != nullptr) {
      data = new char[strlen(gps.getLatestGGA()) + 1];
      printf("- GGA (raw:%s)\n", gps.getLatestGGA());
      if (data) {
        strcpy(data, gps.getLatestGGA());
        parsed = nmea_parse(data, strlen(data), 0);
        if (parsed && parsed->type == NMEA_GPGGA) {
          nmea_gpgga_s *gga = (nmea_gpgga_s *) parsed;
          printf("  Time: %02u:%02u:%02u\n", gga->time.tm_hour, gga->time.tm_min, gga->time.tm_sec);
          printf("  Longitude: %d degee %lf %c\n", gga->longitude.degrees, gga->longitude.minutes, gga->longitude.cardinal);
          printf("  Latitude: %d degee %lf %c\n", gga->latitude.degrees, gga->latitude.minutes, gga->latitude.cardinal);
          printf("  Altitude: %d %c\n", gga->altitude, gga->altitude_unit);
          printf("  Number of satellites: %d\n", gga->n_satellites);
        } else {
          printf("  Parsing GGA error (%p)\n", parsed);
        }
        delete data;
      } else {
        printf("  No memory to parse GGA\n");
      }
    } else {
      printf("- GGA: (null)\n");
    }

    if (parsed) {
      nmea_free(parsed);
    }

    if (gps.getLatestGSV() != nullptr) {
      printf("- GSV (raw:%s)\n", gps.getLatestGSV());
      data = new char[strlen(gps.getLatestGSV()) + 1];
      if (data) {
        strcpy(data, gps.getLatestGSV());
        parsed = nmea_parse(data, strlen(data), 0);
        if (parsed && parsed->type == NMEA_GPGSV) {
          nmea_gpgsv_s *gsv = (nmea_gpgsv_s *) parsed;
          printf("  Sentence number:%u\n", gsv->sentence_number);
          printf("  Number of satellites:%u\n", gsv->satellites);
          for (uint8_t i = 0; i < 4; i++) {
            printf("  SAT[%u] ", i);
            if (gsv->sat[i].prn >= 0) {
              printf(
                "PRN:%d, elevation:%d, azimuth:%d, snr:%d\n",
                gsv->sat[i].prn, gsv->sat[i].elevation, gsv->sat[i].azimuth, gsv->sat[i].snr
              );
            } else {
              printf("(null)\n");
            }
          }
        } else {
          printf("  Parsing GSV error (%p)\n", parsed);
        }
        delete data;
      } else {
        printf("  No memory to parse GSV\n");
      }
    } else {
      printf("- GSV: (null)\n");
    }

    if (parsed) {
      nmea_free(parsed);
    }

    if (gps.getLatestRMC() != nullptr) {
      printf("- RMC (raw:%s)\n", gps.getLatestRMC());
      data = new char[strlen(gps.getLatestRMC()) + 1];
      if (data) {
        strcpy(data, gps.getLatestRMC());
        parsed = nmea_parse(data, strlen(data), 0);
        if (parsed && parsed->type == NMEA_GPRMC) {
          nmea_gprmc_s *rmc = (nmea_gprmc_s *) parsed;
          printf(
            "  Date time: %u-%02u-%02u %02u:%02u:%02u\n",
            rmc->time.tm_year + 1900, rmc->time.tm_mon + 1, rmc->time.tm_mday,
            rmc->time.tm_hour, rmc->time.tm_min, rmc->time.tm_sec
          );
          printf("  Longitude: %d degree %lf %c\n", rmc->longitude.degrees, rmc->longitude.minutes, rmc->longitude.cardinal);
          printf("  Latitude: %d degree %lf %c\n", rmc->latitude.degrees, rmc->latitude.minutes, rmc->latitude.cardinal);
          printf("  Speed: %lf\n", rmc->speed);
        } else {
          printf("  Parsing RMC error (%p)\n", parsed);
        }
        delete data;
      } else {
        printf("  No memory to parse RMC\n");
      }
    } else {
      printf("- RMC: (null)\n");
    }

    if (parsed) {
      nmea_free(parsed);
    }


    printf("GSA: %s\n", (gps.getLatestGSA()) ? gps.getLatestGSA() : "(null)");
  };
}
