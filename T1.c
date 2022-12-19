//Vitor A. C. Z. Passagem - C. Computação - Integral
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

void converter_exercicio(){
    printf("Qual o nome do arquivo deseja converter (maximo de 64 ex por arquivo)? ");
    char arquivotxt[32];
    scanf("%s", arquivotxt);
    getchar();
    FILE *arq = fopen(arquivotxt, "r");
    if (arq == 0) {
        printf("\nNão foi possível abrir arquivo\n\n");
        return;
    }
    Exercicio ex[64];
    int i = 0;
        do{
            fgets (ex[i].assunto_p, 64, arq);
            fgets (ex[i].assunto_s, 64, arq);
            fscanf (arq, "%d\n", &ex[i].dificuldade);
            fgets (ex[i].sentenca_v, 256, arq);
            fgets (ex[i].sentenca_f, 256, arq);
            fscanf (arq, "\n");
            i++;
            printf("%d\n", i);
        }while (feof (arq) == 0);
        fclose (arq);
        arq = NULL;
        printf ("Qual o nome do arquivo binario a ser criado? ");
        char arquivobin[32];
        scanf("%s", arquivobin);
        arq = fopen (arquivobin, "wb");
        fwrite (&ex, sizeof(Exercicio), 64, arq);
        fclose (arq);
}

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
    FILE *arq = fopen("exercicios.txt", "ab"); //abre ou cria se nao existir, arquivo binario no formato append
    Exercicio ex = {"", "", 0, "", ""}; //inicia o vetor nulo
    if (arq==0)
        printf("Erro na abertura de arquivo");
    char op;
    do{
        fflush(stdin);
        ex = novo_input(ex);
        fwrite(&ex, sizeof(Exercicio), 1, arq); //escreve no arquivo binario
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
    FILE *arq = fopen("exercicios.txt","rb"); //abre arquivo para ler binario (rb)
    Exercicio ex[64] = {"","",0,"",""};
    int i=0, j;
    if (arq == NULL)
        printf ("O arquivo não foi aberto");
    else{
        fread(&ex, sizeof(Exercicio), 64, arq); //lê arquivo e salva no vetor ex
        do{
            i++;
        } while (ex[i].dificuldade<4 && ex[i].dificuldade != 0); //conta quantos exercicios tem no arquivo
        for(j=0; j<i; j++){
            printf("\n%d\n", j+1);
            printf("Assunto primario: %s", ex[j].assunto_p);
            printf("Assunto secundario: %s", ex[j].assunto_s);
            printf("Dificuldade: %d\n", ex[j].dificuldade);
            printf("Exercicio VERDADEIRO: %s", ex[j].sentenca_v);
            printf("Exercicio FALSO: %s\n", ex[j].sentenca_f);
        } //imprime todos os exercicios
    }
}

int gerar_prova(){
    srand(time(NULL));
    FILE *arq = fopen("exercicios.txt", "rb");
    if (arq == NULL){
        printf("Falha ao abrir arquivo com os exercicios.\n");
        return 0;
    }
    int quant;
    quant = rand()%9; //pega o inteiro gigante q sai do sorteio e usa o resto da divisão por 9
    if (quant < 3){
        while (quant < 3) //enquanto sortear um numero menor q 3, refaz o sorteio
            quant = rand() % 9;
    }
    Exercicio ex[64];
    fread (&ex, sizeof (Exercicio), 64, arq);
    int id[quant], i, j;
    for(i=0; i<quant; i++){ //faz o sorteio do íncice dos exercicios
        id[i] = rand() % 64;
        if (i>0){ //checa se sorteou um indice repetido, e caso tenha, sorteia de novo
            for(j=i-1; j>=0; j--){
                if (id[i]==id[j]){
                    while (id[i] == id[j])
                        id[i] = rand() % 64;
                }
            }
        }
    }

    Exercicio prova[quant]; //atribui valores do banco de exercicios ao vetor prova
    for (i = 0; i < quant; i++){
        prova[i] = ex[id[i]];
    }

    ExProva questao[quant]; //array com questões que serão impressas na prova
    Gabarito gabarito[quant]; //gabarito da prova
    int v_f[quant];
    int teste = 1;
    do{
        for (i = 0; i < quant; i++){
            v_f[i] = rand() % 2;
        }
        for (i = 0; i < quant; i++){
            if (v_f[i] ==  v_f[i+1])
                teste = 1;
            else
                teste = 0;
                break;
        }
    }while (teste == 1); //testa se todas são verdadeiras ou falsas (iguais)

    for (i = 0; i < quant; i++){  //loop vai de questao em questao e definindo dados da questao de prova e gabarito
        if (v_f[i] == 1){
            strcpy (questao[i].sentenca, prova[i].sentenca_v); //copia a string a para a string b
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
        fprintf(arq, "\t%s", gabarito[i].assunto_p);
        fprintf(arq, "\t%s", gabarito[i].assunto_s);
        fprintf(arq, "%c\n\n", gabarito[i].resposta);
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
    printf("Digite 3 para gerar prova e gabarito\n");
    printf("Digite 4 para transformar arquivo de texto em binario\n\n");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
    case 0:
        printf("\nAdeus...\n\n");
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
        converter_exercicio();
        break;
    default:
        printf("Opcao invalida\n\n");
        break;
    }
    }while(opcao != 0);
}