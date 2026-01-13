#include "Nextion.h"

// ============================================================
// DEFINIÇÕES E CONFIGURAÇÕES
// ============================================================

enum MachineState {
  ST_INICIO,        // 0
  ST_AGUARDA_MOEDA, // 1
  ST_AGUARDA_COPO,  // 2
  ST_ESCOLHA,       // 3
  ST_PREPARO_CAFE,  // 4
  ST_PREPARO_CHA,   // 5
  ST_FIM_CAFE,      // 6
  ST_FIM_CHA        // 7
};

/* MachineState */ estadoAtual = ST_INICIO;

// Definição de Pinos
const uint8_t PIN_MOEDA_TRIG = 32;
const uint8_t PIN_MOEDA_ECHO = 33;
const uint8_t PIN_COPO_TRIG  = 19;
const uint8_t PIN_COPO_ECHO  = 18;
const uint8_t PIN_RELE_CAFE  = 25;
const uint8_t PIN_RELE_CHA   = 26;

const bool RELE_ATIVO_BAIXO = true; 

// ============================================================
// OBJETOS NEXTION
// ============================================================

NexPage page_initial    = NexPage(0, 0, "page_initial");
NexPage page_moeda      = NexPage(1, 0, "page_moeda");
NexPage page_in_copo    = NexPage(2, 0, "page_in_copo");
NexPage page_escolha    = NexPage(3, 0, "page_escolha");
NexPage page_cafe       = NexPage(4, 0, "page_cafe");
NexPage page_cha        = NexPage(5, 0, "page_cha");
NexPage page_fim_cafe   = NexPage(6, 0, "page_fim_cafe");
NexPage page_fim_cha    = NexPage(7, 0, "page_fim_cha");

NexButton bCafe         = NexButton(3, 2, "bCafe");
NexButton bCha          = NexButton(3, 3, "bCha");

NexProgressBar bar_cafe = NexProgressBar(4, 2, "j0");
NexProgressBar bar_cha  = NexProgressBar(5, 2, "j0");

NexTouch *nex_listen_list[] = {
  &bCafe,
  &bCha,
  NULL
};

// ============================================================
// FUNÇÕES AUXILIARES
// ============================================================

// Função ajustada para retornar MILÍMETROS
int lerDistanciaMM(uint8_t pinTrig, uint8_t pinEcho) {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  long duracao = pulseIn(pinEcho, HIGH, 30000); 

  // FÓRMULA ALTERADA: (Duracao * 10) / 58
  // Isso equivale a dividir por 5.8, convertendo para mm sem usar float pesado
  return (int)((duracao * 10) / 58);
}

void controlarRele(uint8_t pino, bool ligar) {
  int nivelLogico = (RELE_ATIVO_BAIXO ^ ligar) ? HIGH : LOW;
  digitalWrite(pino, nivelLogico);
}

// ============================================================
// CALLBACKS NEXTION
// ============================================================

void bCafePopCallback(void *ptr) {
  estadoAtual = ST_PREPARO_CAFE;
}

void bChaPopCallback(void *ptr) {
  estadoAtual = ST_PREPARO_CHA;
}

// ============================================================
// LÓGICA DE NEGÓCIO
// ============================================================

void processarEnchimento(NexProgressBar &barra, uint8_t pinoMotor) {
  int progresso = 0;
  barra.setValue(0);
  
  unsigned long tempoInicio = millis();
  unsigned long ultAtualizacao = millis();
  
  const unsigned long DURACAO_TOTAL = 26000;
  const unsigned long TIMEOUT_SEM_COPO = 10000;
  const unsigned long INTERVALO_INCREMENTO = 1000;
  
  bool processoAtivo = true;
  
  Serial.println(F("Iniciando processo (MM)..."));

  while (processoAtivo && progresso < 100) {
    nexLoop(nex_listen_list);

    if (millis() - tempoInicio > (DURACAO_TOTAL + TIMEOUT_SEM_COPO + 5000)) {
       processoAtivo = false;
       break;
    }

    int distMM = lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO);

    Serial.print("Distancia:  ");
    Serial.println(distMM);

    // --- LÓGICA ATUALIZADA PARA MM ---
    // Antes: <= 6 cm | Agora: <= 60 mm
    if (distMM <= 62) {
      delay(1000);
      controlarRele(pinoMotor, true);

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
      controlarRele(pinoMotor, false);
      //Serial.println(F("Copo removido! Pausando..."));
      
      unsigned long inicioPausa = millis();
      bool copoVoltou = false;

      while (millis() - inicioPausa < TIMEOUT_SEM_COPO) {
        int d = lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO);
        // Antes: <= 6 cm | Agora: <= 60 mm

        Serial.print("d:  ");
        Serial.println(d);

        if (d <= 60) {
          copoVoltou = true;
          break; 
        }
        delay(50);
      }

      if (!copoVoltou) {
        processoAtivo = false;
      }
    }
  }

  controlarRele(pinoMotor, false);
}

// ============================================================
// SETUP
// ============================================================

void setup(void) {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  nexInit();
  
  bCafe.attachPop(bCafePopCallback, &bCafe);
  bCha.attachPop(bChaPopCallback, &bCha);

  pinMode(PIN_MOEDA_TRIG, OUTPUT);
  pinMode(PIN_MOEDA_ECHO, INPUT);
  pinMode(PIN_COPO_TRIG, OUTPUT);
  pinMode(PIN_COPO_ECHO, INPUT);

  pinMode(PIN_RELE_CAFE, OUTPUT);
  pinMode(PIN_RELE_CHA, OUTPUT);

  controlarRele(PIN_RELE_CAFE, false);
  controlarRele(PIN_RELE_CHA, false);

  page_initial.show();
  Serial.println(F("Sistema Iniciado (Modo MM)"));
}

// ============================================================
// LOOP PRINCIPAL
// ============================================================

void loop(void) {
  nexLoop(nex_listen_list);

  switch (estadoAtual) {
    
    case ST_INICIO:
      delay(2000);
      estadoAtual = ST_AGUARDA_MOEDA;
      break;

    case ST_AGUARDA_MOEDA:
      page_moeda.show();
      while(true) {
        nexLoop(nex_listen_list);
        int distMM = lerDistanciaMM(PIN_MOEDA_TRIG, PIN_MOEDA_ECHO);
        Serial.print("Distancia:  ");
        Serial.println(distMM);
        
        // Antes: > 0 e < 3 cm | Agora: > 0 e < 30 mm
        if (distMM < 25) {
          delay(500);
          break;
        }
        delay(50);
      }
      estadoAtual = ST_AGUARDA_COPO;
      break;

    case ST_AGUARDA_COPO:
      page_in_copo.show();
      while(true) {
        nexLoop(nex_listen_list);
        int distMM = lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO);

        Serial.print("Distancia:  ");
        Serial.println(distMM);
        
        // Antes: > 2 e <= 6 cm | Agora: > 20 e <= 60 mm
        if (distMM > 20 && distMM <= 60) break;
        delay(100);
      }
      estadoAtual = ST_ESCOLHA;
      break;

    case ST_ESCOLHA:
      page_escolha.show();
      while(estadoAtual == ST_ESCOLHA) {
        nexLoop(nex_listen_list);
        delay(50);
      }
      break;

    case ST_PREPARO_CAFE:
      page_cafe.show();
      processarEnchimento(bar_cafe, PIN_RELE_CAFE);
      estadoAtual = ST_FIM_CAFE;
      break;

    case ST_PREPARO_CHA:
      page_cha.show();
      processarEnchimento(bar_cha, PIN_RELE_CHA);
      estadoAtual = ST_FIM_CHA;
      break;

    case ST_FIM_CAFE:
      page_fim_cafe.show();
      // Aguarda retirada do copo. Antes: <= 5 cm | Agora: <= 50 mm
      while(lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO) <= 50) {
        delay(200);
      }
      delay(2000);
      estadoAtual = ST_AGUARDA_MOEDA;
      break;

    case ST_FIM_CHA:
      page_fim_cha.show();
      // Antes: <= 5 cm | Agora: <= 50 mm
      while(lerDistanciaMM(PIN_COPO_TRIG, PIN_COPO_ECHO) <= 50) {
        delay(200);
      }
      delay(2000);
      estadoAtual = ST_AGUARDA_MOEDA;
      break;
  }
}
