# Interval Guess - Jogo de Treinamento de Ouvido

Este é um projeto desenvolvido para a disciplina de Programação Orientada a Objetos (C++). Trata-se de uma aplicação de piano interativo construída com o framework JUCE, com foco em um jogo de treinamento de ouvido musical.

---

## Requisitos

Para compilar e rodar este projeto, você precisará de:

* O framework **JUCE** (versão mais recente de Setembro de 2025).
* Um compilador C++ moderno (ex: Visual Studio 2022 no Windows, Xcode no macOS, ou GCC/Clang no Linux).
* A aplicação **Projucer**, que vem junto com o JUCE.

---

## Como Compilar (Usando o Projucer)

Este projeto foi projetado para ser gerenciado e compilado através do Projucer e de uma IDE compatível (como o Visual Studio). **Não é necessário usar o CMake manualmente.**

1.  **Abra o Arquivo de Projeto:**
    Dê um duplo clique no arquivo `IntervalGuess.jucer` (ou o nome correspondente do seu projeto) para abri-lo no Projucer.

2.  **Configure o Exportador (Exporter):**
    * No Projucer, vá para a aba **"Exporters"** (ícone de engrenagem).
    * Selecione o exportador para a sua IDE (ex: "Visual Studio 2022").
    * Verifique se as configurações estão corretas (o Projucer geralmente cuida disso automaticamente se o seu ambiente estiver bem configurado).

3.  **Salve e Abra na IDE:**
    * Clique no botão **"Save and Open in IDE..."** na parte superior do Projucer.
    * Isso irá gerar os arquivos de projeto necessários e abrir sua IDE (Visual Studio, Xcode, etc.).

4.  **Compile na IDE:**
    * Dentro da sua IDE, simplesmente compile a solução.
    * No Visual Studio, o atalho é **`Ctrl + Shift + B`** (Build Solution).
    * No Xcode, clique no botão "Play" (Build and Run).

O executável será gerado dentro da pasta `Builds/` do seu projeto.

---

## Estrutura do Projeto

* **`Source/`**: Contém todo o código-fonte (`.cpp` e `.h`) da aplicação.
* **`design/`**: Contém os arquivos de arquitetura (cabeçalhos `.h` para a Etapa 1), o diagrama UML e outros documentos de design.

---

**Autor:** Igor Pessoa