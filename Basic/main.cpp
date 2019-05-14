#include <cox.h>

Timer timerHello;

uint8_t x __attribute__ ((section(".noinit"))) = 0;

static void taskHello(void *) {
  struct timeval t;
  gettimeofday(&t, nullptr);
  printf("[%lu.%06lu] Hello World! (%u)\n", (uint32_t) t.tv_sec, t.tv_usec, x++);
  digitalToggle(PA0);
}

static void printGPIOInfo(const char *name, const uint32_t *base) {
  printf(
    "%s => MODER 0x%08lX, OTYPER 0x%08lX, OSPEEDR 0x%08lx, PUPDR 0x%08lX, "
    "IDR 0x%08lX, ODR 0x%08lX, BSRR 0x%08lX, LCKR 0x%08lX, "
    "AFR[0] 0x%08lX, AFR[1] 0x%08lX, BRR 0x%08lX\n",
    name,
    base[0], base[1], base[2], base[3], base[4], base[5],
    base[6], base[7], base[8], base[9], base[10]
  );
}

void setup() {
  Serial.begin(115200);
  printf("\n*** [AcSip S76G] Basic ***\n");

  timerHello.onFired(taskHello, NULL);
  // timerHello.startPeriodic(1000);

  pinMode(PA0, OUTPUT);
  digitalWrite(PA0, LOW);

  printGPIOInfo("GPIOA", (const uint32_t *) 0x50000000U);
  printGPIOInfo("GPIOB", (const uint32_t *) 0x50000400U);
  printGPIOInfo("GPIOC", (const uint32_t *) 0x50000800U);
  printGPIOInfo("GPIOD", (const uint32_t *) 0x50000C00U);
  printGPIOInfo("GPIOE", (const uint32_t *) 0x50001000U);
  printGPIOInfo("GPIOH", (const uint32_t *) 0x50001C00U);

  printf(
    "RCC IOPENR 0x%08lX, AHBENR 0x%08lX, APB2ENR 0x%08lX, APB1ENR 0x%08lX\n",
    *((const uint32_t *) (0x40021000U + 0x2C)),
    *((const uint32_t *) (0x40021000U + 0x30)),
    *((const uint32_t *) (0x40021000U + 0x34)),
    *((const uint32_t *) (0x40021000U + 0x38))
  );

  gps.begin();
}
