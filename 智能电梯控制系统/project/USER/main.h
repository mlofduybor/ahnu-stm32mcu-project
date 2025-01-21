#define RCC_APB1ENR *(unsigned int *) 0x4002101C
#define RCC_APB2ENR *(unsigned int *) 0x40021018

#define EXTI_IMR     *(unsigned int *) 0x40010400
#define EXTI_FTSR    *(unsigned int *) 0x4001040c
#define EXTI_PR      *(unsigned int *) 0x40010414

#define NVIC_ISR0    *(unsigned int *) 0xE000E100
#define NVIC_PR      *(unsigned int *) 0xE000E280
#define AFIO_EXTICR1 *(unsigned int *) 0x40010008
#define AFIO_EXTICR2 *(unsigned int *) 0x4001000c

