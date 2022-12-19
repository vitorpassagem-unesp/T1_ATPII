#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(){
    printf("Qual o nome do arquivo que deseja criar? ");
    char nomearq[32];
    scanf("%s", nomearq);
    FILE *arq = fopen(nomearq, "w");
    srand(time(NULL));
    int i, a;
    for (i=1; i<=64; i++){
        fprintf(arq, "primario %d\n", i);
        fprintf(arq, "secundario %d\n", i);
        a = rand() % 4;
        while (a == 0)
            a = rand() % 4;
        fprintf(arq, "%d\n", a);
        fprintf(arq, "verdadeiro %d\n", i);
        fprintf(arq, "falso %d\n\n", i);
    }
    fclose(arq);
    return 0;
}