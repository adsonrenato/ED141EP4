/******************************************************************************/
/* Nome: Christian M. T. Takagi             No. USP: 7136971                  */
/* Disciplina: MAC323                       Prof.  Andre Fujita               */
/* Exercicio Programa 4                     Arquivo: ep4.c                    */
/******************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define D 2
#define MAX_CHAVES 2*D-1 
#define MAX_FILHOS 2*D
#define MIN_OCUP D-1

typedef struct pagina_arvoreB Pagina_ArvoreB;
struct pagina_arvoreB {
    int num_chaves; /*Quantidades de chaves contida na página*/
    int chaves[MAX_CHAVES]; /*Chaves armazenadas na página*/
    Pagina_ArvoreB *filhos[MAX_FILHOS]; /*Ponteiro para os filhos*/
};

typedef struct arvoreB ArvoreB;
struct arvoreB {
    int ordem;
    Pagina_ArvoreB *raiz;
};

ArvoreB* inicializa_ArvoreB( int ordem) {
    ArvoreB *x = (ArvoreB*) malloc( sizeof (x));
    x->ordem = ordem;
    x->raiz = NULL;
    return x;
}


/* Procura a chave em uma pagina */
int busca_binaria(Pagina_ArvoreB *pagina, int info) 
{ 
    int meio, i, f; 
    i = 0;
    f = pagina->num_chaves-1;

    while (i <= f)
    {
        meio = (i + f)/2;
        if (pagina->chaves[meio] == info)
            return(meio); /*Encontrou. Retorna a posíção em que a chave está.*/
        else if (pagina->chaves[meio] > info)
            f = meio - 1;
        else i = meio + 1;
    }
    return(i); /*Não encontrou. Retorna a posição do ponteiro para o filho.*/
}

int busca(Pagina_ArvoreB *raiz, int info)
{
    Pagina_ArvoreB *pagina;
    int pos; /*posição retornada pelo busca binária.*/

    pagina = raiz;
    while (pagina != NULL)
    {
        pos = busca_binaria(pagina, info);
        if (pos < pagina->num_chaves && pagina->chaves[pos] == info)
            return(1);
        else pagina = pagina->filhos[pos];
    }
    return(0);
}


/*Insere uma chave e o ponteiro para o filho da direita em uma página*/
void insere_chave(Pagina_ArvoreB *raiz, int info, Pagina_ArvoreB *filhodir)
{
    int k, pos;

    /*busca para obter a posição ideal para inserir a nova chave*/
    pos = busca_binaria(raiz, info);
    k = raiz->num_chaves;

    /*realiza o remanejamento para manter as chaves ordenadas*/
    while (k > pos && info < raiz->chaves[k-1])
    {
        raiz->chaves[k] = raiz->chaves[k-1];
        raiz->filhos[k+1] = raiz->filhos[k];
        k--;
    }
    /*insere a chave na posição ideal*/
    raiz->chaves[pos] = info;
    raiz->filhos[pos+1] = filhodir;
    raiz->num_chaves++;
}

/*Realiza a busca da página para inserir a chave e faz as subdivisões quando necessárias*/
Pagina_ArvoreB *insere(ArvoreB *T, Pagina_ArvoreB *raiz, int info, int *h, int *info_retorno)
{
    int i, pos,
        info_mediano; /*auxiliar para armazenar a chave que irá subir para o pai*/
    Pagina_ArvoreB *temp, *filho_dir; /*ponteiro para o filho à direita da chave */

    if (raiz == NULL)
    {
        /*a página anterior é o ideal para inserir a nova chave (chegou em uma página folha)*/
        *h = 1;
        *info_retorno = info;
        return(NULL);
    }
    else {
        pos = busca_binaria(raiz,info);
        if (raiz->num_chaves > pos && raiz->chaves[pos] == info)
        {
            printf("Chave já contida na Árvore");
            *h = 0;
        }
        else {
            /*desce na árvore até encontrar a página folha para inserir a chave.*/
            filho_dir = insere(T, raiz->filhos[pos], info, h, info_retorno);
            if (*h) /*Se 1 deve inserir a info_retorno na página.*/
            {
                if (raiz->num_chaves < MAX_CHAVES) /*Tem espaço na página*/
                {
                    insere_chave(raiz, *info_retorno, filho_dir);
                    *h = 0;
                }
                else { /*Overflow. Precisa subdividir*/
                    temp = (Pagina_ArvoreB *) malloc (sizeof(Pagina_ArvoreB));
                    temp->num_chaves = 0;

                    /*inicializa filhos com NULL*/
                    for (i = 0; i < MAX_FILHOS; i++)
                        temp->filhos[i] = NULL;

                    /*elemento mediano que vai subir para o pai*/
                    info_mediano = raiz->chaves[MIN_OCUP];

                    /*insere metade da página raiz no temp (efetua subdivisão)*/
                    temp->filhos[0] = raiz->filhos[MIN_OCUP+1];
                    for (i = MIN_OCUP + 1; i < MAX_CHAVES; i++)
                        insere_chave(temp, raiz->chaves[i], raiz->filhos[i+1]);

                    /*atualiza raiz. */
                    for (i = MIN_OCUP; i<MAX_CHAVES; i++)
                    {
                        raiz->chaves[i] = 0;
                        raiz->filhos[i+1] = NULL;
                    }
                    raiz->num_chaves = MIN_OCUP;

                    /*Verifica em qualpágina será inserida a nova chave*/
                    if (pos <= MIN_OCUP)
                        insere_chave(raiz, *info_retorno, filho_dir);
                    else insere_chave(temp, *info_retorno, filho_dir);

                    /*retorna o mediano para inserí-lo na página pai e o temp como filho direito do mediano.*/
                    *info_retorno = info_mediano;
                    return(temp);
                }
            }
        }
    }
    return(NULL);
}



Pagina_ArvoreB *insere_arvoreB(ArvoreB *T, Pagina_ArvoreB *raiz, int info)
{
    int h = 0;
    int info_retorno, i;
    Pagina_ArvoreB *filho_dir, *nova_raiz;

    filho_dir = insere(T, raiz,info,&h,&info_retorno);
    if (h)
    { /*Aumetará a altura da árvore*/
		nova_raiz = (Pagina_ArvoreB *) malloc (sizeof(Pagina_ArvoreB));
		nova_raiz->num_chaves = 1;
        nova_raiz->chaves[0] = info_retorno;
        nova_raiz->filhos[0] = raiz;
        nova_raiz->filhos[1] = filho_dir;
        for (i = 2; i <= MAX_CHAVES; i++)
            nova_raiz->filhos[i] = NULL;
        return(nova_raiz);
    }
    else return(raiz);
}


/* Funcao que imprime a arvore horizontalmente (i.e. Windows Explorer). */
void imprime( Pagina_ArvoreB *raiz, char *prefixo, char ultimo) {
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

int main() {
    int ordem, info, proced;
    char prefixo[256] = "", ultimo = 'v';
    ArvoreB *T;
    
    printf("=== Operacoes sobre Arvore B ===\n Digite o valor da ordem.\n");
    scanf("%d", &ordem);
    printf("\n(Dummy) Ordem digitada: %d\nOrdem padrão = 2\n", ordem);
    T = inicializa_ArvoreB( ordem);

    while(1) {
        printf("\n\n* Digite a operacao desejada:\n");
        printf("1. Busca\n2. Insercao\n3. Remocao\n4. Fechar o programa\n");
        scanf("%d", &proced);
        switch(proced) {
            case 1: 
                {
                    printf("Busca.\n");
                    break;
                }
            case 2: 
                {
                    printf("Insercao.\n");
                    printf("Digite a chave a ser inserida.\n");
                    scanf("%d", &info);
                    T->raiz = insere_arvoreB(T, T->raiz, info);
                    printf("\n= Representacao =:\n");
                    prefixo[0] = '\0';
                    imprime(T->raiz, prefixo, ultimo);
                    break;
                }
            case 3: 
                {
                    printf("Remocao.\n");
                    break;
                }
            case 4: 
                {
                    printf("Obrigado por utilizar o Btree 3000!!! - a vendas nas melhores lojas.\n\n");
                    exit(1);
                }
            default:
                {
                    printf("Opcao Invalida. Tente novamente.");
                    break;
                }
        }
    }
}
