# 🚀 Projeto: Envio de Dados do Joystick para a Nuvem via Wi-Fi – BitDogLab RP2040

Este projeto utiliza a placa **BitDogLab RP2040** para capturar os dados de um joystick (via ADC), interpretar sua direção e enviar esses dados em formato JSON para um servidor na nuvem via protocolo HTTP, utilizando a conexão Wi-Fi integrada do chip **CYW43**.

---

## 📦 Estrutura do Projeto

📁 projeto/
├── CMakeLists.txt # Configuração do build
├── main.c # Função principal (main)
├── utils/
│ ├── joystick/
│ │ ├── joystick.c # Lógica de leitura e interpretação do joystick
│ │ └── joystick.h # Cabeçalho do joystick
│ └── conexao_servidor/
│ ├── conexao_usuario.c # Lógica de conexão com o servidor e envio dos dados
│ └── conexao_usuario.h # Definições de conexão e função principal

---

## 🧠 O que o código faz

1. Inicializa o sistema (ADC e Wi-Fi).
2. Conecta-se automaticamente à rede Wi-Fi especificada.
3. Lê os valores dos eixos X e Y do joystick.
4. Interpreta a direção (ex: Norte, Sul, Centro, Nordeste, etc.).
5. Envia os dados em formato JSON para um servidor via HTTP POST.
6. Repete o processo a cada segundo.

---

## 📶 Conexão Wi-Fi

- A rede e a senha são definidas diretamente no `main.c`:

```c
#define NOME_REDE_WIFI "Alastro"
#define SENHA_REDE_WIFI "alastralis" 
```
Atenção: Troque esses dados conforme sua rede local.

🌐 Envio dos dados
Os dados são enviados para um servidor proxy HTTP (ex: Railway) usando o seguinte destino definido em conexao_usuario.h:

#define PROXY_HOST "switchback.proxy.rlwy.net"
#define PROXY_PORT  23445

🔄 Exemplo de payload JSON enviado:
{
  "direcao": "Norte",
  "x": 84,
  "y": 95
}
⚙️ Requisitos:
Placa BitDogLab RP2040
SDK do Raspberry Pi Pico instalado
CMake e ferramentas de build (cmake, ninja ou make)
Conexão Wi-Fi disponível
Servidor (ex: Railway ou local) preparado para receber requisições HTTP POST

🛠️ Como compilar e rodar

1. Clone este repositório:
   git clone https://github.com/seu-usuario/projeto-joystick-wifi.git
   cd projeto-joystick-wifi
   
2. Configure e compile:
mkdir build
cd build
cmake ..
make

3. Grave o .uf2 na placa BitDogLab (pressionando o BOOTSEL e conectando via USB).
4. Abra um terminal serial (ex: minicom, screen, putty, Thonny) para visualizar a saída.
   
🧩 Como adaptar ou usar este código
Para mudar o servidor de destino, edite PROXY_HOST e PROXY_PORT no arquivo conexao_usuario.h.
Para adaptar o envio para outro tipo de sensor (ex: temperatura, umidade), basta modificar o corpo do JSON em conexao_usuario.c.
Todo o código está modularizado: cada componente tem sua própria pasta com .c e .h, facilitando a reutilização.

📄 CMakeLists.txt
O CMakeLists.txt principal está configurado para compilar os arquivos do main.c e das bibliotecas utilitárias (joystick e conexao_servidor). 
Basta adicionar os novos módulos dentro da estrutura do CMake se quiser expandir.

Exemplo de trecho no CMakeLists.txt:
add_executable(meu_projeto main.c
    utils/joystick/joystick.c
    utils/conexao_servidor/conexao_usuario.c
)
target_link_libraries(meu_projeto pico_stdlib pico_cyw43_arch_lwip_threadsafe_background hardware_adc)


🧑‍💻 Contribuição
Contribuições são bem-vindas! Você pode abrir uma issue ou enviar um pull request com sugestões de melhorias ou novos recursos.

🧾 Licença
Este projeto está sob a licença MIT. Sinta-se livre para usar, modificar e distribuir.
