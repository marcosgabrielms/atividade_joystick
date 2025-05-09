#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Define os pinos conectados aos eixos X e Y do joystick (VRX e VRY)
#define VRX_JOYSTICK 27
#define VRY_JOYSTICK 26

// Limites para considerar uma movimentação em determinada direção
#define JOYSTICK_LIMITE_SUPERIOR 80  // Valor acima do qual é considerado movimentação
#define JOYSTICK_LIMITE_INFERIOR 20  // Valor abaixo do qual é considerado movimentação

// Protótipos das funções disponíveis para uso com o joystick
void inicializar_adc();               // Inicializa o ADC e os pinos do joystick
uint8_t leitura_joystick_x();         // Retorna o valor normalizado do eixo X (0 a 100)
uint8_t leitura_joystick_y();         // Retorna o valor normalizado do eixo Y (0 a 100)
const char* verificar_movimento();    // Retorna a direção atual do joystick como string

#endif
