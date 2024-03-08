/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_border_validator.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:14 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/08 15:39:11 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int cub_is_border_valid(const char *file) {
    FILE *fp;
    char line[MAX_LINE_LENGTH + 1];
    char prevLine[MAX_LINE_LENGTH + 1] = {0};
    char nextLine[MAX_LINE_LENGTH + 1];
    int lineNum = 0, isValid = 1;

    fp = fopen(file, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        lineNum++;
        line[strcspn(line, "\n")] = '\0';

        // Skip validation for the first and last lines
        if (lineNum == 1 || feof(fp)) {
            strncpy(prevLine, line, MAX_LINE_LENGTH);
            continue;
        }

        if (fgets(nextLine, MAX_LINE_LENGTH, fp)) {
            fseek(fp, -ft_strlen(nextLine), SEEK_CUR);
            nextLine[strcspn(nextLine, "\n")] = '\0';
        } else {
            nextLine[0] = '\0';
        }

        for (int i = 1; line[i + 1] != '\0'; i++) { // Start from 1 and end one character early to skip first and last character
            if (line[i] == '.') {
                if (line[i - 1] != '1' && line[i - 1] != '.') isValid = 0;
                if (line[i + 1] != '1' && line[i + 1] != '.') isValid = 0;
                if (prevLine[i] != '1' && prevLine[i] != '.') isValid = 0;
                if (nextLine[i] != '1' && nextLine[i] != '.') isValid = 0;
            }
        }

        if (!isValid) break;

        strncpy(prevLine, line, MAX_LINE_LENGTH);
    }

    fclose(fp);
    return isValid;
}
