# Política de Segurança

Este projeto é um sistema embarcado acadêmico (ESP32 + Nextion). Levamos a segurança a sério, especialmente no que tange à integridade do firmware e à segurança física dos acionamentos (bombas e relés).

## Versões Suportadas

Atualmente, aplicamos correções de segurança apenas na versão mais recente da branch principal.

| Versão | Suportada          | Notas |
| ------ | ------------------ | ----- |
| `main` | :white_check_mark: | Última versão estável do firmware e HMI |
| tags antigas | :x: | Versões de desenvolvimento anteriores não recebem patches |

## Como Reportar uma Vulnerabilidade

Se você descobriu uma falha de segurança (seja no código C++ do ESP32 ou na lógica da tela Nextion), pedimos que **NÃO abra uma Issue pública** imediatamente, para evitar que falhas críticas sejam exploradas antes de uma correção.

Por favor, siga estes passos:

1. Entre em contato com um dos autores listados no README.
2. No assunto, coloque: `SECURITY REPORT - Máquina de Bebidas`.
3. Inclua detalhes sobre como reproduzir a falha (ex: payload serial específico, sequência de toques na tela, estouro de buffer).

Nós nos comprometemos a analisar o relatório e responder dentro de **48 a 72 horas**.

## Escopo de Segurança (IoT e Hardware)

Devido à natureza híbrida (Software + Hardware) deste projeto, consideramos os seguintes cenários como vulnerabilidades críticas:

### O que consideramos vulnerabilidade:
* **Falhas de Injeção Serial:** Comandos maliciosos enviados via UART que permitam acionar bombas ou relés sem autorização/pagamento.
* **Buffer Overflows:** Erros no tratamento de strings no ESP32 que causem *crash* ou comportamento inesperado do microcontrolador.
* **Falhas de Lógica de Atuadores:** Bugs que possam deixar bombas ou relés ligados indefinidamente (risco de vazamento, curto-circuito ou dano à bateria VRLA).

### O que NÃO consideramos vulnerabilidade (Fora de Escopo):
* **Acesso Físico Direto:** Se um atacante tem acesso físico à placa ESP32 (pode plugar um cabo USB e regravar o código), isso não é considerado uma falha de segurança do software, mas uma limitação física do case.
* **Engenharia Reversa do Hardware:** A desmontagem da máquina ou impressão 3D para acesso aos componentes internos.

## Práticas de Desenvolvimento Seguro

Para mantenedores e contribuintes, seguimos estas diretrizes:

1. **Watchdog Timer:** O firmware deve implementar Watchdog (WDT) para reiniciar o ESP32 em caso de travamento, evitando que bombas permaneçam ligadas por erro de software.
2. **Sanitização de Entradas:** Todos os dados recebidos da tela Nextion via Serial devem ser validados antes de executar uma ação.
3. **Credenciais:** Nunca commite senhas de Wi-Fi ou tokens de API no código-fonte. Use arquivos de cabeçalho (`.h`) ignorados pelo `.gitignore` ou variáveis de ambiente.

## Divulgação

Após a correção da vulnerabilidade, publicaremos uma nota na seção "Releases" descrevendo o problema (de forma resumida) e creditando o pesquisador que fez a descoberta, caso ele autorize.

---
