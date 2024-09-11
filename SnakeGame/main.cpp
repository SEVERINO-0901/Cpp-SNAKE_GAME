/*
NOME DO PROJETO: SnakeGame                                                           
AUTOR DO PROJETO: Leonardo Severino  
DATA DE CRIAÇÃO: 29/08/2024 - 10:18:14
DESCRIÇÃO:
Implementação de um simples jogo da cobrinha desenvolvido em linguagem C/C++ utilizando a biblioteca ncurses. 
O jogo segue o estilo clássico onde o jogador controla uma cobra que se move pela tela para comer frutas e aumentar de tamanho, sem colidir com as bordas da tela ou com a própria cauda.
*/

//Inclusão das bibliotecas
#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>

using namespace std;

//Variáveis globais
bool gameOver; //Controle de fim de jogo
const int width = 20; //Largura da janela de jogo
const int height = 10; //Altura da janela de jogo
int x, y; //Coordenadas da cabeça da cobra
int fruitX, fruitY; //Coordenadas da fruta
int score; //Pontuação atual 
int nTail; //Comprimento da cauda da cobra
int tailX[100], tailY[100]; //Coordenadas da cauda da cobra
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN }; //Direções para as quais a cobra pode se mover
eDirection dir; //Direção atual da cobra

//Protótipo das funções
void Setup();
void Draw();
void Input();
void Logic();

int main(){ //Função 'main'
    initscr(); //Evoca 'initscr()' para inicializar a janela no modo ncurses
    cbreak(); //Evoca 'cbreak()' para desabilitar o buffer de linha
    noecho(); //Evoca 'noecho()' para que os caracteres não sejam exibidos em tela
    timeout(100); //Evoca 'timeout' de 100ms para a função 'getch()'
    keypad(stdscr, TRUE); //Evoca 'keypad()' para habilitar manuseio de teclas especiais
    Setup(); //Evoca a função 'Setup()'
    //Loop principal de jogo
    while(gameOver != true){ //Enquanto não tiver ocorrido o fim de jogo
        Draw(); //Evoca a função 'Draw'
        Input(); //Evoca a função 'Input'
        Logic(); //Evoca a função 'Logic'
        usleep(200000); //Aguarda 200ms para controlar a velocidade
    }
    endwin(); //Finaliza o modo ncurses
    
    return 0;
}

void Setup(){ //Função 'Setup()', que configura as variáveis iniciais do jogo
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw(){
    //Variáveis locais
    int i, j, k; //Contadores
    bool printTail; //Controle da impressão da cauda da cobra
    
    clear(); //Limpa a tela
    //Loop para imprimir a borda superior
    for(i = 0; i < width + 2; i++){
        printw("#");
    }
    printw("\n");
    //Loop para imprimir as linha da área de jogo
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            if(j == 0){
                printw("#"); //Imprime a borda esquerda
            }
            if(i == y && j == x){
                printw("O"); //Imprime a cabeça da cobra
            }
            else if(i == fruitY && j == fruitX){
                printw("F"); //Imprime a fruta
            }
            else{
                printTail = false;
                for(int k = 0; k < nTail; k++){
                    if(tailX[k] == j && tailY[k] == i) {
                        printw("o"); //Imprime o segmento da cauda
                        printTail = true;
                    }
                }
                if(!printTail){
                    printw(" "); //Imprime espaço vazio
                }
            }
            if(j == width - 1){
                printw("#"); //Imprime a borda direita
            }
        }
        printw("\n");
    }
    //Loop para imprimir a borda inferior
    for(int i = 0; i < width + 2; i++){
        printw("#");
    }
    printw("\n");
    printw("Score: %d\n", score); //Imprime a pontuação atual
    refresh(); //Atualiza a tela
}

void Input(){ //Função 'Input()', que processa as entradas do usuário
    //Variavéis locais
    int ch; //Caracteres inserido pelo usuário

    ch = getch(); //Lê caractere inserido pelo usuário
    switch(ch){
        case KEY_LEFT: //Caso seja o caractere correspondente a mover para a esquerda 
            if(dir != RIGHT){ //Se não estiver se movendo para a direita
                dir = LEFT; //Move para a esquerda
                break;
            }
        case KEY_RIGHT: //Caso seja o caractere correspondente a mover para a direita
            if(dir != LEFT){ //Se não estiver se movendo para esquerda
                dir = RIGHT; //Move para a direita
                break;
            }
        case KEY_UP: //Caso seja o caractere correspondente a mover para cima
            if(dir != DOWN){ //Se não estiver se movendo para baixo
                dir = UP; //Move para cima
                break;
            }
        case KEY_DOWN: //Caso seja o caractere correspondente a mover para baixo
            if(dir != UP){ //Se não estiver se movendo para cima
                dir = DOWN; //Move para baixo
                break;
            }
        case 'x': //Se o caractere for 'x'
            gameOver = true; //Encerra o jogo
            break;
    }
}

void Logic(){ //Função 'Logic()', que controla a lógica do jogo
    //Variáveis locais
    int i; //Contador
    int prevX, prevY, prev2X, prev2Y; //Posições anteriores da cabeça da cobra

    prevX = tailX[0]; //Registra a posição horizontal anterior da cabeça da cobra
    prevY = tailY[0]; //Registra a posição vertical anterior da cabeça da cobra 
    tailX[0] = x; //Atualiza a nova posição horizontal da cabeça da cobra
    tailY[0] = y; //Atualiza a nova posição vertical da cabeça da cobra
    //Loop de repetição para mover a cauda da cobra
    for(i = 1; i < nTail; i++){
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    //Move a cobra na direção atual
    switch(dir){
        case LEFT:
            x--; //Move para a esquerda
            break;
        case RIGHT:
            x++; //Move para a direita
            break;
        case UP:
            y--; //Move para cima
            break;
        case DOWN:
            y++; //Move para baixo
            break;
        default:
            break;
    }
    //Verifica se a cobra colidiu com as bordas
    if(x < 0 || x >= width || y < 0 || y >= height){ //Se sim
        gameOver = true; //Encerra o jogo
    }
    //Loop de repetição para verificar se a cobra colidiu com a própria cauda
    for(i = 0; i < nTail; i++){
        if(tailX[i] == x && tailY[i] == y){ //Se sim
            gameOver = true; //Encerra o jogo
        }
    }
    if(x == fruitX && y == fruitY){ //Se a cobra comeu a fruta
        score += 10; //Incrementa a pontuação
        //Reposiciona a fruta aleatoriamente
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++; //Aumenta o tamanho da cauda
    }
}
