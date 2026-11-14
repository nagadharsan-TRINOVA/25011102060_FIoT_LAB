/* STM32 NUCLEO-F401RE + BME280 sensor, USART2 PuTTY output
 * I2C1: Pins selected in STM32CubeMX (e.g. PB8 SCL, PB9 SDA)
 * USART2: ST-LINK Virtual COM port at 115200 baud
 * Bosch BME280 API: version with separate struct bme280_settings
 */
#include "main.h"
#include "bme280.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

static struct bme280_dev dev;
static struct bme280_data comp_data;
static struct bme280_settings settings;
static int8_t rslt = BME280_OK;
static uint8_t dev_addr = BME280_I2C_ADDR_PRIM;
static char uart_buf[160];

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
void Error_Handler(void);

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                     uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                      uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);

static void pc_print(const char *text)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)text,
                      (uint16_t)strlen(text), HAL_MAX_DELAY);
}

/* Print two decimal places without requiring printf float support. */
static void format_measurements(void)
{
    /* This matches BME280_DOUBLE_ENABLE: deg C, Pa, percent RH. */
    double t = comp_data.temperature;
    double p = comp_data.pressure / 100.0;
    double h = comp_data.humidity;
    int32_t ti = (int32_t)(t * 100.0 + (t >= 0 ? 0.5 : -0.5));
    int32_t pi = (int32_t)(p * 100.0 + 0.5);
    int32_t hi = (int32_t)(h * 100.0 + 0.5);
    uint32_t ta = (uint32_t)(ti < 0 ? -ti : ti);
    snprintf(uart_buf, sizeof(uart_buf),
             "Chip ID: 0x%02X | Temp: %s%lu.%02lu C | "
             "Press: %ld.%02ld hPa | Hum: %ld.%02ld %%\r\n",
             (unsigned int)dev.chip_id,
             (ti < 0 ? "-" : ""), (unsigned long)(ta / 100U),
             (unsigned long)(ta % 100U),
             (long)(pi / 100), (long)(pi % 100),
             (long)(hi / 100), (long)(hi % 100));
    pc_print(uart_buf);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    /* UART verified independently: initialize it FIRST. */
    MX_USART2_UART_Init();
    pc_print("\r\n[BOOT] USART2 working. Starting BME280 test...\r\n");

    MX_I2C1_Init();
    /* USART1 not required for this first sensor + PuTTY test. */

    memset(&dev, 0, sizeof(dev));
    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    pc_print("[INIT] Trying BME280 address 0x76...\r\n");
    rslt = bme280_init(&dev);
    if (rslt != BME280_OK)
    {
        dev_addr = BME280_I2C_ADDR_SEC;
        pc_print("[INIT] Trying BME280 address 0x77...\r\n");
        rslt = bme280_init(&dev);
    }

    if (rslt != BME280_OK)
    {
        snprintf(uart_buf, sizeof(uart_buf),
                 "[FAIL] Sensor not detected; BME280 error=%d; HAL I2C error=0x%08lX\r\n",
                 rslt, (unsigned long)HAL_I2C_GetError(&hi2c1));
        pc_print(uart_buf);
        pc_print("[CHECK] VCC/GND, SDA/SCL, pull-ups and CubeMX I2C1 pin mapping.\r\n");
        while (1) HAL_Delay(1000);
    }

    snprintf(uart_buf, sizeof(uart_buf),
             "[PASS] I2C address: 0x%02X | Chip ID: 0x%02X\r\n",
             (unsigned int)dev_addr, (unsigned int)dev.chip_id);
    pc_print(uart_buf);

    memset(&settings, 0, sizeof(settings));
    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_1X;
    settings.osr_t = BME280_OVERSAMPLING_1X;
    settings.filter = BME280_FILTER_COEFF_OFF;

    rslt = bme280_set_sensor_settings(
        BME280_SEL_OSR_PRESS | BME280_SEL_OSR_TEMP |
        BME280_SEL_OSR_HUM | BME280_SEL_FILTER, &settings, &dev);
    if (rslt == BME280_OK)
        rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev);
    if (rslt != BME280_OK)
    {
        snprintf(uart_buf, sizeof(uart_buf),
                 "[FAIL] Sensor settings failed, code=%d\r\n", rslt);
        pc_print(uart_buf);
        while (1) HAL_Delay(1000);
    }

    pc_print("[PASS] BME280 initialized. Reading every second...\r\n");
    HAL_Delay(100);
    while (1)
    {
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
        if (rslt == BME280_OK)
            format_measurements();
        else
        {
            snprintf(uart_buf, sizeof(uart_buf),
                     "[FAIL] Sensor read: code=%d, HAL I2C error=0x%08lX\r\n",
                     rslt, (unsigned long)HAL_I2C_GetError(&hi2c1));
            pc_print(uart_buf);
        }
        HAL_Delay(1000);
    }
}

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                     uint32_t len, void *intf_ptr)
{
    uint8_t addr = *(uint8_t *)intf_ptr;
    if (HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(addr << 1), reg_addr,
                         I2C_MEMADD_SIZE_8BIT, reg_data,
                         (uint16_t)len, 1000) == HAL_OK)
        return BME280_OK;
    return BME280_E_COMM_FAIL;
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                      uint32_t len, void *intf_ptr)
{
    uint8_t addr = *(uint8_t *)intf_ptr;
    if (HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(addr << 1), reg_addr,
                          I2C_MEMADD_SIZE_8BIT, (uint8_t *)reg_data,
                          (uint16_t)len, 1000) == HAL_OK)
        return BME280_OK;
    return BME280_E_COMM_FAIL;
}

void user_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;
    /* Bosch callback uses microseconds; HAL_Delay rounds up to milliseconds. */
    HAL_Delay((period + 999U) / 1000U);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc.HSEState = RCC_HSE_BYPASS;
    osc.PLL.PLLState = RCC_PLL_ON;
    osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc.PLL.PLLM = 4;
    osc.PLL.PLLN = 84;
    osc.PLL.PLLP = RCC_PLLP_DIV2;
    osc.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK) Error_Handler();
    clk.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV2;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2) != HAL_OK) Error_Handler();
}

static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) Error_Handler();
}

static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) Error_Handler();
}

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) Error_Handler();
}

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) { }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif
