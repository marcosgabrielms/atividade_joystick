#include "conexao_usuario.h" // Inclui cabeçalho com configurações do servidor e funções do joystick

// --- Callback: chamada quando uma resposta do servidor é recebida ---
static err_t callback_resposta_recebida(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    
    if (!p) { // Se o buffer estiver vazio, o servidor fechou a conexão
        printf("Conexão fechada pelo servidor.\n");
        tcp_close(pcb); // Fecha a conexão TCP
        return ERR_OK;
    }

    printf("Resposta do servidor:\n");

    // Aloca memória para copiar os dados recebidos
    char *dados = (char *)malloc(p->tot_len + 1);
    if (dados) {
        pbuf_copy_partial(p, dados, p->tot_len, 0); // Copia os dados da pbuf para o buffer
        dados[p->tot_len] = '\0'; // Termina a string
        printf("%s\n", dados);    // Mostra a resposta
        free(dados);              // Libera memória
    }

    pbuf_free(p); // Libera o buffer original
    return ERR_OK;
}

// --- Callback: chamada quando a conexão com o servidor é estabelecida ---
static err_t callback_conectado(void *arg, struct tcp_pcb *pcb, err_t err) {
    if (err != ERR_OK) {
        printf("Erro ao conectar: %d\n", err);
        tcp_abort(pcb); // Aborta a conexão
        return err;
    }

    tcp_recv(pcb, callback_resposta_recebida); // Define a função que será chamada quando houver resposta

    // Coleta os dados do joystick
    uint8_t x = leitura_joystick_x();
    uint8_t y = leitura_joystick_y();
    const char* direcao = verificar_movimento();

    // Monta o corpo do JSON com os dados do joystick
    char corpo_json[128];
    snprintf(corpo_json, sizeof(corpo_json),
             "{\"direcao\": \"%s\", \"x\": %d, \"y\": %d}",
             direcao, x, y);

    // Monta a requisição HTTP POST
    char requisicao[512];
    snprintf(requisicao, sizeof(requisicao),
             "POST /dados HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             PROXY_HOST, strlen(corpo_json), corpo_json);

    // Envia os dados protegendo a pilha LWIP (usado com Wi-Fi)
    cyw43_arch_lwip_begin();
    err_t erro_envio = tcp_write(pcb, requisicao, strlen(requisicao), TCP_WRITE_FLAG_COPY);
    if (erro_envio == ERR_OK) {
        tcp_output(pcb); // Garante que os dados foram enviados
        printf("Requisição enviada para %s:%d:\n%s\n", PROXY_HOST, PROXY_PORT, requisicao);
    } else {
        printf("Erro ao enviar dados: %d\n", erro_envio);
        tcp_abort(pcb); // Se falhar, aborta
    }
    cyw43_arch_lwip_end();

    return ERR_OK;
}

// --- Callback: chamada quando o DNS resolver o IP do host ---
static void callback_dns_resolvido(const char *nome_host, const ip_addr_t *ip_resolvido, void *arg) {
    if (!ip_resolvido) {
        printf("Erro: DNS falhou para %s\n", nome_host);
        return;
    }

    printf("DNS resolveu %s para %s\n", nome_host, ipaddr_ntoa(ip_resolvido));

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_V4); // Cria novo PCB TCP
    if (!pcb) {
        printf("Erro ao criar pcb\n");
        return;
    }

    // Tenta conectar ao servidor via IP resolvido
    err_t erro = tcp_connect(pcb, ip_resolvido, PROXY_PORT, callback_conectado);
    if (erro != ERR_OK) {
        printf("Erro ao conectar a %s:%d: %d\n", nome_host, PROXY_PORT, erro);
        tcp_abort(pcb);
    }
}

// --- Função principal chamada para iniciar a comunicação TCP ---
void enviar_dados_para_nuvem() {
    ip_addr_t endereco_ip;

    // Resolve o nome do servidor (DNS)
    err_t resultado_dns = dns_gethostbyname(PROXY_HOST, &endereco_ip, callback_dns_resolvido, NULL);

    if (resultado_dns == ERR_OK) {
        // IP já estava no cache DNS: conecta direto
        printf("Host %s já resolvido para %s\n", PROXY_HOST, ipaddr_ntoa(&endereco_ip));

        struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
        if (!pcb) {
            printf("Erro ao criar pcb (cache)\n");
            return;
        }

        err_t erro = tcp_connect(pcb, &endereco_ip, PROXY_PORT, callback_conectado);
        if (erro != ERR_OK) {
            printf("Erro ao conectar (cache) a %s:%d: %d\n", PROXY_HOST, PROXY_PORT, erro);
            tcp_abort(pcb);
        }

    } else if (resultado_dns == ERR_INPROGRESS) {
        // DNS está resolvendo, `callback_dns_resolvido` será chamado depois
        printf("Resolução DNS em andamento para %s...\n", PROXY_HOST);
    } else {
        printf("Erro ao iniciar DNS para %s: %d\n", PROXY_HOST, resultado_dns);
    }
}
