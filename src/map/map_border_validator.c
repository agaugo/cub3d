/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_border_validator.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:14 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 13:47:31 by trstn4        ########   odam.nl         */
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

    fp = fopen(file, "r");
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
