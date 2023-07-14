
#include <stdint.h>

#define GPIOS 0x3ff44000

// https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf#iomuxgpio
struct gpioRegs {
    volatile uint32_t GPIO_OUT_REG;
    volatile uint32_t GPIO_OUT_W1TS_REG;
    volatile uint32_t GPIO_OUT_W1TC_REG;
    volatile uint32_t GPIO_OUT1_REG;
    volatile uint32_t GPIO_OUT1_W1TS_REG;
    volatile uint32_t GPIO_OUT1_W1TC_REG;
    volatile uint32_t RESERVED1;
    volatile uint32_t GPIO_ENABLE_REG;
    volatile uint32_t GPIO_ENABLE_W1TS_REG;
    volatile uint32_t GPIO_ENABLE_W1TC_REG;
    volatile uint32_t GPIO_ENABLE1_REG;
    volatile uint32_t GPIO_ENABLE1_W1TS_REG;
    volatile uint32_t GPIO_ENABLE1_W1TC_REG;
    volatile uint32_t GPIO_STRAP_REG;
    volatile uint32_t GPIO_IN_REG;
    volatile uint32_t GPIO_IN1_REG;
    volatile uint32_t GPIO_STATUS_REG;
    volatile uint32_t GPIO_STATUS_W1TS_REG;
    volatile uint32_t GPIO_STATUS_W1TC_REG;
    volatile uint32_t GPIO_STATUS1_REG;
    volatile uint32_t GPIO_STATUS1_W1TS_REG;
    volatile uint32_t GPIO_STATUS1_W1TC_REG;
    volatile uint32_t RESERVED2;
    volatile uint32_t GPIO_ACPU_INT_REG;
    volatile uint32_t GPIO_ACPU_NMI_INT_REG;
    volatile uint32_t GPIO_PCPU_INT_REG;
    volatile uint32_t GPIO_PCPU_NMI_INT_REG;
    volatile uint32_t RESERVED3;
    volatile uint32_t GPIO_ACPU_INT1_REG;
    volatile uint32_t GPIO_ACPU_NMI_INT1_REG;
    volatile uint32_t GPIO_PCPU_INT1_REG;
    volatile uint32_t GPIO_PCPU_NMI_INT1_REG;
};
#define GPIO ((struct gpioRegs *) (0x3ff44004))


// Starts at pin 0
// https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf#iomuxgpio
struct pinFuncOut {
    volatile uint32_t pins[40]; 
};
#define FUNCOUT ((struct pinFuncOut *) 0x3ff44530)

#define BIT(x) (uint32_t)(1U << x)

enum vals {
    LOW = 0,
    HIGH = 1
};

enum modes {
    INPUT = 0,
    OUTPUT = 1
};


void setMode(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
    {
        FUNCOUT->pins[pin] = 256;
        GPIO->GPIO_ENABLE_REG &= ~BIT(pin);
        GPIO->GPIO_ENABLE_REG |= (mode ? 1U : 0U) << pin;
    }
    else if (mode == INPUT)
    {
        ;
    }
}

void togglePin(uint8_t pin)
{
    GPIO->GPIO_OUT_REG ^= BIT(pin);
}


void writePin(uint8_t pin, uint8_t val)
{
    GPIO->GPIO_OUT_REG &= ~BIT(pin);     // Clear first
    GPIO->GPIO_OUT_REG |= (val) << pin;  // Then set
}