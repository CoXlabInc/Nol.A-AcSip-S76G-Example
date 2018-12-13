#include <cox.h>

Timer timerHello;

static void taskHello(void *) {
  printf("Hello World!\n");
}

void setup() {
  Serial.begin(115200);
  timerHello.onFired(taskHello, NULL);
  timerHello.startPeriodic(1000);
}
