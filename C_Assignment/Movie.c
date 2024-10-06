#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define FILE_NAME "user_data.txt"
#define MOVIE_DATABASE_FILENAME "movie_database.txt"
#define USER_RATINGS_FILENAME "user_ratings.txt"

// Define a structure for a user
typedef struct {
    char *username;
} User; 

// Define a structure for a movie
typedef struct {
    char title[MAX_NAME_LENGTH];
    char genre[MAX_NAME_LENGTH];
    float rating;
} Movie;

// Define a structure for user ratings
typedef struct {
    int userId;
    int movieId;
    float rating;
} UserRating;

// Define a structure for a recommendation
typedef struct {
    char title[MAX_NAME_LENGTH];
    char genre[MAX_NAME_LENGTH];
    float predictedRating;
} Recommendation;

// Function to check if a user already exists in the user_data.txt 
int checkIfUserExists(char *username){
    FILE *file = fopen("user_data.txt", "r");
    if(file == NULL){
        return 0; // File doesn't exist, user doesn't exist
    }

    char buffer[MAX_NAME_LENGTH];
    while(fscanf(file, "%s", buffer) == 1){
        if(strcasecmp(buffer, username) == 0) { // case-insensitive comparision
            fclose(file);
            return 1; // User exists
        }
    }
    fclose(file);
    return 0; // User doesn't exist
}

// Function to register a new user
void registerUser(){
    User *newUser = malloc(sizeof(User));
    if (newUser == NULL){
        perror("Error allocating memory");
        exit(1);
    }

    newUser->username = malloc(MAX_NAME_LENGTH * sizeof(char)); // Allocate memory for the username
    if (newUser->username == NULL) {
        perror("Error allocating memory");
        exit(1);
    }

    // Get user input
    printf("Enter username for registration: ");
    scanf("%s", newUser->username);

    // Check if user already exists
    while(checkIfUserExists(newUser->username)){
        printf("User already exists. Please choose a different name.\n");
        printf("Enter username for registration: ");
        scanf("%s", newUser->username);
    }

    // Append the new username to user_data.txt
    FILE *file = fopen("user_data.txt", "a");
    if(file == NULL){
        perror("Error opening file");
        exit(1);
    }

    fprintf(file, "%s\n", newUser->username);
    fclose(file);

    printf("User %s is successfully registered.\n", newUser->username);

    // Free dynamically allocated memory
    free(newUser->username);
    free(newUser);

    // Update user ratings file with new user
    FILE *ratingsFile = fopen("user_ratings.txt", "r+");
    if (ratingsFile == NULL) {
        perror("Error opening file");
        return;
    }

    int numUsers, numMovies;
    fscanf(ratingsFile, "%d %d", &numUsers, &numMovies);
    numUsers++;

    // Move file pointer to the beginning to update the number of users
    rewind(ratingsFile);
    fprintf(ratingsFile, "%d %d\n", numUsers, numMovies);
    fclose(ratingsFile);

    // Append new user's ratings (initialized to 0) to the file
    FILE *ratingsAppendFile = fopen("user_ratings.txt", "a");
    if (ratingsAppendFile == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < numMovies; i++) {
        fprintf(ratingsAppendFile, "0.0 ");
    }
    fprintf(ratingsAppendFile, "\n");

    fclose(ratingsAppendFile);
}

// Function to display the main menu
void displayMenu(){
    printf("\n***** Movie Recommendation System *****\n");
    printf("1. Register User\n");
    printf("2. Display Movies\n");
    printf("3. Rate a Movie\n");
    printf("4. Get Movie Recommendation\n");
    printf("0. Exit\n");
}

// Function to display all movies from the movie database
void displayMovies() {
    FILE *file = fopen("movie_database.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    printf("\n***** Movie Recommendation System *****\n");
    Movie movie;

    int index = 1;
    while (fscanf(file, "%s %s %f", movie.title, movie.genre, &movie.rating) == 3) {
        // Replace underscores with spaces in the title
        for (int i = 0; movie.title[i] != '\0'; i++) {
            if (movie.title[i] == '_') {
                movie.title[i] = ' ';
            }
        }
        printf("%d. %s (%s) - %.1f\n", index, movie.title, movie.genre, movie.rating);
        index++;
    }

    fclose(file);
}

int validateMovieIndex(int numMovies){
    int movieIndex;
    do {
        displayMovies();
        printf("Enter the number of the movie you want to rate: ");
        scanf("%d", &movieIndex);
        if(movieIndex < 1 || movieIndex > numMovies){
            printf("Invalid movie index. Please enter a number between 1 and %d.\n", numMovies);
        }
    } while (movieIndex < 1 || movieIndex > numMovies);
    return movieIndex;
}

// Function to update the rating in user_ratings.txt
void updateRating(float **ratings, int numUsers, int numMovies, int userIndex, int movieIndex, float rating) {
    ratings[userIndex][movieIndex] = rating; // Update the rating

    // Open user_ratings.txt for writing
    FILE *file = fopen("user_ratings.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write the updated ratings to the file
    fprintf(file, "%d %d\n", numUsers, numMovies);
    for (int i = 0; i < numUsers; i++) {
        for (int j = 0; j < numMovies; j++) {
            fprintf(file, "%.1f ", ratings[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Rating recorded successfully.\n");
}

void rateMovie() {
    char username[MAX_NAME_LENGTH];

    // Get username
    printf("Enter your username: ");
    scanf("%s", username);
    if (!checkIfUserExists(username)) {
        printf("User not found. Please register first.\n");
        return;
    }

    // Get the index of the user
    int userIndex = -1;
    char line[MAX_NAME_LENGTH];
    FILE *userFile = fopen("user_data.txt", "r");
    if (userFile == NULL) {
        printf("Error opening rating file.\n");
        return;
    }
    while (fgets(line, sizeof(line), userFile) != NULL) {
        userIndex++;
        if (strstr(line, username) != NULL) {
            break;
        }
    }
    fclose(userFile);

    if (userIndex == -1) {
        printf("Error: User not found.\n");
        return;
    }

    // Read the existing ratings from user_ratings.txt into a 2D array
    FILE *ratingsFile = fopen("user_ratings.txt", "r");
    if (ratingsFile == NULL) {
        perror("Error opening file");
        return;
    }

    int numUsers, numMovies;
    fscanf(ratingsFile, "%d %d", &numUsers, &numMovies);

    // Allocate memory for ratings array
    float **ratings = malloc(numUsers * sizeof(float *));
    for (int i = 0; i < numUsers; i++) {
        ratings[i] = malloc(numMovies * sizeof(float));
    }

    // Read ratings into the array
    for (int i = 0; i < numUsers; i++) {
        for (int j = 0; j < numMovies; j++) {
            fscanf(ratingsFile, "%f", &ratings[i][j]);
        }
    }
    fclose(ratingsFile);
    
    // Validate movie index
    int movieIndex = validateMovieIndex(numMovies);

    // Validate rating
    float rating;
    do {
        printf("Enter your rating: ");
        scanf("%f", &rating);
        if (rating < 1 || rating > 5) {
            printf("Invalid rating. Please enter a number between 1 and 5.\n");
        }
    } while (rating < 1 || rating > 5);

    // Update the rating in the array and write back to file
    updateRating(ratings, numUsers, numMovies, userIndex, movieIndex - 1, rating); // Adjusting for 0-based indexing

    // Free dynamically allocated memory
    for (int i = 0; i < numUsers; i++) {
        free(ratings[i]);
    }
    free(ratings);
}

// Function to get movie recommendations for a user
void getMovieRecommendation() {
    char username[MAX_NAME_LENGTH];

    // Get username
    printf("Enter your username: ");
    scanf("%s", username);

    // Check if user exists
    if (!checkIfUserExists(username)) {
        printf("User not found. Please register first.\n");
        return;
    }

    // Read the existing ratings from user_ratings.txt into a 2D array
    FILE *ratingsFile = fopen("user_ratings.txt", "r");
    if (ratingsFile == NULL) {
        perror("Error opening file");
        return;
    }

    int numUsers, numMovies;
    fscanf(ratingsFile, "%d %d", &numUsers, &numMovies);

    // Allocate memory for ratings array
    float **ratings = malloc(numUsers * sizeof(float *));
    for (int i = 0; i < numUsers; i++) {
        ratings[i] = malloc(numMovies * sizeof(float));
    }

    // Read ratings into the array
    for (int i = 0; i < numUsers; i++) {
        for (int j = 0; j < numMovies; j++) {
            fscanf(ratingsFile, "%f", &ratings[i][j]);
        }
    }
    fclose(ratingsFile);

    // Find user index
    int userIndex = -1;
    FILE *userDataFile = fopen("user_data.txt", "r");
    if (userDataFile == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[MAX_NAME_LENGTH];
    while (fscanf(userDataFile, "%s", buffer) == 1) {
        userIndex++;
        if (strcmp(buffer, username) == 0) {
            break;
        }
    }
    fclose(userDataFile);

    if (userIndex == -1) {
        printf("User not found. Please register first.\n");
        return;
    }

    // Find movies not rated by the user
    printf("\n***** Recommended Movies *****\n");
    for (int i = 0; i < numMovies; i++) {
        if (ratings[userIndex][i] == 0) { // Movie not rated by the user
            float sum = 0;
            int count = 0;
            for (int j = 0; j < numUsers; j++) {
                if (j != userIndex && ratings[j][i] != 0) { // Exclude the target user's rating
                    sum += ratings[j][i];
                    count++;
                }
            }
            if (count > 0) {
                float predictedRating = sum / count;
                // Display movie details
                FILE *file = fopen("movie_database.txt", "r");
                if (file == NULL) {
                    perror("Error opening file");
                    return;
                }

                char title[MAX_NAME_LENGTH], genre[MAX_NAME_LENGTH];
                float rating;
                int index = 0;
                while (fscanf(file, "%s %s %f", title, genre, &rating) == 3) {
                    if (index == i) {
                        // Replace underscores with spaces in the title
                        for (int i = 0; title[i] != '\0'; i++) {
                            if (title[i] == '_') {
                                title[i] = ' ';
                            }
                        }
                        printf("%d. %s (%s) - Predicted Rating: %.1f\n", i + 1, title, genre, predictedRating);
                        break;
                    }
                    index++;
                }
                fclose(file);
            }
        }
    }

    // Free dynamically allocated memory
    for (int i = 0; i < numUsers; i++) {
        free(ratings[i]);
    }
    free(ratings);
}

int main(){
    int choice;

    do{
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                registerUser();
                break;

            case 2:
                displayMovies();
                break;

            case 3:
                rateMovie();
                break;

            case 4:
                getMovieRecommendation();
                break;
            
            case 0:
                printf("Exiting the program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        } 
    } while (choice != 0);

    return 0;
}