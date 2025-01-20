/* Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u čvorove binarnog stabla.
 a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
 sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraća
 pokazivač na korijen stabla.
 b) Napisati funkciju replace koja će svaki element stabla zamijeniti sumom elemenata u
 njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
 vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat će se u stablo na
 slici Slika 2.
 c) Prepraviti program na način da umjesto predefiniranog cjelobrojnog polja korištenjem
 funkcije rand() generira slučajne brojeve u rasponu <10, 90>. Također, potrebno je
 upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
 dijela zadatka*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _binaryTree;
typedef struct _binaryTree* Position;
typedef struct _binaryTree {
    int el;
    Position left;
    Position right;
} BinaryTree;

Position insert(int el, Position root);
int inorder(Position root);
int replace(Position root);
int random(Position root);
int inorderInFile(Position root, FILE* filePointer);

int main() {

    Position root = NULL;
    int el = 0;
    printf("Unesite vrijednost prvog clana binarnog stabla: ");
    scanf("%d", &el);
    root = insert(el, root);

    FILE* filePointer = NULL;
    filePointer = fopen("binaryTree.txt", "w");

    int command = 1;
    do {
        printf("\nMenu:\n");
        printf("1 - Unos novog elementa\n");
        printf("2 - Ispis binarnog stabla - inorder\n");
        printf("3 - Replace (mjenja element sa zbrojem djece)\n");
        printf("4 - Random generiranje elemenata stabla\n");
        printf("5 - Ispis binarnog stabla u tekstualnu datoteku - inorder\n");
        printf("6 - izlaz\n");
        printf("[----->");
        scanf("%d", &command);

        switch (command) {
        case 1:
            printf("Unesite vrijednost novog elementa: ");
            scanf("%d", &el);
            insert(el, root);
            break;
        case 2:
            inorder(root);
            break;
        case 3:
            replace(root);
            break;
        case 4:
            random(root);
            break;
        case 5:
            inorderInFile(root, filePointer);
            break;
        case 6:
            printf("Izlaz\n");
            break;
        default:
            printf("Ne postoji ta naredba\n");
        }
    } while (command != 6);

    fclose(filePointer);
    return 0;
}

Position insert(int el, Position root) {
    if (root == NULL) {
        Position newElement = (Position)malloc(sizeof(BinaryTree));
        if (newElement == NULL) {
            printf("Krivo alocirana memorija\n");
            return -1;
        }
        else {
            newElement->el = el;
            newElement->left = NULL;
            newElement->right = NULL;
        }
        root = newElement;
    }
    else if (el < root->el) {
        root->left = insert(el, root->left);
    }
    else if (el >= root->el) {
        root->right = insert(el, root->right);
    }
    return root;
}

int inorder(Position root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->el);
        inorder(root->right);
    }
    return 0;
}

int replace(Position root) {
    if (root == NULL) { //ako je root dosa do kraja vraca se 0 
        return 0;
    }

    int sumLeft = 0, sumRight = 0;

    sumLeft += replace(root->left);
    sumRight += replace(root->right);

    int temp = root->el;
    root->el = sumLeft + sumRight;
    return temp + root->el;
}

int random(Position root) {
    int randomnumber = 0;
    randomnumber = rand() % (90-10) + 11;
    printf("%d ", randomnumber);
    insert(randomnumber, root);
    return 0;
}

int inorderInFile(Position root, FILE* filePointer) {
    if (root != NULL) {
        inorderInFile(root->left, filePointer);
        fprintf(filePointer, "%d ", root->el);
        inorderInFile(root->right, filePointer);
    }
    return 0;
}