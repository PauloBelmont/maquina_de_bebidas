## 📝 Descrição
<!-- Resuma as mudanças que este Pull Request traz. -->
<!-- Explique o "Porquê" dessa mudança. -->
<!-- Exemplo: "Corrige o tempo de acionamento da bomba 1 que estava transbordando o copo." -->

## 🔗 Issue Relacionada
<!-- Se houver uma issue aberta, linke-a aqui. Ex: "Closes #12" -->

## 🏷️ Tipo de Mudança
<!-- Marque as opções relevantes com um [x] -->

- [ ] 🐛 **Bug fix** (Correção de erro no código ou lógica)
- [ ] ✨ **Nova Funcionalidade** (Adição de sensor, novo menu, etc.)
- [ ] 🎨 **Interface (HMI)** (Alterações visuais na tela Nextion)
- [ ] 🔌 **Hardware/Pinagem** (Mudança na conexão dos fios ou componentes)
- [ ] 📚 **Documentação** (Readme, diagramas, comentários)
- [ ] 🔧 **Refatoração** (Melhoria de código sem mudar funcionalidade)

---

## 🧪 Testes Realizados
<!-- Descreva como você testou essa mudança. É CRÍTICO saber se foi testado no hardware real. -->

- [ ] **Simulação** (TinkerCad / Wokwi)
- [ ] **Hardware Real** (Testado na máquina física com ESP32)
- [ ] **Compilação** (Apenas verificou se o código compila na IDE Arduino)

**Detalhes dos testes:**
<!-- Ex: "Testei enchendo 3 copos seguidos, o relé desligou corretamente em todas as vezes." -->

---

## ⚡ Verificação de Hardware & Segurança
<!-- ATENÇÃO: Mudanças de hardware incorretas podem queimar componentes! -->

- [ ] Esta mudança requer alteração na **Pinagem** do ESP32?
    - *Se SIM, você atualizou o arquivo de documentação dos pinos?*
- [ ] Esta mudança afeta o comportamento de **Bombas ou Relés**?
    - *Se SIM, você verificou se há risco de as bombas ficarem ligadas para sempre (loop infinito)?*

---

## 📸 Screenshots / Vídeos (Opcional)
<!-- Se você alterou a interface Nextion (.HMI), cole aqui um print ou foto da tela. -->

---

## ✅ Checklist Final
- [ ] O código compila sem erros.
- [ ] Removi arquivos temporários ou desnecessários antes de commitar.
- [ ] Não subi senhas de Wi-Fi ou chaves privadas no código.
- [ ] Comentei trechos complexos do código para facilitar o entendimento.
