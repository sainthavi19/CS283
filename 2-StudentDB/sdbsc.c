#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>

// Define a structure to represent a student
typedef struct {
    int id;
    char fname[20];
    char lname[20];
    int gpa;
} student_t;

// Define constants for magic numbers
#define MIN_ID 1000
#define MAX_ID 9999
#define MAX_GPA 400

// Define a constant for the database file name
#define DB_FILE "dbfile.db"

// Define a constant for the temporary database file name
#define TEMP_DB_FILE "temp.db"

// Function to open the database file
int open_db(const char *dbFile, bool shouldTruncate) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_RDWR | O_CREAT;

    if (shouldTruncate) {
        flags |= O_TRUNC;
    }

    int fd = open(dbFile, flags, mode);
    if (fd == -1) {
        perror("Error opening database file");
        return -1;
    }

    return fd;
}

// Function to add a student to the database
int add_student(int fd, int id, const char *fname, const char *lname, int gpa) {
    // Check if the id and gpa are within the valid range
    if (id < MIN_ID || id > MAX_ID) {
        printf("Error: Invalid id\n");
        return -1;
    }

    if (gpa < 0 || gpa > MAX_GPA) {
        printf("Error: Invalid gpa\n");
        return -1;
    }

    // Check if the student already exists
    student_t temp;
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &temp, sizeof(student_t)) > 0) {
        if (temp.id == id) {
            printf("Error: Student already exists\n");
            return -1;
        }
    }

    // Add the student to the database
    student_t newStudent = {id, {0}, {0}, gpa};
    strcpy(newStudent.fname, fname);
    strcpy(newStudent.lname, lname);
    lseek(fd, 0, SEEK_END);
    if (write(fd, &newStudent, sizeof(student_t)) == -1) {
        perror("Error writing to database file");
        return -1;
    }

    printf("Student added successfully\n");
    return 0;
}

// Function to get a student from the database
int get_student(int fd, int id, student_t *s) {
    lseek(fd, 0, SEEK_SET);
    student_t temp;
    while (read(fd, &temp, sizeof(student_t)) > 0) {
        if (temp.id == id) {
            *s = temp;
            return 0;
        }
    }

    printf("Error: Student not found\n");
    return -1;
}

// Function to delete a student from the database
int del_student(int fd, int id) {
    lseek(fd, 0, SEEK_SET);
    student_t temp;
    while (read(fd, &temp, sizeof(student_t)) > 0) {
        if (temp.id == id) {
            // Delete the student by overwriting with an empty record
            student_t emptyStudent = {0};
            lseek(fd, -sizeof(student_t), SEEK_CUR);
            if (write(fd, &emptyStudent, sizeof(student_t)) == -1) {
                perror("Error writing to database file");
                return -1;
            }

            printf("Student deleted successfully\n");
            return 0;
        }
    }

    printf("Error: Student not found\n");
    return -1;
}

// Function to count the number of students in the database
int count_db_records(int fd) {
    lseek(fd, 0, SEEK_SET);
    student_t temp;
    int count = 0;
    while (read(fd, &temp, sizeof(student_t)) > 0) {
        if (temp.id != 0) {
            count++;
        }
    }

    printf("Number of students: %d\n", count);
    return count;
}

// Function to print all students in the database
int print_db(int fd) {
    lseek(fd, 0, SEEK_SET);
    student_t temp;
    while (read(fd, &temp, sizeof(student_t)) > 0) {
        if (temp.id != 0) {
            printf("ID: %d, Name: %s %s, GPA: %d\n", temp.id, temp.fname, temp.lname, temp.gpa);
        }
    }

    return 0;
}

// Function to compress the database
int compress_db(int fd) {
    int tempFd = open(TEMP_DB_FILE, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (tempFd == -1) {
        perror("Error creating temporary database file");
        return -1;
    }

    lseek(fd, 0, SEEK_SET);
    student_t temp;
    while (read(fd, &temp, sizeof(student_t)) > 0) {
        if (temp.id != 0) {
            if (write(tempFd, &temp, sizeof(student_t)) == -1) {
                perror("Error writing to temporary database file");
                close(tempFd);
                return -1;
            }
        }
    }

    close(fd);
    close(tempFd);
    remove(DB_FILE);
    rename(TEMP_DB_FILE, DB_FILE);

    printf("Database compressed successfully\n");
    return 0;
}

int main(int argc, char *argv[]) {
    int fd = open_db(DB_FILE, false);
    if (fd == -1) {
        return 1;
    }

    student_t newStudent = {1000, {0}, {0}, 300};
    strcpy(newStudent.fname, "John");
    strcpy(newStudent.lname, "Doe");
    lseek(fd, 0, SEEK_END);
    if (write(fd, &newStudent, sizeof(student_t)) == -1) {
        perror("Error writing to database file");
        return 1;
    }

    printf("Student added successfully\n");

    close(fd);
    return 0;
}
