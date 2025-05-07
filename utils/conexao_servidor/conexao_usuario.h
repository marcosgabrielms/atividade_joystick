#ifndef CLIENTE_HTTP_H
#define CLIENTE_HTTP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/cyw43_arch.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "utils/joystick/joystick.h"

#define PROXY_HOST "switchback.proxy.rlwy.net"
#define PROXY_PORT "48443"

void enviar_dados();

#endif