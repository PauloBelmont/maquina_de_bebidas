# Guia de Contribuição

Obrigado por seu interesse em contribuir com o projeto **Máquina de Bebidas (ESP32 + Nextion)**! Este repositório contém os artefatos desenvolvidos para a disciplina **DCC903 - Sistemas Embarcados**, integrando hardware, firmware e design mecânico.

Siga as etapas abaixo para colaborar de forma eficiente e segura com o hardware.

## 🧾 Estrutura do Repositório

Entenda onde cada parte do projeto está localizada:

* **`maquina_cafe_tela/`**: O coração do projeto.
    * **Código Fonte (`.ino`):** Lógica de controle do ESP32 (sensores, bombas, relés).
    * **Interface (`.HMI` / `.tft`):** Arquivos do Editor Nextion para a tela touch.
* **`docs/`**: Documentação de engenharia (requisitos, diagramas de pinagem, manuais e relatório técnico).
* **Hardware/3D**: Links para os arquivos STL das peças impressas (Case, moedeiro).

---

## 📌 Requisitos para Contribuir

Para rodar e modificar este projeto, você precisará das seguintes ferramentas:

1. **Hardware (para testes reais):**
   * ESP32 Devkit V1 e Display Nextion (modelo 3224T024_011).
   * Sensores HC-SR04 e atuadores conforme lista de materiais.
2. **Software (obrigatório):**
   * [Arduino IDE](https://www.arduino.cc/en/software) (com as bibliotecas do ESP32 instaladas).
   * [Editor Nextion LTS](https://nextion.tech/download/nextion-setup-vLTS.zip) para editar a interface gráfica.
3. **Simulação (Opcional):**
   * Conta no [TinkerCad](https://www.tinkercad.com/) caso não possua o hardware físico para validar a lógica básica.

---

## 🚀 Etapas para Contribuir

1. **Faça um Fork e Clone**
   ```bash
   git clone https://github.com/PauloBelmont/maquina_de_bebidas.git
   cd maquina_de_bebidas
   ```

2. **Crie uma Branch Descritiva**
   Use prefixos para identificar se a alteração é no firmware, na tela ou na doc:
   ```bash
   git checkout -b firmware/correcao-leitura-sensor
   # ou
   git checkout -b hmi/novo-botao-cafe
   ```

3. **Desenvolvimento e Testes**

   * **Se mexer no Código (.ino):** Certifique-se de que compila na Arduino IDE sem erros. Se adicionar bibliotecas novas, atualize a documentação.
   * **Se mexer na Tela (.HMI):** Gere o arquivo compilado no Editor Nextion e teste a comunicação Serial com o ESP32.
   * **Se mexer na Pinagem:** **CUIDADO!** Alterações de pinos devem ser refletidas imediatamente no arquivo `docs/` e no `README.md` para evitar curto-circuitos na montagem física.

4. **Commit e Push**
   Descreva se a alteração exige atualização do hardware (ex: mudar um jumper de lugar).
   ```bash
   git add .
   git commit -m "Ajusta tempo de acionamento da Bomba 1 para 5 segundos"
   git push origin firmware/correcao-leitura-sensor
   ```

5. **Abra um Pull Request**
   * Descreva quais componentes foram testados.
   * Se alterou a interface, anexe um *screenshot* ou foto da tela Nextion.

---

## ✅ Boas Práticas de Sistemas Embarcados

* **Pinagem (Pinout):** Nunca altere a definição de pinos no código sem verificar se isso conflita com o hardware montado (ex: pinos de *boot* do ESP32 ou pinos exclusivos de entrada).
* **Bibliotecas:** Evite usar bibliotecas muito pesadas que ocupem muita memória do ESP32 sem necessidade.
* **Interface Nextion:** Ao nomear variáveis na tela Nextion, use nomes curtos e claros, pois isso facilita o envio de comandos via Serial.
* **Arquivos Gerados:** Evite subir arquivos compilados temporários, mas **mantenha** o `.tft` (compilado da tela) atualizado se houver mudanças visuais, para facilitar a gravação por outros devs.

---

## 🛠 Sugestões de Contribuição

* **Refatoração:** Melhorar a máquina de estados do código Arduino.
* **Calibração:** Ajustar os valores do sensor ultrassônico (HC-SR04) para detectar copos com maior precisão.
* **Interface:** Melhorar a UX/UI da tela Nextion (ícones, feedback visual de "preparando").
* **Documentação:** Preencher a seção de "Pinagem ESP32" no README principal, que atualmente está vazia.

---

## 📫 Dúvidas?

Abra uma [issue](https://github.com/PauloBelmont/maquina_de_bebidas/issues) se tiver dúvidas sobre as conexões elétricas ou sobre a lógica de comunicação Serial entre o ESP32 e o Nextion.

---
