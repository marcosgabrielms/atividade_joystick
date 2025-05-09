#include <stdio.h>                          // Biblioteca padrão para entrada e saída
#include "pico/stdlib.h"                    // Biblioteca do SDK Pico para funções básicas
#include "pico/cyw43_arch.h"                // Biblioteca para lidar com Wi-Fi (chip CYW43)
#include <string.h>                         // Biblioteca para manipulação de strings
#include <stdlib.h>                         // Biblioteca para alocação de memória, conversões etc.
#include "utils/joystick/joystick.h"        // Arquivo do usuário para lidar com o joystick (ADC)
#include "utils/conexao_servidor/conexao_usuario.h"  // Lógica do usuário para enviar dados à nuvem

// Define o nome e a senha da rede Wi-Fi à qual o dispositivo vai se conectar
#define NOME_REDE_WIFI "Alastro"
#define SENHA_REDE_WIFI "alastralis"

// --- Função Principal (início do programa) ---
int main()
{
    stdio_init_all();           // Inicializa a saída padrão (serial via USB para debug)
    inicializar_adc();          // Configura os canais ADC para leitura do joystick

    // Tenta inicializar o módulo Wi-Fi (CYW43). Se falhar, exibe erro e encerra.
    while (cyw43_arch_init())
    {
        printf("Falha ao inicializar Wi-Fi\n");
        sleep_ms(100);
        return -1;
    }

    cyw43_arch_enable_sta_mode();  // Habilita o modo "station" (cliente Wi-Fi)

    printf("Conectando ao Wi-Fi '%s'...\n", NOME_REDE_WIFI);
    
    // Tenta conectar ao Wi-Fi. Caso falhe, exibe mensagem e tenta novamente após 5 segundos.
    while (cyw43_arch_wifi_connect_timeout_ms(NOME_REDE_WIFI, SENHA_REDE_WIFI, CYW43_AUTH_WPA2_AES_PSK, 20000))
    {
        printf("Falha ao conectar ao Wi-Fi...Tentando novamente...\n");
        sleep_ms(5000);
    }
    printf("Conectado ao Wi-Fi\n");
    
    // Se a interface de rede foi atribuída corretamente, mostra o IP obtido
    if (netif_default)
    {
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    // Loop principal do programa
    while (true)
    {
        cyw43_arch_poll();              // Mantém o driver de rede funcionando corretamente
        enviar_dados_para_nuvem();      // Função do usuário que envia os dados do joystick para a nuvem
        sleep_ms(1000);                 // Aguarda 1 segundo antes de repetir
    }

    // (Este trecho nunca será executado porque o loop é infinito, mas é boa prática incluir)
    cyw43_arch_deinit();  // Finaliza a comunicação Wi-Fi de forma segura
    return 0;
}
