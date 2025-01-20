//10. Napisati program koji ?ita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih dr?ava.Uz
//ime dr?ave u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
//dr?ave.Svaka datoteka koja predstavlja dr?avu sadr?i popis gradova u formatu naziv_grada,
//broj_stanovnika.
//a) Potrebno je formirati sortiranu vezanu listu dr?ava po nazivu.Svaki ?vor vezane liste
//sadr?i stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
//b) Potrebno je formirati stablo dr?ava sortirano po nazivu.Svaki ?vor stabla sadr?i vezanu
//listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _listCity;
typedef struct _listCity* PositionCity;
typedef struct _listCity {
	char name[50];
	int numCitizens;
	PositionCity next;
} City;

struct _binaryTreeCountry;
typedef struct _binaryTreeCountry* PositionCountry;
typedef struct _binaryTreeCountry {
	char name[50];
	PositionCountry left;
	PositionCountry right;
	PositionCity head; //ovo ce vezat vezanu listu gradova na drzavu
} Country;

PositionCountry readCountryNames(PositionCountry root);
PositionCountry insertCountry(PositionCountry root, char* countryName);
PositionCity insertCity(char* fileName);
int insertIntoList(PositionCity Head, int numOfCitizens, char* city);
PositionCity search(PositionCountry root, char* countryName);
int inorder(PositionCountry root);
int printList(PositionCity Head);

int main() {

	PositionCountry root = NULL;


	root = readCountryNames(root);

	inorder(root);

	return 0;
}

PositionCountry readCountryNames(PositionCountry root) {
	FILE* filePointer = NULL;
	filePointer = fopen("drzave.txt", "r");
	if (!filePointer) {
		printf("Nije moguce otvoriti datoteku drzave\n");
		return -1;
	}

	//triba nan root prvog elementa stabla
	char countryName[50] = "", fileName[50] = "";
	fscanf(filePointer, " %s %s", countryName, fileName);
	root = insertCountry(root, countryName);

	PositionCity head = insertCity(fileName);
	root->head = head;

	while (!feof(filePointer)) {
		char countryName[50] = "", fileName[50] = "";
		fscanf(filePointer, " %s %s", countryName, fileName);
		//printf("%s %s\n", countryName, fileName); //provjera
		insertCountry(root, countryName);

		
		PositionCountry currentCountry = insertCountry(root, countryName);

		PositionCity head = insertCity(fileName);
		currentCountry->head = head;
	}
	fclose(filePointer);
	return root;
}

PositionCountry insertCountry(PositionCountry root, char* countryName) {
	if (root == NULL) {
		PositionCountry newElement = (PositionCountry)malloc(sizeof(Country));
		if (newElement == NULL) {
			printf("Krivo alocirana memorija\n");
			return -1;
		}
		else {
			strcpy(newElement->name, countryName);
			newElement->left = NULL;
			newElement->right = NULL;
			newElement->head = NULL;
		}
		root = newElement;
	}
	else if (strcmp(countryName, root->name) < 0) {
		root->left = insertCountry(root->left, countryName);
	}
	else if (strcmp(countryName, root->name) > 0) {
		root->right = insertCountry(root->right, countryName);
	}
	return root;
}

PositionCity insertCity(char* fileName) {
	FILE* filePointer = NULL;
	filePointer = fopen(fileName, "r");
	if (!filePointer) {
		printf("Nije moguce otvoriti datoteku drzave\n");
		return -1;
	}

	City Head = { .name = "", .numCitizens = 0, .next = NULL };

	while (!feof(filePointer)) {
		char city[50] = "";
		int numOfCitizens = 0;
		fscanf(filePointer, " %s %d", city, &numOfCitizens);

		insertIntoList(&Head, numOfCitizens, city);
	}
	fclose(filePointer);
	//printList(&Head);
	return &Head;
}

int insertIntoList(PositionCity Head, int numOfCitizens, char* city) {
	PositionCity newElement = NULL;
	newElement = (PositionCity)malloc(sizeof(City));
	if (!newElement) {
		printf("Kivo alocirana memorija\n");
		return -1;
	}

	strcpy(newElement->name, city);
	newElement->numCitizens = numOfCitizens;
	newElement->next = NULL;

	//sortiranje u listu gradova
	if (Head->next != NULL) {
		while (Head->next != NULL && Head->next->numCitizens > numOfCitizens) {
			Head = Head->next;
		}
		newElement->next = Head->next;
		Head->next = newElement;
	}
	else {
		newElement->next = Head->next;
		Head->next = newElement;
	}
	return 0;
}

PositionCity search(PositionCountry root, char* countryName) {
	if (root == NULL) {
		return NULL;
	}
	else if (strcmp(countryName, root->name) < 0) {
		root->left = search(root->left, countryName);
	}
	else if (strcmp(countryName, root->name) > 0) {
		root->right = search(root->right, countryName);
	}
	else if (strcmp(countryName, root->name) == 0) {
		return root;
	}
	else {
		return root;
	}
}

int inorder(PositionCountry root) {
	if (root != NULL) {
		inorder(root->left);
		printf(" %s", root->name);
		//printList(root->head);
		inorder(root->right);
	}
	return 0;
}

int printList(PositionCity Head) {
	if (Head->next == NULL) {
		printf("Greska nema gradova\n");
		return -1;
	}
	else {
		PositionCity current = Head->next;
		while (current->next != NULL) {
			printf(" %s", current->name);
		}
	}
	return 0;
}