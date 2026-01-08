# Guia de Contribuição

Obrigado por seu interesse em contribuir com o projeto **Máquina de Bebidas**! Este repositório visa criar uma simulação robusta e modular de uma máquina de venda de chá e café, aplicando boas práticas de programação.

Siga as etapas abaixo para colaborar de forma eficiente.

## 🧾 O que está neste repositório?

* **Código Fonte:** Lógica principal da máquina, gerenciamento de estoque e processamento de pagamentos;
* **Testes:** Casos de teste para validar a lógica de troco e seleção de produtos;
* **Documentação:** BigPicture, Esquema de conexões, asset da tela e instruções de uso.

---

## 📌 Requisitos para contribuir

Antes de enviar contribuições, certifique-se de:

* Possuir o ambiente de desenvolvimento configurado (Arduino IDE);
* Conhecer a lógica básica do sistema;
* Você tem o [Git](https://git-scm.com/) instalado e sabe usar pull requests no GitHub.

---

## 🚀 Etapas para contribuir

1. **Faça um fork do repositório**

   * Clique em "Fork" no topo da página do projeto.

2. **Clone o seu fork**

   ```bash
   git clone https://github.com/PauloBelmont/maquina_de_bebidas.git
   cd maquina_de_bebidas
   ```

3. **Crie uma nova branch**
   Nomeie a branch de forma descritiva conforme a funcionalidade ou correção:

   ```bash
   git checkout -b feature/nova-bebida
   # ou
   git checkout -b fix/calculo-troco
   ```

4. **Implemente suas alterações**

   * Mantenha o código limpo e organizado;
   * Comente lógicas complexas se necessário;

5. **Faça commit das suas alterações**
   Escreva mensagens de commit claras e objetivas (use inglês ou português, mas mantenha a consistência):

   ```bash
   git add .
   git commit -m "Adiciona botão Sair"
   ```

6. **Envie sua branch para o GitHub**

   ```bash
   git push origin feature/nova-bebida
   ```

7. **Abra um Pull Request**

   * Vá até o seu repositório forkado;
   * Clique em "Compare & pull request";
   * Descreva claramente o que foi alterado e o impacto na simulação.

---

## ✅ Boas práticas

* **Testes:** Antes de enviar, verifique se a máquina compila e executa sem erros;
* **Modularidade:** Evite criar "God Classes" (classes que fazem tudo). Mantenha a separação entre a interface (menu) e a lógica de negócio;
* **Arquivos Ignorados:** Não suba arquivos binários ou de configuração da sua IDE (ex: `.class`, `.exe`, pasta `.idea` ou `.vscode`);
* **Estilo:** Siga a indentação e estilo de código já presentes no projeto.

---

## 🛠 Sugestões de contribuição

* Melhoria no algoritmo de temporisação;
* Tratamento de exceções;
* Criação de Deshbord para visualizar Status da máquina;
* Refatoração para aplicar Design Patterns.

---

## 📫 Dúvidas?

Abra uma [issue](https://github.com/PauloBelmont/maquina_de_bebidas/issues) para relatar bugs, sugerir melhorias na lógica ou tirar dúvidas sobre a estrutura do código.

---

Agradecemos por colaborar com este projeto! ☕🥤
