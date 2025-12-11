#include "Nextion.h" // Inclui a biblioteca para comunicação com o display Nextion

// =================================================================
// 🚀 DEFINIÇÕES E OBJETOS DO NEXTION
// =================================================================

// Mapeamento das páginas no Display Nextion. O formato é: NexPage(ID da Página, ID do Componente, "Nome da Página")
NexPage page_initial    = NexPage(0, 0, "page_initial"); // Tela inicial/standby
NexPage page_moeda      = NexPage(1, 0, "page_moeda");   // Aguardando inserção de moeda
NexPage page_in_copo    = NexPage(2, 0, "page_in_copo"); // Aguardando inserção do copo
NexPage page_escolha    = NexPage(3, 0, "page_escolha"); // Tela de escolha (Café ou Chá)
NexPage page_cafe       = NexPage(4, 0, "page_cafe");    // Processo de preparo do Café
NexPage page_cha        = NexPage(5, 0, "page_cha");     // Processo de preparo do Chá
NexPage page_fim_cafe   = NexPage(6, 0, "page_fim_cafe"); // Tela de finalização do Café
NexPage page_fim_cha    = NexPage(7, 0, "page_fim_cha");  // Tela de finalização do Chá

// Componentes Interativos na Tela de Escolha
NexButton bCafe = NexButton(3, 2, "bCafe"); // Botão "Café" na Página 3
NexButton bCha = NexButton(3, 3, "bCha");   // Botão "Chá" na Página 3

// Barras de Progresso nas Telas de Preparo
NexProgressBar ProgressBar_cafe  = NexProgressBar(4, 2, "j0"); // Barra de progresso na Página do Café (ID j0)
NexProgressBar ProgressBar_cha  = NexProgressBar(5, 2, "j0");  // Barra de progresso na Página do Chá (ID j0)

// =================================================================
// ⚙️ VARIÁVEIS GLOBAIS E PINOS DE I/O
// =================================================================

// Variáveis de Controle de Estado e Fluxo
int estado = 0;             // Variável principal para a Máquina de Estados (0=Standby, 1=Moeda, 2=Copo...)
bool escolha_show = false;  // Flag para garantir que a tela de escolha seja mostrada apenas uma vez
char buffer[100] = {0};     // Buffer genérico (não muito utilizado aqui, mas comum)
long duracao;               // Armazena a duração do pulso de ECHO do sensor ultrassônico (em microssegundos)
int distanciaCm;            // Distância calculada em centímetros

// --- PINOS DO ESP32 PARA SENSORES ULTRASSÔNICOS ---
const int moeda_trigger = 34; // Trigger do sensor para detecção de moeda
const int moeda_echo = 35;    // Echo do sensor para detecção de moeda

const int copo_trigger = 19;  // Trigger do sensor para detecção/presença do copo
const int copo_echo = 18;     // Echo do sensor para detecção/presença do copo

// --- PINOS DO ESP32 PARA RELÉS (Motores/Bombas) ---
const int pinoReleCafe = 26; // Pino para o Relé do motor/bomba do Café
const int pinoReleCha = 25;  // Pino para o Relé do motor/bomba do Chá

// --- CONSTANTES DE LÓGICA DE SEGURANÇA ---
const bool RELE_ATIVO_BAIXO = true;        // Define a lógica: TRUE -> LOW (0V) aciona o relé.
const int COPO_PRESENTE_LIMITE = 6;       // Distância máxima (em cm) para considerar o copo presente.
const unsigned long TIMEOUT_ESPERA_MS = 10000; // Tempo limite (10 segundos) para recolocar o copo, se removido.

// =================================================================
// 🛠️ FUNÇÕES DE SERVIÇO
// =================================================================

// Função para controlar o estado LIGADO/DESLIGADO do relé
void controlarRele(int pino, bool estado) {
  // Define o estado físico (LOW ou HIGH) baseado se o relé é Ativo Baixo
  int estadoLigar = RELE_ATIVO_BAIXO ? LOW : HIGH; 
  int estadoDesligar = RELE_ATIVO_BAIXO ? HIGH : LOW;

  if (estado) {
    digitalWrite(pino, estadoLigar); // Envia o sinal que LIGA o relé (LOW se for Ativo Baixo)
    Serial.print("Rele PINO "); Serial.print(pino); Serial.println(" LIGADO.");
  } else {
    digitalWrite(pino, estadoDesligar); // Envia o sinal que DESLIGA o relé (HIGH se for Ativo Baixo)
    Serial.print("Rele PINO "); Serial.print(pino); Serial.println(" DESLIGADO.");
  }
}

// Lista de componentes Nextion que devem ser monitorados para eventos de toque
NexTouch *nex_listen_list[] = {
  &bCafe,
  &bCha,
  NULL // O último elemento deve ser NULL para sinalizar o fim da lista
};

// =================================================================
// ⚙️ SETUP (Configurações Iniciais)
// =================================================================

void setup(void) {  
  // Inicializa a comunicação Serial2 com o display Nextion (pino 16=RX, 17=TX)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.begin(115200); // Inicializa a Serial de debug

  // Configuração dos pinos dos sensores ultrassônicos (Trigger=OUTPUT, Echo=INPUT)
  pinMode(moeda_trigger, OUTPUT);
  pinMode(moeda_echo, INPUT);
  pinMode(copo_trigger, OUTPUT);
  pinMode(copo_echo, INPUT);

  // Configuração dos pinos dos relés como SAÍDA
  pinMode(pinoReleCafe, OUTPUT);
  pinMode(pinoReleCha, OUTPUT);
  
  // Garante que os relés estejam DESLIGADOS (estado 'false') na inicialização
  controlarRele(pinoReleCafe, false); 
  controlarRele(pinoReleCha, false); 
  
  nexInit(); // Inicializa a biblioteca Nextion
  dbSerialPrintln("setup begin");
  page_initial.show(); // Exibe a primeira página (Standby)
  
  // Associa as funções de callback aos botões do Nextion
  bCafe.attachPop(bCafePopCallback, &bCafe);
  bCha.attachPop(bChaPopCallback, &bCha);
  dbSerialPrintln("setup end");
}

// =================================================================
// 🔄 LOOP PRINCIPAL (Máquina de Estados)
// =================================================================

void loop(void) {
  nexLoop(nex_listen_list); // Monitora eventos de toque do Nextion

  // A variável 'estado' controla o fluxo da máquina de venda
  switch (estado) {
    case 0: // Inicial: Aguarda 2s antes de ir para o primeiro passo (Moeda)
      delay(2000);
      estado = 1;
      break;

    case 1: // Aguardando Moeda
      page_moeda.show();
      acionar_sensor_moeda(); // BLOQUEIA até a moeda ser detectada
      estado = 2;
      break;

    case 2: // Aguardando Copo
      page_in_copo.show();
      acionar_sensor_copo(); // BLOQUEIA até o copo ser detectado
      estado = 3;
      break;

    case 3: // Escolha da Bebida
      if (!escolha_show){
        page_escolha.show();
        escolha_show = true;
      }
      // O programa espera aqui até um dos botões chamar o callback (estado 4 ou 5)
      break;

    case 4: // Preparo do Café (Acionado por bCafePopCallback)
      escolha_show = false; // Reseta flag
      page_cafe.show();
      acionar_progress_bar(); // Inicia o preparo com a lógica de tempo e relé
      estado = 6;
      break;

    case 5: // Preparo do Chá (Acionado por bChaPopCallback)
      escolha_show = false; // Reseta flag
      page_cha.show();
      acionar_progress_bar(); // Inicia o preparo com a lógica de tempo e relé
      estado = 7;
      break;
      
    case 6: // Fim do Café: Aguardando retirada do copo
      page_fim_cafe.show();
      acionar_sensor_copo_saida(); // BLOQUEIA até o copo ser removido
      delay(2000);
      estado = 1; // Volta para o estado de Moeda
      break;
      
    case 7: // Fim do Chá: Aguardando retirada do copo
      page_fim_cha.show();
      acionar_sensor_copo_saida(); // BLOQUEIA até o copo ser removido
      delay(2000);
      estado = 1; // Volta para o estado de Moeda
      break;
  }
}

// =================================================================
// 📞 CALLBACKS DO NEXTION
// =================================================================

// Chamado quando o botão "Café" é pressionado
void bCafePopCallback(void *ptr) {
  dbSerialPrintln("bCafePopCallback");
  estado = 4; // Muda para o estado de Preparo do Café
}

// Chamado quando o botão "Chá" é pressionado
void bChaPopCallback(void *ptr) {
  dbSerialPrintln("bChaPopCallback");
  estado = 5; // Muda para o estado de Preparo do Chá
}

// =================================================================
// ⏱️ LÓGICA DE PREPARO (Relé e Progressão)
// =================================================================

// Variáveis de controle de progresso (globais para manter o contexto)
uint32_t number = 0; 
unsigned long lastUpdateTime; // Marca o tempo da última atualização da barra (1s)

// Função principal que controla o motor/relé, a barra de progresso e a segurança do copo
void acionar_progress_bar() {
  number = 0; // Zera o progresso
  
  // Determina qual barra e qual relé usar com base no 'estado' atual
  NexProgressBar *currentProgressBar = (estado == 4) ? &ProgressBar_cafe : &ProgressBar_cha;
  int pinoMotor = (estado == 4) ? pinoReleCafe : pinoReleCha;
  
  currentProgressBar->setValue(number);
  
  Serial.println("Iniciando preparo com lógica de pausa/retomada...");
  
  lastUpdateTime = millis(); // Inicializa o cronômetro para a atualização da barra
  
  const unsigned long duration = 26000; // Tempo total necessário de motor ligado (26 segundos)
  unsigned long time_motor_on_ms = 0;   // Contador: Tempo acumulado que o motor ficou LIGADO (0 a 26000)
  unsigned long last_loop_time = millis(); // Marca o tempo no início de cada ciclo (usado para calcular delta_time)
  
  bool continueProcess = true; 
  const unsigned long updateInterval = 1000; // Atualiza a barra a cada 1 segundo
  
  // Loop de preparo: continua enquanto o tempo acumulado for menor que o tempo total (duration)
  while (time_motor_on_ms < duration && continueProcess) {
    
    // 1. Calcula o tempo real decorrido na última iteração do loop
    unsigned long current_loop_time = millis();
    unsigned long delta_time = current_loop_time - last_loop_time;
    last_loop_time = current_loop_time; // Reseta o marcador para o próximo ciclo

    // 2. VERIFICAÇÃO CONSTANTE DO SENSOR DE COPO
    int distancia = ler_distancia_cm(copo_trigger, copo_echo);
    
    // Condição: Copo está presente (OK para continuar)
    if (distancia <= COPO_PRESENTE_LIMITE) {
      
      controlarRele(pinoMotor, true); // Motor LIGA
      
      // Acumula o tempo de preparo: SOMENTE adiciona delta_time se o motor está ligado
      time_motor_on_ms += delta_time; 
      
      // 3. ATUALIZAÇÃO DA BARRA DE PROGRESSO (A cada 1 segundo)
      if (millis() - lastUpdateTime >= updateInterval) {
          
          // Calcula o percentual baseado no tempo acumulado vs tempo total
          number = (uint32_t)((float)time_motor_on_ms / duration * 100.0);
          
          if (number > 100) { number = 100; }
          currentProgressBar->setValue(number);
          
          lastUpdateTime = millis();
          
          Serial.print("Tempo acumulado: ");
          Serial.print(time_motor_on_ms / 1000);
          Serial.print("s -> Progresso: ");
          Serial.print(number);
          Serial.println("%");
      }
    } 
    // Condição: Copo foi removido (ATIVAR LÓGICA DE PAUSA)
    else {
      
      controlarRele(pinoMotor, false); // Motor DESLIGA IMEDIATAMENTE (Segurança)
      Serial.println("Copo removido. Iniciando espera de 10 segundos para retomar...");
      
      unsigned long waitStartTime = millis();
      bool returnedToCondition = false;
      
      // Loop de Espera de 10 segundos (TIMEOUT)
      while ((millis() - waitStartTime) < TIMEOUT_ESPERA_MS) {
          distancia = ler_distancia_cm(copo_trigger, copo_echo);
          
          if (distancia <= COPO_PRESENTE_LIMITE) {
              returnedToCondition = true;
              Serial.println("Copo retornado! Retomando preparo...");
              
              // CRUCIAL: Reseta last_loop_time para o momento atual. 
              // Isso garante que o 'delta_time' no próximo ciclo não inclua o tempo da pausa.
              last_loop_time = millis(); 
              break; 
          }
          
          delay(10); // Pequeno delay no loop de espera
      }
      
      // Se saiu do loop de espera e o copo não retornou: CANCELA
      if (!returnedToCondition) {
          Serial.println("Tempo esgotado (10s). Preparo CANCELADO.");
          continueProcess = false; // Força a saída do loop principal
      }
    }
    
    delay(10); // Pequeno delay no loop principal
  }
  
  // --- FIM DO PROCESSO ---
  controlarRele(pinoMotor, false); // Garante que o motor esteja desligado

  if (time_motor_on_ms >= duration) {
      number = 100;
      currentProgressBar->setValue(number);
      Serial.println("Preparo Completo: 100%. Bebida finalizada.");
  } else {
      // Se o processo foi cancelado por timeout
      Serial.print("Preparo interrompido e cancelado no tempo: ");
      Serial.print(time_motor_on_ms / 1000);
      Serial.println("s.");
  }
}

// =================================================================
// 📡 FUNÇÕES DE LEITURA DO ULTRASSÔNICO
// =================================================================

// Função genérica para ler distância de qualquer sensor (usada internamente)
int ler_distancia_cm(int trigger_pin, int echo_pin) {
  // Sequência padrão para disparo do ultrassônico
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  
  // Mede a duração do pulso de retorno (tempo de voo do som)
  long duracao = pulseIn(echo_pin, HIGH, 30000); // 30ms de timeout
  int distanciaCm = duracao / 58; // Conversão de tempo para distância (D = tempo / 58)
  
  if (duracao == 0) {
    return 999; // Retorna valor alto se houve timeout (fora de alcance)
  }
  
  return distanciaCm;
}

// BLOQUEIA o programa até que a moeda seja detectada
void acionar_sensor_moeda() {
   dbSerial.println("Sensor Moeda ");
  while(1) {
    distanciaCm = ler_distancia_cm(moeda_trigger, moeda_echo);
    dbSerial.println(distanciaCm);
    if (distanciaCm <= 3 && distanciaCm > 0) return; // Moeda detectada (próxima)
    delay(100);
  }
}

// BLOQUEIA o programa até que o copo seja detectado
void acionar_sensor_copo() {
  dbSerial.println("Sensor Copo Saida");
  while(1) {
    distanciaCm = ler_distancia_cm(copo_trigger, copo_echo);
    dbSerial.println(distanciaCm);
    if (distanciaCm > 2 && distanciaCm <= COPO_PRESENTE_LIMITE) return; // Copo no local
    delay(100);
  }
}

// BLOQUEIA o programa até que o copo seja removido
void acionar_sensor_copo_saida() {
  dbSerial.println("Sensor Copo Saida ");
  while(1) {
    distanciaCm = ler_distancia_cm(copo_trigger, copo_echo);
    dbSerial.println(distanciaCm);
    if (distanciaCm > COPO_PRESENTE_LIMITE) return; // Distância alta = copo removido
    delay(100);
  }
}