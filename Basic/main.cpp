#include <cox.h>

Timer timerHello;

static void taskHello(void *) {
  printf("Hello World!\n");
}

void setup() {
  Serial.begin(115200);
  printf("\n*** [AcSip S76G] Basic ***\n");

  timerHello.onFired(taskHello, NULL);
  timerHello.startPeriodic(1000);
}
