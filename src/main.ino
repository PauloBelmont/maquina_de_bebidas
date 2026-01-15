#include "Nextion.h"

// ============================================================
// DEFINIÇÕES E CONFIGURAÇÕES
// ============================================================
// Define os estados da máquina de bebidas
enum MachineState {
  ST_INICIO,        // Estado inicial - aguarda inicialização
  ST_AGUARDA_MOEDA, // Aguarda inserção de moeda
  ST_AGUARDA_COPO,  // Aguarda colocação do copo
  ST_ESCOLHA,       // Aguarda escolha de bebida (café ou chá)
  ST_PREPARO_CAFE,  // Preparando café - enchimento em progresso
  ST_PREPARO_CHA,   // Preparando chá - enchimento em progresso
  ST_FIM_CAFE,      // Café pronto - aguarda retirada do copo
  ST_FIM_CHA        // Chá pronto - aguarda retirada do copo
};

// Estado atual da máquina
MachineState estadoAtual = ST_INICIO;

// --- Configuração de Pinos do ESP32 ---
// Sensores ultrassônicos para detecção de moeda
const uint8_t PIN_MOEDA_TRIG = 32;  // Pino de trigger (saída)
const uint8_t PIN_MOEDA_ECHO = 33;  // Pino de echo (entrada)

// Sensores ultrassônicos para detecção de copo
const uint8_t PIN_COPO_TRIG  = 19;  // Pino de trigger (saída)
const uint8_t PIN_COPO_ECHO  = 18;  // Pino de echo (entrada)

// Relés para controle dos motores de bomba
const uint8_t PIN_RELE_CAFE  = 25;  // Relé do motor de café
const uint8_t PIN_RELE_CHA   = 26;  // Relé do motor de chá

// Flag para lógica de ativação de relés (true = ativo em nível LOW)
const bool RELE_ATIVO_BAIXO = true;

// ============================================================
// OBJETOS DA TELA NEXTION
// ============================================================
// Páginas da interface HMI
NexPage page_initial    = NexPage(0, 0, "page_initial");   // Página inicial
NexPage page_moeda      = NexPage(1, 0, "page_moeda");     // Aguardando moeda
NexPage page_in_copo    = NexPage(2, 0, "page_in_copo");   // Aguardando copo
NexPage page_escolha    = NexPage(3, 0, "page_escolha");   // Escolha de bebida
NexPage page_cafe       = NexPage(4, 0, "page_cafe");      // Preparando café
NexPage page_cha        = NexPage(5, 0, "page_cha");       // Preparando chá
NexPage page_fim_cafe   = NexPage(6, 0, "page_fim_cafe");  // Café pronto
NexPage page_fim_cha    = NexPage(7, 0, "page_fim_cha");   // Chá pronto

// Botões da tela de escolha
NexButton bCafe         = NexButton(3, 2, "bCafe");  // Botão selecionar café
NexButton bCha          = NexButton(3, 3, "bCha");   // Botão selecionar chá

// Barras de progresso para visualizar enchimento
NexProgressBar bar_cafe = NexProgressBar(4, 2, "j0");  // Progresso do café
NexProgressBar bar_cha  = NexProgressBar(5, 2, "j0");  // Progresso do chá

// Lista de objetos que disparam eventos ao Nextion
NexTouch *nex_listen_list[] = {
  &bCafe,
  &bCha,
  NULL
};

// ============================================================
// FUNÇÕES AUXILIARES
// ============================================================

/**
 * Lê a distância em milímetros usando sensor ultrassônico HC-SR04
 * 
 * @param pinTrig - Pino de trigger (disparo do pulso)
 * @param pinEcho - Pino de echo (recepção da reflexão)
 * @return Distância em milímetros (sem uso de float para economizar memória)
 * 
 * Fórmula: (Duração * 10) / 58 = Distância em mm
 * (Equivalente a dividir por 5.8 usando apenas operações inteiras)
 */
int lerDistanciaMM(uint8_t pinTrig, uint8_t pinEcho) {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  // Aguarda o retorno do sinal (timeout de 30ms = ~5 metros)
  long duracao = pulseIn(pinEcho, HIGH, 30000);

  // Converte duração em milímetros (sem uso de float)
  return (int)((duracao * 10) / 58);
}

/**
 * Controla o estado de um relé (ligar/desligar motor)
 * 
 * @param pino - Pino do relé (GPIO)
 * @param ligar - true para ligar, false para desligar
 * 
 * Nota: A lógica XOR (^) com RELE_ATIVO_BAIXO inverte o sinal se necessário
 */
void controlarRele(uint8_t pino, bool ligar) {
  int nivelLogico = (RELE_ATIVO_BAIXO ^ ligar) ? HIGH : LOW;
  digitalWrite(pino, nivelLogico);
}

// ============================================================
// CALLBACKS NEXTION (Interrupções de eventos)
// ============================================================

/**
 * Callback disparado quando o botão de café é pressionado
 * Altera o estado da máquina para preparar café
 */
void bCafePopCallback(void *ptr) {
  estadoAtual = ST_PREPARO_CAFE;
}

/**
 * Callback disparado quando o botão de chá é pressionado
 * Altera o estado da máquina para preparar chá
 */
void bChaPopCallback(void *ptr) {
  estadoAtual = ST_PREPARO_CHA;
}

// ============================================================
// LÓGICA DE NEGÓCIO
// ============================================================

/**
 * Processa o enchimento de bebida com monitor de copo
 * 
 * Responsabilidades:
 * - Controla o motor da bomba
 * - Incrementa a barra de progresso
 * - Detecta remoção do copo durante o processo
 * - Pausa e retoma automaticamente se copo for recolocado
 * 
 * @param barra - Referência à barra de progresso Nextion
 * @param pinoMotor - Pino do relé do motor (CAFE ou CHA)
 */
void processarEnchimento(NexProgressBar &barra, uint8_t pinoMotor) {
  int progresso = 0;
  barra.setValue(0);
  
  unsigned long tempoInicio = millis();
  unsigned long ultAtualizacao = millis();
  
  // Constantes de timing (em milissegundos)
  const unsigned long DURACAO_TOTAL = 26000;         // Duração total de enchimento
  const unsigned long TIMEOUT_SEM_COPO = 10000;      // Tempo máximo sem copo detectado
  const unsigned long INTERVALO_INCREMENTO = 1000;   // Intervalo para incrementar barra
  
  bool processoAtivo = true;
  
  Serial.println(F("Iniciando processo de enchimento..."));

  while (processoAtivo && progresso < 100) {
    // Processa eventos da tela Nextion
    nexLoop(nex_listen_list);

    // Timeout geral (evita travamento)
    if (millis() - tempoInicio > (DURACAO_TOTAL + TIMEOUT_SEM_COPO + 5000)) {
       processoAtivo = false;
       break;
    }

    // Lê distância do copo
    int distMM = lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO);

    Serial.print("Distancia: ");
    Serial.println(distMM);

    // --- Lógica de Detecção de Copo (62mm ≈ 6.2cm) ---
    if (distMM <= 62) {
      // Copo detectado
      delay(1000);
      controlarRele(pinoMotor, true);  // Liga o motor

      // Incrementa barra a cada 1 segundo
      if (millis() - ultAtualizacao >= INTERVALO_INCREMENTO) {
        progresso += 4;
        if (progresso > 100) progresso = 100;
        
        barra.setValue(progresso);
        ultAtualizacao = millis();
        
        Serial.print(F("Dist: ")); Serial.print(distMM); 
        Serial.print(F("mm - Progresso: ")); Serial.println(progresso);
      }
    } 
    else {
      // Copo removido - pausa o enchimento
      controlarRele(pinoMotor, false);  // Desliga o motor
      
      // Aguarda recolocação do copo por até TIMEOUT_SEM_COPO ms
      unsigned long inicioPausa = millis();
      bool copoVoltou = false;

      while (millis() - inicioPausa < TIMEOUT_SEM_COPO) {
        int d = lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO);

        Serial.print("Aguardando copo: ");
        Serial.println(d);

        if (d <= 60) {
          copoVoltou = true;  // Copo foi recolocado
          break; 
        }
        delay(50);
      }

      // Se copo não retornou, encerra o processo
      if (!copoVoltou) {
        processoAtivo = false;
      }
    }
  }

  // Garante que o motor seja desligado ao final
  controlarRele(pinoMotor, false);
}

// ============================================================
// INICIALIZAÇÃO (SETUP)
// ============================================================

/**
 * Configura os periféricos e inicializa o sistema
 * - Abre comunicação serial
 * - Inicializa display Nextion
 * - Registra callbacks dos botões
 * - Configura pinos GPIO
 * - Desliga relés inicialmente
 */
void setup(void) {
  // Serial para debug (monitor serial)
  Serial.begin(115200);
  
  // Serial2 para comunicação com display Nextion (HMI)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  // Inicializa display Nextion
  nexInit();
  
  // Registra callbacks para interação com botões
  bCafe.attachPop(bCafePopCallback, &bCafe);
  bCha.attachPop(bChaPopCallback, &bCha);

  // Configura pinos dos sensores ultrassônicos
  pinMode(PIN_MOEDA_TRIG, OUTPUT);
  pinMode(PIN_MOEDA_ECHO, INPUT);
  pinMode(PIN_COPO_TRIG, OUTPUT);
  pinMode(PIN_COPO_ECHO, INPUT);

  // Configura pinos dos relés como saídas
  pinMode(PIN_RELE_CAFE, OUTPUT);
  pinMode(PIN_RELE_CHA, OUTPUT);

  // Desliga todos os relés por segurança
  controlarRele(PIN_RELE_CAFE, false);
  controlarRele(PIN_RELE_CHA, false);

  // Exibe página inicial no display
  page_initial.show();
  Serial.println(F("Sistema Iniciado (Sensor em MM)"));
}

// ============================================================
// LOOP PRINCIPAL
// ============================================================

/**
 * Máquina de estados principal
 * Gerencia as transições entre os diferentes estados da máquina de bebidas
 */
void loop(void) {
  // Processa eventos da tela Nextion em todos os estados
  nexLoop(nex_listen_list);

  switch (estadoAtual) {
    
    case ST_INICIO:
      // Aguarda 2 segundos e passa para detecção de moeda
      delay(2000);
      estadoAtual = ST_AGUARDA_MOEDA;
      break;

    case ST_AGUARDA_MOEDA:
      // Mostra página de aguardar moeda
      page_moeda.show();
      
      // Aguarda detecção de moeda (distância < 25mm)
      while(true) {
        nexLoop(nex_listen_list);
        int distMM = lerDistanciaMM(PIN_MOEDA_TRIG, PIN_MOEDA_ECHO);
        Serial.print("Distancia moeda: ");
        Serial.println(distMM);
        
        // Moeda detectada quando distância < 25mm (2.5cm)
        if (distMM < 25) {
          delay(500);  // Debounce
          break;
        }
        delay(50);
      }
      estadoAtual = ST_AGUARDA_COPO;
      break;

    case ST_AGUARDA_COPO:
      // Mostra página de aguardar colocação do copo
      page_in_copo.show();
      
      // Aguarda copo dentro da faixa de detecção (20-60mm)
      while(true) {
        nexLoop(nex_listen_list);
        int distMM = lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO);

        Serial.print("Distancia copo: ");
        Serial.println(distMM);
        
        // Copo detectado quando 20mm < distância <= 60mm
        if (distMM > 20 && distMM <= 60) break;
        delay(100);
      }
      estadoAtual = ST_ESCOLHA;
      break;

    case ST_ESCOLHA:
      // Mostra tela de escolha (café ou chá)
      page_escolha.show();
      
      // Aguarda escolha do usuário (callbacks atualizarão estadoAtual)
      while(estadoAtual == ST_ESCOLHA) {
        nexLoop(nex_listen_list);
        delay(50);
      }
      break;

    case ST_PREPARO_CAFE:
      // Mostra tela de preparo e inicia enchimento de café
      page_cafe.show();
      processarEnchimento(bar_cafe, PIN_RELE_CAFE);
      estadoAtual = ST_FIM_CAFE;
      break;

    case ST_PREPARO_CHA:
      // Mostra tela de preparo e inicia enchimento de chá
      page_cha.show();
      processarEnchimento(bar_cha, PIN_RELE_CHA);
      estadoAtual = ST_FIM_CHA;
      break;

    case ST_FIM_CAFE:
      // Café pronto - aguarda retirada do copo
      page_fim_cafe.show();
      while(lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO) <= 50) {
        delay(200);  // Verifica a cada 200ms
      }
      delay(2000);  // Aguarda 2 segundos adicionais
      estadoAtual = ST_AGUARDA_MOEDA;  // Volta para início do ciclo
      break;

    case ST_FIM_CHA:
      // Chá pronto - aguarda retirada do copo
      page_fim_cha.show();
      while(lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO) <= 50) {
        delay(200);  // Verifica a cada 200ms
      }
      delay(2000);  // Aguarda 2 segundos adicionais
      estadoAtual = ST_AGUARDA_MOEDA;  // Volta para início do ciclo
      break;
  }
}
