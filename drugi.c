/* Definirati strukturu osoba (ime, prezime, godina ro?enja) i napisati program koji:
 A. dinami?ki dodaje novi element na po?etak liste,
 B. ispisuje listu,
 C. dinami?ki dodaje novi element na kraj liste,
 D. pronalazi element u listi (po prezimenu),
 E. briše odre?eni element iz liste,
 U zadatku se ne smiju koristiti globalne varijable.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define MAX_LINE 1024
#define SCANF_ERROR -1
#define MALLOC_ERROR -2

struct _person;
typedef struct _person* Position;
typedef struct _person {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;

	Position Next;
}Person;

//functions:
Position createPerson();
int addToFrontOfList(Position);
int printPerson(Position);
int printList(Position);
int addTOEndOfList(Position);
Position findPersonBySurname(Position, char*);
Position findPersonPrevious(Position, char*);
int deletePerson(Position, char*);
int deleteList(Position);

int main() {
	Person head = { .Next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
	char input;
	char surname[MAX_LINE];
	printf("\nActions:\nf-add to front of the list\ne-add to end of the list\np-print list\ns-find person by surname\nd-delete person");
	do {
		printf("\nChoose action:");
		if (scanf(" %c", &input) != 1) return SCANF_ERROR;
		switch (input) {
		case 'f':
			addToFrontOfList(&head);
			break;
		case 'e':
			addToEndOfList(&head);
			break;
		case 'p':
			printList(head.Next);
			break;
		case 's':
			printf("\nInsert surname:");
			if (scanf(" %s", surname) != 1) return SCANF_ERROR;
			Position found = findPersonBySurname(head.Next, surname);
			if (found != NULL) {
				printPerson(found);
			}
			else printf("\nPerson with surname %s does not exist", surname);
			break;
		case 'd':
			printf("\nDelete person with surname: ");
			if (scanf(" %s", surname) != 1) return SCANF_ERROR;
			deletePerson(&head, surname);
			break;
		}
	} while (input != '0');
	deleteList(&head);
	return 0;
}

Position createPerson() {
	Position newPerson = NULL;
	newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL) {
		printf("Unsuccessfull!");
		return NULL; 
	}
	printf("\nName: ");
	if (scanf(" %s", newPerson->name) != 1) return SCANF_ERROR;
	printf("\nSurname: ");
	if (scanf(" %s", newPerson->surname) != 1) return SCANF_ERROR;
	printf("\nBirth year: ");
	if (scanf(" %d", &newPerson->birthYear) != 1) return SCANF_ERROR;

	return newPerson;
}

int addToFrontOfList(Position head) {
	Position newPerson;
	newPerson = createPerson();
	if (!newPerson) { 
		printf("\nMalloc error!");
		return MALLOC_ERROR;
	}
	newPerson->Next = head->Next;
	head->Next = newPerson;

	return EXIT_SUCCESS;
}

int printPerson(Position person) {
	printf("\n %s %s born %d.", person->name, person->surname, person->birthYear);
	return EXIT_SUCCESS;
}

int printList(Position current) {
	if (current == NULL) {
		printf("\nThis list is empty!");
	}
	while (current != NULL) {
		printPerson(current);
		current = current->Next;
	}
	return EXIT_SUCCESS;
}

int addToEndOfList(Position head) {
	Position newPerson;
	newPerson = createPerson();
	if (!newPerson) {
		printf("\nMalloc error!");
		return MALLOC_ERROR;
	}
	while (head->Next != NULL) {
		head = head->Next;
	}
	newPerson->Next = head->Next;
	head->Next = newPerson;

	return EXIT_SUCCESS;
}

Position findPersonBySurname(Position current, char* sur) {
	while (current != NULL && strcmp(current->surname, sur)) {
		current = current->Next;
	}
	return current;
}

Position findPersonPrevious(Position current, char* sur) {
	while (current->Next != NULL && strcmp(current->Next->surname, sur)) {
		current = current->Next;
	}
	if (current->Next == NULL) {
		return NULL;
	}
	else return current;
}
int deletePerson(Position head, char* sur) {
	Position temp = NULL;
	Position previous = NULL;

	previous = findPersonPrevious(head, sur);
	if (previous != NULL) {
		temp = previous->Next;
		previous->Next = temp->Next;
		free(temp);
		printf("\nPerson with surname %s is deleted from list.", sur);
	}
	else printf("\nPerson with surname %s does not exist!", sur);

	return EXIT_SUCCESS;
}

int deleteList(Position head) {
	Position temp = NULL;
	while (head->Next != NULL) {
		temp = head->Next;
		head->Next = head->Next->Next;
		free(temp);
	}
	return EXIT_SUCCESS;
}
