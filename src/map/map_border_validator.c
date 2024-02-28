#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int isValidAdjacentChar(char c) {
    return c == '1' || c == '.' || c == '\n' || c == '\0';
}

int isMapValid(const char *filename) {
    FILE *fp;
    char line[MAX_LINE_LENGTH + 1];
    char prevLine[MAX_LINE_LENGTH + 1] = {0};
    char nextLine[MAX_LINE_LENGTH + 1];

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    if (!fgets(prevLine, MAX_LINE_LENGTH, fp)) {
        fclose(fp);
        return -1;
    }
    prevLine[strcspn(prevLine, "\n")] = '\0';

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        if (!feof(fp)) {
            long int currentPos = ftell(fp);
            if (fgets(nextLine, MAX_LINE_LENGTH, fp)) {
                fseek(fp, currentPos, SEEK_SET);
                nextLine[strcspn(nextLine, "\n")] = '\0';
            } else {
                nextLine[0] = '\0';
            }
        } else {
            nextLine[0] = '\0';
        }

        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '.') {
                if (i > 0 && line[i - 1] != '1' && line[i - 1] != '.') return 0;
                if (line[i + 1] != '\0' && line[i + 1] != '1' && line[i + 1] != '.') return 0;
                if (prevLine[i] != '1' && prevLine[i] != '.') return 0;
                if (nextLine[i] != '1' && nextLine[i] != '.') return 0;
            }
        }

        strncpy(prevLine, line, MAX_LINE_LENGTH);
    }

    fclose(fp);
    return 1;
}

int start_map() {
    FILE *fp, *fpWrite;
    char *inputFilename = "map.cub";
    char *outputFilename = "output.txt";
    char line[MAX_LINE_LENGTH];
    char *modifiedLines[1000];
    int longestLineLength = 0;
    int lineCount = 0;

    fp = fopen(inputFilename, "r");
    if (fp == NULL) {
        perror("Error opening input file");
        return(-1);
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        line[strcspn(line, "\n")] = 0;
        for (int i = 0; line[i]; i++) {
            if (line[i] == ' ') {
                line[i] = '.';
            }
        }

        int length = strlen(line);
        if (length > longestLineLength) {
            longestLineLength = length;
        }

        modifiedLines[lineCount] = strdup(line);
        lineCount++;
    }
    fclose(fp);

    fpWrite = fopen(outputFilename, "w");
    if (fpWrite == NULL) {
        perror("Error opening output file");
        return(-1);
    }

    for (int i = 0; i < lineCount; i++) {
        int currentLength = strlen(modifiedLines[i]);
        char *paddedLine = (char *)malloc(longestLineLength + 1);
        if (paddedLine == NULL) {
            perror("Failed to allocate memory");
            return(-1);
        }
        strcpy(paddedLine, modifiedLines[i]);
        memset(paddedLine + currentLength, '.', longestLineLength - currentLength);
        paddedLine[longestLineLength] = '\0';

        fprintf(fpWrite, "%s\n", paddedLine);
        free(paddedLine);
        free(modifiedLines[i]);
    }
    fclose(fpWrite);

    if (isMapValid(outputFilename)) {
        printf("The map is valid.\n");
    } else {
        printf("The map is not valid.\n");
    }

    return 0;
}
