#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

typedef struct exercicio {
    char assunto_p[64];
    char assunto_s[64];
    int dificuldade;
    char sentenca_v[256];
    char sentenca_f[256];
}Exercicio;

typedef struct exprova {
    char sentenca[256];
    int n_questao;
}ExProva;

typedef struct gabarito {
    char assunto_p[64];
    char assunto_s[64];
    int dificuldade;
    char resposta;
    int n_questao;
}Gabarito;

Exercicio novo_input (Exercicio ex){
    printf ("Qual o assunto primário do exercicio? ");
    gets(ex.assunto_p);
    printf ("Qual o assunto secundario do exercicio? ");
    gets(ex.assunto_s);
    printf ("Qual a dificuldade (1, 2 ou 3)? ");
    scanf("%d", &ex.dificuldade);
    getchar();
    printf ("Insira a sentença verdadeira: ");
    gets(ex.sentenca_v);
    printf ("Insira a sentença falsa: ");
    gets(ex.sentenca_f);

    return ex;
}

void novo_exercicio (){
    FILE *arq = fopen("exercicios.txt", "ab");
    Exercicio ex = {"", "", 0, "", ""};
    if (arq==0)
        printf("Erro na abertura de arquivo");
    char op;
    do{
        fflush(stdin);
        ex = novo_input(ex);
        fwrite(&ex, sizeof(Exercicio), 1, arq);
        printf("Deseja inserir outro exercicio (s/n)? ");
        scanf("%c", &op);
        getchar();
    }while(op == 's');
    fclose(arq);
}

void imprimir_todos(){
    //printf("De qual arquivo deseja ler os exercicios? ");
    //char nome_arq[32];
    //scanf("%s", nome_arq);
    FILE *arq = fopen("exercicios.txt","rb");
    Exercicio ex[64] = {"","",0,"",""};
    int i=0, j;
    if (arq == NULL)
        printf ("O arquivo não foi aberto");
    else{
        fread(&ex, sizeof(Exercicio), 64, arq);
        do{
            i++;
        } while (ex[i].dificuldade!=0); //conta quantos exercicios tem no arquivo
        for(j=0; j<i; j++){
            printf("\nAssunto primario: %s\n", ex[j].assunto_p);
            printf("Assunto secundario: %s\n", ex[j].assunto_s);
            printf("Dificuldade: %d\n", ex[j].dificuldade);
            printf("Exercicio VERDADEIRO: %s\n", ex[j].sentenca_v);
            printf("Exercicio FALSO: %s\n\n", ex[j].sentenca_f);
        }
    }
}

int gerar_prova(){
    srand(time(NULL));
    FILE *arq = fopen("exercicio.txt", "rb");
    if (arq == NULL){
        printf("Falha ao abrir arquivo com os exercicios.\n");
        return 0;
    }
    int quant;
    quant = rand() % 8;
    while (quant < 3)
        quant = rand() % 8;
    Exercicio ex[64];
    fread (&ex, sizeof (Exercicio), 64, arq);
    int id[quant], i, j;
    for(i=0; i<quant; i++){ //faz o sorteio do íncice dos exercicios
        id[i] = rand() % 64;
        if (i>0){ //checa se sorteou um indice repetido, e caso tenha, sorteia de novo
            for(j=i; j>=0; j--){
                while (id[i] == id[j])
                    id[i] = rand() % 64;
            }
        }
    }

    Exercicio prova[quant]; //atribui valores do banco de exercicios ao vetor prova
    for (i = 0; i < quant; i++){
        prova[i] = ex[id[i]];
    }

    ExProva questao[quant]; //array com questões que serão impressas na prova
    Gabarito gabarito[quant]; //gabarito da prova
    int v_f;

    for (i = 0; i < quant; i++){  //loop vai de questao em questao e definindo dados da questao de prova e gabarito
        v_f = rand() % 1;
        if (v_f == 1){
            strcpy (questao[i].sentenca, prova[i].sentenca_v);
            gabarito[i].resposta = 'V';
        }
        else{
            strcpy (questao[i].sentenca, prova[i].sentenca_f);
            gabarito[i].resposta = 'F';
        }
        strcpy (gabarito[i].assunto_p, prova[i].assunto_p);
        strcpy (gabarito[i].assunto_s, prova[i].assunto_s);
        gabarito[i].dificuldade = prova[i].dificuldade;
        questao[i].n_questao = i+1;
        gabarito[i].n_questao = i+1;
    }
    fclose(arq);
    arq = NULL;
    arq = fopen("prova.txt", "w");
    for (i = 0; i < quant; i++) { //grava exercicios no arquivo txt da prova
        fprintf(arq, "%d\n", questao[i].n_questao);
        fprintf(arq, "\t%s\n\n", questao[i].sentenca);
    }
    fclose(arq);
    arq = NULL;
    arq = fopen("gabarito.txt", "w");
    for (i = 0; i < quant; i++) { //grava respostas no arquivo txt do gabarito
        fprintf(arq, "%d\n", gabarito[i].n_questao);
        fprintf(arq, "\t%s\n", gabarito[i].assunto_p);
        fprintf(arq, "\t%s\n", gabarito[i].assunto_s);
        fprintf(arq, "%d\n\n", gabarito[i].resposta);
    }
    fclose(arq);
    arq = NULL;
    return 1;
}

int main(){
    Exercicio ex;
    int opcao;
    do{
    printf("\nDigite 0 para sair\n");
    printf("Digite 1 para inserir novo exercicio\n");
    printf("Digite 2 para imprimir lista de exercicios\n");
    printf("Digite 3 para gerar prova e gabarito\n\n");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
    case 0:
        printf("\nAdeus\n\n");
        break;
    case 1:
        novo_exercicio();
        break;
    case 2:
        imprimir_todos();
        break;
    case 3:
        gerar_prova();
        break;
    case 4:
        break;
    default:
        break;
    }
    }while(opcao != 0);
}