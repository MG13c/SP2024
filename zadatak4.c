//		4. Napisati program za zbrajanje i mnozenje polinoma.
//		Koeficijenti i eksponenti se citaju iz datoteke.
//		Napomena: Eksponenti u datoteci nisu nuzno sortirani.


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)
#define MAX_SIZE (50)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)
#define EMPTY_LISTS (-2)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
	int coefficient;
	int exponent;
	Position next;
} Element;

int readFile(Position headPoly1, Position headPoly2, char* fileName);
int printPoly(char* polynomeName, Position first);
int addPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2);
int multiplyPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2);
int freeMemory(Position head);
int parseStringIntoList(Position head, char* buffer);
Position createElement(coefficient, exponent);
int insertSorted(Position head, Position newElement);
int mergeAfter(Position current, Position newElement);
int insertAfter(Position current, Position newElement);
int deleteAfter(Position current);

int main()
{
	Element headPoly1 = { .coefficient = 0, .exponent = 0, .next = NULL };
	Element headPoly2 = { .coefficient = 0, .exponent = 0, .next = NULL };
	Element headPolyAdd = { .coefficient = 0, .exponent = 0, .next = NULL };
	Element headPolyMultiply = { .coefficient = 0, .exponent = 0, .next = NULL };
	char* fileName = "polynomes.txt";

	if (readFile(&headPoly1, &headPoly2, fileName) == EXIT_SUCCESS) {

		printPoly("First polynome: ", headPoly1.next);
		printPoly("Second polynome: ", headPoly2.next);

		addPoly(&headPolyAdd, headPoly1.next, headPoly2.next);
		multiplyPoly(&headPolyMultiply, headPoly1.next, headPoly2.next);

		printPoly("Added polynome: ", headPolyAdd.next);
		printPoly("Multiplied polynome: ", headPolyMultiply.next);

		freeMemory(&headPoly1);
		freeMemory(&headPoly2);
		freeMemory(&headPolyAdd);
		freeMemory(&headPolyMultiply);

	}

	return EXIT_SUCCESS;
}

int readFile(Position headPoly1, Position headPoly2, char* fileName) {
	FILE* filePointer = NULL;
	char buffer[MAX_LINE] = { 0 };
	int status = EXIT_SUCCESS;

	filePointer = fopen(fileName, "r");

	if (!filePointer) {
		printf("Can't open the file!\n");
		return FILE_NOT_OPEN;
	}

	fgets(buffer, MAX_SIZE, filePointer);
	status = parseStringIntoList(headPoly1, buffer);
	if (status != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	fgets(buffer, MAX_SIZE, filePointer);
	status = parseStringIntoList(headPoly2, buffer);
	if (status != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int printPoly(char* polynomeName, Position first) {
	printf(" %s", polynomeName);
	if (first != NULL) {
		if (first->exponent < 0) {
			if (first->coefficient == 1) {
				printf("x^(%d)", first->exponent);
			}
			else {
				printf("%dx^(%d)", first->coefficient, first->exponent);
			}
		}
		else {
			if (first->coefficient == 1) {
				printf("x^%d", first->exponent);
			}
			else {
				printf("%dx^%d", first->coefficient, first->exponent);
			}
		}

		first = first->next;
	}

	for (; first != NULL; first = first->next) {
		if (first->coefficient < 0) {
			if (first->exponent < 0) {
				printf(" - %dx^(%d)", first->coefficient, first->exponent);
			}
			else printf(" - %dx^%d", first->coefficient, first->exponent);
		}
		else {
			if (first->exponent < 0) {
				if (first->coefficient == 1) {
					printf(" + x^(%d)", first->exponent);
				}
				else {
					printf(" + %dx^(%d)", first->coefficient, first->exponent);
				}
			}
			else {
				if (first->coefficient == 1) {
					printf(" + x^%d", first->exponent);
				}
				else {
					printf(" + %dx^%d", first->coefficient, first->exponent);
				}
			}
		}
	}

	printf("\n");
	return EXIT_SUCCESS;
}

int parseStringIntoList(Position head, char* buffer) {
	char* currentBuffer = buffer;
	int coefficient = 0;
	int exponent = 0;
	int numBytes = 0;
	int status = 0;
	Position newElement = NULL;

	while (strlen(currentBuffer) > 0) {
		status = sscanf(currentBuffer, " %dx^%d %n", &coefficient, &exponent, &numBytes);

		if (status != 2) {
			//ovaj if ce bit istina ako imamo smo je sscanf procita npr 2 podatka koji nisu valjanih tipova
			printf("Datoteka nije valjana.");
			printf("File not good.\n");
			return EXIT_FAILURE;
		}
		//ako je prosa prethodni if, onda mozemo ic dalje stvarat element
		newElement = createElement(coefficient, exponent);
		if (newElement == NULL) {
			return EXIT_FAILURE;
		}

		insertSorted(head, newElement);
		//posto je currentBuffer tipa char*, onda kad napisemo currentBuffer to oznacava i adresu cijelog niza znakova, ali i adresu prvog clana(currentBuffer+0)
	   //zato, kada napisemo ovo ispod, currentBuffer= currentBuffer + brMjesta, ako smo u prethodnom citanju i sscanf-iranju ucitali iz buffera npr 1 2 iz...
	   //... buffera koji sadrzi 1 2 8 9 15 12, onda ce brMjesta bit 4 a ova linija ispod ce izgledat ovako currentBuffer= currentBuffer + 4; odnosno ...
	   // currentBuffer ce se smanjit i ovime ce sada pocinjat od svog prijasnjeg 4.elementa, i tako cemo ostvarit da hvatamo tocno dva po dva broja

		currentBuffer += numBytes;
	}

	return EXIT_SUCCESS;
}

Position createElement(coefficient, exponent) {
	Position element = NULL;

	element = (Position)malloc(sizeof(Element));
	if (element == NULL) {
		printf("Can't allocate memory!");
		return FAILED_MEMORY_ALLOCATION;
	}

	element->coefficient = coefficient;
	element->exponent = exponent;
	element->next = NULL;

	return element;
}
int insertSorted(Position head, Position newElement) {
	Position current = head;

	while (current->next != NULL && current->next->exponent > current->exponent) {
		current = current->next;
	}

	mergeAfter(current, newElement);

	return EXIT_SUCCESS;
}

int mergeAfter(Position current, Position newElement) {

	if (current->next == NULL || current->next->exponent != newElement->exponent) {
		insertAfter(current, newElement);
	}
	else {
		int resultCoefficient = current->next->coefficient + newElement->coefficient;

		if (resultCoefficient == 0) {
			deleteAfter(current);
		}
		else {
			current->next->coefficient = resultCoefficient;
		}
		free(newElement);
	}

	return EXIT_SUCCESS;
}

int insertAfter(Position current, Position newElement) {

	newElement->next = current->next;
	current->next = newElement;

	return EXIT_SUCCESS;
}

int freeMemory(Position head) {

	Position current = head;

	while (current->next != NULL) {
		deleteAfter(current);
	}

	return EXIT_SUCCESS;
}

int deleteAfter(Position current) {

	Position toDelete = NULL;

	toDelete = current->next;
	current->next = toDelete->next;
	free(toDelete);

	return EXIT_SUCCESS;
}
int createAndInsertAfter(int coefficient, int exponent, Position current) {
	Position newElement = createElement(coefficient, exponent);

	if (newElement == NULL) {
		return EXIT_FAILURE;
	}

	insertAfter(current, newElement);

	return EXIT_SUCCESS;
}
int addPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2) {
	Position currentPoly1 = firstElementPoly1;
	Position currentPoly2 = firstElementPoly2;
	Position currentResult = resultHead;
	Position remainingPoly = NULL;

	while (currentPoly1 != NULL && currentPoly2 != NULL) {
		if (currentPoly1->exponent == currentPoly2->exponent) {
			createAndInsertAfter(currentPoly1->coefficient + currentPoly2->coefficient, currentPoly1->exponent, currentResult);
			currentPoly1 = currentPoly1->next;
			currentPoly2 = currentPoly2->next;
			currentResult = currentResult->next;
		}
		else if (currentPoly1->exponent < currentPoly2->exponent) {
			createAndInsertAfter(currentPoly1->coefficient, currentPoly1->exponent, currentResult);
			currentPoly1 = currentPoly1->next;
			currentResult = currentResult->next;
		}
		else if (currentPoly1->exponent > currentPoly2->exponent) {
			createAndInsertAfter(currentPoly2->coefficient, currentPoly2->exponent, currentResult);
			currentPoly2 = currentPoly2->next;
			currentResult = currentResult->next;
		}
	}

	if (currentPoly2 == NULL) {
		remainingPoly = currentPoly1;
	}
	else {
		remainingPoly = currentPoly2;
	}

	while (remainingPoly != NULL) {
		createAndInsertAfter(remainingPoly->coefficient, remainingPoly->exponent, currentResult);
		remainingPoly = remainingPoly->next;
		currentResult->next;
	}

	return EXIT_SUCCESS;
}

int multiplyPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2) {

	if (firstElementPoly1 != NULL || firstElementPoly2 != NULL) {
		for (Position currentPoly1 = firstElementPoly1; currentPoly1 != NULL; currentPoly1 = currentPoly1->next) {
			for (Position currentPoly2 = firstElementPoly2; currentPoly2 != NULL; currentPoly2 = currentPoly2->next) {
				Position newElement = createElement(currentPoly1->coefficient * currentPoly2->coefficient, currentPoly1->exponent * currentPoly2->exponent);

				if (newElement == NULL) {
					return EXIT_FAILURE;
				}

				insertSorted(resultHead, newElement);
			}
		}
		return EXIT_SUCCESS;
	}
	return EMPTY_LISTS;
}