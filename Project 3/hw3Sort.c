/*
�Your statement that the program is entirely your own work and that you have neither developed
your code together with any another person, nor copied program code
from any other person, nor permitted your code to be copied or otherwise used
by any other person, nor have you copied, modified, or otherwise used program
code that you have found in any external source, including but not limited to,
online sources�
*/


//Name: Jonathan Cotto
//Project: Homework 3
//Class: COP 3502c Summer 2017

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct airPdata
{
	char *LocID; //second field
	char *fieldName; //third field
	char *city; //fourth field
	float latitude; //9th field
	float longitude;//10th field
}airPdata;

struct node
{
	airPdata *airport; //the airport to be stored
	unsigned int stringToBinary; //for LOCID sorting
	struct node *left; //left child
	struct node *right; //right child
};

//prototype
void fieldSetter(char *word, int fieldPlace, airPdata *airport);
void freeStructure(struct node *root);
void mallocStructMembers(airPdata *airPort);
FILE* openFile(char *fileName);
void parseLine(airPdata *airport, char *buffer);
float sexag2decimal(char *degreeString);
struct node* newNode(airPdata airport, unsigned int num);
struct node* insert(struct node  *node, airPdata *airport);
struct node* insertLocID(struct node  *node, airPdata *airport, unsigned int stringToBinary);
void sortByLatitude(airPdata **airports);
void sortByLocID(airPdata **airports);
void inOrder(struct node*);
unsigned int bitMasking(char *locID);


main(int argc, char *argv[]) {

	//argc = the amount fo arguments passedin
	//argv is a pointer to an array of strings, argv[1] is the fileName
	airPdata *airPort = NULL;
	FILE* inputFile = NULL;
	//char* fileName = "flRaw.csv"; // remove this line when running on eustis
	char *fileName = argv[1]; //do this when testing on eustis
	char *sortOrder = argv[2]; //do this when testing on eustis
	//char *sortOrder = "A";
	char buffer[300]; //this is for the parsed line which contains each airport info
	airPdata *airPorts[1000] = { NULL }; //this will store all our airports. 1000 airports max.
	int count = 0; //keep track of how much airports we are storing in buffer


	inputFile = openFile(fileName);
	while (fgets(buffer, 300, inputFile) != NULL) {
		airPort = malloc(sizeof(airPdata));
		if (NULL == airPort) {
			fprintf(stderr, "Malloc Failed for airport structure!\n");
			exit(-1);
		}
		mallocStructMembers(airPort);
		parseLine(airPort, buffer);
		if (airPort->LocID[0] >= 'A' && airPort->LocID[1] >= 'A' && airPort->LocID[2] >= 'A' && (airPort->LocID[3] >= 'A' || airPort->LocID[3] == '\0')){ //only allow airports that contain 3 or 4 letters only. Anything else is a bad/corrupted case.
			airPorts[count] = airPort; //store airport
			count++;
		}
		memset(buffer, '\0', sizeof(buffer)); //reset buffer to null
		airPort = NULL;
	}

	switch (sortOrder[0]){
	case 'n': //north Bound Exit sort
		sortByLatitude(airPorts);
		break;
	case 'N':
		sortByLatitude(airPorts);
		break;
	case 'a': //alphabetical sort
		sortByLocID(airPorts);
		break;
	case 'A':
		sortByLocID(airPorts);
		break;
	default:
		fprintf(stderr, "hw3Sort ERROR: valid sort parameters are a or n.\n");
	}

	fclose(inputFile);
	//system("pause");
}



void fieldSetter(char *word, int fieldPlace, airPdata *airPort) {
	float temp;

	switch (fieldPlace) {
	case 2: //locId
		strcpy(airPort->LocID, word);
		break;
	case 3: //fieldName
		strcpy(airPort->fieldName, word);
		break;
	case 4: //city
		strcpy(airPort->city, word);
		break;
	case 9: //latitude
		temp = sexag2decimal(word);
		if (word[0] == NULL)
			airPort->LocID[0] = 2;// get rid of airport that hass bad degree cases by assign LOCID a invalid entry (must be > 'A')
		airPort->latitude = temp;
		break;
	case 10: //longitude
		temp = sexag2decimal(word);
		if (word[0] == NULL)
			airPort->LocID[0] = 2;// get rid of airport that has bad degree cases by assigning LOCID a invalid entry (must by > 'A')
		airPort->longitude = temp;
		break;
	default:
		break;

	}//end switch
}


void freeStructure(struct node *root) {
	if (root != NULL){
		freeStructure(root->left);
		freeStructure(root->right);
		free(root->airport->LocID);
		free(root->airport->fieldName);
		free(root->airport->city);
		free(root->airport);
		free(root);
	}
}

void mallocStructMembers(airPdata *airPort) {
	airPort->LocID = malloc(20 * sizeof(char));
	//memset(airPort, '\0', 5);
	airPort->fieldName = malloc(51 * sizeof(char));
	airPort->city = malloc(51 * sizeof(char));
}

FILE* openFile(char *fileName) {
	FILE* ifp = NULL;
	ifp = fopen(fileName, "r");
	if (NULL == ifp) {
		fprintf(stderr, "et1 ERROR: File \"%s\" not found.", fileName);
		exit(-1); //EXIT_NONSUCESS
	}

	return ifp;
}

void parseLine(airPdata *airport, char *buffer) {
	int strLen = 0;
	char word[60] = { 0 }; //when this is around the size of 53 the word stack gets corrupted?
	int i;
	int j = 0; //needs to start at zero
	int fieldCounter = 0;

	strLen = strlen(buffer);

	for (i = 0; i <= strLen; i++) {

		if (buffer[i] == ',') {
			j = 0; //reset word index to zero because a comma has ended
			fieldCounter++;

			if (buffer[i] && buffer[i - 1] == ',') { //handling double commas
				fieldSetter(NULL, fieldCounter, airport); //send null for a double comma
				continue;
			}
			fieldSetter(word, fieldCounter, airport); //set fieldCounter to word
			memset(word, '\0', sizeof(word)); //reset the word string to null
			continue;//go to the next iteration
		}//end if

		if (i == strLen) { //sending the last word into the fieldsetter because it does not end with a comma
			fieldCounter = 19; //we only have 19 fields
			fieldSetter(word, fieldCounter, airport);
		}

		word[j] = buffer[i];
		j++;
	}//end for loop
}


float sexag2decimal(char *degreeString){
	float degrees;
	float min;
	float seconds;
	float direction;
	char temp[7] = { NULL }; //holds or temporary converted degree, min, seconds
	int i;
	int count = 0;


	if (degreeString == NULL){
		degreeString[0] = NULL;
		return 0.0;
	}

	//get degree
	for (i = 0; i < 4; i++){
		if (degreeString[count] >= '0' && degreeString[count] <= '9'){ //valid chars encountered
			temp[i] = degreeString[count];
			count++;
		}
		else{
			if (degreeString[count] == '-'){ //degrees are done if we encounter this case
				count++;
				break;
			}
			if (degreeString[count] < '0' || degreeString[count] > '9'){ //invalid chars encountered return
				degreeString[0] = NULL;
				return 0.0;
			}
		}
	}

	degrees = atof(temp); //convert string to float
	if (degrees <= 0.0 || degrees > 180){ //degree out of bounds
		degreeString[0] = NULL;
		return 0.0;
	}

	memset(temp, '\0', 7); //reset temp string

	//get min
	for (i = 0; i < 3; i++) {
		if (degreeString[count] >= '0' && degreeString[count] <= '9') { //valid number encountered
			temp[i] = degreeString[count];
			count++;
		}
		else {
			if (degreeString[count] == '-'){ // mins are done if we encounter this case
				count++;
				break;
			}
			if (degreeString[count] < '0' || degreeString[count] > '9'){ //invalid char return 
				degreeString[0] = NULL;
				return 0.0;
			}
		}
	}

	min = atof(temp); //convert string to float
	if (min < 0 || min > 59) { //min out of bounds 
		degreeString[0] = NULL;
		return 0.0;
	}
	memset(temp, '\0', 7); //reset temp

	//get sec
	for (i = 0; i < 7; i++) {
		if (degreeString[count] >= '0' && degreeString[count] <= '9' ) { //a number encountered
			temp[i] = degreeString[count];
			count++;
		}
		else {
			if (degreeString[count] == '.') { //period encountered go to next count
				temp[i] = degreeString[count];
				count++;
				continue;
			}
			if (degreeString[count] < '0' || degreeString[count] > '9'){ //invalid char encountered return
				degreeString[0] = NULL;
				return 0.0;
			}
		}
	}
	seconds = atof(temp); //convert string to float
	if (seconds < 0.0 || seconds > 59.9999) { //return if out of bounds
		degreeString[0] = NULL;
		return 0.0;
	}
	memset(temp, '\0', 7); //reset temp string

	switch (degreeString[count]) { //last is direction: positive or negative 1 times given formula provided by HW3.pdf
	case 'N': //poisitve 
		return 1 * (degrees + (min / 60) + (seconds / (60 * 60)));
		break;
	case 'S': //negative
		return -1 * (degrees + (min / 60) + (seconds / (60 * 60)));
		break;
	case 'E': //positive
		return 1 * (degrees + (min / 60) + (seconds / (60 * 60)));
		break;
	case 'W': //negative
		return -1 * (degrees + (min / 60) + (seconds / (60 * 60)));
		break;
	default:
		return 0.0;
	}
}



//create new node for binary search tree
struct node* newNode(airPdata *airport, unsigned int num)
{
	struct node* node = malloc(sizeof(struct node));
	if (node == NULL){ //if we fail to malloc print to fpf
		fprintf(stdin, "Malloc Error for Node");
			return node;
	}
	else{ //assign new node values and return
		node->airport = airport;
		node->stringToBinary = num;
		node->left = NULL;
		node->right = NULL;
		return node;
	}
}

//insert into binary search tree by latitude
struct node* insert(struct node  *node, airPdata *airport){
	if (node == NULL) //create node
		return newNode(airport, NULL);
	if (airport->latitude < node->airport->latitude) //go to the left
		node->left = insert(node->left, airport);
	else
		node->right = insert(node->right, airport); //go the the right

	return node;
}


//insert into binary search tree by LOCID
struct node* insertLocID(struct node  *node, airPdata *airport, unsigned int stringToBinary){
	if (node == NULL) //create new node
		return newNode(airport, stringToBinary);
	if (stringToBinary < node->stringToBinary)
		node->left = insertLocID(node->left, airport, stringToBinary);
	else
		node->right = insertLocID(node->right, airport, stringToBinary);

	return node;
}

//inorder traversale through binary search tree
void inOrder(struct node *node){
	if (node != NULL){
		inOrder(node->left);
		printf("%s,%s,%s,%.4f,%.4f\n", node->airport->LocID, node->airport->fieldName, node->airport->city, node->airport->latitude, node->airport->longitude);
		inOrder(node->right);
	}
	else
		return;
}

void sortByLatitude(airPdata **airports){
	struct node *root = NULL;
	int i = 0;
	while (airports[i] != NULL){
		root = insert(root, airports[i]); //insert airport into binary search tree
		i++;
	}

	printf("code,name,city,lat,lon\n");
	inOrder(root); //print inorder to simulate flying from south to north
	freeStructure(root);

}

//bitmask to convert a string of chars (LOCID) to binary
unsigned int bitMasking(unsigned char *locID){ 
	unsigned int stringToBinary = 0;
	unsigned int temp = 0;
	unsigned int mask = 255;
	int i;
	unsigned char c;

	for (i = 0; i < 4; i++){
		if (locID[i] >= 'A' && locID[i] <= 'z'){ //make sure they are letters
			c = toupper(locID[i]); //upper case
			temp = c & mask; //bitmask (get value of char)
			stringToBinary = stringToBinary | temp; //place 8 bits of convert char into StringToBinary
			if (i < 2) //only shift first two letters of loc ID
			stringToBinary = stringToBinary << 8;// shift to the left 8 bits for next char to binary conversion
			temp = 0; //reset temp (bitmask)
		}
		else{
			stringToBinary = stringToBinary << 8; //is not a letter shift to the left 8 bits (for null cases)
		}
	}

	return stringToBinary;
}

void sortByLocID(airPdata **airports){
	struct node *root = NULL;
	int i = 0;
	unsigned int stringToBinary = 0;
	while (airports[i] != NULL){
		stringToBinary = bitMasking(airports[i]->LocID); //convert a string of LOCID into binary to sort LOCID
		root = insertLocID(root, airports[i], stringToBinary); //insert airport into binary search tree
		i++;
		stringToBinary = 0; //reset string to binary
	}
	printf("code,name,city,lat,lon\n");
	inOrder(root); //print 
	freeStructure(root);
}



