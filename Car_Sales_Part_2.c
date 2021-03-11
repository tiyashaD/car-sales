//This program helps the user to buy cars from a list of cars available

#define _CRT_SECURE_NO_WARNINGS  // to let Visual Studio work with scanf()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h> // to let Visual Studio work with isalpha()


extern int errno;

#define MAX_SALES 10	//defining global constants to be used anywhere in the program 
#define CSV_FILE "C:\\data.csv" 

// constants straight built in to make the code easy to read
#define MENU_OPTION_BUY_CARS 'a'
#define MENU_OPTION_VIEW_SALES 'b' 
#define MENU_OPTION_EXIT 'x' 


unsigned short no_OfCarsAvailable = 10;
unsigned short numberOfSales = 0; //array declared to keep a record of the number of sales
unsigned short no_OfCarSoldPerSale[MAX_SALES]; // array to store the number of car sold in each sale
unsigned short typeOfCarPerSale[MAX_SALES]; // this relates to the carTypes[] and is used to record the type of car sold
char customerNames[MAX_SALES][201];

float carPrices[] = {19723.5f, 21347.5f, 18563.5f, 19294.5f, 25634.5f, 31248.5f, 87623.5f, 63992.5f ,64528.5f, 73256.5f};

char carTypes[][201]={"Maruti Suzuki Vitara","Hyundai Kona Electric","Honda Jazz","Ford Fiesta","David Brown Speedback GT",
					  "Radical SR8","Rolls-Royce Phantom","Aston Martin Vantage","Ginetta G55","Mercedes Benz GLS"};

// constants related to files
#define FILE_OPENED 0
#define FILE_CLOSED 1
#define FILE_ERROR 2

FILE* file;
unsigned char fileStatus = FILE_CLOSED;

/* various functions to be used multiple times during the program */
void clearScreen() 
{
		system("cls");
}

void inputUserChoice(char userChoice) //funtion to let the user decide what to choose based on previous output
{
	
	if (userChoice == MENU_OPTION_EXIT) 
	{
		printf("\n\nPress Enter to Exit...");
	}
	else 
	{
		printf("\n\nPress Enter to return to the Menu...");
	}

	if (userChoice != '_') 
	{
		getchar();
	}

	getchar();
}

char getCharFromConsole(char message[201]) // function to return input of char data type
{
	char userInput;
	
	printf(message);
	
	scanf("\n%c", &userInput);
	
	return userInput;
}

unsigned short getUnsignedShortFromConsole(char message[201])  // function to let user input an unsigned short value
{
	
	unsigned short userInput;

	printf(message);
	scanf("%hd", &userInput);

	return userInput;
}

void getStringFromConsole(char message[201], char* str) // function to input a sentence from the user
{
	printf(message);
	
	scanf("\n%[^\n]s", str);
}

void swapUnsignedShort(unsigned short* a, unsigned short* b) //to handle unsigned short data type swapping
{
	unsigned short temp;
	temp = *a;
	*a = *b;
	*b = temp;
}


/* START FILES FUNCTIONS */
FILE* createFile(char fileName[201])  //function with one parameter that will try to a create file 
{
	file = fopen(fileName, "w");
	if (file != NULL) 
	{
		fclose(file);
	}
	return file;
}

void openFile(char fileName[201], char mode[4]) // function to open the file already specified to read or to write
{
	file = fopen(fileName, mode); //use of fopen() to try to open the file

	if (file == NULL) //if file is NULL, it means we were not successful in opening it so we will try to create a new file
	{

		if (createFile(fileName) == NULL) // using return value from createFile() to create new file. And if 
		{
			fileStatus = FILE_ERROR;

			printf("Error opening %s: %s", fileName, strerror(errno));
		}
		else 
		{
			openFile(fileName, mode);
		}

	}
	else 
	{
		fileStatus = FILE_OPENED;
	}
}

void closeFile() 
{
	if (fileStatus == FILE_OPENED) 
	{
		fclose(file);
		fileStatus = FILE_CLOSED;
	}
}
void readDataFromFile() // to return array of strings representing the customer names 
{
	int lineCounter = 0;

	while (1) 
	{
		unsigned short carSoldPerSaleValue = 0, typeOfCarPerSaleValue = 0;
		char customerNameValue[201] = "";

		int scanResult = fscanf
        	(file, // the file stream
			"%hd,%hd,%[^\n]s", 
			&carSoldPerSaleValue, 
			&typeOfCarPerSaleValue,
			&customerNameValue);

		if (scanResult == EOF) 
		{
			break;
		}

		no_OfCarSoldPerSale[lineCounter] = carSoldPerSaleValue;
		typeOfCarPerSale[lineCounter] = typeOfCarPerSaleValue;
		strcpy(customerNames[lineCounter], customerNameValue);

		lineCounter++;
	}

	numberOfSales = lineCounter;
}

void getDataFromFile() 
{
	openFile(CSV_FILE, "r");

	if (fileStatus == FILE_OPENED) 
	{
		readDataFromFile();
	}
	else if (fileStatus == FILE_ERROR) 
	{
		printf("There was an error trying to read from the file %s.", CSV_FILE);
		inputUserChoice('_');
	}

	closeFile();
}

void writeDataToFile() 
{
	for (int i = 0; i < numberOfSales; i++) 
	{

		char line[201];
		char data[50];

		_itoa((int)no_OfCarSoldPerSale[i], data, 10);
		strcpy(line, data);
		strcat(line, ",");

		_itoa((int)typeOfCarPerSale[i], data, 10);
		strcat(line, data);
		strcat(line, ",");

		strcat(line, customerNames[i]);

		fprintf(file, line);

		if (i < numberOfSales - 1) 
		{
			fprintf(file, "\n");
		}
	}
}

void saveDataToFile() 
{
	openFile(CSV_FILE, "w");

	if (fileStatus == FILE_OPENED) 
	{
		writeDataToFile();
	}
	else if (fileStatus == FILE_ERROR) 
	{
		printf("There was an error trying to write to the file %s.", CSV_FILE);
		inputUserChoice('_');
	}

	closeFile();
}
/* END FILES FUNCTIONS */


void menu_greetCustomer() // function to be used to greet the customer
{
	printf("Welcome to the car Sales office!\n\n");
}

void menu_showMenu() // to show the customer the required menu
{
	printf("Menu:\n");
	printf("%c. Buy Cars\n", MENU_OPTION_BUY_CARS);
	printf("%c. View Sales Stats\n", MENU_OPTION_VIEW_SALES);
	printf("%c. Exit\n\n", MENU_OPTION_EXIT);
}

void menu_showCarTypes() // to show the customer the car types available
{
	int numberOfCars = sizeof(carPrices) / sizeof(float);

	printf("\nCar available with us:\n");

	for (int i = 0; i < numberOfCars; i++) 
	{
		printf("%d - %s\n", i, carTypes[i]);
	}
}

void menu_buyCars() // to let the user buy any car
{
	printf("Buy Cars:\n\n");

	if (no_OfCarsAvailable > 0) // to check if we have atleast one car
	{
		printf("There are %hd Cars available.\n\n", no_OfCarsAvailable);
	}
	else 
	{
		printf("Sorry, there are no more Cars available.");
		return;
	}

	unsigned short carsNeeded = 0, userAge = 0, carType;
	float totalPrice = 0;

	getStringFromConsole("What is your name? Name: ", customerNames[numberOfSales]);
	
	//to validate the name entered by the customer
	char ch; 
	short len = strlen(customerNames[numberOfSales]);
	for (int i = 0; i<=len; i++)
	{
		ch = customerNames[numberOfSales][i];
		if(isalpha(ch) == 0)
		{
			printf("Please enter a correct name!!");
			return; // this will terminate the return to the previous menu
		}
		else // if the user enters a correct name
		{
			carsNeeded = getUnsignedShortFromConsole("\nWhat is the number of cars are you looking for?: ");

			if (no_OfCarsAvailable < carsNeeded) 
			{
				printf("Sorry, there are fewer Cars remaining than you require.");
				return;
			}

			no_OfCarSoldPerSale[numberOfSales] = carsNeeded; // record the number of cars needed in the no_OfCarSoldPerSale array

			menu_showCarTypes();

			carType = getUnsignedShortFromConsole("\nWhat type of car are you looking for? Please choose from the list: ");

			if (carType > 9)
			{
				printf("\nUnfortunately, we do not have the car your're looking for available with us. Please come back later!");
				return;
			}
	
			typeOfCarPerSale[numberOfSales] = carType;	

			totalPrice = carsNeeded * carPrices[carType];

			no_OfCarsAvailable -= carsNeeded; 	// update number of cars available by subtracting from it the amount of cars needed

			userAge = getUnsignedShortFromConsole("\nThat's a great choice!! How old are you?: ");

			if (userAge < 18)  //validating the age entered by user
			{
				printf("Sorry, you are not eligible to purchase a car today! Please come back later.");
				return;
			}

 			// present the outcome
			printf("\n\nThank you.\n");
			printf("You have bought %hd Cars.\n", carsNeeded);
			printf("Total cost is %f GBP.\n", totalPrice);
			printf("\nThere are %hd Cars remaining.", no_OfCarsAvailable);

			numberOfSales++;	
			break;
		}
	}
}

void sortArraysByNumberOfCarsSoldPerSale() // bubble sort used to sort the array in descending order
{

	for (int i = 0; i < numberOfSales; i++) 
	{
		for (int j = i + 1; j < numberOfSales; j++) 

		{
			if (no_OfCarSoldPerSale[i] < no_OfCarSoldPerSale[j]) 
			{
				swapUnsignedShort(&no_OfCarSoldPerSale[i], &no_OfCarSoldPerSale[j]);

				swapUnsignedShort(&typeOfCarPerSale[i], &typeOfCarPerSale[j]);

				char temp[201];
				strcpy(temp, customerNames[i]);
				strcpy(customerNames[i], customerNames[j]);
				strcpy(customerNames[j], temp);

			} 

		} 

	} 

}

void printSalesDataAtPosition(int position) // function to display a row representing the sales data at a given array position
{
	int typeOfCar = typeOfCarPerSale[position];

	float price = no_OfCarSoldPerSale[position] * carPrices[typeOfCar];

	printf("Sale Index: %d | Sale Amount: %f | Type of Car: %s | "
		   "Car Price: %f | Number of Car: %hd | "
		   "| Customer Name: %s\n",
		   position, price, carTypes[typeOfCar],
		   carPrices[typeOfCar], no_OfCarSoldPerSale[position],
		   customerNames[position]);

}

void menu_viewSales() 
{
	sortArraysByNumberOfCarsSoldPerSale(); //sort array before showing data

	float totalSalesValue = 0;
	unsigned int carsSold = 0;

	printf("All Sales Data:\n\n");

	for (int i = 0; i < numberOfSales; i++) 
	{
		int typeOfCar = typeOfCarPerSale[i];
		float price = no_OfCarSoldPerSale[i] * carPrices[typeOfCar];

		printSalesDataAtPosition(i); //calling function to print the row of sales data at position i

		totalSalesValue += price; //adding up total sales value 
		carsSold += no_OfCarSoldPerSale[i]; //adding up total car sold

	}

	printf("\n%hd cars have been sold with a total value of %f GBP. There are %hd cars unsold.\n",
		   carsSold, totalSalesValue, no_OfCarsAvailable);  //display some overall statistics
}

void printSalesDataBasedOnCarsSoldBetweenMinMax(unsigned short min, unsigned short max) 
{
	printf("Sales Data containing between %hd and %hd cars sold:\n\n", min, max);

	for (int i = 0; i < numberOfSales; i++) 
	{
		if (min <= no_OfCarSoldPerSale[i] && no_OfCarSoldPerSale[i] <= max) 
		{
			printSalesDataAtPosition(i);

		}

	}

}

void menu_exit() 
{
	printf("Thank you for using this Car Sales program. Bye-bye!");

	saveDataToFile();
}

void main()
{
	getDataFromFile();

	char userChoice;

	do {
		clearScreen();

		menu_greetCustomer();
		menu_showMenu();

		userChoice = getCharFromConsole("Please choose one: ");
		
		clearScreen();

		switch (userChoice) 
		{
			case MENU_OPTION_BUY_CARS:
				menu_buyCars();
				break;

			case MENU_OPTION_VIEW_SALES:
				menu_viewSales();
				break;

			case MENU_OPTION_EXIT:
				menu_exit();
				break;
			
			default :
				printf("You have chosen a wrong choice. Please try again!");
				break;
				
		}

		inputUserChoice(userChoice);

	} while (userChoice != MENU_OPTION_EXIT);
	
	clearScreen();

	printf("\n\nHave a good day!\n\n"); //greeting customer
}