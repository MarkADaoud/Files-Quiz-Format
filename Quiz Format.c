/*
 Author: Mark Daoud
 Course: CCOMP1010, Computing I
 Date: 11/30/23
 Description: This program opens two files one for reading and one for writing and checks to see if they both opened correctley. Then the 
 program prints a general header for each column in the output file. The program then reads each line of the file and finds the names of 
 tthe indiviuals and their test scores. The program then prints to the output file the names of the indiviuals in last, first and each of their 
 quiz scores on one line with the average at the end assuming that the number of quizzes was 10 and it was a 0 if they did not take it. The program 
 then closes those two files and thn opens the files in the reverse order and reads from the output file and prints that information in the input 
 file essentially swapping the initial input file wih the names and quiz grades with the formatted output file. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const int total_quizzes = 10;
//Pre-condition: The file assigned to fp has been opened
//Post-condition: The string of characters has been read into the character arrays either first_name or last_name
char* read_string(FILE* fp, char buffer[], int size); 

//Pre-condition: The strings of characters have been read into the character arrays first_name and last_name 
//Post-condition: The character in the chracter arrays of first_name and last_name have been combined into one character array full_name
void combine_names(char first_name[], char last_name[], char full_name[], int size); 

//Pre-condition: The file assigned to fp has been opened 
//Post-condition: The scores have been read and their values have been stored in the scores integer array and the number of scores
//has been stored in the address of the integer num_scores 
void process_scores(FILE* fp, int scores[], int* num_scores);

//Pre-condition: The scores have beeen stored in the scores integer array and the number of scores has been stored in the address of the integer num_scores 
//Post-condition: The average of thae scores has been returned as a double 
double calculate_average(int scores[], int num_scores, int total_quizzes);  

//Pre-condition: The file assigned to ofp has been opened, the scores have beeen stored in the scores integer array and the number of scores
//has been stored in the address of the integer num_scores 
//Post-condition: The scores have beeen printed to the file in the file pointer ofp
void print_scores(FILE* ofp, int scores[], int num_scores, int total_quizzes); 


int main(int argc, char* argv[])
{
	FILE* fp;
	FILE* ofp;
    char first_name[21];
    char last_name[21];
    char full_name[21];
    int scores[10]; 
    double average;
    

	fp = fopen("quiz.txt", "r");
	ofp = fopen("average.txt", "w");

	if (fp == NULL || ofp == NULL)
	{
		printf("Error opening Files");
		exit(1);
	}

    //Print a header at the top of the file for each column followed by a new line 
    fprintf(ofp, "%-20s", "Student Name");  
    for (int i = 1; i <= total_quizzes; ++i)  
    {
        fprintf(ofp, "%3s%d ", "Q", i); 
    }
    fprintf(ofp, "%10s\n", "Average"); 

    fprintf(ofp, "\n");

    //While there is a name to be read into the chracter array first_name, read the next name into the chracter array last_name
    //and read the scores of that person and combine them into one chracter array full_name and then print the scores 
    //and calculate the average for each full_name 
    while (read_string(fp, first_name, 21)) 
    {
        read_string(fp, last_name, 21); 
       
        int num_scores = 0;
        process_scores(fp, scores, &num_scores); 

        combine_names(last_name, first_name, full_name, 21); 

        fprintf(ofp, "%-20s", full_name); //Print the full name left justified 20 spaces
       
        print_scores(ofp, scores, num_scores, total_quizzes);   
       
        average = calculate_average(scores, num_scores, total_quizzes);   
        fprintf(ofp, "%10.2f\n", average); //Print tne Average right justified 10 spaces with a float of 2 places  
    }
    
	fclose(fp);
	fclose(ofp);

    fp = fopen("average.txt", "r");
    ofp = fopen("quiz.txt", "w");

    if (fp == NULL || ofp == NULL) 
    {
        printf("Error opening Files"); 
        exit(1); 
    } 
    
    int noc = 1;
    char c;

    noc = fscanf(fp, "%c", &c);

    while (noc == 1)
    {
        fprintf(ofp, "%c", c);
        noc = fscanf(fp, "%c", &c);  
    }

    fclose(fp);
    fclose(ofp);


	return 0;
}

void process_scores(FILE* input, int scores[], int* num_scores)
{
    int score;
    *num_scores = 0;

    //While there is a score to be read, set that score equal to that spot in the scores array and increase the number of scores 
    while (fscanf(input, "%d", &score) == 1) 
    {
        scores[*num_scores] = score; 
        *num_scores += 1; 
    }
} 
double calculate_average(int scores[], int num_scores, int total_quizzes)  
{
    int total_scores = 0;  
    if (num_scores == 0)  
    {
        return 0.0; // If there is no scores return the average being 0.0
    }

    //While we are under the number of scores increase the value of i then add the value of the score at that spot to the total score 
    for (int i = 0; i < num_scores; ++i) 
    {
        total_scores += scores[i]; 
    }

    //Return the average of that persons scores 
    return (double)total_scores / total_quizzes;    
}
void print_scores(FILE* ofp, int scores[], int num_scores, int total_quizzes)
{
    for (int i = 0; i < total_quizzes; ++i)
    {
        if (i < num_scores)
        {
            fprintf(ofp, "%4d ", scores[i]);   //Print each score right justified 4
        }
        else
        {
            fprintf(ofp, "%4s ", "-");  // If there is no score print a - right justified 4 
        } 
    }
}

char* read_string(FILE* fp, char buffer[], int size)
{
    char c;
    int character;
    int noc;
    int i = 0;
    if (size < 2)
    {
        return NULL;
    }
    noc = fscanf(fp, " %c", &c);
    if (noc == EOF)
    {
        return NULL;
    }
    buffer[i] = c;
    i++;

    character = fgetc(fp);
    while (character != EOF && !isspace(character) && i < size - 1)
    {
        buffer[i] = character;
        i++;
        character = fgetc(fp);
    }
    if (character != EOF)
    {
        ungetc(character, fp);
    }

    buffer[i] = '\0';
    return buffer;
}

void combine_names(char first_name[], char last_name[], char full_name[], int size)
{
    int i = 0;
    int j = 0;
    for (i = 0; first_name[i] != '\0' && i < size - 1; i++)
    {
        full_name[i] = first_name[i];  
    }

    if (i < size - 1)
    {
        full_name[i] = ',';
        i++;
        full_name[i] = ' '; 
        i++; 
    }

    while (last_name[j] != '\0' && i < size - 1)
    {
        full_name[i] = last_name[j]; 
        i++;
        j++;
    } 

    full_name[i] = '\0'; 
} 
