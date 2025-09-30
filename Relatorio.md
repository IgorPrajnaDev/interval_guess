\# Relatório de Arquitetura: Aplicação de Piano Interativo

\*\*Autor:\*\* Igor Pessoa  
\*\*Data:\*\* 30 de Setembro de 2025  
\*\*Disciplina:\*\* Programação Orientada a Objetos (C++)

\---

\#\#\# 1\. Introdução

Este documento detalha as decisões de arquitetura e design para a Etapa 1 do projeto de uma aplicação de piano interativo, desenvolvida em C++ com o framework JUCE. O objetivo principal do design é criar uma base de código modular, extensível e que demonstre claramente a aplicação dos princípios fundamentais da Programação Orientada a Objetos e das boas práticas do C++ moderno.

A aplicação permitirá que o usuário toque piano livremente via teclado MIDI e participe de um modo de "treinamento de ouvido", no qual o sistema gera uma sequência de notas que o usuário deve reproduzir, similar a um exercício do Duolingo. A arquitetura foi pensada para separar claramente a lógica do jogo da sua representação visual e do controle de entrada do usuário.

\---

\#\#\# 2\. Decisões de Arquitetura

\#\#\#\# 2.1. Herança e Polimorfismo: O Padrão \*Strategy\* para Desafios

Um dos requisitos centrais do projeto é a capacidade de gerar diferentes desafios para o usuário. Para acomodar essa necessidade de forma flexível e extensível, foi utilizada uma hierarquia de classes baseada em herança e polimorfismo, implementando o padrão de projeto \*\*Strategy\*\*.

\* \*\*\`Challenge\` (Classe Base Abstrata):\*\* Define a interface comum para qualquer tipo de desafio. Possui métodos virtuais puros como \`generateSequence()\` e \`isCorrect()\`, forçando as classes derivadas a implementarem sua própria lógica para gerar e validar uma sequência de notas.

\* \*\*\`MelodyChallenge\` (Classe Derivada):\*\* Uma implementação concreta de \`Challenge\`, onde o objetivo é reproduzir uma melodia na ordem correta.

A classe \`GameEngine\` interage com os desafios através de um ponteiro para a classe base (\`std::unique\_ptr\<Challenge\>\`). Isso significa que o motor do jogo não precisa saber qual tipo específico de desafio está ativo. Ele simplesmente chama os métodos da interface \`Challenge\`, e o polimorfismo garante que a implementação correta seja executada. Esta abordagem torna o sistema extremamente extensível: para adicionar um novo tipo de desafio (por exemplo, um \`ChordChallenge\` para adivinhar acordes), basta criar uma nova classe que herde de \`Challenge\`, sem precisar modificar o \`GameEngine\`.

\#\#\#\# 2.2. Composição sobre Herança

A filosofia de "preferir composição sobre herança" foi um pilar central no design do sistema, resultando em um acoplamento mais baixo e maior flexibilidade.

\* \*\*\`MainComponent\` e suas partes:\*\* A classe principal da interface, \`MainComponent\`, \*\*possui um\*\* \`GameEngine\` e \*\*possui um\*\* \`VirtualKeyboardComponent\`. \`MainComponent\` não \*é um\* \`GameEngine\`; seu papel é atuar como um controlador, recebendo eventos da interface e do MIDI e delegando as ações para a lógica do jogo, que está totalmente encapsulada em \`GameEngine\`. Esta separação clara (padrão MVC simplificado) permite que a lógica do jogo e a interface evoluam de forma independente.

\* \*\*\`NoteSequence\` e \`Note\`:\*\* Uma \`NoteSequence\` \*\*é composta por\*\* uma coleção de objetos \`Note\`. Esta é uma relação natural de "todo-parte" (\`whole-part\`), onde a composição é a única escolha lógica.

A composição foi escolhida em detrimento da herança em quase todos os casos porque ela modela relações do tipo "tem um" (\`has-a\`), que são mais flexíveis e dinâmicas do que as relações rígidas do tipo "é um" (\`is-a\`) da herança.

\#\#\#\# 2.3. Gerenciamento de Recursos com Smart Pointers (RAII)

Para garantir um gerenciamento de memória seguro e automático, o projeto utiliza as práticas do C++ moderno, especificamente ponteiros inteligentes (\`smart pointers\`) e o princípio RAII (Resource Acquisition Is Initialization).

\* \*\*\`std::unique\_ptr\<Challenge\>\`:\*\* A classe \`GameEngine\` armazena seu desafio atual em um \`std::unique\_ptr\`. Isso garante que, quando um novo desafio é criado e atribuído a este ponteiro, o desafio antigo é automaticamente destruído, prevenindo vazamentos de memória. Da mesma forma, quando o objeto \`GameEngine\` é destruído, o \`unique\_ptr\` garante que o \`Challenge\` que ele possui também seja destruído. Isso elimina a necessidade de gerenciamento manual de memória com \`new\` e \`delete\`, tornando o código mais limpo, seguro e menos propenso a erros.

\---

\#\#\# 3\. Padrões e Práticas Adotadas

Além dos pontos principais, a arquitetura segue um conjunto de padrões e boas práticas:

\* \*\*Separação de Responsabilidades (MVC):\*\* O design se aproxima do padrão Model-View-Controller, com o \*\*Modelo\*\* (\`GameEngine\`, \`Challenge\`, \`Note\`), a \*\*Visão\*\* (\`VirtualKeyboardComponent\`) e o \*\*Controlador\*\* (\`MainComponent\`).

\* \*\*Uso da STL:\*\* A classe \`NoteSequence\` utiliza \`std::vector\` para armazenar as notas, aproveitando um contêiner robusto e eficiente da Standard Template Library.

\* \*\*Sobrecarga de Operadores:\*\* A classe \`Note\` sobrecarrega o \`operator==\`, permitindo uma comparação natural e legível entre duas notas, o que é fundamental para a lógica de validação dos desafios.

\---

\#\#\# 4\. Conclusão

A arquitetura proposta para a Etapa 1 estabelece uma fundação sólida e bem projetada. Através do uso estratégico de herança para polimorfismo, da preferência por composição para flexibilidade, e do gerenciamento de memória moderno com \`smart pointers\`, o design não apenas atende aos requisitos funcionais da aplicação, mas também demonstra de forma clara e justificada os principais conceitos de Programação Orientada a Objetos em C++.  
