/* Definirati strukturu osoba (ime, prezime, godina rodenja) i napisati program koji:
 A. dinamicki dodaje novi element na po?etak liste,
 B. ispisuje listu,
 C. dinamicki dodaje novi element na kraj liste,
 D. pronalazi element u listi (po prezimenu),
 E. briše odredeni element iz liste,
 U zadatku se ne smiju koristiti globalne varijable.*/
 /*3. Prethodnom zadatku dodati funkcije:
 A. dinamicki dodaje novi element iza odredenog elementa,
 B. dinamicki dodaje novi element ispred odredenog elementa,
 C. sortira listu po prezimenima osoba,
 D. upisuje listu u datoteku,
 E. cita listu iz datoteke.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define MAX_LINE 1024
#define SCANF_ERROR -1
#define MALLOC_ERROR -2
#define FILE_OPEN_ERROR -3

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
int addPersonAfter(Position, char*);
int addPersonBefore(Position, char*);
int writeInFile(Position);
int readFile(Position); //reads and stores file content into list

int main() {
	Person head = { .Next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
	//Person headR = { .Next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
	char input;
	char surname[MAX_LINE];
	FILE* f = NULL;
	printf("\nActions:\nf-add to front of the list\ne-add to end of the list\np-print list\ns-find person by surname\nd-delete person\na-add person after");
	printf("\nb-add person before\nw-write in file\nr-read from file");
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
		case 'a':
			printf("\nAdd after person with surname: ");
			if (scanf(" %s", surname) != 1) return SCANF_ERROR;
			addPersonAfter(&head, surname);
			break;
		case 'b':
			printf("\nAdd person before: ");
			if (scanf(" %s", surname) != 1) return SCANF_ERROR;
			addPersonBefore(&head, surname);
			break;
		case 'w':
			printf("\nWriting list into file...");
			writeInFile(head.Next);
			break;
		case 'r':
			printf("\nReading list from file...");
			readFile(&head);
			printList(head.Next);
			break;
		case '0':
			//end program
			break;
		default:
			printf("\nInvalid input. Try again.");
			break;
		}
	} while (input != '0');
	deleteList(&head);
	//deleteList(&headR);
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

int addPersonAfter(Position head, char* sur) {
	Position newPerson = NULL;
	Position personBySurname = NULL;
	personBySurname = findPersonBySurname(head, sur);

	if (personBySurname != NULL) {
		newPerson = createPerson();
		if (!newPerson) {
			printf("\nMalloc error!");
			return MALLOC_ERROR;
		}
		newPerson->Next = personBySurname->Next;
		personBySurname->Next = newPerson;
	}
	return EXIT_SUCCESS;
}

int addPersonBefore(Position head, char* sur) {
	Position newPerson = NULL;
	Position personBeforeSurname = NULL;
	personBeforeSurname = findPersonPrevious(head, sur);
	if (personBeforeSurname != NULL) {
		newPerson = createPerson();
		if (!newPerson) {
			printf("\nMalloc error!");
			return MALLOC_ERROR;
		}
		newPerson->Next = personBeforeSurname->Next;
		personBeforeSurname->Next = newPerson;
	}
	return EXIT_SUCCESS;
}

int writeInFile(Position current) {
	FILE* f = NULL;
	f = fopen("person.txt", "w");
	if (f == NULL) {
		printf("\nFile could not be opened!");
		return FILE_OPEN_ERROR;
	}
	while (current != NULL) {
		fprintf(f, " %s %s %d\n", current->name, current->surname, current->birthYear);
		current = current->Next;
	}
	fclose(f);
	printf("\nSuccess!");

	return EXIT_SUCCESS;
}


int readFile(Position head) {
	FILE* f = NULL;
	f = fopen("person.txt", "r");
	if (f == NULL) {
		printf("\nFile could not be opened!");
		return FILE_OPEN_ERROR;
	}
	Position prev = NULL;
	Position current = NULL;

	deleteList(head);
	prev = head;

	while (!feof(f)) {
		current = (Position)malloc(sizeof(Person));
		if (!current) {
			printf("\Malloc error!");
			return MALLOC_ERROR;
		}
		if (fscanf(f, " %s %s %d", current->name, current->surname, &current->birthYear) != 3) return SCANF_ERROR;

		current->Next = prev->Next;
		prev->Next = current;

		prev = current;
	}
	fclose(f);

	return EXIT_SUCCESS;
}