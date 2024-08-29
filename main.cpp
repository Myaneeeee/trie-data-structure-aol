#include<stdio.h> // Used for standard input-output functions
#include<stdlib.h> // Functions for memory allocation, string conversion, and other common functions
#include<conio.h> // Contains functions for console-based input-output
#include<windows.h> // Used to access basic functions of the Windows operating system
#include<winuser.h> // Part of the Windows API and contains functions, messages, and structures related to the Windows interface

//ANSI escape codes defined as macros used to set the color of the text output in the console.
//Regular bold text
#define BRED "\e[1;31m" //This sets the text color to bright red.
#define BGRN "\e[1;32m" //This sets the text color to bright green.
#define BYEL "\e[1;33m" //This sets the text color to bright yellow.
#define BBLU "\e[1;34m" //This sets the text color to bright blue.
#define BMAG "\e[1;35m" //This sets the text color to bright magenta.
#define BCYN "\e[1;36m" //This sets the text color to bright cyan.
#define BWHT "\e[1;37m" //This sets the text color to bright white.
#define RST "\e[0m" //This resets the text color to the default color.
//Regular background
#define BLKB "\e[40m" //This sets the text background color to black.
#define REDB "\e[41m" //This sets the text background color to red.
#define GRNB "\e[42m" //This sets the text background color to green.
#define YELB "\e[43m" //This sets the text background color to yellow.
#define BLUB "\e[44m" //This sets the text background color to blue.
#define MAGB "\e[45m" //This sets the text background color to magenta.
#define CYNB "\e[46m" //This sets the text background color to cyan.
#define WHTB "\e[47m" //This sets the text background color to white.

//define struct
typedef struct TrieNode{
	char chr; //the character stored in the node
	int word; //1 means there is a word ends at this node, 0 otherwise
	char desc[1000]; //the description of the word
	struct TrieNode *edge[128]; //ASCII representation; ex: 'A' will be saved in edge [65], 'a' in edge[97]
} TrieNode;

//global variables:
int leftClickCurrentlyPressed = 0; //to detect if left click is currently pressed
int number; //counter to number the words when they are printed out
TrieNode *root = NULL; //declare and initialize the root of the Trie
bool isEmpty = true; //to determine whether the trie is still empty or not

//functions related to DS and program's algorithm (important/penting):
void initRoot(); //function to initialize the root
TrieNode *createNewNode(char x); //function to create new node
void insert(TrieNode *curr, char word[], char desc[]); //function to insert new node to the Trie
void printAllWordsDFS(TrieNode *curr, char word[], int level); //performs DFS traversal to print all words stored in the Trie
void printAllWords(); //function to print all words in the Trie.
void find(TrieNode* root, char* word); //function to find a word in the Trie
void searchSlangWPrefixDFS(TrieNode *curr, char word[], int level, char prefix[]); //performs DFS traversal to print words with certain prefix
void searchSlangWPrefix(char prefix[]); //function to print all words in the Trie with certain prefix

//functions not related to DS (decoration/menu/tambahan di luar Trie):
COORD mouseLeftClickPos(); //to detect left mouse click position
void pressEnter(); //to print "Press enter to continue"
void printLogo(); //to print Boogle Logo
	//menu related:
int menuController(); //function handles mouse input to determine which menu option the user selects.
void printMenu(); //display the menu's view
void releaseNewSlang(); //menu for release a new slang word
bool validWord(char word[]); //to determine if a word is valid
bool validDesc(char desc[]); //to determine if a desc is valid
void searchSlang(); //menu for searching a slang word
void searchPrefix(); //menu for searching a slang word with prefix

int main(){
	initRoot(); //start by initialize the root
	
	while(1){ //loop until user choose to exit
		system("cls"); //clearing the screen and display menu
		printLogo(); 
		printMenu();
		int chosenMenu = menuController();
		
		//menu selection:
		switch(chosenMenu){
			case 1:
				system("cls");
				//puts("Release a new slang word");
				releaseNewSlang();
	            pressEnter();
	            break;
			case 2:
				system("cls");
				//puts("Search a slang word");
				searchSlang();
	            pressEnter();
	            break;
			case 3:
				system("cls");
				//puts("View all slang words starting with a certain prefix word");
				searchPrefix();
	            pressEnter();
	            break;
			case 4:
				system("cls");
				//puts("View all slang words");
				printAllWords();
	            pressEnter();
	            break;
        	case 5:
        		system("cls");
        		printLogo();
				printf(BGRN"\nThank you... Have a nice day :)\n"RST);
				pressEnter();
				return 0; //exits the program
            	break;
		}
	}
}

//functions related to DS and program's algorithm (important/penting):
void initRoot(){
	root = createNewNode('\0');
}

TrieNode *createNewNode(char x){
	TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
	node->chr = x;
	node->word = 0;
	strcpy(node->desc, "\0");
	for (int i = 0; i < 128; i++){
		node->edge[i] = 0;
	}
	return node;
}

void insert(TrieNode *curr, char word[], char desc[]) {
    if (*word == '\0') {
        curr->word = 1;
        // Check if the word already has a description
        if (strcmp(curr->desc, "\0") != 0) {
            printf(BWHT GRNB"\nSuccessfully updated a slang word.\n\n"RST);
        } else {
            printf(BWHT GRNB"\nSuccessfully released new slang word.\n\n"RST);
        }
        // Update the description
        strcpy(curr->desc, desc);
    } else {
        // Convert the character to lower case
        char lower_case_char = tolower(*word);
        // Get the relative position in the alphabet list
        int idx = (int) lower_case_char - 'a';
        
        if (curr->edge[idx] == NULL) {
            // If the corresponding child doesn't exist, simply create that child!
            curr->edge[idx] = createNewNode(lower_case_char);
        }
        
        // Go down a level, to the child referenced by idx since we have a prefix match
        insert(curr->edge[idx], word + 1, desc);
    }
}

void printAllWordsDFS(TrieNode *curr, char word[], int level) {
    if (curr->word) {
        // The current node represents a word in the Trie
        word[level] = '\0';  // Null terminate the word
        printf(BWHT"%d. %s\n"RST, number, word);
        number++;
    }

    for (int i = 0; i < 128; i++) {
        if (curr->edge[i]) {
            // There is an edge from the current node
            word[level] = i + 'a';  // Add the edge character to the word
            printAllWordsDFS(curr->edge[i], word, level + 1);
        }
    }
}

void printAllWords() {
    char word[1000];  // This will hold the words as we traverse the Trie
	if (isEmpty) {
        printf(BWHT REDB"\nNo Data\n\n"RST);
    } else {
    	number = 1;
        printAllWordsDFS(root, word, 0);
    }
}

void find(TrieNode* root, char* word) {
    TrieNode* temp = root;

    for(int i = 0; word[i] != '\0'; i++) {
        int position = word[i] - 'a';
        if (temp->edge[position] == NULL) {
            printf(BWHT REDB"\nThere is no word %s in the dictionary.\n\n"RST, word);
            return;
        }
        temp = temp->edge[position];
    }
    
    if (temp != NULL && temp->word == 1) {
        printf(BGRN"\nSlang word\t: %s\n", word);
        printf("Description\t: %s\n\n"RST, temp->desc);
    }
    else {
        printf(BWHT REDB"\nThere is no word %s in the dictionary.\n\n"RST, word);
    }
}

void searchSlangWPrefix(char prefix[]) {
    // Create a temporary array to store the words
    char word[1000];
    number = 1;
    
    // Call the helper function
    searchSlangWPrefixDFS(root, word, 0, prefix);
    
    // If no words were found
    if (number == 1) {
        printf(BWHT REDB"\nThere is no prefix \"%s\" in the dictionary.\n\n"RST, prefix);
    }
}

void searchSlangWPrefixDFS(TrieNode *curr, char word[], int level, char prefix[]) {
    // If the current node represents a word in the Trie and the prefix matches
    if (curr->word && strncmp(word, prefix, strlen(prefix)) == 0) {
        word[level] = '\0';  // Null terminate the word
        printf(BWHT"%d. %s\n"RST, number, word);
        number++;
    }

    for (int i = 0; i < 128; i++) {
        if (curr->edge[i]) {
            // There is an edge from the current node
            word[level] = i + 'a';  // Add the edge character to the word
            searchSlangWPrefixDFS(curr->edge[i], word, level + 1, prefix);
        }
    }
}

//functions not related to DS (decoration/menu/tambahan di luar Trie):
void searchPrefix(){
	char prefix[1000];
    do {
        printf(BYEL"Input a prefix to be searched [Must be more than 1 characters and contains no space]: "RST);
        scanf("%[^\n]", prefix);
        getchar();	
    } while(!validWord(prefix));
    
    searchSlangWPrefix(prefix);
}

void searchSlang(){
	char searchWord[1000];
	do {
		printf(BYEL"Input a slang word to be searched [Must be more than 1 characters and contains no space]: "RST);
		scanf("%[^\n]", searchWord);
		getchar();	
	} while(!validWord(searchWord));
	find(root, searchWord);
}

bool validWord(char word[]) {
    if (strlen(word) < 2) {
        return false;
    }

    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == ' ') {
            return false;
        }
    }
    return true;
}

bool validDesc(char desc[]){
	int spaceCount = 0;
	
	for (int i = 0; desc[i] != '\0'; i++) {
        if (desc[i] == ' ') {
            spaceCount++;
        }
    }
    
    if (spaceCount >= 2) {
        return true;
    } else {
        return false;
    }
}

void releaseNewSlang(){
	char word[1000];
	do{
		printf(BYEL"Input a new slang word [Must be more than 1 characters and contains no space]: "RST);
		scanf("%[^\n]", word);
		getchar();
		
//		printf("%s\n", word);
	} while(!validWord(word));
	
	char desc[1000];
	do{
		printf(BWHT"Input a new slang word description [Must be more than 2 words]: "RST);
		scanf("%[^\n]", desc);
		getchar();
	} while(!validDesc(desc));
	
	insert(root, word, desc);
	isEmpty = false;
}

int menuController(){
	int chosenMenu = 0;
	while (1) {
        COORD currentPos = mouseLeftClickPos();
//			printf("Mouse position: Row (Y) %d, Column (X) %d\n", currentPos.Y, currentPos.X);
        if (currentPos.X >= 16 && currentPos.X <= 55 && currentPos.Y >= 14 && currentPos.Y <= 18) {
            chosenMenu = 1;
            break;
        } else if (currentPos.X >= 59 && currentPos.X <= 98 && currentPos.Y >= 14 && currentPos.Y <= 18) {
            chosenMenu = 2;
            break;
        } else if (currentPos.X >= 16 && currentPos.X <= 55 && currentPos.Y >= 19 && currentPos.Y <= 23) {
            chosenMenu = 3;
            break;
        } else if (currentPos.X >= 59 && currentPos.X <= 98 && currentPos.Y >= 19 && currentPos.Y <= 23) {
            chosenMenu = 4;
            break;
        } else if (currentPos.X >= 43 && currentPos.X <= 72 && currentPos.Y >= 24 && currentPos.Y <= 28) {
            chosenMenu = 5;
            break;
        }
    }
    return chosenMenu;
}

void printMenu(){
	
	printf(BWHT"\n\t    =============================");
	printf(BYEL" David Christian / 2702253143 ");
	printf(BWHT"================================\n"RST);
		
	printf("\t\t");
	for (int i = 0; i < 40; i++) printf(BYEL"%c"RST, 220);
	printf("   ");
	for (int i = 0; i < 40; i++) printf(BYEL"%c"RST, 220);
	puts("");
	printf(BYEL"\t\t%c                                      %c   %c                                      %c\n", 219, 219, 219, 219);
	printf(BYEL"\t\t%c       Release a new slang word       %c   %c         Search a slang word          %c\n", 219, 219, 219, 219);
	printf(BYEL"\t\t%c                                      %c   %c                                      %c\n"RST, 219, 219, 219, 219);
	printf("\t\t");
	for (int i = 0; i < 40; i++) printf(BYEL"%c"RST, 223);
	printf("   ");
	for (int i = 0; i < 40; i++) printf(BYEL"%c"RST, 223);
	puts("");
	
	printf("\t\t");
	for (int i = 0; i < 40; i++) printf(BYEL"%c"RST, 220);
	printf("   ");
	for (int i = 0; i < 40; i++) printf(BYEL"%c"RST, 220);
	puts("");
	printf(BYEL"\t\t%c                                      %c   %c                                      %c\n", 219, 219, 219, 219);
	printf(BYEL"\t\t%c   View all slang words with prefix   %c   %c         View all slang words         %c\n", 219, 219, 219, 219);
	printf(BYEL"\t\t%c                                      %c   %c                                      %c\n"RST, 219, 219, 219, 219);
	printf("\t\t");
	for (int i = 0; i < 40; i++) printf(BYEL"%c"RST, 223);
	printf("   ");
	for (int i = 0; i < 40; i++) printf(BYEL"%c"RST, 223);
	puts("");
	
	printf("\t\t\t\t\t   ");
	for (int i = 0; i < 30; i++) printf(BRED"%c"RST, 220);
	puts("");
	printf(BRED"\t\t\t\t\t   %c                            %c\n", 219, 219);
	printf(BRED"\t\t\t\t\t   %c            Exit            %c\n", 219, 219);
	printf(BRED"\t\t\t\t\t   %c                            %c\n"RST, 219, 219);
	printf("\t\t\t\t\t   ");
	for (int i = 0; i < 30; i++) printf(BRED"%c"RST, 223);
	
	puts("");
}

void printLogo() {
	printf(BYEL"\t\t               .-'''-.         .-'''-.                                            \n");
    printf("\t\t               '   _    \\      '   _    \\              .---.                       \n");
    printf("\t\t ||         .   |     \\  '  .   |     \\  '    .--./)   |   |    .-''         '.    \n");
    printf("\t\t ||         |   '      |  ' |   '      |  '  /.''\\\\    |   |   /     .-''\"'-.  `.  \n");
    printf("\t\t ||  __     \\    \\     / /  \\    \\     / /  | |  | |   |   |  /     /________\\   \\ \n");
    printf("\t\t ||/'__ '.   `.   ` ..' /    `.   ` ..' /    \\`-' /    |   |  |                  | \n");
    printf("\t\t |:/`  '. '     '-...-'`        '-...-'`     /(\"'`     |   |  \\    .-------------'\n");
    printf("\t\t ||     | |                                  \\ '---.   |   |   \\    '-.____...---.\n");
    printf("\t\t ||\\    / '                                   /'\"\"'.\\  |   |    `.             .' \n");
    printf("\t\t |/\\'..' /                                   ||     || '---'      `''-...... -'   \n");
    printf("\t\t '  `'-'`                                    \\'. __//                               \n");
    printf("\t\t                                              `'---'                                \n"RST);
}

void pressEnter(){
	printf(BYEL"Press Enter to Continue . . .\n"RST);
	getchar();
}

COORD mouseLeftClickPos(){
	
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);
	
	DWORD readEvents;
    INPUT_RECORD inputRecord;
	
    COORD currentPos;
	
    ReadConsoleInput(hInput, &inputRecord, 1, &readEvents);
    if(inputRecord.EventType == MOUSE_EVENT){
        if(inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
        	if(leftClickCurrentlyPressed == 0){
    			currentPos.X = inputRecord.Event.MouseEvent.dwMousePosition.X;
	            currentPos.Y = inputRecord.Event.MouseEvent.dwMousePosition.Y;
//	            printf("X: %hu, Y: %hu\n", currentPos.X, currentPos.Y);
	            leftClickCurrentlyPressed = 1;
			}
		}
		else{
			leftClickCurrentlyPressed = 0;
		}
    }

    return currentPos;
}



