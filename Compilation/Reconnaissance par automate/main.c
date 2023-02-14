#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//fonction de verifier (refactorise)
int verify(char *ptrc, char *X, int x_len) {
    for (unsigned long i = 0; i < strlen(ptrc); i++) {
        printf("%c\n", ptrc[i]);
        if (memchr(X, ptrc[i],x_len) == NULL) {
            return 0;
        }
    }
    return 1;
}

//fonction de position (refactorise)
int position(char *ca, char *X) {
    char *ptr = strchr(X, *ca);
    if (ptr != NULL) {
        return ptr-X;
    }
    return -1;
}

//fonction final (refactorise)
int final(int etat, int *F, int f_len) {
    return memchr(F, etat, f_len) != NULL;
}


//fonction parse_file (lire de fichier en utilisant des pointeurs)
//on va utiliser des variable locaux au class main, c'est plus facile que des variables globales, car ils sont moins sujet aux erreurs

int parse_file(char *filename,char **X, int *x_len, int *** trans, int *trans_items, int *trans_connections, int **F, int *f_len) {
    FILE *fp = fopen(filename, "r");
    
    //prendre le nombre des lignes dans le fichier
    int total_lines = 0;
    while (EOF != (fscanf(fp, "%*[^\n]"), fscanf(fp,"%*c"))) total_lines++;

    //remettre le pointeur au debut de fichier pour l'utiliser dans le fonction buffer
    rewind(fp);

    //buffer pour storer les contenu de lignes, partie par partie
    char buffer[256];

    //on prend x_len and f_len pour faciliter les iterations de fonctions en classe main
    *x_len = 0;
    *f_len = 0;

    //on lit le fichier ligne par ligne, on retourne NULL pour les lignes vides and non-NULL pour les lignes normales
    //on prend un maximum de 256 characteres par lines par fois, c'est suffisant pour ces automates
    //le var "line" est un reference pour les lignes que le fonction est entrain de lire
    for (int line = 0; fgets(buffer,256,fp) != NULL; line++) {
        //affichage des 2 lines au debut de fichier
        if (line < 2) {
            printf("%s",buffer);
            continue;
        }
        //ce code block est pour lire les noms de variable
        if (line == 2) {
            for (unsigned int i = 0; i < strlen(buffer); i++) {
                //on ne lire pas les espaces ' '
                if (buffer[i] == ' ' || buffer[i] == '\n') {
                    continue;
                }

                //ajout des caracteres au tableau X en utilisant realloc (fonction standard de stdlib.h)
                //on prend le longeur de X et on ajout 1 pour l'enregistrer puis on affecte un caractere a l'espace reserver par realloc
                (*x_len)++;
                *X = realloc(*X, (*x_len));
                (*X)[*x_len - 1] = buffer[i];
            }
        }

        //dimensions
        if (line == 3) {
            //sscanf() est une fonction de standlib pour prendre les inputs des utilisateurs
            sscanf(buffer,"%d %d", &(*trans_items), &(*trans_connections));
            (*trans) = malloc(sizeof(int*)*(*trans_items));
            for (int i = 0; i < (*trans_items); i++) (*trans)[i] = malloc(sizeof(int)*(*trans_connections));
        }
        //lire les variables (donnees)
        if (line > 3 && line < total_lines) {
            char *cursor = buffer;
            //on a le nombre des iterations d'apres le ligne precedant dans le fichier
            for (int i = 0; i < (*trans_connections); i++) {

                //on prend un nombre 'int' en utilisant sscanf
                //contrairement à le cas de tableau X, on a deja reserver les espaces dans trans
                //onsait que le trans est a partir de ligne 4
                sscanf(cursor, "%d", &(*trans)[line - 4][i]);
                cursor = strchr(cursor+1, ' ');
                if (cursor == NULL) {
                    break;
                }
            }
            continue;
        }

        //lire donnees de F (le longeur est le contenu)
        if (line == total_lines) {
           // char *cursor = buffer;
            for (char *cursor = buffer; cursor != NULL; cursor = strchr(cursor + 1, ' ')) {
                (*f_len)++;
                *F = realloc(*F, sizeof(char*)*(*f_len));
                sscanf(cursor,"%d", &(*F)[(*f_len) - 1]);
            }
            continue;
        }
    }
    return 0;
}

int main(void) {

    //definitions des variables
    char *X;
    int x_len;

    int **trans;
    int trans_items;
    int trans_connections;

    int *F;
    int f_len;
    parse_file("lang.txt",&X,&x_len, &trans, &trans_items, &trans_connections, &F, &f_len);    
    for (int i = 0; i < x_len; i++) {
        printf("X: %c %d!\n",X[i],i);
    }
    for (int i = 0; i < trans_items; i++) {
        for (int j = 0; j < trans_connections; j++) {
            printf("trans: %d ",trans[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < f_len; i++) {
        printf("le contenu de F est: %d\n",F[i]);
    }

    char *buffer = calloc(100,1);

    
    int q = 0;
    printf("donner une phrase a verifier: ");
    fgets(buffer, 99, stdin);
    //this is NOT ok but im doing it anyways
    //il y'a une autre methode de lire le string a verifier, mais pour ce cas fgets est suffisant
    while (strchr(buffer, '\n') != NULL) {
        *strchr(buffer, '\n') = '\0';
    };
    printf("longeur = %lu\n", strlen(buffer));
    puts(buffer);
    if (verify(buffer,X,x_len)) {
        for (int i = 0; (unsigned long) i < strlen(buffer); i++) {
            printf("traitement du caractere: %c\n", buffer[i]);
            q = trans[q][strchr(X, buffer[i]) - X];
        }
        if (final(q,F,f_len)) {
            printf("%s est acceptee\n", buffer);
        }
        else
            printf("%s est refusee \n", buffer);
    } else
        printf("%s ne peut pas etre analysee \n", buffer);
    getchar();
    return 0;
}