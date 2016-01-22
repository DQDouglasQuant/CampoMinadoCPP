# Campo Minado em C++

Clone do classico Campo Minado (Minesweeper) feito em C++ com a API do Windows.

## Sobre o projeto

Todo mundo que usou Windows nos anos 90 e 2000 jogou Campo Minado. Quis recriar esse jogo classico do zero em C++ para praticar logica de programacao. Foi um dos projetos mais desafiadores porque precisei aprender sobre recursao (flood fill) e uma tecnica chamada subclassing de janelas para capturar o clique com o botao direito nos botoes.

## Como jogar

- Clique esquerdo em uma celula para revelar
- Clique direito (ou Shift + clique) para colocar ou remover uma bandeira (B)
- Celulas com numero indicam quantas minas existem ao redor
- Celulas vazias revelam automaticamente as celulas vizinhas
- Revele todas as celulas sem mina para vencer
- Se clicar em uma mina... BOOM! O jogo reinicia automaticamente

## Configuracoes do tabuleiro

- Grade: 10 x 10 celulas
- Numero de minas: 10
- Reinicio automatico ao ganhar ou perder

## Como compilar

Precisa do Dev-C++ ou MinGW instalado.

No Dev-C++ abra o `main.cpp` como projeto Win32 e compile.

Ou pelo terminal:
```
g++ -o CampoMinado.exe main.cpp -lgdi32 -luser32 -mwindows
```

Ou use o `build.bat` incluido.

## Tecnologias usadas

- C++
- Win32 API
- Recursao (flood fill para revelar celulas vazias em volta)
- Button Subclassing (tecnica Win32 para capturar clique direito em botoes)

## O que aprendi

Aprendi sobre recursao na pratica com o algoritmo flood fill que revela automaticamente as celulas vazias ao redor. Tambem aprendi uma tecnica avancada de Win32 chamada subclassing de janela, que permite interceptar mensagens de um controle para capturar o clique direito do mouse nos botoes.

---

Autor: Douglas Machado
