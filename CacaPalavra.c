#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nomeJogador[17];
    char palavrasJogador[50][17];
    int pontuacaoTotal;
    int pontuacaoGanha;
    int pontuacaoRecebida;
} tJogador;

typedef struct {
    char palavra[17];
    int jogada;
    int pontuacao;
} tEstatisticas;

typedef struct {
    char coor1;
    char coor2;
    char vet[100][202];
} tMapa;

int EncontraDeterminadaPalavraEstatistica(int MAX, char PalavraAchada[], tEstatisticas estatistica[]) {
    int i, j, EhIgual;
    for (i = 0; i < MAX - 1; i++) {
        for (j = 0; PalavraAchada[j]; j++) {
            if (PalavraAchada[j] == estatistica[i].palavra[j] ||
                    PalavraAchada[j] + 32 == estatistica[i].palavra[j]) {
                EhIgual = 1;
            } else {
                EhIgual = 0;
                break;
            }
        }
        if (estatistica[i].palavra[j] != '\0') EhIgual = 0;
        if (EhIgual) {
            break;
        }
    }
    return i;
}

tJogador AtualizaPontuacaoGanha(tJogador jogador) {
    jogador.pontuacaoTotal = jogador.pontuacaoTotal + 2;
    jogador.pontuacaoGanha = jogador.pontuacaoGanha + 2;
    return jogador;
}

tJogador AtualizaPontuacaoRecebida(tJogador jogador) {
    jogador.pontuacaoTotal = jogador.pontuacaoTotal + 1;
    jogador.pontuacaoRecebida = jogador.pontuacaoRecebida + 1;
    return jogador;
}

void InicializadaCoordenadasJogadas(int vet[][2], int MAX, int VALOR) {
    int i, j;
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < 2; j++) {
            vet[i][j] = VALOR + 10;
        }
    }
}

void OrganizaTabelaEstatisticas(tEstatisticas estatistica[], int TAM) {
    tEstatisticas AUX[1];
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < i; j++) {
            if (estatistica[i].pontuacao > estatistica[j].pontuacao) {
                AUX[0] = estatistica[i];
                estatistica[i] = estatistica[j];
                estatistica[j] = AUX[0];
            }
            if (estatistica[i].pontuacao == estatistica[j].pontuacao) {
                if (strcmp(estatistica[i].palavra, estatistica[j].palavra) < 0) {
                    AUX[0] = estatistica[i];
                    estatistica[i] = estatistica[j];
                    estatistica[j] = AUX[0];
                }
            }
        }
    }
}

void InicializaEstatisticas(tEstatisticas estatisticas[], int TAM) {
    for (int i = 0; i < TAM * 2; i++) {
        estatisticas[i].jogada = 0;
        estatisticas[i].pontuacao = 0;
    }
}

void OrganizaPalavrasEstatistica(tJogador J1, tJogador J2, tEstatisticas Esta[], int PalavrasDeCadaJogador) {
    int i, j = 0;
    for (i = 0; i < PalavrasDeCadaJogador; i++) {
        strcpy(Esta[i].palavra, J1.palavrasJogador[i]);
    }
    for (i; i < PalavrasDeCadaJogador * 2; i++, j++) {
        strcpy(Esta[i].palavra, J2.palavrasJogador[j]);
    }
}

void Vencedor(tJogador J1, tJogador J2, FILE* saida) {
    if (J1.pontuacaoTotal > J2.pontuacaoTotal) {
        fprintf(saida, "\n%s Ganhou!!!\n", J1.nomeJogador);
        printf("\n%s Ganhou!!!\n", J1.nomeJogador);
    }
    if (J1.pontuacaoTotal < J2.pontuacaoTotal) {
        fprintf(saida, "\n%s Ganhou!!!\n", J2.nomeJogador);
        printf("\n%s Ganhou!!!\n", J2.nomeJogador);
    }
    if (J1.pontuacaoTotal == J2.pontuacaoTotal) {
        fprintf(saida, "\nEmpate\n");
        printf("\nEmpate\n");
    }
}

tJogador EliminaPalavraTabela(char PalavraAchada[], int PalavrasDeCadaJogador, tJogador Jogador) {
    int i, condicao = 0, j;
    for (i = 0; i < PalavrasDeCadaJogador; i++) {
        for (j = 0; Jogador.palavrasJogador[i][j] != '\0'; j++) {
            if ((Jogador.palavrasJogador[i][j] == PalavraAchada[j]) ||
                    (Jogador.palavrasJogador[i][j] == PalavraAchada[j] + 32)) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            break;
        }
    }
    if (condicao) {
        for (j = i; j < PalavrasDeCadaJogador - 1; j++) {
            strcpy(Jogador.palavrasJogador[j], Jogador.palavrasJogador[j + 1]);
        }
        Jogador.palavrasJogador[j][0] = '\0';
    }
    return Jogador;
}

tMapa DDC(int coordenada1, int coordenada2, int a, char PalavraAchada[], char tabuleiro[][a], tMapa mapa) {
    int i = 0, j = 0, x = coordenada1, y = coordenada2 * 2;
    while (PalavraAchada[i]) {
        if (!i) {
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            tabuleiro[coordenada1 - i][coordenada2 + i] = PalavraAchada[j] - 32;
        } else {
            y = y + 2;
            x = x - 1;
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            if (tabuleiro[coordenada1 - i][coordenada2 + i] >= 'A' &&
                    tabuleiro[coordenada1 - i][coordenada2 + i] <= 'Z') {
                PalavraAchada[j] = PalavraAchada[j] - 32;
            }
            tabuleiro[coordenada1 - i][coordenada2 + i] = PalavraAchada[j];
        }
        i++;
        j++;
    }
    return mapa;
}

tMapa DDB(int coordenada1, int coordenada2, int a, char PalavraAchada[], char tabuleiro[][a], tMapa mapa) {
    int i = 0, j = 0, x = coordenada1, y = coordenada2 * 2;
    while (PalavraAchada[i]) {
        if (!i) {
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            tabuleiro[coordenada1 + i][coordenada2 + i] = PalavraAchada[j] - 32;
        } else {
            y = y + 2;
            x = x + 1;
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            if (tabuleiro[coordenada1 + i][coordenada2 + i] >= 'A' &&
                    tabuleiro[coordenada1 + i][coordenada2 + i] <= 'Z') {
                PalavraAchada[j] = PalavraAchada[j] - 32;
            }
            tabuleiro[coordenada1 + i][coordenada2 + i] = PalavraAchada[j];
        }
        i++;
        j++;
    }
    return mapa;
}

tMapa DEC(int coordenada1, int coordenada2, int a, char PalavraAchada[], char tabuleiro[][a], tMapa mapa) {
    int i = 0, j = 0, x = coordenada1, y = coordenada2 * 2;
    while (PalavraAchada[i]) {
        if (!i) {
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            tabuleiro[coordenada1 - i][coordenada2 - i] = PalavraAchada[j] - 32;
        } else {
            y = y - 2;
            x = x - 1;
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            if (tabuleiro[coordenada1 - i][coordenada2 - i] >= 'A' &&
                    tabuleiro[coordenada1 - i][coordenada2 - i] <= 'Z') {
                PalavraAchada[j] = PalavraAchada[j] - 32;
            }
            tabuleiro[coordenada1 - i][coordenada2 - i] = PalavraAchada[j];
        }
        i++;
        j++;
    }
    return mapa;
}

tMapa DEB(int coordenada1, int coordenada2, int a, char PalavraAchada[], char tabuleiro[][a], tMapa mapa) {
    int i = 0, j = 0, x = coordenada1, y = coordenada2 * 2;
    while (PalavraAchada[i]) {
        if (!i) {
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            tabuleiro[coordenada1 + i][coordenada2 - i] = PalavraAchada[j] - 32;
        } else {
            y = y - 2;
            x = x + 1;
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            if (tabuleiro[coordenada1 + i][coordenada2 - i] >= 'A' &&
                    tabuleiro[coordenada1 + i][coordenada2 - i] <= 'Z') {
                PalavraAchada[j] = PalavraAchada[j] - 32;
            }
            tabuleiro[coordenada1 + i][coordenada2 - i] = PalavraAchada[j];
        }
        i++;
        j++;
    }
    return mapa;
}

tMapa Direita(int coordenada1, int coordenada2, int a, char PalavraAchada[], char tabuleiro[][a], tMapa mapa) {
    int i = 0, j = 0, x = coordenada1, y = coordenada2 * 2;
    while (PalavraAchada[i]) {
        if (!i) {
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            tabuleiro[coordenada1][coordenada2 + i] = PalavraAchada[j] - 32;
        } else {
            y = y + 2;
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            if (tabuleiro[coordenada1][coordenada2 + i] >= 'A' &&
                    tabuleiro[coordenada1][coordenada2 + i] <= 'Z') {
                PalavraAchada[j] = PalavraAchada[j] - 32;
            }
            tabuleiro[coordenada1][coordenada2 + i] = PalavraAchada[j];
        }
        i++;
        j++;
    }
    return mapa;
}

tMapa Esquerda(int coordenada1, int coordenada2, int a, char PalavraAchada[], char tabuleiro[][a], tMapa mapa) {
    int i = 0, j = 0, x = coordenada1, y = coordenada2 * 2;
    while (PalavraAchada[i]) {
        if (!i) {
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            tabuleiro[coordenada1][coordenada2 - i] = PalavraAchada[j] - 32;
        } else {
            y = y - 2;
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            if (tabuleiro[coordenada1][coordenada2 - i] >= 'A' &&
                    tabuleiro[coordenada1][coordenada2 - i] <= 'Z') {
                PalavraAchada[j] = PalavraAchada[j] - 32;
            }
            tabuleiro[coordenada1][coordenada2 - i] = PalavraAchada[j];
        }
        i++;
        j++;
    }
    return mapa;
}

tMapa Cima(int coordenada1, int coordenada2, int a, char PalavraAchada[], char tabuleiro[][a], tMapa mapa) {
    int i = 0, j = 0, x = coordenada1, y = coordenada2 * 2;
    while (PalavraAchada[i]) {
        if (!i) {
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            tabuleiro[coordenada1 - i][coordenada2] = PalavraAchada[j] - 32;
        } else {
            x = x - 1;
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            if (tabuleiro[coordenada1 - i][coordenada2] >= 'A' &&
                    tabuleiro[coordenada1 - i][coordenada2] <= 'Z') {
                PalavraAchada[j] = PalavraAchada[j] - 32;
            }
            tabuleiro[coordenada1 - i][coordenada2] = PalavraAchada[j];
        }
        i++;
        j++;
    }
    return mapa;
}

tMapa Baixo(int coordenada1, int coordenada2, int a, char PalavraAchada[], char tabuleiro[][a], tMapa mapa) {
    int i = 0, j = 0, x = coordenada1, y = coordenada2 * 2;
    while (PalavraAchada[i]) {
        if (!i) {
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            tabuleiro[coordenada1 + i][coordenada2] = PalavraAchada[j] - 32;
        } else {
            x = x + 1;
            if (mapa.vet[x][y] == '-') {
                mapa.vet[x][y] = mapa.coor1;
                mapa.vet[x][y + 1] = mapa.coor2;
            }
            if (tabuleiro[coordenada1 + i][coordenada2] >= 'A' &&
                    tabuleiro[coordenada1 + i][coordenada2] <= 'Z') {
                PalavraAchada[j] = PalavraAchada[j] - 32;
            }
            tabuleiro[coordenada1 + i][coordenada2] = PalavraAchada[j];
        }
        i++;
        j++;
    }
    return mapa;
}

int ProcuraPalavraJogador(int matriz, int palavras, char Letras[matriz][matriz],
        char palavrasJogador[palavras][17], int coordenada1,
        int coordenada2, char Achada[17]) {
    char verificacao[101];
    int i = 0, j = 0, j2 = coordenada2, condicao = 0;
    //DIREITA
    while (i < matriz - coordenada2) {
        verificacao[j] = Letras[coordenada1][j2];
        j++;
        j2++;
        i++;
        verificacao[j] = '\0';
    }
    for (j = 0; j < palavras; j++) {
        for (i = 0; palavrasJogador[j][i] != '\0'; i++) {
            if (verificacao[i] == palavrasJogador[j][i]) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            strcpy(Achada, palavrasJogador[j]);
            return 1; //RETORNA 1 SE FOR A PALAVRA PARA DIREITA
        }
    }
    //ESQUERDA
    j = 0, i = 0, j2 = coordenada2;
    while (i < coordenada2 + 1) {
        verificacao[j] = Letras[coordenada1][j2];
        j++;
        j2--;
        i++;
        verificacao[j] = '\0';
    }
    for (j = 0; j < palavras; j++) {
        for (i = 0; palavrasJogador[j][i] != '\0'; i++) {
            if (verificacao[i] == palavrasJogador[j][i]) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            strcpy(Achada, palavrasJogador[j]);
            return 2; //RETORNA 2 SE FOR A PALAVRA PARA ESQUERDA 
        }
    }
    //CIMA
    j = 0, i = 0, j2 = coordenada1;
    while (i < coordenada1 + 1) {
        verificacao[j] = Letras[j2][coordenada2];
        j++;
        j2--;
        i++;
        verificacao[j] = '\0';
    }
    for (j = 0; j < palavras; j++) {
        for (i = 0; palavrasJogador[j][i] != '\0'; i++) {
            if (verificacao[i] == palavrasJogador[j][i]) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            strcpy(Achada, palavrasJogador[j]);
            return 3; //RETORNA 3 SE FOR A PALAVRA PARA CIMA
        }
    }
    //BAIXO
    j = 0, i = 0, j2 = coordenada1;
    while (i < matriz - coordenada1) {
        verificacao[j] = Letras[j2][coordenada2];
        j++;
        j2++;
        i++;
        verificacao[j] = '\0';
    }

    for (j = 0; j < palavras; j++) {
        for (i = 0; palavrasJogador[j][i] != '\0'; i++) {
            if (verificacao[i] == palavrasJogador[j][i]) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            strcpy(Achada, palavrasJogador[j]);
            return 4; //RETORNA 4 SE FOR A PALAVRA PARA BAIXO
        }
    }

    //DIAGONAL DIREITA CIMA
    int j3 = coordenada2;
    j = 0, i = 0, j2 = coordenada1;
    while (j2 > -1 && j3 < matriz) {
        verificacao[j] = Letras[j2][j3];
        j++;
        j3++;
        j2--;
        i++;
        verificacao[j] = '\0';
    }
    for (j = 0; j < palavras; j++) {
        for (i = 0; palavrasJogador[j][i] != '\0'; i++) {
            if (verificacao[i] == palavrasJogador[j][i]) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            strcpy(Achada, palavrasJogador[j]);
            return 5; //RETORNA 5 SE FOR A PALAVRA PARA DIAGONAL DIREITA CIMA
        }
    }
    //DIAGONAL DIREITA BAIXO
    j3 = coordenada2;
    j = 0, i = 0, j2 = coordenada1;
    while (j2 < matriz && j3 < matriz) {
        verificacao[j] = Letras[j2][j3];
        j++;
        j3++;
        j2++;
        i++;
        verificacao[j] = '\0';
    }
    for (j = 0; j < palavras; j++) {
        for (i = 0; palavrasJogador[j][i] != '\0'; i++) {
            if (verificacao[i] == palavrasJogador[j][i]) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            strcpy(Achada, palavrasJogador[j]);
            return 6; //RETORNA 6 SE FOR A PALAVRA PARA DIAGONAL DIREITA BAIXO
        }
    }
    //DIAGONAL ESQUERDA CIMA
    j3 = coordenada2;
    j = 0, i = 0, j2 = coordenada1;
    while (j2 > -1 && j3 > -1) {
        verificacao[j] = Letras[j2][j3];
        j++;
        j3--;
        j2--;
        i++;
        verificacao[j] = '\0';
    }
    for (j = 0; j < palavras; j++) {
        for (i = 0; palavrasJogador[j][i] != '\0'; i++) {
            if (verificacao[i] == palavrasJogador[j][i]) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            strcpy(Achada, palavrasJogador[j]);
            return 7; //RETORNA 7 SE FOR A PALAVRA PARA DIAGONAL ESQUERDA CIMA
        }
    }
    //DIAGONAL ESQUERDA BAIXO
    j3 = coordenada2;
    j = 0, i = 0, j2 = coordenada1;
    while (j2 < matriz && j3 > -1) {
        verificacao[j] = Letras[j2][j3];
        j++;
        j3--;
        j2++;
        i++;
        verificacao[j] = '\0';
    }
    for (j = 0; j < palavras; j++) {
        for (i = 0; palavrasJogador[j][i] != '\0'; i++) {
            if (verificacao[i] == palavrasJogador[j][i]) {
                condicao = 1;
            } else {
                condicao = 0;
                break;
            }
        }
        if (condicao) {
            strcpy(Achada, palavrasJogador[j]);
            return 8; //RETORNA 8 SE FOR A PALAVRA PARA DIAGONAL ESQUERDA BAIXO
        }
    }
    return 0;
}
//Solicita o nome dos jogadores e os armazena 

void NomeDosJogadores(char J1[], char J2[], FILE*arquivo) {
    fprintf(arquivo, "Nome do Jogador 1:\n");
    printf("Nome do Jogador 1:\n");
    scanf("%s", J1);
    fprintf(arquivo, "Nome do Jogador 2:\n");
    printf("Nome do Jogador 2:\n");
    scanf("%s", J2);
}
//Conta o numero de letras dos nomes dos jogadores

int ContaLetrasNome(char vet[]) {
    int i = 0;
    while (vet[i]) {
        i++;
    }
    return i;
}

//Conta o numero de letras das palavras de cada jogador

int ContaLetras(int i, char PalavraJogador[][17]) {
    int j = 0;
    while (PalavraJogador[i][j]) {
        j++;
    }
    return j;
}

//Acha a maior palavra de cada jogador

int MaiorPalavra(int MAX, char vet[MAX][17]) {
    int i = 0, cont = 0, j = 0, MAIOR = 0, ID;

    while (j < MAX) {
        i = 0;
        cont = 0;
        while (vet[j][i]) {
            cont++;
            i++;
        }
        if (cont > MAIOR) {
            ID = j;
            MAIOR = cont;
        }
        j++;
    }
    return ID;
}

//Imprime as letras iniciais no arquivo de inicializacao

void ImprimeLetrasIniciais(int MAX, char aux[], FILE*Inicializacao) {
    int condicao, i, cont;
    char letra = '\0';
    for (condicao = 0; letra != 'z'; condicao++) {
        letra = 'a' + condicao;
        for (i = 0, cont = 0; i < MAX; i++) {
            if (aux[i] == letra) {
                cont++;
            }
        }
        if (cont != 0) {
            fprintf(Inicializacao, "%c -> %d\n", letra, cont);
            
        }
    }
}

void ImprimePalavrasInicializacao(int MAX, tJogador jogador, FILE*arquivo) {
    for (int i = 0; i < MAX; i++) {
        fprintf(arquivo, "%s\n", jogador.palavrasJogador[i]);
    }
}

void InicializaLetrasIniciais(int MAX, tJogador jogador, char aux[]) {
    for (int i = 0; i < MAX; i++) {
        aux[i] = jogador.palavrasJogador[i][0];
    }
}
//Nessa função gera todo arquivo de inicialização 

void arquivoInicializacao(int MAX, tJogador Jogador1, tJogador Jogador2, char InicializacaoTxt[]) {
    FILE* Inicializacao = fopen(InicializacaoTxt, "w");
    int i;
    char aux[MAX];
    fprintf(Inicializacao, "--Jogador 1--\nNome: %s\nPalavras:\n", Jogador1.nomeJogador);
    ImprimePalavrasInicializacao(MAX, Jogador1, Inicializacao);
    fprintf(Inicializacao, "Maior Palavra:\n%s\n", Jogador1.palavrasJogador[MaiorPalavra(MAX, Jogador1.palavrasJogador)]);
    fprintf(Inicializacao, "Letras Iniciais:\n");
    InicializaLetrasIniciais(MAX, Jogador1, aux);
    ImprimeLetrasIniciais(MAX, aux, Inicializacao);

    fprintf(Inicializacao, "\n--Jogador 2--\nNome: %s\nPalavras:\n", Jogador2.nomeJogador);
    ImprimePalavrasInicializacao(MAX, Jogador2, Inicializacao);
    fprintf(Inicializacao, "Maior Palavra:\n%s\n", Jogador2.palavrasJogador[MaiorPalavra(MAX, Jogador2.palavrasJogador)]);
    fprintf(Inicializacao, "Letras Iniciais:\n");
    InicializaLetrasIniciais(MAX, Jogador2, aux);
    ImprimeLetrasIniciais(MAX, aux, Inicializacao);
    fclose(Inicializacao);
}

//Preenche a matriz de letras, ou seja, o tabuleiro com as palavras embaralhadas

void PreencheMatriz(int Matriz, char Letras[][Matriz], FILE* config) {
    int i, j;
    for (i = 0; i < Matriz; i++) {
        for (j = 0; j < Matriz; j++) {
            fscanf(config, "%c", &Letras[i][j]);
        }
        fscanf(config, "\n");
    }
}
//Trata o caso de erro quando o arquivo não é inserido  ou não existe

int CasoDeERRO(int a) {
    if (a <= 1) { //Caso o usuário não informar o arquivo de configuração
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 1;
    }
    return 0;
}
int CasoDeERRO2(char *b[], FILE* config) {
    if (config == NULL) { //Caso o usuário usasse um arquivo diferente do config.txt
        printf("Arquivo invalido! O arquivo lido foi %s\n", b[1]);
        return 1;
    }
    return 0;
}
//Preenche matriz das palavras de cada jogador

void preenchePalavrasJogador(FILE* config, char palavrasJogador[][17], int PalavrasDeCadaJogador) {
    char vet[PalavrasDeCadaJogador][17];
    int i;

    for (i = 0; i < PalavrasDeCadaJogador; i++) {
        fscanf(config, "%s\n", vet[i]);
        strcpy(palavrasJogador[i], vet[i]);
    }
}
//Imprime na tela a tabela do jogo, com as palavras, nome dos jogadores e suas pontuações

void ApresentaTabelaJogo(int PalavrasDeCadaJogador, tJogador Jogador1, tJogador Jogador2, FILE * arquivo) {
    int condicao = 0, i = 0;
    fprintf(arquivo, "|            Palavras Restantes           |");
    printf("|            Palavras Restantes           |");
    int LetrasNome1 = ContaLetrasNome(Jogador1.nomeJogador);
    int LetrasNome2 = ContaLetrasNome(Jogador2.nomeJogador);
    fprintf(arquivo, "\n|%s", Jogador1.nomeJogador);
    printf("\n|%s", Jogador1.nomeJogador);
    while (condicao < 16 - LetrasNome1) {
        condicao++;
        fprintf(arquivo, " ");
        printf(" ");
    }
    condicao = 0;
    fprintf(arquivo, "(%.2d)|%s", Jogador1.pontuacaoTotal, Jogador2.nomeJogador);
    printf("(%.2d)|%s", Jogador1.pontuacaoTotal, Jogador2.nomeJogador);
    while (condicao < 16 - LetrasNome2) {
        condicao++;
        fprintf(arquivo, " ");
        printf(" ");
    }
    fprintf(arquivo, "(%.2d)|", Jogador2.pontuacaoTotal);
    fprintf(arquivo, "\n|--------------------|--------------------|");
    printf("(%.2d)|", Jogador2.pontuacaoTotal);
    printf("\n|--------------------|--------------------|");

    while (i < PalavrasDeCadaJogador) {

        condicao = 0;
        int letras1 = ContaLetras(i, Jogador1.palavrasJogador);
        int letras2 = ContaLetras(i, Jogador2.palavrasJogador);

        if (Jogador1.palavrasJogador[i][0] != '\0' || Jogador2.palavrasJogador[i][0] != '\0') {
            fprintf(arquivo, "\n");
            fprintf(arquivo, "|%s", Jogador1.palavrasJogador[i]);
            printf("\n");
            printf("|%s", Jogador1.palavrasJogador[i]);
            while (condicao < 20 - letras1) {
                condicao++;
                fprintf(arquivo, " ");
                printf(" ");
            }

            fprintf(arquivo, "|");
            fprintf(arquivo, "%s", Jogador2.palavrasJogador[i]);
            printf("|");
            printf("%s", Jogador2.palavrasJogador[i]);

            condicao = 0;
            while (condicao < 20 - letras2) {
                condicao++;
                fprintf(arquivo, " ");
                printf(" ");
            }
            fprintf(arquivo, "|");
            printf("|");

        }
        i++;
    }
    fprintf(arquivo, "\n");
    printf("\n");
}
//Imprime o tabuleiro com os '-' ou com as letras ja jogadas

void ImprimeTabuleiro(int Matriz, char vet[Matriz][Matriz], FILE* arquivo) {
    int i, j;

    fprintf(arquivo, "   ");
    printf("   ");
    for (i = 0; i < Matriz; i++) {
        fprintf(arquivo, " %.2d", i);
        printf(" %.2d", i);
    }
    fprintf(arquivo, "\n");
    printf("\n");

    for (i = 0; i < Matriz; i++) {
        fprintf(arquivo, "%.2d|", i);
        printf("%.2d|", i);
        for (j = 0; j < Matriz; j++) {
            fprintf(arquivo, " %c ", vet[i][j]);
            printf(" %c ", vet[i][j]);
        }
        fprintf(arquivo, "|\n");
        printf("|\n");
    }
}
//Faz a analise de coordenadas para ver se ja foram jogadas

int AnaliseJogadas(int X, int Y, int vet[][2], int Z) {
    int i = 0;
    while (i < Z - 1) {
        if (X == vet[i][0] && Y == vet[i][1]) {
            return 1;
        }
        i++;
    }
    return 0;
}
//Faz a contagem das letras da Estatisticas

int ContaLetrasEstatistica(char Palavra[17]) {
    int j = 0;
    while (Palavra[j]) {
        j++;
    }
    return j;
}
//Gera o arquivo de Estatisticas.txt 

void Estatisticas(tJogador J1, tJogador J2, tEstatisticas E[], int TAM, char EstatisticaSaida[]) {
    FILE* estatistica = fopen(EstatisticaSaida, "w");
    int i, condicao = 0;
    fprintf(estatistica, "--------------\n");
    fprintf(estatistica, "--- PONTOS ---\n");
    fprintf(estatistica, "--------------\n\n");
    fprintf(estatistica, "%s\n", J1.nomeJogador);
    fprintf(estatistica, "Feitos: %d\n", J1.pontuacaoGanha);
    fprintf(estatistica, "Recebidos: %d\n", J1.pontuacaoRecebida);
    fprintf(estatistica, "Total: %d\n\n", J1.pontuacaoTotal);
    fprintf(estatistica, "%s\n", J2.nomeJogador);
    fprintf(estatistica, "Feitos: %d\n", J2.pontuacaoGanha);
    fprintf(estatistica, "Recebidos: %d\n", J2.pontuacaoRecebida);
    fprintf(estatistica, "Total: %d\n\n", J2.pontuacaoTotal);
    fprintf(estatistica, "---------------------------\n");
    fprintf(estatistica, "--- PALAVRAS POR PONTOS ---\n");
    fprintf(estatistica, "---------------------------\n\n");
    fprintf(estatistica, "|P|     Palavra    | T |\n");
    fprintf(estatistica, "|-|----------------|---|\n");
    for (i = 0; i < TAM * 2; i++) {
        condicao = 0;
        fprintf(estatistica, "|%d|", E[i].pontuacao);
        fprintf(estatistica, "%s", E[i].palavra);
        int letras = ContaLetrasEstatistica(E[i].palavra);
        while (condicao < 16 - letras) {
            condicao++;
            fprintf(estatistica, " ");
        }
        fprintf(estatistica, "|%.3d|\n", E[i].jogada);
    }
    fclose(estatistica);
}
//Inicializa as pontuações para não pegar valor de lixo

tJogador InicializaPontuacao() {
    tJogador jogador;
    jogador.pontuacaoTotal = 0;
    jogador.pontuacaoGanha = 0;
    jogador.pontuacaoRecebida = 0;
    return jogador;
}
//Inicializa o tabuleiro com o '-'

void PreencheTabuleiro(int TAM, char tabuleiro[][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = '-';
        }
    }
}
//Inicializa o mapa ordenado com o '-'

tMapa PreencheMapa(int TAM) {
    tMapa mapa;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM * 2; j++) {
            mapa.vet[i][j] = '-';
        }
    }
    mapa.coor1 = '0';
    mapa.coor2 = '0';
    return mapa;
}

void ImprimeMapaOrdenado(int TAM, tMapa mapa, char MapaOrdenadoSaida[]) {
    FILE* Mapa = fopen(MapaOrdenadoSaida, "w");
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM * 2; j++) {
            fprintf(Mapa, " %c", mapa.vet[i][j]);
            j++;
            fprintf(Mapa, "%c", mapa.vet[i][j]);
        }
        fprintf(Mapa, "\n");
    }
    fclose(Mapa);
}

void ImprimeJogo(int NumeroPalavras, tJogador J1, tJogador J2, int TAM, char tabuleiro[][TAM], FILE*saida) {
    ApresentaTabelaJogo(NumeroPalavras, J1, J2, saida);
    fprintf(saida, "\n");
    printf("\n");
    ImprimeTabuleiro(TAM, tabuleiro, saida);
}

tMapa AtribuiPalavraTabuleiro(int C1, int C2, int TAM, char Palavra[], char tabuleiro[][TAM], int retorno, tMapa mapa) {
    if (retorno == 1) {
        mapa = Direita(C1, C2, TAM, Palavra, tabuleiro, mapa);
    }
    if (retorno == 2) {
        mapa = Esquerda(C1, C2, TAM, Palavra, tabuleiro, mapa);
    }
    if (retorno == 3) {
        mapa = Cima(C1, C2, TAM, Palavra, tabuleiro, mapa);
    }
    if (retorno == 4) {
        mapa = Baixo(C1, C2, TAM, Palavra, tabuleiro, mapa);
    }
    if (retorno == 5) {
        mapa = DDC(C1, C2, TAM, Palavra, tabuleiro, mapa);
    }
    if (retorno == 6) {
        mapa = DDB(C1, C2, TAM, Palavra, tabuleiro, mapa);
    }
    if (retorno == 7) {
        mapa = DEC(C1, C2, TAM, Palavra, tabuleiro, mapa);
    }
    if (retorno == 8) {
        mapa = DEB(C1, C2, TAM, Palavra, tabuleiro, mapa);
    }
    return mapa;
}

void concatenaConfig(char *argv[], char entrada[]) {
    strncpy(entrada, argv[1], strlen(argv[1]));
    int i;
    for (i = 0; entrada[i] != '\0'; i++) {
    }
    
    entrada[i] = '/';
    entrada[i + 1] = 'c';
    entrada[i + 2] = 'o';
    entrada[i + 3] = 'n';
    entrada[i + 4] = 'f';
    entrada[i + 5] = 'i';
    entrada[i + 6] = 'g';
    entrada[i + 7] = '.';
    entrada[i + 8] = 't';
    entrada[i + 9] = 'x';
    entrada[i + 10] = 't';
    entrada[i + 11] = '\0';
}

void Concatenar(char *argv[], char vet[], char preenche[]) {
    strcpy(vet, argv[1]);
    int i, j;
    for (i = 0; vet[i] != '\0'; i++) {
    }
    vet[i] = '/';
    i++;
    for (j = 0; preenche[j] != '\0'; i++) {
        vet[i] = preenche[j];
        j++;
    }
    vet[i] = '\0';
}

int main(int argc, char *argv[]) {
	if (CasoDeERRO(argc)) {
        return 1;
    }//Retorna 1 caso o programa encontre algum erro de arquivos
    char entrada[1000], MapaOrdenadoSaida[1000];
    char SaidaDaSaida[1000], EstatisticaSaida[1000], InicializacaoSaida[1000];
    char SaidaTxt[16] = {'s', 'a', 'i', 'd', 'a', '/', 's', 'a', 'i', 'd', 'a', '.', 't', 'x', 't', '\0'};
    char EstatisticasTxt[23] = {'s', 'a', 'i', 'd', 'a', '/', 'E', 's', 't', 'a',
        't', 'i', 's', 't', 'i', 'c', 'a', 's', '.', 't', 'x', 't', '\0'};
    char MapaOrdenadoTxt[23] = {'s', 'a', 'i', 'd', 'a', '/', 'M', 'a', 'p', 'a',
        'O', 'r', 'd', 'e', 'n', 'a', 'd', 'o', '.', 't', 'x', 't', '\0'};
    char InicializacaoTxt[24] = {'s', 'a', 'i', 'd', 'a', '/', 'I', 'n', 'i',
        'c', 'i', 'a', 'l', 'i', 'z', 'a', 'c', 'a', 'o', '.', 't', 'x', 't', '\0'};
    concatenaConfig(argv, entrada);
    Concatenar(argv, SaidaDaSaida, SaidaTxt);
    Concatenar(argv, EstatisticaSaida, EstatisticasTxt);
    Concatenar(argv, InicializacaoSaida, InicializacaoTxt);
    Concatenar(argv, MapaOrdenadoSaida, MapaOrdenadoTxt);
    FILE* config = fopen(entrada, "r"); //abre um arquivo
    if (CasoDeERRO2(argv, config)) {
        return 1;
    }

    tJogador JogadorUm; //Estrutura do tipo jogador para o jogador um
    tJogador JogadorDois; //Estrutura do tipo jogador para o jogador dois
    tMapa mapa; //Estrutura do tipo mapa para o mapa de coordenadas
    JogadorUm = InicializaPontuacao();
    JogadorDois = InicializaPontuacao();

    int Jogadas, Matriz, i = 0, condicao = 0, retorno;
    char PalavraAchada[17];
    int AnaliseLetra = 0, coordenada1, coordenada2;
    int PalavrasDeCadaJogador, LOL, C, EhIgual, condicaoJogadas = 0;
    int CondicaoFinalJogo = 0, CondicaoFinalJogo2 = 0, condicaoJogadas2 = 0;

    fscanf(config, "%d%d\n", &Jogadas, &Matriz); //Leitura do numero de Jogadas de cada jogador e tamanho matriz
    int CoordenadasJogadas[2 * Jogadas][2];
    char tabuleiro[Matriz][Matriz], Letras[Matriz][Matriz];
    mapa.vet[Matriz][Matriz * 2];
    PreencheMatriz(Matriz, Letras, config);
    mapa = PreencheMapa(Matriz);
    fscanf(config, "%d", &PalavrasDeCadaJogador);

    tEstatisticas estatistica[PalavrasDeCadaJogador * 2];
    InicializaEstatisticas(estatistica, PalavrasDeCadaJogador);

    JogadorUm.palavrasJogador [PalavrasDeCadaJogador][17];
    JogadorDois.palavrasJogador [PalavrasDeCadaJogador][17];

    fscanf(config, "\n");
    preenchePalavrasJogador(config, JogadorUm.palavrasJogador, PalavrasDeCadaJogador); //PRIMEIRO JOGADOR
    preenchePalavrasJogador(config, JogadorDois.palavrasJogador, PalavrasDeCadaJogador); //SEGUNDO JOGADOR
    OrganizaPalavrasEstatistica(JogadorUm, JogadorDois, estatistica, PalavrasDeCadaJogador);

    fclose(config); //Fecha o arquivo de configuracao

    FILE* saida = fopen(SaidaDaSaida, "w"); //Abre o arquivo de saida

    NomeDosJogadores(JogadorUm.nomeJogador, JogadorDois.nomeJogador, saida);
    arquivoInicializacao(PalavrasDeCadaJogador, JogadorUm, JogadorDois, InicializacaoSaida);
    PreencheTabuleiro(Matriz, tabuleiro);
    ImprimeJogo(PalavrasDeCadaJogador, JogadorUm, JogadorDois, Matriz, tabuleiro, saida);
    InicializadaCoordenadasJogadas(CoordenadasJogadas, Jogadas * 2, Matriz);

    //AQUI COMEÇA O JOGO EM SI E DA FIM A TODAS AS CONFIGURAÇÕES ORIGINAIS
    while (condicaoJogadas < Jogadas) {
        condicaoJogadas++;
        condicaoJogadas2++;

        fprintf(saida, "\n%s por favor entre com as coordenadas para a sua jogada:\n", JogadorUm.nomeJogador);
        printf("\n%s por favor entre com as coordenadas para a sua jogada:\n", JogadorUm.nomeJogador);
        while (1) {
            scanf("%d %d", &coordenada1, &coordenada2);
            if (coordenada1 >= Matriz || coordenada2 >= Matriz || coordenada1 < 0 || coordenada2 < 0) {
                ImprimeJogo(PalavrasDeCadaJogador, JogadorUm, JogadorDois, Matriz, tabuleiro, saida);
                fprintf(saida, "\nCoordenadas fora do tabuleiro.%s por favor entre com novas coordenadas para a sua jogada:\n", JogadorUm.nomeJogador);
                printf("\nCoordenadas fora do tabuleiro.%s por favor entre com novas coordenadas para a sua jogada:\n", JogadorUm.nomeJogador);
                continue;
            } else {
                if (tabuleiro[coordenada1][coordenada2] >= 'A' && tabuleiro[coordenada1][coordenada2] <= 'Z') {
                    ImprimeJogo(PalavrasDeCadaJogador, JogadorUm, JogadorDois, Matriz, tabuleiro, saida);
                    fprintf(saida, "\nCoordenadas já jogadas.%s por favor entre com novas coordenadas para a sua jogada:\n", JogadorUm.nomeJogador);
                    printf("\nCoordenadas já jogadas.%s por favor entre com novas coordenadas para a sua jogada:\n", JogadorUm.nomeJogador);
                    continue;
                } else {
                    break;
                }
            }
        }
        CoordenadasJogadas[i][0] = coordenada1;
        CoordenadasJogadas[i][1] = coordenada2;
        i++;
        condicao = 0;

        while (1) {
            if (Letras[coordenada1][coordenada2] == JogadorUm.palavrasJogador[condicao][0] ||
                    Letras[coordenada1][coordenada2] == JogadorDois.palavrasJogador[condicao][0]) {
                AnaliseLetra = 1;
                break;
            }
            condicao++;
            if (condicao == PalavrasDeCadaJogador) {
                AnaliseLetra = 0;
                break;
            }
        }

        if (AnaliseLetra == 0) {
            tabuleiro[coordenada1][coordenada2] = Letras[coordenada1][coordenada2] - 32;
        } else {
            retorno = ProcuraPalavraJogador(Matriz, PalavrasDeCadaJogador, Letras, JogadorUm.palavrasJogador, coordenada1, coordenada2, PalavraAchada);
            if (retorno != 0) {
                mapa.coor2++;
                if (mapa.coor2 > '9') {
                    mapa.coor1++;
                    mapa.coor2 = '0';
                }
            }
            mapa = AtribuiPalavraTabuleiro(coordenada1, coordenada2, Matriz, PalavraAchada, tabuleiro, retorno, mapa);
            if (retorno != 0) {
                LOL = EncontraDeterminadaPalavraEstatistica(PalavrasDeCadaJogador * 2, PalavraAchada, estatistica);
                estatistica[LOL].pontuacao = 2;
                estatistica[LOL].jogada = condicaoJogadas2;
                JogadorUm = EliminaPalavraTabela(PalavraAchada, PalavrasDeCadaJogador, JogadorUm);
                JogadorUm = AtualizaPontuacaoGanha(JogadorUm);
                CondicaoFinalJogo++;

            }
            if (retorno == 0) {
                retorno = ProcuraPalavraJogador(Matriz, PalavrasDeCadaJogador, Letras, JogadorDois.palavrasJogador, coordenada1, coordenada2, PalavraAchada);
                if (retorno != 0) {
                    mapa.coor2++;
                    if (mapa.coor2 > '9') {
                        mapa.coor1++;
                        mapa.coor2 = '0';
                    }
                }
                mapa = AtribuiPalavraTabuleiro(coordenada1, coordenada2, Matriz, PalavraAchada, tabuleiro, retorno, mapa);
                if (retorno != 0) {
                    LOL = EncontraDeterminadaPalavraEstatistica(PalavrasDeCadaJogador * 2, PalavraAchada, estatistica);
                    estatistica[LOL].pontuacao = 1;
                    estatistica[LOL].jogada = condicaoJogadas2;
                    JogadorDois = EliminaPalavraTabela(PalavraAchada, PalavrasDeCadaJogador, JogadorDois);
                    JogadorDois = AtualizaPontuacaoRecebida(JogadorDois);
                    CondicaoFinalJogo2++;
                }
                if (retorno == 0) {
                    tabuleiro[coordenada1][coordenada2] = Letras[coordenada1][coordenada2] - 32;
                }
            }
        }
        ImprimeJogo(PalavrasDeCadaJogador, JogadorUm, JogadorDois, Matriz, tabuleiro, saida);
        if (CondicaoFinalJogo == PalavrasDeCadaJogador || CondicaoFinalJogo2 == PalavrasDeCadaJogador) {
            break;
        }

        //REPETE AÇÃO PARA O SEGUNDO JOGADOR ------------------------------------------------------------------------------------------

        condicaoJogadas2++;
        fprintf(saida, "\n%s por favor entre com as coordenadas para a sua jogada:\n", JogadorDois.nomeJogador);
        printf("\n%s por favor entre com as coordenadas para a sua jogada:\n", JogadorDois.nomeJogador);
        while (1) {
            scanf("%d %d", &coordenada1, &coordenada2);
            if (coordenada1 >= Matriz || coordenada2 >= Matriz || coordenada1 < 0 || coordenada2 < 0) {
                ImprimeJogo(PalavrasDeCadaJogador, JogadorUm, JogadorDois, Matriz, tabuleiro, saida);
                fprintf(saida, "\nCoordenadas fora do tabuleiro.%s por favor entre com novas coordenadas para a sua jogada:\n", JogadorDois.nomeJogador);
                printf("\nCoordenadas fora do tabuleiro.%s por favor entre com novas coordenadas para a sua jogada:\n", JogadorDois.nomeJogador);
                continue;
            } else {
                if (tabuleiro[coordenada1][coordenada2] >= 'A' && tabuleiro[coordenada1][coordenada2] <= 'Z') {
                    ImprimeJogo(PalavrasDeCadaJogador, JogadorUm, JogadorDois, Matriz, tabuleiro, saida);
                    fprintf(saida, "\nCoordenadas já jogadas.%s por favor entre com novas coordenadas para a sua jogada:\n", JogadorDois.nomeJogador);
                    printf("\nCoordenadas já jogadas.%s por favor entre com novas coordenadas para a sua jogada:\n", JogadorDois.nomeJogador);
                    continue;
                } else {
                    break;
                }
            }
        }
        CoordenadasJogadas[i][0] = coordenada1;
        CoordenadasJogadas[i][1] = coordenada2;
        i++;
        condicao = 0;
        AnaliseLetra = 0;
        while (1) {
            if (Letras[coordenada1][coordenada2] == JogadorDois.palavrasJogador[condicao][0] || Letras[coordenada1][coordenada2] == JogadorUm.palavrasJogador[condicao][0]) {
                AnaliseLetra = 1;
                break;
            }
            condicao++;
            if (condicao == PalavrasDeCadaJogador) {
                AnaliseLetra = 0;
                break;
            }
        }
        if (AnaliseLetra == 0) {
            tabuleiro[coordenada1][coordenada2] = Letras[coordenada1][coordenada2] - 32;
        } else {

            retorno = ProcuraPalavraJogador(Matriz, PalavrasDeCadaJogador, Letras, JogadorUm.palavrasJogador, coordenada1, coordenada2, PalavraAchada);
            if (retorno != 0) {
                mapa.coor2++;
                if (mapa.coor2 > '9') {
                    mapa.coor1++;
                    mapa.coor2 = '0';
                }
            }
            mapa = AtribuiPalavraTabuleiro(coordenada1, coordenada2, Matriz, PalavraAchada, tabuleiro, retorno, mapa);
            if (retorno != 0) {
                LOL = EncontraDeterminadaPalavraEstatistica(PalavrasDeCadaJogador * 2, PalavraAchada, estatistica);
                estatistica[LOL].pontuacao = 1;
                estatistica[LOL].jogada = condicaoJogadas2;
                JogadorUm = EliminaPalavraTabela(PalavraAchada, PalavrasDeCadaJogador, JogadorUm);
                JogadorUm = AtualizaPontuacaoRecebida(JogadorUm);
                CondicaoFinalJogo++;
            }
            if (retorno == 0) {
                retorno = ProcuraPalavraJogador(Matriz, PalavrasDeCadaJogador, Letras, JogadorDois.palavrasJogador, coordenada1, coordenada2, PalavraAchada);
                if (retorno != 0) {
                    mapa.coor2++;
                    if (mapa.coor2 > '9') {
                        mapa.coor1++;
                        mapa.coor2 = '0';
                    }
                }
                mapa = AtribuiPalavraTabuleiro(coordenada1, coordenada2, Matriz, PalavraAchada, tabuleiro, retorno, mapa);
                if (retorno != 0) {
                    LOL = EncontraDeterminadaPalavraEstatistica(PalavrasDeCadaJogador * 2, PalavraAchada, estatistica);
                    estatistica[LOL].pontuacao = 2;
                    estatistica[LOL].jogada = condicaoJogadas2;
                    JogadorDois = EliminaPalavraTabela(PalavraAchada, PalavrasDeCadaJogador, JogadorDois);
                    JogadorDois = AtualizaPontuacaoGanha(JogadorDois);
                    CondicaoFinalJogo2++;
                }
                if (retorno == 0) {
                    tabuleiro[coordenada1][coordenada2] = Letras[coordenada1][coordenada2] - 32;
                }
            }
        }
        ImprimeJogo(PalavrasDeCadaJogador, JogadorUm, JogadorDois, Matriz, tabuleiro, saida);
        if (CondicaoFinalJogo == PalavrasDeCadaJogador || CondicaoFinalJogo2 == PalavrasDeCadaJogador) {
            break;
        }
    }
    Vencedor(JogadorUm, JogadorDois, saida);
    fclose(saida);
    OrganizaTabelaEstatisticas(estatistica, PalavrasDeCadaJogador * 2);
    Estatisticas(JogadorUm, JogadorDois, estatistica, PalavrasDeCadaJogador, EstatisticaSaida);
    ImprimeMapaOrdenado(Matriz, mapa, MapaOrdenadoSaida);
    return 0;
}
