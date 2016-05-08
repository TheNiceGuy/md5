#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "md5.h"
#include "config.h"

#define NONE    0
#define STDIN   1
#define FICHIER 2
#define STRING  3

static char* string;
static char* filename;
static int mode;

void help() {
    printf(
    "Utilisation: hash [MODE] ... [ARGUMENTS]\n"
    "Calcule la signature MD5 (128-bit) d'un message.\n\n"

    "Les modes de lecture suivant sont possible :\n"
    "-i, --stdin            lecture à partir de stdin [défaut]\n"
    "-f, --file   [fichier] lecture à partir d'un fichier\n"
    "-s, --string [message] lecture à partir de la commande\n"
    "-h, --help             affiche ce message\n"
    );
}

int parse_arg(int argc, char* argv[]) {
    int i;

    /* On boucle pour chaque argument. */
    mode = STDIN;
    for(i = 1; i < argc; i++) {
        /* On élimine tous les arguments ayant 1 caractère ou moins. */
        if(strlen(argv[i]) < 2)
            goto ERROR;

        /* On s'assure que l'argument comment par un tiret. */
        if(argv[i][0] == '-') {
            /* Un tiret est l'argument court. */
            if(argv[i][1] != '-') {
                switch(argv[i][1]) {
                case 'i':
                    mode = STDIN; break;
                case 'f':
                    filename = argv[i]+2;
                    mode = FICHIER; break;
                case 's':
                    string = argv[i]+2;
                    mode = STRING; break;
                case 'h':
                    help();
                    mode = NONE; return 0;
                default:
                    goto ERROR;
                }

            /* Deux tirets est l'argument long. */
            } else {
                if(strcmp("--stdin", argv[i]) == 0) {
                    mode = STDIN;
                } else
                if(strcmp("--file", argv[i]) == 0) {
                    filename = argv[i+1]; i++;
                    mode = FICHIER;
                } else
                if(strcmp("--string", argv[i]) == 0) {
                    string = argv[i+1]; i++;
                    mode = STRING;
                } else
                if(strcmp("--help", argv[i]) == 0) {
                    help();
                    mode = NONE;
                    return 0;
                } else {
                    goto ERROR;
                }
            }
        } else {
            goto ERROR;
        }
    }

    return 0;
ERROR:
    printf(
    "Option non reconnue -- %s\n"
    "Essayer 'hash --help' pour plus d'information.\n", argv[i]);

    return 1;
}

int main(int argc, char* argv[]) {
    uint8_t buffer[64];
    uint8_t length;
    md5_t md5;
    FILE* file;

    /* On s'assure que les arguments fut compris. */
    if(parse_arg(argc, argv) != 0)
        return 1;
    else if(mode == NONE)
        return 0;

    /* On initialise la structure md5. */
    md5_init(&md5);

    /* On calcule la signature en fonction du mode. */
    switch(mode) {
    case STDIN:
        do {
            /* On lit 512 bits. */
            length = read(STDIN_FILENO, &buffer, 64);

            /* On s'assure qu'il restait des bits à lire. */
            if(!length)
                break;

            /* On calcul le md5 des derniers bits. */
            md5_update(&md5, buffer, length);
        } while(length != 0);

        break;
    case FICHIER:
        /* On ouvre le fichier. */
        file = fopen(filename, "r");

        /* On s'assure que le fichier existe. */
        if(file == NULL) {
            printf("Le fichier '%s' n'existe pas.\n", filename);
            exit(1);
        }

        do {
            /* On lit 512 bits. */
            length = fread(&buffer, 1, 64, file);

            /* On s'assure qu'il restait des bits à lire. */
            if(!length)
                break;

            /* On calcul le md5 des derniers bits. */
            md5_update(&md5, buffer, length);
        } while(length != 0);

        /* On ferme le fichier. */
        fclose(file);
        break;
    case STRING:
        md5_update(&md5, (uint8_t*)string, strlen(string));
        break;
    }

    /* On calcul le dernier bloc. */
    md5_finish(&md5);

    /* On écrit la signature. */
    md5_print(&md5);

    return 0;
}
