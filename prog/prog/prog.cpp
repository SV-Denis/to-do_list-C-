// prog.cpp : This file contains the 'main' function. Program execution begins and ends there.
// !! THIS PROGRAM IS MADE IN VISUAL STUDIO !! 
// it contains  the first 3 lines "_msc_ver" and "_crt_secure_no_warnings" to avoid some errors in visual studio regarding the use of "scanf" and "gets" functions
//the program should run on other compilers too, but i can't guarantee that it will work properly, future tests will be made to ensure that the program works on other compilers too
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define DEBUG
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"  // Resets color to default
const char* colors[] = { RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN };
FILE* fptr;
FILE* fptr1;
struct date
{
    int day;
    int month;
    int year;
};
int done = 0;
void CHAOTIC_TEXT(const char* text) {
	srand(time(NULL)); // Seed the random number generator
	for (int i = 0; text[i] != '\0'; i++) {
		int  random_color = rand() % 6; // Generate a random number between 0 and 5
		printf("%s%c", colors[random_color], text[i]);
	}
	printf("%s", RESET);
}
void header()
{
    printf(GREEN "To" RESET "-" GREEN "do" RESET " app\n");
    char a[] = ("--------------\n");
	CHAOTIC_TEXT(a);
    printf(CYAN "1" RESET " - Add list of tasks\n");
    printf(CYAN"2" RESET " - Display lists\n");
    printf(CYAN"3" RESET " - Delete lists\n");
    printf(CYAN"0" RESET " - Exit\n");
    CHAOTIC_TEXT(a);
}


void logappend(char* fullFilename) {
    fptr = fopen("list-of-notes.txt", "a");
    if (fptr == NULL) {
        printf(RED "The log file is not opened. The program will "
            "now exit." RESET);
        exit(0);
    }
    fprintf(fptr, "%s\n", fullFilename);
    fclose(fptr);
}


void tasks(char* fullFilename) {
    char ask;
    char task[256];
    char fulltask[1024];
    int number;
    fptr1 = fopen(fullFilename, "a");
    if (fptr1 == NULL) {
        printf("The file is not opened. The program will "
            "now exit.");
        exit(0);
    }
    printf("add " CYAN "task" RESET " ? " GREEN "Y" RESET "/" RED "N" RESET);
    getchar();
    scanf(" %c", &ask);
    number = 1;
    while (ask == 'Y' or ask == 'y')
    {
        printf(CYAN "task" RESET ":");
        getchar();
        scanf("%[^\n]s", task);
        fprintf(fptr1, "%d - [ ] %s\n", number, task);
        system("cls");
        printf("add " CYAN "task" RESET " ? " GREEN "Y" RESET "/" RED "N" RESET);
        getchar();
        scanf(" %c", &ask);
        number++;
    }
    fclose(fptr1);
}

void refresh_list(int option) {
    fptr = fopen("list-of-notes.txt", "r");
    fptr1 = fopen("copy-of-notes.txt", "w");
    char line[256];
	char final[256];
    int o;
    int i;
    i = 1;
    const char* deletion = "copy-of-notes.txt";
    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0;
        if (sscanf(line, "%d - %[^\n]", &o, final) == 2)
        {
            if (isdigit(line[0]))
            {
                if (o != option)
                {
                    fprintf(fptr1, "%d - %s\n", i, final);
                    i++;
                }
            }
		}
        else {
			fprintf(fptr1, "%d - %s\n", i, line);
			i++;
        }
    }
    fclose(fptr);
    fclose(fptr1);
    fptr1 = fopen("copy-of-notes.txt", "r");
    fptr = fopen("list-of-notes.txt", "w");
    while (fgets(line, sizeof(line), fptr1)) {
        fprintf(fptr, "%s",line);
    }
    fclose(fptr);
    fclose(fptr1);
    if (remove(deletion) != 0) {
        // If error occurs, print the error description using strerror
        printf("Error deleting file: %s\n", strerror(errno));
    }
}
void add() {
    char filename[100];
    /*int day, month, year;
    char date[11];*/
    int check;
	struct date curentDate;
	bool boolcheck = false;
    system("cls");
    printf("write the " CYAN "name" RESET " of the " CYAN "file" RESET " (" RED "without extension" RESET ") :\n");
    getchar();
    scanf("%s", filename);
    char fullFilename[110];
    snprintf(fullFilename, sizeof(fullFilename), "%s.txt", filename);
    fptr1 = fopen(fullFilename, "w");
    if (fptr1 == NULL) {
        printf("The file is not opened. The program will "
            "now " RED "exit." RESET);
        exit(0);
    }
	system("cls");
	printf("enter the " CYAN "date" RESET " (" GREEN "DD" RESET"." GREEN "MM" RESET "." GREEN "YYYY" RESET ") : ");
	getchar();
	check = scanf("%d.%d.%d", &curentDate.day, &curentDate.month, &curentDate.year);
    if (check != 3)
    {
		printf(RED "Invalid input! " RESET " Please enter the date in the format " GREEN "DD" RESET"." GREEN "MM" RESET "." GREEN "YYYY" RESET ".\n");
		printf(RED "The program will now exit." RESET);
        fclose(fptr1);
        remove(fullFilename);
        exit(0);
    }
	system("cls");
	if (curentDate.day > 31 || curentDate.month > 12 || curentDate.day < 1 || curentDate.month < 1 || curentDate.year < 1)
	{
		printf(RED "Invalid" RESET " input format!\n");
		fclose(fptr1);
		remove(fullFilename);
		exit(0);
	}
	fprintf(fptr1, "-= Tasks %02d.%02d.%04d =-\n", curentDate.day, curentDate.month, curentDate.year);
	fclose(fptr1);
    tasks(fullFilename);
    logappend(fullFilename);
    refresh_list(0);
    system("cls");
}
void delete_selector(char* cfilename, int list)
{
    int number;
    char line[256];
    fptr = fopen("list-of-notes.txt", "r");
    while (fgets(line, sizeof(line), fptr)) {
        if (isdigit(line[0]))
            if (sscanf(line, "%d", &number) == 1) {
                if (number == list)
                {
                    sscanf(line, "%*d - %[^\n]", cfilename);
                    cfilename[strcspn(cfilename, "\n")] = '\0';
                    remove(cfilename);
                    refresh_list(number);
					done = 1;
                }
            }
    }
    fclose(fptr);
	list = 0;
}
void refresh_task(char* cfilename,int task,int list) {
    FILE* point;
    fptr1 = fopen("copy-of-notes.txt", "w");
    point = fopen(cfilename, "r");
    char line[256];
    int i=0;
    int conf=0;
    char s,t[4];
    int nr;
    char liner[256];
    const char* deletion = "copy-of-notes.txt";
    while (fgets(line, sizeof(line), point)) {
        fprintf(fptr1, "%s",line);
    }
    fclose(point);
    fclose(fptr1);
    fptr1 = fopen("copy-of-notes.txt", "r");
    if (fptr1 == NULL) {
        printf("The file is not opened. The program will "
            "now exit.");
        exit(0);
    }
    point = fopen(cfilename, "w");
    if (point == NULL) {
        printf("The file is not opened. The program will "
            "now exit.");
        exit(0);
    }
    while (fgets(line, sizeof(line), fptr1)) {
        char* openBracket = strchr(line, '[');
		if (openBracket != NULL) {
			if (openBracket[1] == 'x') {
				i++;
				conf++;
                int taskFromLine;
                if (sscanf(line, "%d", &taskFromLine) == 1) {
                    if (taskFromLine == task) {
                        printf("task " CYAN "%d" RESET " is already " GREEN "completed\n" RESET, task);
                        task = 0;
                    }
                }
			}
			else if (openBracket[1] == ' ') {
				i++;
			}
		}
        if (sscanf(line, "%s", t) == 1 && strcmp(t, "-=") == 0) {
            fprintf(point, "%s", line);
        }
        if (isdigit(line[0]))
            if (sscanf(line, "%d", &nr) == 1)
            {
                if (nr == task)
                {
                    sscanf(line, "%*d - [ ] %s", liner);
                    fprintf(point, "%d - [x] %s\n", task, liner);
                    conf++;
                }
                else {
                    fprintf(point, "%s", line);
                }
            }
    }
    fclose(point);
    fclose(fptr1);
    if (remove(deletion) != 0) {
        // If error occurs, print the error description using strerror
        printf("Error deleting file: %s\n", strerror(errno));
    }
    if (i == conf)
    {
        char in;
        delete_selector(cfilename,list);
		printf("All tasks are " GREEN "completed!\n" RESET);
        printf("Press " CYAN "enter" RESET " to continue");
        getchar();
		while (getchar() != '\n');
		return;
	}
    if (task != 0)
    {
        printf(GREEN "Task " CYAN "%d" RESET " is " GREEN "completed!\n" RESET, task);
    }
}
struct reader
{
	int optioner;
	char tasker;
	char liner[256];
};
void display() {
    int option;
    char line[256];
    int list;
    char cfilename[256];
    char ask;
    int task;
    struct date current;
    struct reader reader = { 0 };
    system("cls");
    fptr = fopen("list-of-notes.txt", "r");
    if (fptr == NULL) {
        printf("The file is not opened. The program will "
            "now exit.");
        exit(0);
    }
    while (fgets(line, sizeof(line), fptr)) {
		line[strcspn(line, "\n")] = 0;
		if (sscanf(line, "%d - %[^\n]", &list, cfilename) == 2)
		{
			printf(CYAN "%d" RESET " - %s\n", list, cfilename);
		}
    }
    printf("\n");
	printf("do you want to see a " CYAN "note?" GREEN " Y" RESET "/" RED "N" RESET ": ");
	getchar();
	scanf(" %c", &ask);
    if (ask == 'N' || ask == 'n')
    {
		return;
    }
    printf("select the " CYAN "note" RESET ": ");
    (void)getchar();
    scanf("%d", &option);
    system("cls");
    rewind(fptr);
    while (fgets(line, sizeof(line), fptr)) {
        if (isdigit(line[0]))
        {
            if (sscanf(line, "%d", &list) == 1) {
                if (list == option)
                {
                    sscanf(line, "%*d - %[^\n]", cfilename);
                    cfilename[strcspn(cfilename, "\n")] = '\0';
                    fptr1 = fopen(cfilename, "r");
                    if (fptr1 == NULL) {
                        printf("The file is not opened. The program will "
                            "now " RED "exit." RESET);
                        exit(0);
                    }
                    while (fgets(line, sizeof(line), fptr1))
                    {
                        line[strcspn(line, "\n")] = 0;
                        if (sscanf(line, "%d - [%c] %[^\n]", &reader.optioner, &reader.tasker, reader.liner) == 3)
                        {
                            if (reader.tasker == 'x')
                            {
                                printf(CYAN "%d" RESET " - [" GREEN "%c" RESET "] %s\n", reader.optioner, reader.tasker, reader.liner);
                            }
                            else if (reader.tasker == ' ')
                            {
                                printf(CYAN "%d" RESET " - [ ] " YELLOW "%s\n" RESET, reader.optioner, reader.liner);
                            }
                        }
                        else
                        {
                            sscanf(line, "-= Tasks %d.%d.%d =", &current.day, &current.month, &current.year);
                            printf("Tasks " GREEN "%02d" RESET "." GREEN "%02d" RESET "." GREEN "%04d" RESET "\n", current.day, current.month, current.year);
                        }
                    }
                        fclose(fptr1);
                        printf("\n do you want to mark a " CYAN "task" RESET " as complete? " GREEN "Y" RESET "/" RED "N" RESET ": ");
                        (void)getchar();
                        scanf(" %c", &ask);
                        while (ask == 'Y' or ask == 'y')
                        {
                            printf("select " CYAN "task:" RESET);
                            (void)getchar();
                            scanf("%d", &task);
                            refresh_task(cfilename, task, list);
                            if (done == 1)
                            {
                                fclose(fptr);
                                done = 0;
                                system("cls");
                                return;
                            }
                            printf("\n do you want to mark a " CYAN "task" RESET " as complete?" GREEN " Y" RESET "/" RED "N" RESET ": ");
                            (void)getchar();
                            scanf(" %c", &ask);
                        }
                }
            }
        }
    }
    fclose(fptr);
    system("cls");
}


void ddelete()
{
    int option;
    char line[256];
    int number;
    char cfilename[256];
    char ask;
    int task;
    int list;
    system("cls");
    fptr = fopen("list-of-notes.txt", "r");
    if (fptr == NULL) {
        printf("The file is not opened. The program will "
            "now exit.");
        exit(0);
    }
    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0;
        if (sscanf(line, "%d - %[^\n]", &list, cfilename) == 2)
        {
            printf(CYAN "%d" RESET " - %s\n", list, cfilename);
        }
    }
    printf("\n");
    printf("do you want to " RED "delete" RESET " a" CYAN " note?" GREEN " Y" RESET "/" RED "N" RESET ": ");
    getchar();
    scanf(" %c", &ask);
    if (ask == 'N' or ask == 'n')
    {
        return;
    }
    printf("select the " CYAN "note" RESET ": ");
    (void)getchar();
    scanf("%d", &option);
    rewind(fptr);
    while (fgets(line, sizeof(line), fptr)) {
        if (isdigit(line[0]))
        if (sscanf(line, "%d", &number) == 1) {
            if (number == option)
            {
                sscanf(line, "%*d - %[^\n]", cfilename);
                cfilename[strcspn(cfilename, "\n")] = '\0';
				remove(cfilename);
				refresh_list(number);
            }
        }
    }
    fclose(fptr);
    system("cls");
}


void menu(int op)
{
    switch (op)
    {
    case 1:
        add();
        break;
    case 2:
        display();
        break;
    case 3:
        ddelete();
        break;
    default:
        printf("EXIT!\n");
    }
}


int main()
{
    int option;

    do {
		system("cls");
        header();
        printf("Enter " CYAN "option" RESET": ");
        scanf("%d", &option);
        menu(option);
    } while (option > 0 && option <= 3);

    return 0;
}

// !! THIS PROGRAM IS MADE IN VISUAL STUDIO !!
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
