#include "joystick.h"  // Inclui o cabeçalho com as definições e protótipos do joystick

// Inicializa o ADC (Conversor Analógico-Digital) e os pinos do joystick
void inicializar_adc() 
{
    adc_init();                           // Inicializa o módulo ADC do RP2040
    adc_gpio_init(VRX_JOYSTICK);         // Configura o pino VRX como entrada analógica
    adc_gpio_init(VRY_JOYSTICK);         // Configura o pino VRY como entrada analógica
}

// Lê o valor analógico no eixo X do joystick e converte para escala de 0 a 100
uint8_t leitura_joystick_x()
{
    adc_select_input(1);                 // Seleciona o canal ADC 1 (VRX)
    return adc_read() * 100 / 4095;      // Normaliza o valor lido para 0–100
}

// Lê o valor analógico no eixo Y do joystick e converte para escala de 0 a 100
uint8_t leitura_joystick_y()
{
    adc_select_input(0);                 // Seleciona o canal ADC 0 (VRY)
    return adc_read() * 100 / 4095;      // Normaliza o valor lido para 0–100
}

// Analisa os valores X e Y para determinar a direção do movimento do joystick
const char* verificar_movimento() 
{
    uint8_t x = leitura_joystick_x();
    uint8_t y = leitura_joystick_y();

    // Verifica as direções com base nos limites definidos
    if (x > JOYSTICK_LIMITE_SUPERIOR && y > JOYSTICK_LIMITE_SUPERIOR)
        return "Nordeste";
    else if (x > JOYSTICK_LIMITE_SUPERIOR && y < JOYSTICK_LIMITE_INFERIOR)
        return "Sudeste";
    else if (x < JOYSTICK_LIMITE_INFERIOR && y > JOYSTICK_LIMITE_SUPERIOR)
        return "Noroeste";
    else if (x < JOYSTICK_LIMITE_INFERIOR && y < JOYSTICK_LIMITE_INFERIOR)
        return "Sudoeste";
    else if (x > JOYSTICK_LIMITE_SUPERIOR)
        return "Leste";
    else if (x < JOYSTICK_LIMITE_INFERIOR)
        return "Oeste";
    else if (y > JOYSTICK_LIMITE_SUPERIOR)
        return "Norte";
    else if (y < JOYSTICK_LIMITE_INFERIOR)
        return "Sul";
    else
        return "Centro"; // Quando está dentro dos limites aceitáveis (posição neutra)
}
