/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_setup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 12:20:59 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/07 18:05:46 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int cub_setup_map_checks(char *file) {
    FILE *fp, *fpWrite;
    char *outputFilename = "output.txt";
    char line[MAX_LINE_LENGTH];
    char *modifiedLines[1000];
    int longestLineLength = 0;
    int lineCount = 0;

    fp = fopen(file, "r");
    if (fp == NULL) {
        perror("Error opening input file");
        return(-1);
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        line[strcspn(line, "\n")] = 0; // Remove newline character
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

    // Write top padding
    for (int i = 0; i < longestLineLength + 2; i++) {
        fputc('.', fpWrite);
    }
    fputc('\n', fpWrite);

    for (int i = 0; i < lineCount; i++) {
        int currentLength = strlen(modifiedLines[i]);
        char *paddedLine = (char *)malloc(longestLineLength + 3); // Additional 2 for dots at both ends + 1 for null terminator
        if (paddedLine == NULL) {
            perror("Failed to allocate memory");
            return(-1);
        }
        paddedLine[0] = '.'; // Start with a dot
        strcpy(paddedLine + 1, modifiedLines[i]); // Copy the line after the starting dot
        memset(paddedLine + currentLength + 1, '.', longestLineLength - currentLength + 1); // Fill with dots
        paddedLine[longestLineLength + 1] = '.'; // End with a dot
        paddedLine[longestLineLength + 2] = '\0'; // Null terminator

        fprintf(fpWrite, "%s\n", paddedLine);
        free(paddedLine);
        free(modifiedLines[i]);
    }

    // Write bottom padding
    for (int i = 0; i < longestLineLength + 2; i++) {
        fputc('.', fpWrite);
    }
    fputc('\n', fpWrite);

    fclose(fpWrite);

    if (cub_is_border_valid(outputFilename)) {
        printf("The map is valid.\n");
    } else {
        printf("The map is not valid.\n");
    }

    return (0);
}
