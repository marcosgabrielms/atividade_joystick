#ifndef conexao_usuario         // Evita que este cabeçalho seja incluído mais de uma vez
#define conexao_usuario

#include <stdio.h>             // Para uso de funções como printf()
#include <string.h>            // Para manipulação de strings (strlen, snprintf, etc.)
#include <stdlib.h>            // Para alocação dinâmica de memória (malloc, free)

#include "pico/cyw43_arch.h"   // Biblioteca da Pico W para inicializar e gerenciar o chip Wi-Fi (CYW43)
#include "lwip/dns.h"          // Permite resolver nomes de domínio (como 'example.com') para IPs
#include "lwip/ip_addr.h"      // Fornece estrutura para lidar com endereços IP
#include "lwip/tcp.h"          // Permite criar e controlar conexões TCP

#include "utils/joystick/joystick.h" // Permite usar as funções de leitura e verificação do joystick

#define PROXY_HOST "switchback.proxy.rlwy.net" // Endereço do servidor (proxy) para onde os dados serão enviados
#define PROXY_PORT 23445                  // Porta do servidor (proxy) usada para comunicação TCP

void enviar_dados_para_nuvem();  // Função que conecta ao servidor e envia os dados do joystick via HTTP

#endif
