#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __win32__
    #include <windows.h>
#endif
#include "md5.h"
#include "config.h"

#define NONE    0
#define STDIN   1
#define FICHIER 2
#define STRING  3

static char* string;
static char* filename;
static int mode;

void help(char* exec) {
    printf(
    "Utilisation: %s [MODE] ... [ARGUMENTS]\n"
    "Calcule la signature MD5 (128-bit) d'un message.\n\n"

    "Les modes de lecture suivant sont possible :\n"
    "-i, --stdin            lecture %c%c partir de stdin [d%c%cfaut]\n"
    "-f, --file   [fichier] lecture %c%c partir d'un fichier\n"
    "-s, --string [message] lecture %c%c partir de la commande\n"
    "-h, --help             affiche ce message\n\n"

    "Copyright (c) 2016 Gabriel-Andrew Pollo-Guilbert\n",
    exec, '\xC3', '\xA0', '\xC3', '\xA9', '\xC3', '\xA0', '\xC3', '\xA0');
}

int parse_arg(int argc, char* argv[]) {
    int i;

    /* On boucle pour chaque argument. */
    mode = STDIN;
    for(i = 1; i < argc; i++) {
        /* On élimine tous les arguments ayant 1 caractère ou moins. */
        if(strlen(argv[i]) < 2)
            goto ERR;

        /* On s'assure que l'argument commence par un tiret. */
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
                    help(argv[0]);
                    mode = NONE; return 0;
                default:
                    goto ERR;
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
                    help(argv[0]);
                    mode = NONE;
                    return 0;
                } else {
                    goto ERR;
                }
            }
        } else {
            goto ERR;
        }
    }

    return 0;
ERR:
    printf(
    "Option non reconnue -- %s\n"
    "Essayer '%s --help' pour plus d'information.\n", argv[i], argv[0]);

    return 1;
}

int main(int argc, char* argv[]) {
    uint8_t buffer[64];
    uint8_t length;
    md5_t md5;
    FILE* file;

#ifdef __win32__
    SetConsoleOutputCP(CP_UTF8);
#endif

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
