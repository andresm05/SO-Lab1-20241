//Authors: Jaime Andrés Muñoz and Estefania Goez Moreno

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 

#define MAX_LINE_LENGTH 1024


void printErrorMessage(const char *message) {
    fprintf(stderr, "%s\n", message);
}


void printErrorMessages(const char *message, const char *filename) {
    fprintf(stderr, "reverse: %s '%s'\n", message, filename);
}

//This function reverse the order of the lines in the array
void reverseLines(char **lines, int numLines) {
    int start = 0;
    int end = numLines - 1;
    char *temp;

    while (start < end) {
        temp = lines[start];
        lines[start] = lines[end];
        lines[end] = temp;
        start++;
        end--;
    }
}

int main(int argc, char *argv[]) {
    FILE *inputFile;
    FILE *outputFile;

    // Check the number of arguments
    if (argc > 3) {
        printErrorMessage("usage: reverse <input> <output>");
        exit(1);
    }

    // Managed the input and output files
    if (argc == 1) {
        // Doesn't specify any argument, read from stdin and write to stdout
        inputFile = stdin;
        outputFile = stdout;
    } else if (argc == 2) {
        // Just one argument is specified, read from the input file and write to stdout
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            printErrorMessages("cannot open file", argv[1]);
            exit(1);
        }
        outputFile = stdout;
    } else {
        // Two arguments are specified, read from the input file and write to the output file
        struct stat input_stat, output_stat;
        if (stat(argv[1], &input_stat) == 0 && stat(argv[2], &output_stat) == 0){
             if (input_stat.st_ino == output_stat.st_ino){
                printErrorMessage("reverse: input and output file must differ");
                exit(1);
             }
        }
        
        // Open the input and output files
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            printErrorMessages("cannot open file", argv[1]);
            exit(1);
        }
        outputFile = fopen(argv[2], "w");
        if (outputFile == NULL) {
            printErrorMessage("cannot open output file");
            exit(1);
        }

        // Make sure that output file is different from input file
        if (strcmp(argv[1], argv[2]) == 0) {
            printErrorMessage("reverse: input and output file must differ");
            exit(1);
        }
    }

    // Save the lines in an array
    char **lines = NULL;
    int numLines = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        // Delete the newline character
        line[strcspn(line, "\n")] = '\0';

        // Add line to the array
        lines = realloc(lines, (numLines + 1) * sizeof(char *));
        if (lines == NULL) {
            printErrorMessage("malloc failed");
            exit(1);
        }
        lines[numLines] = strdup(line);
        if (lines[numLines] == NULL) {
            printErrorMessage("malloc failed");
            exit(1);
        }
        numLines++;
    }

    // Reverse the order of the lines
    reverseLines(lines, numLines);

    // Print the lines in the output file
    for (int i = 0; i < numLines; i++) {
        fprintf(outputFile, "%s\n", lines[i]);
        free(lines[i]);
    }
    free(lines);

    // Close lines if they're not stdin/stdout
    if (inputFile != stdin) fclose(inputFile);
    if (outputFile != stdout) fclose(outputFile);

    return 0;
}