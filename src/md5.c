#include <stdlib.h>
#include <stdio.h>
#include "md5.h"
#include "config.h"

/* On d�finit les fonctions auxiliaires. */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* On d�finit une fonction pour appliquer une rotation circulaire vers la gauche. */
#define ROTATION(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* On d�finit la fonction g�n�rale de l'algorithme. */
#define FF(a, b, f, x, t, s) { \
    (a) += (f) + (x) + (t);    \
    (a)  = ROTATION((a), (s)); \
    (a) += (b);                \
}

/* Table de valeur construite � partir de la fonction sinus. */
#define T (uint32_t[]) {0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, \
                        0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501, \
                        0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, \
                        0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821, \
                        0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, \
                        0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8, \
                        0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, \
                        0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A, \
                        0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, \
                        0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70, \
                        0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, \
                        0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665, \
                        0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, \
                        0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1, \
                        0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, \
                        0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391}

/* Table des rotations. */
#define S (uint32_t[]) {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, \
                        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, \
                        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, \
                        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21}

/* Table servant � l'extension du message. */
#define PAD (uint8_t[]) {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

void md5_init(md5_t* md5) {
    /* On initialise les registres. */
    md5->digest[0] = (uint32_t)0x67452301;
    md5->digest[1] = (uint32_t)0xEFCDAB89;
    md5->digest[2] = (uint32_t)0x98BADCFE;
    md5->digest[3] = (uint32_t)0x10325476;

    /* On initialise la structure. */
    md5->pos = 0;
    md5->len = 0;
}

void md5_update(md5_t* md5, uint8_t* data, size_t len) {
    uint8_t i;
    uint32_t r[4];

    /* On actualise la taille du message. */
    md5->len += len;

    while(len--) {
        /* On copie chaque octet dans le prochain bloc de 512-bits. */
        md5->buffer[md5->pos++] = *data++;

        /* Si le bloc a une taille de 512-bits, on applique la transformation. */
        if(md5->pos == 0x40) {
            /* On copie les registres. */
            for(i = 0; i < 4; i++)
                r[i] = md5->digest[i];

            /* On applique la transformation sur le bloc. */
            md5_transform(r, (uint32_t*)md5->buffer);

            /* On additionne le r�sultat. */
            for(i = 0; i < 4; i++)
                md5->digest[i] += r[i];

            /* On recommence un nouveau bloc. */
            md5->pos = 0x0;
        }
    }
}

void md5_finish(md5_t* md5) {
    uint8_t i;
    uint32_t pad;
    uint32_t r[4];
    uint64_t len;

    /* On sauvegarde la taille du message. */
    len = md5->len;

    /* On calcule la quantit� de pad � ajouter afin que len448mod512. */
    pad = (len%64 < 56)?(56 - len%64):(120 - len%64);

    /* On ajoute le pad et la taille du message. */
    md5_update(md5, PAD, pad);
    ((uint32_t*)md5->buffer)[14] = (uint32_t)(len*8);
    ((uint32_t*)md5->buffer)[15] = (uint32_t)(len*8 >> 32);

    /* On copie les registres. */
    for(i = 0; i < 4; i++)
        r[i] = md5->digest[i];

    /* On applique la transformation sur le bloc. */
    md5_transform(r, (uint32_t*)md5->buffer);

    /* On additionne le r�sultat. */
    for(i = 0; i < 4; i++)
        md5->digest[i] += r[i];
}

void md5_transform(uint32_t* r, uint32_t* x) {
    uint8_t i;
    uint32_t f, k, t;

    /* On boucle 4 fois pour les 16 blocs de 32-bits. */
    for(i = 0; i < 64; i++) {
        /* On calcule la fonction auxiliaire.*/
        if(i <= 15) {
            f = F(r[1], r[2], r[3]);
            k = i;
        } else if(i <= 31) {
            f = G(r[1], r[2], r[3]);
            k = (5*i+1)%16;
        } else if(i <= 47) {
            f = H(r[1], r[2], r[3]);
            k = (3*i+5)%16;
        } else {
            f = I(r[1], r[2], r[3]);
            k = (7*i+0)%16;
        }

        /* On sauvegarde une valeur pour la permutation. */
        t = r[3];

        /* On effectue la permutation des registres. */
        r[3] = r[2];
        r[2] = r[1];
        FF(r[0], r[1], f, x[k], T[i], S[i]);
        r[1] = r[0];
        r[0] = t;
    }
}

void md5_print(md5_t* md5) {
    uint8_t i;
    uint8_t digest[16];

    /* On repositionne les octets. */
    for(i = 0; i < 4; i++) {
        digest[   i] = (uint8_t)(md5->digest[0] >> 8*i);
        digest[ 4+i] = (uint8_t)(md5->digest[1] >> 8*i);
        digest[ 8+i] = (uint8_t)(md5->digest[2] >> 8*i);
        digest[12+i] = (uint8_t)(md5->digest[3] >> 8*i);
    }

    /* On �crit les octets en ordre. */
    for (i = 0; i < 16; i++)
        printf("%02x", digest[i]);
    printf("\n");
}
