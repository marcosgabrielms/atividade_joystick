#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils/joystick/joystick.h"
#include "utils/conexao_servidor/conexao_usuario.h"

#define NOME_REDE_WIFI "Alastro"
#define SENHA_REDE_WIFI "alastralis"

// --- Função Principal (início do programa) ---
int main()
{
    stdio_init_all();           // Inicializa a comunicação serial
    inicializar_adc();          // Inicializa os sensores (adc(1) e adc(2)) para leitura do joystick

    // Inicializa o Wi-Fi e verifica o erro
    while (cyw43_arch_init())
    {
        printf("Falha ao inicializar Wi-Fi\n");
        sleep_ms(100);
        return -1;
    }

    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi '%s'...\n", NOME_REDE_WIFI);
    // Conecta ao Wi-Fi e verifica o erro
    while (cyw43_arch_wifi_connect_timeout_ms(NOME_REDE_WIFI, SENHA_REDE_WIFI, CYW43_AUTH_WPA2_AES_PSK, 20000))
    {
        printf("Falha ao conectar ao Wi-Fi...Tentando novamente...\n");
        sleep_ms(5000);
    }
    printf("Conectado ao Wi-Fi\n");
    
    // Verifica o endereço IP do dispositivo
    if (netif_default)
    {
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    while (true)
    {
        cyw43_arch_poll();
        enviar_dados_para_nuvem();
        sleep_ms(1000);
    }

    cyw43_arch_deinit();
    return 0;
}
