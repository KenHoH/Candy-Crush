#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define KMAG  "\x1B[35m"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"
#define KCYN  "\x1B[36m"

#define row 6
#define col 6

// temp array for containing the user
char user[100][100];
int totalUser = 0;
int fullExit = 0;

// hashtable struct
struct Node {
	char name[100];
	char pass[100];
	int score;
	
	struct Node* prev;
	struct Node* next;
};

struct Node* head[36];
struct Node* tail[36];

struct Node* create(char name[], char pass[], int score)
{
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	
	strcpy(temp->name, name);
	strcpy(temp->pass, pass);
	temp->score = score;
	temp->prev = NULL;
	temp->next = NULL;
	
	return temp;
}

int hashing(char name[])
{
	int key = name[0] - 'a';
	// return the hash key
	return key;	
}

// if adding new user, score = 0 
void insertNode(char name[], char pass[], int score)
{
	int index = hashing(name);
	struct Node* newNode = create(name, pass, score);
	
	//if there isnt any node
	if(head[index] == NULL)
	{
		head[index] = tail[index] = newNode;
	}
	// there is a node and the head is already there
	else
	{
		// if head is exist, then we need to pushTail
		// [a] -> null 
		//  t
		// [a] -> newNode
		tail[index]->next = newNode;
		newNode->prev = tail[index];
		
		// new tail is new node
		// [a] -> [new] -> null
		//          t
		tail[index] = newNode;
		
	}
};

int userExist = 0;
// search username in the hash table
void searchUser(char name[])
{
	// loop hash
	for(int i = 0; i < 36; i++)
	{
		// head isnt exist yet
		if(head[i] == NULL)
		{
			userExist = 0;
		}
		// there is a head
		else
		{
			// loop from head to the end
			struct Node* curr = head[i];
			while(curr)
			{
				printf("Name : %s = %s\n", curr->name, name);
				if(strcmp(curr->name, name) == 0) 
				{
					//user exist
					userExist = 1;
					return;
				}
				curr = curr->next;
			}
			// loop finished and still didnt find the user
			// user didnt exist
			// so we return false;
			userExist = 0;
		}
	}
	printf("USER EXIST = %d\n",userExist);
	return;
}

int validPass = 0;
void checkPass(char name[], char pass[])
{
	for(int i = 0; i < 36; i++)
	{
		if(head[i] == NULL)
		{
			validPass = 0;
		}
		else
		{
			struct Node* curr = head[i];
			
			while(curr)
			{
				if(strcmp(curr->name, name) == 0 && strcmp(curr->pass, pass) == 0)
				{
					validPass = 1;
					return;
				}
				curr = curr->next;
			}
			validPass = 0;
		}
	}
	return;
}

void printAll()
{
	for(int i = 0; i < 36; i++)
	{
		if(head[i] == NULL)
		{
			printf("%d = No data\n", i + 1);
		}
		// there is a head node
		else{
			// itterate like using linked list
			// from head to tail
			struct Node* curr = head[i];
			printf("%d ", i + 1);
			while(curr != NULL)
			{
				printf("-> %s %s %d", curr->name, curr->pass, curr->score);
				curr = curr->next;
			}
			printf("\n");
		}
	}
}

// call update node when game is finished
// search by user name only
// insert the name that we want to update and the score that we wanted to change to
void updateNode(char name[], int score)
{
	for(int i = 0; i < 36; i++)
	{
		if(head[i] == NULL)
		{
			printf("No data\n");
		}
		else{
			struct Node* curr = head[i];
			while(curr != NULL)
			{
				if(strcmp(curr->name, name) == 0) 
				{
					printf("Found the user\n");
					// temp exchange
					char tempName[100]; int tempScore;
					strcpy(tempName, curr->name);
					tempScore = curr->score;
					//update the user score
					int newScore = score + tempScore;
					curr->score = newScore;
				}
				curr = curr->next;
			}
		}
	}
}

// double link for sorting the user from hash table using priority queue
struct Link {
	char name[100];
	int score;
	
	struct Link* prev;
	struct Link* next;
};

struct Link* first = NULL; // first means head
struct Link* last = NULL;  // last means tail

struct Link* createLink(char name[], int score)
{
	struct Link* newLink = (struct Link*)malloc(sizeof(struct Link));
	
	strcpy(newLink->name, name);
	newLink->score = score;
	newLink->next = NULL;
	newLink->prev = NULL;
	
	return newLink;
}

// HEAD = FIRST, LAST = TAIL
void pushTail(char name[], int score)
{
	struct Link* temp = createLink(name, score);
	// if no node
	if(first == NULL)
	{
		first = last = temp; 
	}	
	else{
		last->next = temp;
		temp->prev = last;
		last =  temp;
	}
}

void pushHead(char name[], int score)
{
	struct Link* temp = createLink(name, score);
	
	if(first == NULL)
	{
		first = last = temp;
	}
	else
	{
		first->prev = temp;
		temp->next = first;
		first = temp;
	}
}

//  t  b
//  a2 b2 b1
//  a3
void smartPush(char name[], int score)
{
	// create node
	struct Link* temp = createLink(name, score);
	// get curr
	struct Link* curr = first;
	if(first == NULL)
	{
		first = last = temp;
		return;
	}
	else{
		if(curr->score < score) pushHead(name, score);
		else if(last->score >= score) pushTail(name, score);
		else{
			// PUSH MID
			struct Link* curr = first;
			while(curr->next->score >= score)
			{
				curr = curr->next;
			}
			temp->next = curr->next;
			curr->next->prev = temp;
			curr->next = temp;
			temp->prev = curr;
			return;
		}
	}
}

void sortUser()
{
//	printf("Sort User\n");
	for(int i = 0; i < 36; i++)
	{
		if(head[i] == NULL)
		{
//			printf("No data\n");
			continue;
		}
		else
		{
			struct Node* curr = head[i];
			while(curr != NULL)
			{
				char tempName[100];
				int tempScore;
				strcpy(tempName, curr->name);
				tempScore = curr->score;
				smartPush(tempName, tempScore);
				curr = curr->next;
			}
		}
	}
}

void printSortUser()
{
	printf("\033[0;35m All USER \033[0;37m\n");
	struct Link* curr = first;
	printf("\033[0;32m");
	printf("         Name  |    Score  |\n");
	printf("============================\n");
	if(first == NULL)
	{
		printf("	   NO DATA\n");
		return;
	}
	else
	{
		while(curr)
		{
			printf("%13s  |   %5d   |\n", curr->name, curr->score);
			curr = curr->next;
		}
	}
	printf("============================\033[0;37m\n");
	return;
}

void deleteAll()
{
	struct Link* curr = first;
	if(first == NULL)
	{
		return;
	}
	while(curr)
	{
		struct Link* temp = curr;
		first = curr->next;
		curr = curr->next;
		free(temp);
		temp = NULL;
	}
}

// encrypt the user password if contain alphabet
void encrypt(char pass[])
{
	int len = strlen(pass);
	for(int i = 0; i < len; i++)
	{
		char temp = pass[i];
		// check if number
		if(isdigit(temp) != 0) continue;
		// not number
		else
		{
			int newChar = temp + 10;
			newChar = newChar > 122 ? newChar - 26 : newChar;
			pass[i] = newChar;
		}
	}
}

void decrypt(char pass[])
{
	int len = strlen(pass);
	for(int i = 0; i < len; i++)
	{
		char temp = pass[i];
		if(isdigit(temp) != 0) continue;
		else{
			int newChar = temp - 10;
			newChar = newChar < 97 ? newChar + 26 : newChar;
			pass[i] = newChar;
		}
	}
}

// everytime user finished register 
// appending all the node in the hash into a file
void writeFile()
{	
	FILE *file = fopen("data.txt", "w");

	for(int i = 0; i < 36; i++)
	{
		if(head[i] == NULL)
		{
			printf("%d = No data\n", i + 1);
		}
		// there is a head node
		else{
			// itterate like using linked list
			// from head to tail
			struct Node* curr = head[i];
			while(curr != NULL)
			{
				// format Username#HashedPassword#HighScore
				// encrypt the pass in the file only not the hash
				char tempPass[100];
				strcpy(tempPass, curr->pass);
				encrypt(tempPass);
				// write the file
				fprintf(file, "%s#%s#%d\n", curr->name, tempPass, curr->score);
				curr = curr->next;
				// refresh the temPass
				strcpy(tempPass, "");
			}
		}
	}
	fclose(file);
}

void readFile()
{	
	FILE *file = fopen("data.txt", "r");
	
	if ( file == NULL )
	{
	    printf( "Could not open source file\n" );
	    exit(1);
	} 
	
	char userName[100];
	char pass[100];
	int score;
	while(!feof(file))
	{
		fscanf(file, "%[^#]#%[^#]#%d\n", userName, pass, &score);
//		printf("username : %s pass : %s\n", userName, pass);
		char tempPass[100];
		strcpy(tempPass, pass);
		decrypt(tempPass);
//		printf("username : %s decc pass : %s\n",userName, tempPass);
		insertNode(userName, tempPass, score);
		strcpy(tempPass, "");
	}
//	char goBack; printf("Debug enter..."); scanf("%c", &goBack); if(goBack == '\n') return;	
	fclose(file);
}


int map[row][col];
int moves = 10;
int score = 0;
void generate()
{
	srand(time(0));
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			int candy = rand() % 4 + 1;
			map[i][j] = candy;
		}
	}
}

void update();

void swap(int x, int y, char dir)
{
	printf("x : %d, y : %d, dir : %c\n", x, y, dir);
	int temp;
	switch(dir)
	{
		// x = y = row
		// y = x = col
		case 'w' : 
			temp = map[x][y];
			map[x][y] = map[x-1][y];
			map[x-1][y] = temp;
			temp = 0;
			break;
		case 's' : 
			temp = map[x][y];
			map[x][y] = map[x+1][y];
			map[x+1][y] = temp;
			temp = 0;
			break;
		case 'a' :
			temp = map[x][y];
			map[x][y] = map[x][y-1];
			map[x][y-1] = temp; 
			temp = 0;
			break;
		case 'd' : 
			temp = map[x][y];
			map[x][y] = map[x][y+1];
			map[x][y+1] = temp; 		
			temp = 0;
			break;
	}
	moves--;
};


void draw();

int loopAgain = 1;
void logic()
{
	printf("Thinking...\n");
	
	int finish = 1;
//	Crush the cols
//  checking for every column in 1 row, horizontal check
	for(int i = 0; i < row; i++)
	{
//		draw();
		// create window checker 
		for(int j = 0; j < col - 2; j++)
		{
			int num1 = abs(map[i][j]);
			int num2 = abs(map[i][j+1]);
			int num3 = abs(map[i][j+2]);
//			printf("(%d, %d) %d, %d, %d\n", i, j, num1, num2, num3);
			// checking if the 3 number are the same
			// if the same tag them
			if(num1 == num2 && num1 == num3 && num1 != 0)
			{
				map[i][j] = -num1;
				map[i][j+1] = -num2;
				map[i][j+2] = -num3;
//				printf("Same %d, %d, %d\n", map[i][j], map[i][j+1], map[i][j+2]);
				score += 10;
				finish = 0;
			}
		}
	}
	
	
//	Crush the rows
// 	vertical line check
	for(int a = 0; a < col; a++)
	{
//		draw();
		for(int b = 0; b < row - 2; b++)
		{
			int numR1 = map[b][a];
			int numR2 = map[b+1][a];
			int numR3 = map[b+2][a];
			// tag them by multiplying them with -1
			if(numR1 == numR2 && numR1 == numR3 && numR1 != 0)
			{
				map[b][a] = -numR1;
				map[b+1][a] = -numR2;
				map[b+2][a] = -numR3;
				score += 10;
				finish = 0;
			}
		}
	}
	if(finish == 1) loopAgain = 0;
	
//	move the candy
// 	finish == 1 if there arent candy to crush
	if(finish == 0)
	{
		// 
		for(int i = 0; i < col; i++)
		{
			//move the positive ones to the bottom
			int anchor = row-1;
			for(int j = row-1; j > -1; j--)
			{
				// if positive
				if(map[j][i] > 0)
				{
					map[anchor][i] = map[j][i];
					anchor--;
				}
			}
			
			// replace the crush candy
			for(int y = anchor; y > -1; y--)
			{
				map[y][i] = 0;
			}
		}	
	}	
};

void update()
{
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			int temp = map[i][j];
			if(temp == 0)
			{
				int randomNumber = rand() % 4 + 1;
				map[i][j] = randomNumber;
			}
		}
	}
}

void gameMenu(char name[]);
void input(char name[])
{
	int validDir = 0;
	int userX, userY;
	char userDir;
	printf("Total MOVES : %s%d%s \n",KCYN, moves, RESET);
	printf("Score : %s%d%s \n",KMAG, score, RESET);
	do{
		printf("\nEnter -1 to QUIT\n\n");
		printf("Enter target row (0-5) : "); scanf("%d", &userX); getchar();
		if(userX == -1) gameMenu(name);
	}while(userX < 0 || userX > 5);
	do{
		printf("Enter target column (0-5) : "); scanf("%d", &userY); getchar();		
		if(userY == -1) gameMenu(name);
	}while(userY < 0 || userY > 5);
	printf("You chosee ");
	int temp = map[userX][userY];
	switch(temp)
	{
		case 1 : printf("%s", RED); break;
		case 2 : printf("%s", GRN); break;
		case 3 : printf("%s", YEL); break;
		case 4 : printf("%s", BLU); break;
	}
	printf("%d %s(Row : %d, Col : %d)\n", map[userX][userY], RESET, userX, userY);
	printf("\n 'w' -> up 's' -> down 'd' -> right 'a' -> left\n")	;
	do
	{
		do{
			printf("Direction to swap CASE SENSITIVE : ");
			scanf("%c", &userDir); getchar();
			if(userDir == 'w' || userDir == 's' || userDir == 'a' || userDir == 'd') break;	
		}while(true);
		// checking if the userDir is valid? hit a wall or not? or does the dir exist a candy?
		switch(userDir)
		{
			// userX = row = y
			// userY = col = x
			case 'w' : 
				if((userX - 1) >= 0)
				{
					validDir = 1;
				}else printf("You can''t swap a wall\n");
				break;
			case 's' : 
				if((userX + 1) < 6)
				{
					validDir = 1;
				}else printf("You can''t swap a wall\n");
				break;		
			case 'a' : 
				if((userY - 1) >= 0)
				{
					validDir = 1;
				}else printf("You can''t swap a wall\n");
				break;
			case 'd' : 
				if((userY + 1) < 6)
				{
					validDir = 1;
				}else printf("You can''t swap a wall\n");
				break;	
		}
	}while(validDir == 0);
	
	
	swap(userX, userY, userDir);
	printf("Success, Updating...\n");
}

void draw()
{
	printf(" Col  0   1   2   3   4   5  \n");
	printf(" Row\n");
	for(int i = 0; i < row; i++)
	{
		printf("  %d  ", i);
		for(int j = 0; j < col; j++)
		{
			int temp = map[i][j];
			switch(temp)
			{
				case 1 : printf("%s", RED); break;
				case 2 : printf("%s", GRN); break;
				case 3 : printf("%s", YEL); break;
				case 4 : printf("%s", BLU); break;
			}
			printf(" %d ", map[i][j]);
			printf("%s", RESET);
			printf("|");
		}
		printf("\n");
	}
}


void gameRun(char name[])
{
	moves = 10;
	do
	{
		// reset the player row and col pos
		system("cls");
		draw();
		input(name);
		while(loopAgain == 1)
		{
			printf("in while\n");
			logic();
		}
		loopAgain = 1;
		update();
	}while(moves != 0);
	system("cls");	
	
	printf("YOU RAN OUT OF MOVES\n");
	printf("YOU SCORED %d\n", score);
	printf("Press enter...");
	char gp; scanf("%c", &gp); 
	int tempoScore = score;
	score = 0;
	char tempUsername[100];
	strcpy(tempUsername, name);
	updateNode(tempUsername, tempoScore);
	gameMenu(tempUsername);	
}

void game(char name[])
{
	system("cls");
	generate();
	while(loopAgain == 1)
	{
		printf("in while\n");
		logic();
	}
	loopAgain = 1;
	update();
	gameRun(name);	
}

bool validInput(char input[])
{
	char sentence[100];
	strcpy(sentence, input);
	int n = strlen(input);
	int valid = 0;
	for(int i = 0; i < n; i++)
	{
		char check = sentence[i];
		if(isalnum(check) == 0) return false;
	}
	return true;
}

void hallOfFame(){
	char userinput;
	do{
		system("cls");
		printf("HALL OF FAME \n");
		// sort the Linked(name and score only) list that contain all the user from the hash
		sortUser();
		// print the user
		printSortUser();
		// delete all the linked list
		deleteAll();
		char goBack; printf("\x1b[mPress enter to continue..."); scanf("%c", &goBack); if(goBack == '\n') return;	
	}while(true);
};

// CASTING MAIN MENU FIRST, SO THAT GAME MENU CAN ACC MAIN MENU 
void mainMenu();

// GAME MENU
int backFromGameMenu = 0;
void gameMenu(char name[])
{
	int userInput;
	do{
		system("cls");
		printf("WELCOME TO XYTRUS,\033[0;34m %s \033[0;37m score : \n", name, score);
		printf("1. PLAY GAME\n");
		printf("2. HALL OF FAME\n");
		printf("3. LOG OUT \n");
		printf("4. Exit the game\n");	
		printf(">>> "); scanf("%d", &userInput); getchar();
		switch(userInput)
		{
			case 1 : game(name); break;
			case 2 : hallOfFame(); break;
			case 3 : 
				printf("Logging out....\n");
				printf("Press enter to continue..."); 
				char goBack; scanf("%c", &goBack); backFromGameMenu = 1; writeFile(); if(goBack == '\n') mainMenu();
				break;
			case 4 : system("cls"); printf("Exiting..."); readFile(); exit(0);
		};
	}while(userInput != 4);	
}

// LOGIN MENU
void loginMenu()
{	
	int userInput;
	char username[100], password[100];
	do{
		printf("LOGIN MENU\n");
		do{
			printf("Type BACK to EXIT\n");	
			
			printf("Input your USERNAME : "); scanf("%[^\n]", username); getchar();
			if(strcmp(username, "BACK") == 0) return;
			
			printf("Input your PASSWORD : "); scanf("%[^\n]", password); getchar();	
			if(strcmp(password, "BACK") == 0) return;
			
			if(validInput(username) && validInput(password)) break;
			else if( !( validInput(username) && validInput(password) ) );
				printf("USERNAME and PASSWORD doesn't have any SPECIAL character \n");
		}while(true);	
		// 1 means user existed
		searchUser(username);
		checkPass(username, password);
		if(userExist == 1 && validPass == 1) 
		{
			system("cls");
			printf("LOGIN SUCCESS\n");
			// start the candy crush game
			char goBack; printf("Press enter to continue..."); scanf("%c", &goBack); if(goBack == '\n') gameMenu(username);	
		};
		system("cls");
		// 0 means user doesnt existed
		if (userExist == 0 && validPass == 0) printf("USER NOT FOUND AND WRONG PASSWORD\n");
		else if(userExist == 0) printf("USER NOT FOUND PLEASE TYPE AGAIN\n");
		else if(validPass == 0) printf("WRONG PASS PLEASE TYPE AGAIN\n");
	}while(true);	
}

// REGISTER MENU
void registerMenu()
{
	int userInput;
	char username[100], password[100];
	strcpy(username, ""); strcpy(password, "");
	do{
		printf("REGISTER MENU\n");
		
		do{
			printf("CREATE NEW USERNAME\n");
			printf("Type BACK to EXIT\n");
				
			printf("Input your USERNAME : "); scanf("%[^\n]", username); getchar();
			if(strcmp(username, "BACK") == 0) return;
			
			printf("Input your PASSWORD : "); scanf("%[^\n]", password); getchar();
			if(strcmp(password, "BACK") == 0) return;
			
			if( validInput(username) && validInput(password) ) break;
			else if(!(validInput(username) == 1 && validInput(password) == 1))
				printf("USERNAME and PASSWORD doesn't have any special char'\n");
		}while(true);	
		// checking if user is already existed, if exist userExist = 1;
		searchUser(username);
		if(userExist == 0) // 0 means false, no user
		{
			// append the new user to hash
			// new user score always be 0 NODE SECTION
			insertNode(username, password, 0); // HASH TABLE
			
			writeFile(); // update the file for the new user
			system("cls");
			printf("Account succesfully created\n");
			char goBack; printf("Press enter to continue..."); scanf("%c", &goBack); if(goBack == '\n') return;	
		};
		system("cls");
		if(userExist == 1) printf("Username is already existed\n");
	}while(true);
}

void mainMenu()
{
	if(backFromGameMenu == 0) readFile();
	else backFromGameMenu = 0;
	int userInput;
	if(fullExit == 0)
	{
		do{
			system("cls");
			printf("WELCOME TO XYTRUS\n");
			printf("1. Login\n");
			printf("2. Registerr\n");
			printf("3. Hall of fame\n");
			printf("4. Exit the game\n");	
			printf(">>> "); scanf("%d", &userInput); getchar();
			system("cls");
			switch(userInput)
			{
				case 1 : loginMenu(); break;
				case 2 : registerMenu(); break;
				case 3 : hallOfFame(); break;
				case 4 : system("cls"); printf("Exiting...\n"); readFile(); exit(0);
			};
		}while(true);		
	} 
}

int main()
{
	mainMenu();
	return 0;
}

// decor 1 jam

