#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Estrutura do nó
typedef struct NO {
    int info;
    int altura; // altura da subárvore
    struct NO *esq;
    struct NO *dir;
} NO;

typedef NO* AVLTree;

// Função para obter altura de um nó
int altura_NO(NO* no) {
    if (no == NULL)
        return -1;
    else
        return no->altura;
}

// Função para retornar o maior de dois valores
int maior(int a, int b) {
    return (a > b) ? a : b;
}

// Cálculo do fator de balanceamento
int fatorBalanceamento_NO(NO* no) {
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

// Rotações
void RotacaoLL(AVLTree* raiz) {
    NO* no = (*raiz)->esq;
    (*raiz)->esq = no->dir;
    no->dir = *raiz;
    (*raiz)->altura = maior(altura_NO((*raiz)->esq), altura_NO((*raiz)->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), (*raiz)->altura) + 1;
    *raiz = no;
}

void RotacaoRR(AVLTree* raiz) {
    NO* no = (*raiz)->dir;
    (*raiz)->dir = no->esq;
    no->esq = *raiz;
    (*raiz)->altura = maior(altura_NO((*raiz)->esq), altura_NO((*raiz)->dir)) + 1;
    no->altura = maior(altura_NO(no->dir), (*raiz)->altura) + 1;
    *raiz = no;
}

void RotacaoLR(AVLTree* raiz) {
    RotacaoRR(&(*raiz)->esq);
    RotacaoLL(raiz);
}

void RotacaoRL(AVLTree* raiz) {
    RotacaoLL(&(*raiz)->dir);
    RotacaoRR(raiz);
}

// Função auxiliar para procurar menor nó (usado na remoção)
NO* procuraMenor(NO* atual) {
    NO* no1 = atual;
    NO* no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

// Inserção na árvore AVL
int insere_AVLTree(AVLTree* raiz, int valor) {
    int res;
    if (*raiz == NULL) {
        NO* novo = (NO*)malloc(sizeof(NO));
        if (novo == NULL)
            return 0;
        novo->info = valor;
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    NO* atual = *raiz;
    if (valor < atual->info) {
        if ((res = insere_AVLTree(&(atual->esq), valor)) == 1) {
            if (fatorBalanceamento_NO(atual) >= 2) {
                if (valor < atual->esq->info) {
                    RotacaoLL(raiz);
                } else {
                    RotacaoLR(raiz);
                }
            }
        }
    } else if (valor > atual->info) {
        if ((res = insere_AVLTree(&(atual->dir), valor)) == 1) {
            if (fatorBalanceamento_NO(atual) >= 2) {
                if (valor > atual->dir->info) {
                    RotacaoRR(raiz);
                } else {
                    RotacaoRL(raiz);
                }
            }
        }
    } else {
        printf("Valor duplicado!\n");
        return 0;
    }

    atual->altura = maior(altura_NO(atual->esq), altura_NO(atual->dir)) + 1;
    return res;
}

// Remoção na árvore AVL
int remove_AVLTree(AVLTree* raiz, int valor) {
    if (*raiz == NULL) {
        printf("Valor não existe!\n");
        return 0;
    }

    int res;
    if (valor < (*raiz)->info) {
        if ((res = remove_AVLTree(&(*raiz)->esq, valor)) == 1) {
            if (fatorBalanceamento_NO(*raiz) >= 2) {
                if (altura_NO((*raiz)->dir->esq) <= altura_NO((*raiz)->dir->dir))
                    RotacaoRR(raiz);
                else
                    RotacaoRL(raiz);
            }
        }
    } else if (valor > (*raiz)->info) {
        if ((res = remove_AVLTree(&(*raiz)->dir, valor)) == 1) {
            if (fatorBalanceamento_NO(*raiz) >= 2) {
                if (altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq))
                    RotacaoLL(raiz);
                else
                    RotacaoLR(raiz);
            }
        }
    } else {
        // Encontrou o nó para remover
        if ((*raiz)->esq == NULL || (*raiz)->dir == NULL) {
            NO* oldNode = *raiz;
            if ((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else
                *raiz = (*raiz)->dir;
            free(oldNode);
        } else {
            NO* temp = procuraMenor((*raiz)->dir);
            (*raiz)->info = temp->info;
            remove_AVLTree(&(*raiz)->dir, temp->info);
            if (fatorBalanceamento_NO(*raiz) >= 2) {
                if (altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq))
                    RotacaoLL(raiz);
                else
                    RotacaoLR(raiz);
            }
        }
        return 1;
    }

    (*raiz)->altura = maior(altura_NO((*raiz)->esq), altura_NO((*raiz)->dir)) + 1;
    return res;
}

// Função para imprimir árvore em ordem (auxiliar)
void emOrdem(NO* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("%d ", raiz->info);
        emOrdem(raiz->dir);
    }
}

// Impressão bonita da árvore
void imprime_AVL(NO* raiz, int nivel) {
    if (raiz != NULL) {
        imprime_AVL(raiz->dir, nivel + 1);
        for (int i = 0; i < nivel; i++)
            printf("    ");
        printf("%d\n", raiz->info);
        imprime_AVL(raiz->esq, nivel + 1);
    }
}

int main() {
    AVLTree raiz = NULL;

    insere_AVLTree(&raiz, 10);
    insere_AVLTree(&raiz, 20);
    insere_AVLTree(&raiz, 30);
    insere_AVLTree(&raiz, 40);
    insere_AVLTree(&raiz, 50);
    insere_AVLTree(&raiz, 25);

    printf("Árvore em ordem:\n");
    emOrdem(raiz);
    printf("\n\nÁrvore formatada:\n");
    imprime_AVL(raiz, 0);

    remove_AVLTree(&raiz, 30);

    printf("\nÁrvore após remover 30:\n");
    imprime_AVL(raiz, 0);

    return 0;
}
