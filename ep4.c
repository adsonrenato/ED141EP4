/******************************************************************************/
/* Nome: Christian M. T. Takagi             No. USP: 7136971                  */
/* Disciplina: MAC323                       Prof.  Andre Fujita               */
/* Exercicio Programa 4                     Arquivo: ep4.c                    */
/******************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct paginaB PaginaB;
struct paginaB {
    int num_chaves; /* Quantidades de chaves contida na pagina */
    int *chaves; /* Chaves armazenadas na pagina */
    PaginaB **filhos; /* Ponteiro para os filhos */
};

typedef struct arvoreB ArvoreB;
struct arvoreB {
    int MIN_C; /* Numero minimo de chaves de uma pagina nao raiz e nao folha */
    int MAX_C; /* Numero maximo de chaves de uma pagina */
    int MIN_F; /* Numero minimo de filhos de uma pagina nao raiz e nao folha */
    int MAX_F; /* Numero maximo de filhos de uma pagina */
    PaginaB *raiz;
};

/* Define as propriedades de uma ArvoreB */
ArvoreB* inicializa_ArvoreB( int d) {
    ArvoreB *x = (ArvoreB*) malloc( sizeof (*x));
    x->MAX_F = d;
    x->MIN_F = (d/2 == 1)? 2:d/2;
    x->MAX_C = x->MAX_F - 1;
    x->MIN_C = x->MIN_F - 1;
    x->raiz = NULL;
    return x;
}

/* Cria uma nova pagina com as caracteristicas da arvore */
PaginaB* nova_pagina( ArvoreB *T, int num_c) {
    int i;
    PaginaB *x = (PaginaB*) malloc( sizeof (*x));
    x->num_chaves = num_c;
    x->chaves = (int*) malloc( (T->MAX_C + 1) * sizeof (int));
    x->filhos = (PaginaB **) malloc( (T->MAX_F + 1) * sizeof (PaginaB*));
    for (i = 0; i < T->MAX_F; i++)
        x->filhos[i] = NULL;
    return x;
}

/* Procura a chave info em uma pagina */
int busca_binaria( PaginaB *pagina, int info) {
    int meio, i, f;
    i = 0;
    f = pagina->num_chaves-1;

    while (i <= f) {
        meio = (i + f)/2;
        if (pagina->chaves[meio] == info)
            return(meio); /*Encontrou. Retorna a posicao em que a chave esta.*/
        else if (pagina->chaves[meio] > info)
            f = meio - 1;
        else i = meio + 1;
    }
    return i; /*Nao encontrou. Retorna a posicao do ponteiro para o filho.*/
}

/* Interface para busca da chave info na raiz da sub-arvore. */
/* Retorna 1 se encontrado e 0 c.c. */
int busca_ArvoreB( PaginaB *raiz, int info) {
    PaginaB *pagina;
    int pos; /*posicao retornada pelo busca binaria.*/

    pagina = raiz;
    while (pagina != NULL){
        pos = busca_binaria( pagina, info);
        if (pos < pagina->num_chaves && pagina->chaves[pos] == info)
            return 1;
        else pagina = pagina->filhos[pos];
    }
    return 0;
}


/* Insere a chave info e o ponteiro para seu filho da direita em uma pagina */
void insere_chave( PaginaB *raiz, int info, PaginaB *filhodir) {
    int k, pos;

    /* busca para obter a posicao ideal para inserir a nova chave */
    pos = busca_binaria(raiz, info);
    k = raiz->num_chaves;

    /* realiza o remanejamento para manter as chaves ordenadas */
    while (k > pos && info < raiz->chaves[k-1]) {
        raiz->chaves[k] = raiz->chaves[k-1];
        raiz->filhos[k+1] = raiz->filhos[k];
        k--;
    }
    /* insere a chave na posicao ideal */
    raiz->chaves[pos] = info;
    raiz->filhos[pos+1] = filhodir;
    raiz->num_chaves++;
}

/* Realiza a busca da pagina para inserir a chave e faz as cisoes quando necessarias */
PaginaB *insere(ArvoreB *T, PaginaB *raiz, int info, int *h, int *info_retorno) {
    int i, pos, K,
        info_mediano; /* auxiliar para armazenar a chave que ira subir para o pai */
    PaginaB *temp, *filho_dir; /*ponteiro para o filho a direita da chave */

    if (raiz == NULL) {
        /* chegou em uma pagina folha */
        *h = 1;
        *info_retorno = info;
        return NULL;
    }
    else {
        pos = busca_binaria( raiz, info);
        if (raiz->num_chaves > pos && raiz->chaves[pos] == info) {
            printf("Chave ja contida na Árvore");
            *h = 0;
        }
        else {
            /* desce na arvore ate encontrar a pagina folha para inserir a chave */
            filho_dir = insere(T, raiz->filhos[pos], info, h, info_retorno);
            if (*h) /* Se 1 deve inserir a info_retorno na pagina */ {
                insere_chave(raiz, *info_retorno, filho_dir);
                if (raiz->num_chaves <= T->MAX_C) /* Tem espaco na pagina */ {
                    *h = 0;
                }
                else { /* Overflow. Precisa de cisao */
                    if (T->MAX_F == 3 || T->MAX_F % 2 ==0) {
                        K = 0;
                    }
                    else
                        K = 1;
                    temp = nova_pagina(T, 0);
                    /* elemento mediano que vai subir para o pai */
                    info_mediano = raiz->chaves[T->MIN_C + K];

                    /* efetua cisao*/
                    temp->filhos[0] = raiz->filhos[T->MIN_C + K + 1];
                    for (i = T->MIN_C + K + 1 ; i < T->MAX_F; i++) {
                        insere_chave( temp, raiz->chaves[i], raiz->filhos[i+1]);
                    }
                    /* atualiza raiz */
                    for (i = T->MIN_C + K; i < T->MAX_C + K + 1; i++)
                    {
                        raiz->chaves[i] = 0;
                        raiz->filhos[i+1] = NULL;
                    }
                    raiz->num_chaves = T->MIN_C + K;
                    /*retorna o mediano para inseri-lo na pagina pai e o */
                    /* temp como filho direito do mediano */
                    *info_retorno = info_mediano;
                    if (temp->num_chaves > 0)
                        return temp;
                    else
                        return NULL;
                }
            }
        }
    }
    return NULL;
}


/* Interface para insercao da chave info na raiz da sub-arvore de T */
PaginaB *insere_ArvoreB(ArvoreB *T, PaginaB *raiz, int info) {
    int h = 0;
    int info_retorno;
    PaginaB *filho_dir, *nova_raiz;

    filho_dir = insere( T, raiz, info, &h, &info_retorno);
    if (h) { /*Aumentara a altura da arvore*/
        nova_raiz = nova_pagina(T, 1);
        nova_raiz->chaves[0] = info_retorno;
        nova_raiz->filhos[0] = raiz;
        nova_raiz->filhos[1] = filho_dir;
        return nova_raiz;
    }
    else 
        return raiz;
}


/* Imprime a arvore horizontalmente (i.e. Windows Explorer). */
void imprime( PaginaB *raiz, char *prefixo, char ultimo) {
    int i;
    char temp[256];
    strcpy(temp, prefixo);
    if (raiz != NULL) {
        printf(temp);
        if (ultimo == 'v') {
            printf("-- ");
            strcat( temp, "   ");
        }
        else {
            printf( "|- ");
            strcat( temp, "|  ");
        }
        for (i = 0; i < raiz->num_chaves; i++) {
            printf( "%d  ", raiz->chaves[i]);
        }
        printf("\n");
        ultimo = 'f';
        for (i = 0; i < raiz->num_chaves; i++) {
            imprime( raiz->filhos[i], temp, ultimo);
        }
        ultimo = 'v';
            imprime( raiz->filhos[i], temp, ultimo);
    }
}

/*
PaginaB* remove( Arvore *T, PaginaB *raiz, int info, int *ant) {
    for (i = pos; i < T->MAX_C; i++) {
        raiz->chaves[i] = raiz->chaves[i+1];
    }
    raiz->num_chaves--;
    if (num_chaves < MIN_C) {
        if ()
    }
}


PaginaB* remove_ArvoreB( Arvore *T, PaginaB *raiz, PaginaB *pai, int info) {
    int i, pos;

    pos = busca_binaria( raiz, info);
    if (raiz->num_chaves > pos && raiz->chaves[pos] == info) {
        remove( T, raiz, pai, info);
    }
    else 
        remove_AroreB( T, raiz->filho[pos], pai, info);
}
*/

int main() {
    int d, info, proced;
    char prefixo[256] = "", ultimo = 'v';
    ArvoreB *T;

    printf( "\n    === Operacoes sobre Arvore B ===    \n");
    while(1) {
        printf( "Digite um valor maior ou igual a 3 para a ordem.\n");
        scanf( "%d", &d);
        printf( "\nOrdem digitada: %d\n", d);
        if (d > 2)
            break;
        else
            printf( "Valor Invalido. Tente novamente.\n");
    }
    T = inicializa_ArvoreB( d);
    while(1) {
        prefixo[0] = '\0';
        printf("\n\n* Digite a operacao desejada:\n");
        printf("1. Busca\n2. Insercao\n3. Remocao\n4. Fechar o programa\n");
        scanf("%d", &proced);
        switch(proced) {
            case 1: {
                printf("\n    == Busca ==     \n");
                printf("Digite a chave a ser procurada.\n");
                scanf("%d", &info);
                if (busca_ArvoreB(T->raiz, info) == 1)
                    printf( " (OK) A chave %d esta na arvore.\n",info);
                else
                    printf( " (u_u) A chave %d nao esta na arvore.\n", info);
                printf("\n    = Representacao =    \n");
                imprime(T->raiz, prefixo, ultimo);
                break;
                }
            case 2: {
                printf("\n    == Insercao ==    \n");
                printf("Digite a chave a ser inserida.\n");
                scanf("%d", &info);
                T->raiz = insere_ArvoreB(T, T->raiz, info);
                printf("\n    = Representacao =    \n");
                imprime(T->raiz, prefixo, ultimo);
                break;
                }
            case 3:  {
                printf("\n   ==  Remocao ==    \n");
                break;
                }
            case 4: {
                printf("Obrigado por utilizar o Btree 3000!!! - a vendas nas melhores lojas.\n\n");
                exit(1);
                }
            default: {
                printf("Opcao Invalida. Tente novamente.");
                break;
                }
        }
    }
}
