#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int);
void reverse_string(char *, int);
void write_to_file(char *, char *, int);

// Function to set up buffer with the provided user string
int setup_buff(char *buff, char *user_str, int len) {
    //TODO: #4:  Implement the setup buff as per the directions
    int i;
    for (i = 0; i < len - 1 && user_str[i] != '\0'; i++) {
        buff[i] = user_str[i];
    }
    buff[i] = '\0';
    return i; //for now just so the code compiles. 
}

// Function to print the buffer contents
void print_buff(char *buff, int len) {
    printf("Buffer:  ");
    for (int i = 0; i < len; i++) {
        putchar(*(buff + i));
    }
    putchar('\n');
}

// Function to display usage instructions
void usage(char *exename) {
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

// Function to count words in the buffer
int count_words(char *buff, int len) {
    int count = 0;
    int in_word = 0;

    for (int i = 0; i < len; i++) {
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

// Function to reverse the string in the buffer
void reverse_string(char *buff, int len) {
    int start = 0;
    int end = len - 1;
    char temp;

    // Reverse the buffer in-place
    while (start < end) {
        temp = buff[start];
        buff[start] = buff[end];
        buff[end] = temp;
        start++;
        end--;
    }
}

// Function to write the buffer content to a file
void write_to_file(char *filename, char *buff, int len) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        exit(1);
    }
    fwrite(buff, sizeof(char), len, file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    char *buff;             // Placeholder for the internal buffer
    char *input_string;     // Holds the string provided by the user on cmd line
    char opt;               // Used to capture user option from cmd line
    int rc;                 // Used for return codes
    int user_str_len;       // Length of user supplied string

    // TODO: #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    // This code checks if there are at least two arguments and the first argument starts with 
    // a dash. Without this, trying to use argv[1] would cause undefined behavior.
    // This check ensures the program exits gracefully.
    if ((argc < 2) || (*argv[1] != '-')) {
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1] + 1);   // Get the option flag

    // Handle the help flag and then exit normally
    if (opt == 'h') {
        usage(argv[0]);
        exit(0);
    }

    // WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    // TODO: #2 Document the purpose of the if statement below
    // This ensures that the program has at least three arguments: 
    // the program name, the option flag, and the input string. 
    // Without this check, argv[2] could be NULL, which would lead 
    // to undefined behavior when the program tries to access it. 
    // If the condition is not met, it displays the usage instructions and exits with an error.
    if (argc < 3) {
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; // Capture the user input string

    // TODO: #3 Allocate space for the buffer using malloc and handle error if malloc fails
    // by exiting with a return code of 99
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ); // Set up buffer
    if (user_str_len < 0) {
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt) {
        case 'c':
            // Use the actual string length for word count
            rc = count_words(buff, user_str_len);
            if (rc < 0) {
                printf("Error counting words, rc = %d\n", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'r':
            reverse_string(buff, user_str_len);
            printf("Reversed Buffer: %s\n", buff);
            break;


        case 'w':
            if (argc < 4) { // Ensure a filename is provided
                printf("Error: No filename provided for write operation.\n");
                usage(argv[0]);
                exit(1);
            }
            write_to_file(argv[3], buff, user_str_len); // Pass user_str_len here
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
// starter take both the buffer as well as the length.  Why
// do you think providing both the pointer and the length
// is a good practice, after all we know from main() that 
// the buff variable will have exactly 50 bytes?
// 
// PLACE YOUR ANSWER HERE
// It would reduce the risk of buffer overflow because by passing the length, it makes 
// the function more generic and adaptable for different buffer sizes. If the buffer size 
// changes in the future, functions relying on BUFFER_SZ would need updates. 
// Passing the length ensures the function always works with the correct size.
