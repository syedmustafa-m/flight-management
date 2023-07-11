#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

#define MAX_USERS 100
#define MAX_FLIGHTS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_NAME_LENGTH 50
#define MAX_PASSPORT_LENGTH 20
#define MAX_CITY_LENGTH 10
#define MAX_FLIGHT_CODE_LENGTH 10

// User structure
typedef struct
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

// Flight structure
typedef struct
{
    char name[MAX_NAME_LENGTH];
    char passport[MAX_PASSPORT_LENGTH];
    char departureCity[MAX_CITY_LENGTH];
    char destinationCity[MAX_CITY_LENGTH];
    char flightCode[MAX_FLIGHT_CODE_LENGTH];
    int numPassengers;
} Flight;

// Function to read users from file
int readUsers(User users[])
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%s %s", users[count].username, users[count].password) == 2)
    {
        count++;
    }

    fclose(file);
    return count;
}

// Function to check if user credentials are valid
int authenticate(User users[], int numUsers, char username[], char password[])
{
    for (int i = 0; i < numUsers; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Function to display the main menu
void displayMenu()
{

    printf("\n===== Flight Management System =====\n");
    printf("1. Book a Flight\n\n");
    printf("2. Cancel a Flight\n\n");
    printf("3. View Booked Flights\n\n");
    printf("4. Exit\n\n");
    printf("Enter your choice: ");
}

// Function to book a flight
void bookFlight(Flight flights[], int *numFlights, char username[])
{

    char flightCode[MAX_FLIGHT_CODE_LENGTH];
    char string[100];

    printf("\n\n=====AVAILABLE FLIGHTS======\n\n");

    // Displaying all the flights
    FILE *fptr = fopen("flights.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    char line[100]; // Assuming each line is no longer than 100 characters
    bool flightFound = false;

    while (fgets(line, sizeof(line), fptr))
    {
        printf("%s", line); // Print the flight details
    }

    fclose(fptr);

    printf("\nEnter flight code: ");
    scanf("%s", flightCode);

    fptr = fopen("flights.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }



    while (fgets(line, sizeof(line), fptr))
    {
        // Extract the flight code from the line
        char code[MAX_FLIGHT_CODE_LENGTH];
        sscanf(line, "%*s %*s %s", code);

        if (strcmp(code, flightCode) == 0)
        {
            flightFound = true;
            break;
        }
    }

    fclose(fptr);

    if (flightFound)
    {

        // Checking if the flight is already booked by the current user
        FILE *book = fopen("bookings.txt", "r");
        if (book == NULL)
        {
            printf("Cannot open file \n");
            exit(0);
        }

        bool alreadyBooked = false;

        while(fgets(line, sizeof(line), book))
        {
            // Extract the flight code from booked flights
            char code[MAX_FLIGHT_CODE_LENGTH];
            char storedUsername[MAX_USERNAME_LENGTH];
            char status[MAX_NAME_LENGTH];
            sscanf(line, "%s %s %s", code, storedUsername, status);

            if(strcmp(code, flightCode) == 0 && strcmp(storedUsername, username) == 0 && strcmp(status, "cancelled") != 0)
            {
                alreadyBooked = true;
                break;
            }
        }

        if(alreadyBooked)
        {
            printf("This flight is already booked\n\n");
        }
        else
        {
            FILE *bookingsFile = fopen("bookings.txt", "a");
            if (bookingsFile == NULL)
            {
                printf("Error opening file!\n");
                return;
            }

            char status[MAX_NAME_LENGTH] = "booked";

            fprintf(bookingsFile, "%s %s %s\n", flightCode, username, status);

            fclose(bookingsFile);

            printf("Flight booked successfully!\n");
        }
    }
    else
    {
        printf("The file does not contain the flight with code: %s\n", flightCode);
    }
}


// Function to view flights booked by the user
void viewFlights(Flight flights[], int numFlights, char username[])
{
    printf("\nFlights booked by %s:\n", username);

    // Displaying all the flights
    FILE *fptr = fopen("bookings.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    char line[100]; // Assuming each line is no longer than 100 characters

    while (fgets(line, sizeof(line), fptr))
    {
        char storedUsername[MAX_USERNAME_LENGTH];
        char flightCode[MAX_FLIGHT_CODE_LENGTH];
        char status[MAX_NAME_LENGTH];

        sscanf(line, "%s %s %s", flightCode, storedUsername, status);

        if (strcmp(storedUsername, username) == 0)
        {
            FILE *flightFile = fopen("flights.txt", "r");
            if (flightFile == NULL)
            {
                printf("Cannot open file \n");
                exit(0);
            }

            char flightLine[100];
            bool flightFound = false;
            while (fgets(flightLine, sizeof(flightLine), flightFile))
            {
                char departureCity[4];
                char destinationCity[4];
                char code[MAX_FLIGHT_CODE_LENGTH];
                char departureTime[6];
                char arrivalTime[6];
                int seats;

                sscanf(flightLine, "%s %s %s %s %s %d",
                       departureCity, destinationCity, code, departureTime, arrivalTime, &seats);

                if (strcmp(code, flightCode) == 0)
                {
                    flightFound = true;
                    printf("Departure City: %s\n", departureCity);
                    printf("Destination City: %s\n", destinationCity);
                    printf("Departure Time: %s\n", departureTime);
                    printf("Arrival Time: %s\n", arrivalTime);
                    printf("Flight Code: %s\n", code);
                    printf("Available Seats: %d\n", seats);

                    if (strcmp(status, "cancelled") == 0)
                    {
                        printf("Flight Status: %s\n\n", status);
                    }
                    printf("\n");
                }
            }

            fclose(flightFile);

            if (!flightFound)
            {
                printf("Flight with code %s not found.\n\n", flightCode);
            }
        }
    }


    fclose(fptr);

}

void cancelFlight(Flight flights[], int *numFlights, char username[])
{
    printf("\n\n===== YOUR FLIGHTS ======\n\n");

    FILE *fptr = fopen("bookings.txt", "r+");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    char line[100];
    bool flightFound = false;

    while (fgets(line, sizeof(line), fptr))
    {
        char storedUsername[MAX_USERNAME_LENGTH];
        char status[MAX_NAME_LENGTH];
        sscanf(line, "%*s %s %s", storedUsername, status);

        if (strcmp(storedUsername, username) == 0 && strcmp(status, "cancelled") != 0)
        {
            printf("%s", line);
        }
    }

    char flightCode[MAX_FLIGHT_CODE_LENGTH];

    printf("\nEnter the flight code to cancel: ");
    scanf("%s", flightCode);

    rewind(fptr);  // Reset file pointer to the beginning

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Cannot create temporary file\n");
        fclose(fptr);
        exit(0);
    }

    while (fgets(line, sizeof(line), fptr))
    {
        char code[MAX_FLIGHT_CODE_LENGTH];
        char storedUsername[MAX_USERNAME_LENGTH];
        char status[MAX_NAME_LENGTH];
        sscanf(line, "%s %s %s", code, storedUsername, status);

        if (strcmp(code, flightCode) == 0 && strcmp(storedUsername, username) == 0)
        {
            strcpy(status, "cancelled");
            flightFound = true;
        }

        fprintf(tempFile, "%s %s %s\n", code, storedUsername, status);
    }

    fclose(fptr);
    fclose(tempFile);

    remove("bookings.txt");
    rename("temp.txt", "bookings.txt");

    if (flightFound)
    {
        printf("Flight canceled successfully!\n");
    }
    else
    {
        printf("Flight not found!\n");
    }
}



// Function to clear the screen
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main()
{
    User users[MAX_USERS];
    Flight flights[MAX_FLIGHTS];
    int numUsers, numFlights;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    numUsers = readUsers(users);
    numFlights = 0;

    printf("===== Flight Management System =====\n");

    while (1)
    {
        printf("\nEnter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        if (authenticate(users, numUsers, username, password))
        {
            printf("\nWelcome, %s!\n", username);
            break;
        }
        else
        {
            printf("Invalid username or password. Please try again.\n");
        }
    }

    int choice;
    while (1)
    {
        displayMenu();
        scanf("%d", &choice);



        switch (choice)
        {
        case 1:
            bookFlight(flights, &numFlights, username);
            break;
        case 2:
            cancelFlight(flights, &numFlights, username);
            break;
        case 3:
            viewFlights(flights, numFlights, username);
            break;
        case 4:
            printf("Thank you for using the Flight Management System!\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
