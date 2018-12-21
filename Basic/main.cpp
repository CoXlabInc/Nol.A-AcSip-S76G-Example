#include <cox.h>

Timer timerHello;

static void taskHello(void *) {
  struct timeval t;
  gettimeofday(&t, nullptr);
  printf("[%lu.%06lu] Hello World!\n", (uint32_t) t.tv_sec, t.tv_usec);
  digitalToggle(PA0);
}

void setup() {
  Serial.begin(115200);
  printf("\n*** [AcSip S76G] Basic ***\n");

  timerHello.onFired(taskHello, NULL);
  timerHello.startPeriodic(1000);

  pinMode(PA0, OUTPUT);
  digitalWrite(PA0, HIGH);
}
