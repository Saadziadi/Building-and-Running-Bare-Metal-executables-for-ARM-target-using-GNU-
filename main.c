#include <stdint.h>

#define RCC_BASE 0x40021000
#define GPIOC_BASE 0x40011000
#define USART1_BASE 0x40011000
 #define SCS_BASE            (0xE000E000UL)                             /*!< System Control Space Base Address */
  #define DWT_BASE            (0xE0001000UL)                             /*!< DWT Base Address */
  #define TPI_BASE            (0xE0040000UL)                             /*!< TPI Base Address */
  #define CoreDebug_BASE      (0xE000EDF0UL)                             /*!< Core Debug Base Address */
  #define SysTick_BASE        (SCS_BASE +  0x0010UL)                     /*!< SysTick Base Address */
  #define NVIC_BASE           (SCS_BASE +  0x0100UL)                     /*!< NVIC Base Address */
  #define SCB_BASE            (SCS_BASE +  0x0D00UL)                     /*!< System Control Block Base Address */

typedef struct
{
    uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
  uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
  _ uint32_t VTOR;                   /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
#else
        uint32_t RESERVED0;
#endif
   uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
   uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
  uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
  uint32_t RESERVED1;
  uint32_t SHPR[2U];               /*!< Offset: 0x01C (R/W)  System Handlers Priority Registers. [0] is RESERVED */
  uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
} SCB_Type;

  #define SCB                 ((SCB_Type       *)     SCB_BASE         )
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define GPIOC_CRH (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))
#define USART1_SR (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1 (*(volatile uint32_t *)(USART1_BASE + 0x0C))
#define USART1_CR2 (*(volatile uint32_t *)(USART1_BASE + 0x10))
#define USART1_CR3 (*(volatile uint32_t *)(USART1_BASE + 0x14))

#define RCC_IOPCEN (1 << 4)
#define GPIOC13 (1UL << 13)

void SystemInit(void);
void USART1_Init(void);
void USART1_IRQHandler(void);
void NVIC_SystemReset(void);

void SystemInit(void)
{
    RCC_APB2ENR |= RCC_IOPCEN;
    GPIOC_CRH &= 0xFF0FFFFF;
    GPIOC_CRH |= 0x00200000;
}
void NVIC_SystemReset(void)
{
                                                            /* Ensure all outstanding memory accesses included
                                                                       buffered write are completed before reset */
  SCB->AIRCR  = ((0x5FAUL << 16U) |
                 1UL << 2U);
                                                       /* Ensure completion of memory access */

  for(;;)                                                           /* wait until reset */
  {
    __asm volatile ("nop");
  }

}


void USART1_Init(void)
{
    USART1_BRR = 0x683; // 9600 baud rate (assuming 8MHz clock)

    USART1_CR1 = (1 << 13) | (1 << 3) | (1 << 2); // TE, RE, and UE bits
    USART1_CR2 = 0;
    USART1_CR3 = 0;

    // Enable USART1_IRQn in the NVIC
    *(volatile uint32_t *)(0xE000E100 + 0x0C) |= (1 << 5); // NVIC_ISER1
}

void USART1_IRQHandler(void)
{
    if (USART1_SR & (1 << 5)) // Check if RXNE flag is set
    {
        uint8_t received_data = USART1_DR & 0xFF;
        USART1_SR &= ~(1 << 5); // Clear RXNE flag
       NVIC_SystemReset();


    }
}

int main(void)
{
    SystemInit();
    USART1_Init();

    while (1)
    {
        GPIOC_ODR |= GPIOC13;
        for (int i = 0; i < 500000; i++)
            ; // Arbitrary delay

        GPIOC_ODR &= ~GPIOC13;
        for (int i = 0; i < 500000; i++)
            ; // Arbitrary delay
    }
}
