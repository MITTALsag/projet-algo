// Ne modifiez pas ce fichier, sauf pour modifier les constantes M, L, N, et R : mettez votre code ailleurs !
#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>

#define M 6 // taille mot de passe
#define L 1000 // longueur chaine
#define N 100000 // nb chaines par table
#define R 10 // nb tables

#define SEED 0x0af380103007be01

typedef uint64_t pwhash;

pwhash target_hash_function (const void *data);
void reduction(uint64_t hash,int variation,int allowed_chars_length, char* allowed_chars, char* pass);

/*
Quelques infos qui peuvent vous aider :
Les hashs dans les crackme sont représentés en hexadécimal, on obtient cette représentation avec un print en utilisant le specifier %#010lX
Et donc pour les lire depuis un fichier en tant qu'entiers, on peut utiliser fscanf avec le specifier %lX (ou regarder du coté de strtoul et al).
Pour vos lectures dans les fichiers, attention à la façon dont vous traiter le \n à la fin de chaque ligne : il ne fait pas partie des caractères du hash ou du mot de passe.
*/

#endif