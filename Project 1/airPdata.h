#pragma once
//#ifndef airPdata_H
#define airPdata_H

typedef struct airPdata{
	char *siteNumber; //first field
	char *LocID; //second field
	char *fieldName; //third field
	char *city; //fourth field
	char *state; //fifth field
	char *latitude; //9th field
	char *longitude;//10th field
	char controlTower;//15th field
}airPdata;

