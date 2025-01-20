#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SPACE_CHAR ' '

// Prototypes for functions to handle required functionality
// TODO: #1 Providing prototypes allows us to declare functions before their definitions,
//          enabling the compiler to check for correct usage and function signatures throughout
//          the code. It is especially useful in modular programming where functions might be
//          defined in separate files.
void usage(char *);
int count_words(char *);
void reverse_string(char *);
void word_print(char *);

void usage(char *exename) {
    printf("usage: %s [-h|c|r|w] \"string\" \n", exename);
    printf("\texample: %s -w \"hello class\" \n", exename);
}


int count_words(char *str) {
    int wc = 0;
    bool word_start = false;
    int len = strlen(str);

    // Loop through the string and count words
    for (int i = 0; i < len; i++) {
        if (!word_start) {
            if (str[i] != SPACE_CHAR) {
                wc++;
                word_start = true;
            }
        } else {
            if (str[i] == SPACE_CHAR) {
                word_start = false;
            }
        }
    }
    // TODO: #2 Returning the word count value after counting all the words in the string
    return wc;
}

void reverse_string(char *str) {
    int start_idx = 0;
    int end_idx = strlen(str) - 1;
    char tmp_char;

    while (end_idx > start_idx) {
        tmp_char = str[start_idx];
        str[start_idx] = str[end_idx];
        str[end_idx] = tmp_char;

        start_idx++;
        end_idx--;
    }

    // TODO: #3 The string is reversed in place because arrays in C are mutable,
    //          and we are modifying the characters directly in memory without
    //          creating a new string.
}

void word_print(char *str) {
    int len = strlen(str);
    int wc = 0;
    int wlen = 0;
    bool word_start = false;

    for (int i = 0; i < len; i++) {
        if (!word_start) {
            if (str[i] != SPACE_CHAR) {
                wc++;
                word_start = true;
                wlen = 0;
                printf("%d. ", wc);
            }
        }

        if (word_start) {
            if (str[i] != SPACE_CHAR) {
                printf("%c", str[i]);
                wlen++;
            }

            if ((str[i] == SPACE_CHAR) || (i == len - 1)) {
                printf(" (%d)\n", wlen);
                word_start = false;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    char *input_string;
    char *opt_string;
    char opt;

    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }
    opt_string = argv[1];

    if ((opt_string[0] != '-') || (strlen(opt_string) != 2)) {
        usage(argv[0]);
        exit(1);
    }

    opt = opt_string[1];

    if (opt == 'h') {
        usage(argv[0]);
        exit(0);
    }

    if (argc != 3) {
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2];

    switch (opt) {
        case 'c': {
            int wc = count_words(input_string); // TODO: #2 Call count_words and store the result
            printf("Word Count: %d\n", wc);
            break;
        }
        case 'r': {
            reverse_string(input_string); // TODO: #3 Reverse the string
            printf("Reversed string: %s\n", input_string);
            break;
        }
         //TODO:  #4.  The algorithm provided in the directions 
            //            state we simply return after swapping all 
            //            characters because the string is reversed 
            //            in place.  Briefly explain why the string 
            //            is reversed in place - place in a comment
        case 'w': {
            printf("Word Print\n----------\n");
            word_print(input_string); // TODO: #5 Call word_print to display formatted words
            break;
        }
        default: {
            //TODO: #6. What is the purpose of the default option here?
            //          Please describe replacing this TODO comment with
            //          your thoughts.
            usage(argv[0]);
            printf("Invalid option %c provided, exiting!\n", opt);
            exit(1);
        }
    }

    //TODO: #7. Why did we place a break statement on each case
    //          option, and did not place one on default.  What
    //          would happen if we forgot the break statement?

    return 0;

}
