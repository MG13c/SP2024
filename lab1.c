//Napisati program koji prvo procita koliko redaka ima datoteka, 
//tj.koliko ima studenata zapisanih u datoteci. Nakon toga potrebno 
//je dinamicki alocirati prostor za niz struktura studenata
// (ime, prezime, bodovi) i ucitati iz datoteke sve zapise.
// Na ekran ispisati ime, prezime, apsolutni i relativni broj bodova.
// Napomena: Svaki redak datoteke sadrži ime i prezime studenta, 
//te broj bodova na kolokviju.relatvan_br_bodova = br_bodova / max_br_bodova * 100
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 50
#define MAX_LINE 1024
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3
#define MAX_POINTS 50

typedef struct student {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	double points;
} Student;

int ReadRowsInFile();

int main() {
	int i = 0;
	int NoRows = 0;
	NoRows = ReadRowsInFile();
	printf("The number of rows is: %d\n", NoRows);
	if (NoRows > 0) {
		FILE* f = NULL;
		f = fopen("studenti.txt", "r");
		if (!f) {
			printf("File has not opened successfully!\n");
			return FILE_ERROR_OPEN;
		}
		Student* s = NULL;
		s = (Student*)malloc(sizeof(Student));
		if (s == NULL) {
			printf("Malloc error!\n");
			return MALLOC_ERROR;
		}
		for (i = 0; i < NoRows; i++) {
			if (fscanf(f, " %s %s %lf ", s[i].name, s[i].surname, &s[i].points) != 3) {
				return SCANF_ERROR;
			}
		}


		for (i = 0; i < NoRows; i++) {
			printf("%s %s %.2lf %.2lf\n", s[i].name, s[i].surname, s[i].points, s[i].points / MAX_POINTS * 100);
		}
		fclose(f);
		free(f);
	}
	return 0;
}

int ReadRowsInFile() {
	int counter = 0;
	char buffer[MAX_LINE] = { '\0' };

	FILE* f = NULL;
	f = fopen("studenti.txt", "r");
	if (!f) {
		printf("File is not opened successfully!\n");
		return FILE_ERROR_OPEN;
	}
	while (!feof(f)) {
		fgets(buffer, MAX_LINE, f);
			counter++;
	}
	fclose(f);
	return counter;
}

