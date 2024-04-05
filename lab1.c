#include <stdio.h>
#include <stdlib.h>
/* Algoritmo para desenhar linhas em uma matriz e verificar linhas que
se interceptam. -- variáveis compostas e homogêneas. */
/*
Cabeçalho: Implemente um programa que:

- Receba os parâmetros de duas retas (x1 y1 x2 y2) (x3 y3 x4 y4)
- Desenhe essas duas linhas em uma matriz de [100, 100]
- Verifique se elas se interceptam, e caso se interceptem, exiba o ponto de interseção (Dentro da Matriz).
- Por fim, exiba a matriz, utilizando a função Printmatriz
*/

typedef struct ponto {
  int x, y;
} Ponto;
// y = mx + n
typedef struct reta
{
  Ponto inicio, fim;
  float coeficiente_angular; // m
  float coeficiente_linear; // n
} Reta;

int **CriaMatriz(int altura, int largura);
void DestroiMatriz(int **matriz, int altura);
void ExibeMatriz(int **matriz, int altura, int largura);
void DesenhaLinhas(int **matriz, int altura, int largura, Reta reta);
void ConfiguraCoeficientesReta(Reta *reta);
/* Retorna:
0: Se paralelas
1: Se concorrentes
2: Se coincidentes
*/
int VerificaInterseccao(Reta reta_1, Reta reta_2, Ponto *interseccao);

int main(int argc, char *argv[]) {
  Reta reta_1, reta_2;

  // Read file
  FILE *fp = fopen(argv[1], "r");
  fscanf(
    fp, "(%d %d %d %d) ",
    &reta_1.inicio.x, &reta_1.inicio.y, &reta_1.fim.x, &reta_1.fim.y
  );
  fscanf(
    fp, "(%d %d %d %d)",
    &reta_2.inicio.x, &reta_2.inicio.y, &reta_2.fim.x, &reta_2.fim.y
  );
  fclose(fp);

  int largura = 100, altura = 100;
  int **matriz = CriaMatriz(altura, largura);
  ConfiguraCoeficientesReta(&reta_1);
  ConfiguraCoeficientesReta(&reta_2);
  DesenhaLinhas(matriz, altura, largura, reta_1);
  DesenhaLinhas(matriz, altura, largura, reta_2);
  Ponto interseccao;
  int status = VerificaInterseccao(reta_1, reta_2, &interseccao);
  if (status == 0) {
    printf("Retas são paralelas\n");
  } else if (status == 1) {
    printf("Retas são concorrentes\n");
    if (
      (interseccao.x > 0 && interseccao.x < largura) &&
      (interseccao.y > 0 && interseccao.y < altura)
    ) {
      printf("Intersecção ocorre na matriz\n");
      // Marca o ponto de intersecção com 2
      matriz[99 - interseccao.y][interseccao.x] = 2;
    }
    else {
      printf("Intersecção ocorre fora da matriz\n");
    }
    printf("Intersecção: (%d, %d)\n", interseccao.x, interseccao.y);

  } else if (status == 2) {
    printf("Retas são coincidentes\n");
  }

  ExibeMatriz(matriz, altura, largura);
  DestroiMatriz(matriz, altura);

  return 0;
}

int VerificaInterseccao(Reta reta_1, Reta reta_2, Ponto *interseccao) {
    if (reta_1.inicio.x == reta_1.fim.x && reta_2.inicio.x == reta_2.fim.x) {
        // Caso em que ambas as retas são verticais e paralelas
        if (reta_1.inicio.x == reta_2.inicio.x) {
            if (reta_1.inicio.y >= reta_2.inicio.y && reta_1.inicio.y <= reta_2.fim.y) {
                *interseccao = reta_1.inicio;
                return 1; // Retas concorrentes
            } else if (reta_2.inicio.y >= reta_1.inicio.y && reta_2.inicio.y <= reta_1.fim.y) {
                *interseccao = reta_2.inicio;
                return 1; // Retas concorrentes
            } else {
                return 0; // Retas paralelas e não se interceptam
            }
        } else {
            return 0; // Retas paralelas e não se interceptam
        }
    } else if (reta_1.inicio.x == reta_1.fim.x) {
        // Calcula a coordenada x da interseção usando a equação da reta vertical
        interseccao->x = reta_1.inicio.x;
        // Calcula a coordenada y da interseção usando a equação da reta 2
        interseccao->y = reta_2.coeficiente_angular * interseccao->x + reta_2.coeficiente_linear;
        return 1; // Retas concorrentes
    }
    // Verifica se a reta_2 é vertical
    else if (reta_2.inicio.x == reta_2.fim.x) {
        // Calcula a coordenada x da interseção usando a equação da reta vertical
        interseccao->x = reta_2.inicio.x;
        // Calcula a coordenada y da interseção usando a equação da reta 1
        interseccao->y = reta_1.coeficiente_angular * interseccao->x + reta_1.coeficiente_linear;
        return 1; // Retas concorrentes
    }
    // Caso geral
    else {
        // Calcula as coordenadas x e y da interseção usando a equação das retas
        float interx = ((reta_2.coeficiente_linear - reta_1.coeficiente_linear) / (reta_1.coeficiente_angular - reta_2.coeficiente_angular));
        float intery = (reta_1.coeficiente_angular * interx) + reta_1.coeficiente_linear;
        interseccao->x = (int)(interx);
        interseccao->y = (int)(intery);
        return 1; // Retas concorrentes
    }
}

void DesenhaLinhas(int **matriz, int altura, int largura, Reta reta) {
    int x, y;

    // Verifica se a reta é vertical (não tem coeficiente angular)
    if (reta.coeficiente_angular == 0) {
        // Verifica se a reta é horizontal
        if (reta.inicio.y == reta.fim.y) {
            // Desenha uma linha horizontal
            for (x = reta.inicio.x; x <= reta.fim.x; x++) {
                if (x >= 0 && x < largura && (altura - 1 - reta.inicio.y) >= 0 && (altura - 1 - reta.inicio.y) < altura) {
                    matriz[altura - 1 - reta.inicio.y][x] = 1;
                }
            }
        }
    }
    // Verifica se a reta é vertical (paralela ao eixo y)
    else if (reta.inicio.x == reta.fim.x) {
        // Desenha uma linha vertical
        x = reta.inicio.x;
        for (y = reta.inicio.y; y <= reta.fim.y; y++) {
            if (x >= 0 && x < largura && (altura - 1 - y) >= 0 && (altura - 1 - y) < altura) {
                matriz[altura - 1 - y][x] = 1;
            }
        }
    }
    // Caso geral
    else {
        // Desenha uma linha inclinada
        for (x = reta.inicio.x; x <= reta.fim.x; x++) {
            y = reta.coeficiente_angular * x + reta.coeficiente_linear;
            if (x >= 0 && x < largura && (altura - 1 - y) >= 0 && (altura - 1 - y) < altura) {
                matriz[altura - 1 - y][x] = 1;
            }
        }
    }
}

void ConfiguraCoeficientesReta(Reta *reta) {

  float coeficiente_angular = ((float)(reta->fim.y - reta->inicio.y)) / ((float)(reta->fim.x - reta->inicio.x));
  float coeficiente_linear = reta->inicio.y - coeficiente_angular * reta->inicio.x;
  reta->coeficiente_angular = coeficiente_angular;
  reta->coeficiente_linear = coeficiente_linear;

}

void ExibeMatriz(int **matriz, int altura, int largura) {
  for (int linha = 0; linha < altura; linha++) {
    printf("[ ");
    for (int coluna = 0; coluna < largura; coluna++) {
      printf("%d ", matriz[linha][coluna]);
    }
    printf("]\n");
  }
}

int **CriaMatriz(int altura, int largura) {
  int **matriz = (int **)calloc(altura, sizeof(int *));
  // Para cada linha, aloca memória para suas colunas
  for (int linha = 0; linha < altura; linha++) {
    matriz[linha] = (int *)calloc(largura, sizeof(int));
    // Inicializa os valores da linha
    for (int coluna = 0; coluna < largura; coluna++) {
      matriz[linha][coluna] = 0;
    }
  }

  return matriz;
}

void DestroiMatriz(int **matriz, int altura) {
  if (matriz != NULL) {
    for (int i = 0; i < altura; i++) {
      if (matriz[i] != NULL) {
        free(matriz[i]);
      }
    }
    free(matriz);
  }
}