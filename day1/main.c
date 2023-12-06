#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

char** read_file(const char* filename, int* num_lines) {
    FILE* fp;
    char** lines = NULL;
    char buffer[1024];
    int line_count = 0;

    fp = fopen(filename, "r");
    if (!fp) {
        printf("failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        line_count++;
    }

    fseek(fp, 0, SEEK_SET);

    lines = (char**)malloc(line_count * 1024);
    if (!lines) {
        printf("failed to allocate lines");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < line_count; i++) {
        lines[i] = (char*)malloc(1024);
        if (!lines[i]) {
            printf("failed to allocate line: %d", i);
            exit(EXIT_FAILURE);
        }
        if (!fgets(lines[i], 1024, fp)) {
            printf("failed to read line: %d", i);
            exit(EXIT_FAILURE);
        }
        size_t len = strlen(lines[i]);
        if (len > 0 && lines[i][len - 1] == '\n') {
            lines[i][len- 1] = '\0';
        }
    }

    fclose(fp);
    *num_lines = line_count;
    return lines;
}

void free_file(char** file_content, int num_lines) {
    for (int i = 0; i < num_lines; i++) {
        if (file_content[i])
            free(file_content[i]);
    }
    if (file_content)
        free(file_content);
}

void replace(char *copy, const char *word, int num) {
    char *res = strstr(copy, word);

    while (res != NULL) {
        int wordLength = strlen(word);
        int numLength = snprintf(NULL, 0, "%d", num);
        char tempBuffer[strlen(res) - wordLength + 1];
        strcpy(tempBuffer, res + wordLength);
        snprintf(res, wordLength + 1, "%d", num);
        strcat(res, tempBuffer);
        res += numLength;
        res = strstr(res, word);
    }
}

void solution(char** file_content, int num_lines) {
    const char* words[] = {
            "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    int result = 0;

    for (int i = 0; i < num_lines; i++) {
        char* word = file_content[i];
        char* copy = (char*)malloc((strlen(word) + 1) * sizeof(char));

        if (!copy) {
            printf("failed to alloc copy.\n");
            exit(EXIT_FAILURE);
        }

        strcpy(copy, word);

        //start of part 2
        replace(copy, "oneight", 18);
        replace(copy, "threeight", 38);
        replace(copy, "fiveight", 58);
        replace(copy, "nineight", 98);
        replace(copy, "twone", 21);
        replace(copy, "sevenine", 79);
        replace(copy, "eightwo", 82);

        for (int j = 0; j < ARRAY_LENGTH(words); j++) {
            replace(copy, words[j], j + 1);
        }
        //end of part 2

        char start, end;

        for (size_t j = 0; j <= strlen(copy); j++) {
            if (isdigit(copy[j])) {
                start = copy[j];
                break;
            }
        }

        for (size_t j = strlen(copy); j >= 0; j--) {
            if (isdigit(copy[j])) {
                end = copy[j];
                break;
            }
        }

        char out[3] = {start, end, '\0'};
        result += strtol(out, NULL, 10);
        free(copy);
    }

    printf("%d", result);
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: %s filename.txt", argv[0]);
        return EXIT_FAILURE;
    }

    int num_lines = 0;
    char** file_content = read_file(argv[1], &num_lines);

    solution(file_content, num_lines);

    free_file(file_content, num_lines);
    return EXIT_SUCCESS;
}
