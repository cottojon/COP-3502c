## Objective:

This assignment is based on a class of problem solved in enterprise computing; extraction, transformation,
and loading. This is often referred to as ETL. The inputs will be data extracted from a leading
aviation industry data and consulting firm, GCR. (See GCR.com for additional data.) The data is in a
well known format where each data element is separated from the previous and following data elements
by using a comma. It should be noted that this method of data manipulation is extremely common. The
explicit order of the data fields and the desired outputs are defined in the "Specifications".



## 1.Extraction
The first part of ETL is extraction. The filename of a text file will be passed to your program via the command
line. The data contained in that file is read into memory (i.e., extracted).

## 2. Transformation
The second part of ETL is transformation. A list of comma separated values is convenient for text files, but
it is far less convenient in memory. Once the data for a single airport has been read into a buffer, you will
need to parse the buffer based on the commas between the data fields. The parsed data will then be used to
populate a structure of the type struct airPdata (i.e. the data has been transformed from CSV to a
data structure).

typedef struct airPdata{

char *siteNumber; //FAA Site Number

char *LocID; //Airport’s ‘‘Short Name’’, \textit{e.g.} MCO

char *fieldName; //Airport Name

char *city; //Associated City

char *state; //State

char *latitude; //Latitude

char *longitude; //Longitude

char controlTower;//Control Tower, this is a single character (Y/N)

} airPdata;

## 3. Loading
Finally, the third part of ETL is loading. With the data now in an airPdata structure it can be easily
accessed by functions and/or other programs (i.e., loaded). For this assignment, you will use pass the
airPdata structure to a function ( PrintData(airPdata airport) ) that will print the data to
STDOUT (aka the console).



## TO COMPILE AND RUN IN UNIX ENV
Your program will be
compiled and run on Eustis using the following commands:


gcc -o etl hw1etl.c


./etl inputFile

