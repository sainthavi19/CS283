#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

// Prototypes
void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);
int count_words(char *, int, int);
void reverse_string(char *, int);
void write_to_file(char *, char *, int);
void print_words(char *, int);
void replace_string(char *, char *, char *, int);

// Helper functions

void usage(char *exename) {
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int setup_buff(char *buff, char *user_str, int len) {
    int i;
    for (i = 0; i < len - 1 && user_str[i] != '\0'; i++) {
        buff[i] = user_str[i];
    }
    buff[i] = '\0';
    return i;  // For now just so the code compiles. 
}

int count_words(char *buff, int len, int str_len) {
    int count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (buff[i] != ' ' && buff[i] != '\t' && buff[i] != '\n') {
            if (!in_word) {
                in_word = 1; 
                count++;
            }
        } else {
            in_word = 0;
        }
    }
    return count;
}

void reverse_string(char *buff, int len) {
    int start = 0;
    int end = len - 1;
    while (start < end) {
        char temp = buff[start];
        buff[start] = buff[end];
        buff[end] = temp;
        start++;
        end--;
    }
}

void write_to_file(char *filename, char *buff, int len) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        exit(99);
    }
    fprintf(file, "%s\n", buff);
    fclose(file);
    printf("Written to file %s\n", filename);
}

void print_words(char *buff, int len) {
    char word[BUFFER_SZ];
    int word_index = 0;
    for (int i = 0; i < len; i++) {
        if (buff[i] != ' ' && buff[i] != '\t' && buff[i] != '\n' && buff[i] != '\0') {
            word[word_index++] = buff[i];
        } else {
            if (word_index > 0) {
                word[word_index] = '\0';
                printf("Word: %s, Length: %d\n", word, word_index);
                word_index = 0;
            }
        }
    }
    if (word_index > 0) {
        word[word_index] = '\0';
        printf("Word: %s, Length: %d\n", word, word_index);
    }
}

void replace_string(char *buff, int len, const char *old_sub, const char *new_sub) {
    char temp[BUFFER_SZ];
    char *pos;
    int old_sub_len = strlen(old_sub);
    int new_sub_len = strlen(new_sub);
    int i = 0, j = 0;

    while ((pos = strstr(buff + i, old_sub)) != NULL) {
        // Copy characters before the match
        while (buff + i < pos) {
            temp[j++] = buff[i++];
        }
        // Copy the new substring
        for (int k = 0; k < new_sub_len; k++) {
            temp[j++] = new_sub[k];
        }
        // Move past the old substring
        i += old_sub_len;
    }

    // Copy the remaining part of the string
    while (buff[i] != '\0') {
        temp[j++] = buff[i++];
    }

    temp[j] = '\0';

    // Copy the modified string back into the buffer
    strncpy(buff, temp, len - 1);
    buff[len - 1] = '\0';
}

int main(int argc, char *argv[]) {
    char *buff; 
    char *input_string;
    char opt;
    int rc;
    int user_str_len;

    // TODO: #1. WHY IS THIS SAFE, aka what if argv[1] does not exist?
    // This code checks if there are at least two arguments and if the first argument starts with 
    // a dash. Without this, when trying to use argv[1], it would cause undefined behavior.
    // This check ensures the program exits gracefully.
    if ((argc < 2) || (*argv[1] != '-')) {
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1] + 1); // Get the option flag

    // Handle the help flag and then exit normally
    if (opt == 'h') {
        usage(argv[0]);
        exit(0);
    }

    // TODO: #2 Document the purpose of the if statement below
    // This ensures that the program has at least three arguments: 
    // the program name, the option flag, and the input string. 
    // Without this check, argv[2] could be NULL, which will lead to undefined behavior 
    // when the program is accessing it. If the condition is not met, it displays the usage 
    // instructions and exits with an error.
    if (argc < 3) {
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; // Capture the user input string

    // TODO: #3 Allocate space for the buffer using malloc and
    // handle error if malloc fails by exiting with a return code of 99
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);
    if (user_str_len < 0) {
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt) {
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);
            if (rc < 0) {
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'r':
            reverse_string(buff, user_str_len);
            printf("Reversed Buffer: %s\n", buff);
            break;

        case 'w':
            print_words(buff, user_str_len);
            break;

        case 'x':
            if (argc < 5) {
                printf("Error: Missing arguments for string replacement.\n");
                usage(argv[0]);
                exit(1);
            }
            printf("Replacing '%s' with '%s' in '%s'\n", argv[3], argv[4], buff);
            replace_string(buff, BUFFER_SZ, argv[3], argv[4]);
            printf("Updated Buffer: %s\n", buff);
            break;


        default:
            usage(argv[0]);
            exit(1);
    }

    // TODO: #6 Don't forget to free your buffer before exiting
    print_buff(buff, BUFFER_SZ);
    free(buff);
    exit(0);
}

// TODO: #7 Notice all of the helper functions provided in the 
// starter take both the buffer as well as the length. Why
// do you think providing both the pointer and the length
// is a good practice, after all we know from main() that 
// the buff variable will have exactly 50 bytes?
// It would reduce the risk of buffer overload because with passing the length it 
// makes it more generic and adaptable for different buffer sizes. If the buffer size 
// changes in the future, functions relying on BUFFER_SZ would need updates. 
// Passing the length ensures the function always works with the correct size.

void print_buff(char *buff, int len) {
    printf("Buffer:  ");
    for (int i = 0; i < len; i++) {
        putchar(*(buff + i));
    }
    putchar('\n');
}
