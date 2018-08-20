/*
“I [Jonathan Cotto] ([jo887691]) affirm that this program is entirely my own work and that I have
neither developed my code together with any another person, nor copied any code from any other person,
nor permitted my code to be copied or otherwise used by any other person, nor have I copied, modified, or
otherwise used program code that I have found in any external source, including but not limited to, online
sources. I acknowledge that any violation of the above terms will be treated as academic dishonesty.
*/


//Name: Jonathan Cotto
//Project: Homework 1
//Class: COP 3502c Summer 2017

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "airPdata.h"


//prototype
void fieldSetter(char *word, int fieldPlace, airPdata *airport);
void freeStructure(airPdata *airport);
void mallocStructMembers(airPdata *airPort);
FILE* openFile(char *fileName); 
void parseLine(airPdata *airport, char *buffer);
void printData(airPdata *airport);
void printHeading();


main(int argc, char *argv[]) {

	//argc = the amount fo arguments passedin
	//argv is a pointer to an array of strings, argv[1] is the fileName
	airPdata *airPort = NULL;
	FILE* inputFile = NULL;
	//char* fileName = "florida.csv"; // remove this line when running on eustis
	char *fileName = argv[1]; //do this when testing on eustis
	char buffer[300]; //this is for the parsed line which contains each airport info
	
	airPort = malloc(sizeof(airPdata));
	if (NULL == airPort) {
		fprintf(stderr, "Malloc Failed for airport strructure!\n");
		exit(-1);
	}

	mallocStructMembers(airPort);
	inputFile = openFile(fileName);
	printHeading();
	while (fgets(buffer, 300, inputFile) != NULL) {
			parseLine(airPort, buffer);
			printData(airPort);
			memset(buffer, '\0', sizeof(buffer)); //reset buffer to null
	}

	freeStructure(airPort);
	fclose(inputFile);
	system("pause");
}



void fieldSetter(char *word, int fieldPlace, airPdata *airPort) {

	switch (fieldPlace) {
	case 1: //site number
		strcpy(airPort->siteNumber, word);
		break;
	case 2: //locId
		strcpy(airPort->LocID, word);
		break;
	case 3: //fieldName
		strcpy(airPort->fieldName, word);
		break;
	case 4: //city
		strcpy(airPort->city, word);
		break;
	case 5: //state
		strcpy(airPort->state, word);
		break;
	case 9: //lat
		strcpy(airPort->latitude, word);
		break;
	case 10: //long
		strcpy(airPort->longitude, word);
		break;
	case 15: //controlTower
		airPort->controlTower = word[0];
		break;
	default :
		break;

	}//end switch
}


void freeStructure(airPdata *airport) {
	free(airport->siteNumber);
	free(airport->LocID);
	free(airport->fieldName);
	free(airport->city);
	free(airport->state);
	free(airport->latitude);
	free(airport->longitude);
	free(airport);
}

void mallocStructMembers(airPdata *airPort) { 
	airPort->siteNumber = malloc(51 * sizeof(char));
	airPort->LocID = malloc(51 * sizeof(char));
	airPort->fieldName = malloc(51 * sizeof(char));
	airPort->city = malloc(51 * sizeof(char));
	airPort->state = malloc(51 * sizeof(char));
	airPort->latitude = malloc(51 * sizeof(char));
	airPort->longitude = malloc(51 * sizeof(char));
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

void printData(airPdata *airport) {
	if (NULL == airport) {
		fprintf(stderr, "Airport Struct not allocated\n");
		return(-1);
	}

	printf("%-12s %-11s %-42s %-34s %-3s %-15s %-16s %c\n", airport->siteNumber, airport->LocID, airport->fieldName, airport->city, airport->state, airport->latitude, airport->longitude, airport->controlTower);
}

void printHeading() {
	printf("%-12s %-11s %-42s %-34s %-3s %-15s %-16s Tower\n",
		"FAA Site", "Short Name", "Airport Name", "City", "ST",
		"Latitude", "Longitude");
	printf("%-12s %-11s %-42s %-34s %-3s %-15s %-16s =====\n",
		"========", "==========", "============", "====", "==",
		"========", "=========");
}



