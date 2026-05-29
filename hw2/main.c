#include <stdio.h>
#include <string.h>

/* Token Types */
#define NUMBER_TOKEN 1
#define PLUS_TOKEN 2
#define LEFTPAREN_TOKEN 3
#define RIGHTPAREN_TOKEN 4
#define EOF_TOKEN 5
#define ERROR_TOKEN 6

/* Global Variables */
FILE *fp;
int current_token;
int depth = 0;

/* =========================
   Scanner Helper Functions
   ========================= */

int is_space(char c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

/* =========================
   Print Tree Indent
   ========================= */

void print_indent() {

    int i;

    for (i = 0; i < depth; i++) {
        printf("    ");
    }
}

/* =========================
   Scanner
   ========================= */

int getNextToken() {

    int ch;

    while ((ch = fgetc(fp)) != EOF) {

        /* Ignore spaces */
        if (is_space(ch)) {
            continue;
        }

        /* Number */
        else if (is_digit(ch)) {

            while ((ch = fgetc(fp)) != EOF && is_digit(ch));

            if (ch != EOF) {
                ungetc(ch, fp);
            }

            return NUMBER_TOKEN;
        }

        /* + */
        else if (ch == '+') {
            return PLUS_TOKEN;
        }

        /* ( */
        else if (ch == '(') {
            return LEFTPAREN_TOKEN;
        }

        /* ) */
        else if (ch == ')') {
            return RIGHTPAREN_TOKEN;
        }

        /* Error */
        else {
            return ERROR_TOKEN;
        }
    }

    return EOF_TOKEN;
}

/* =========================
   Error Handling
   ========================= */

void error() {
    printf("Syntax Error\n");
}

/* =========================
   Match Function
   ========================= */

void match(int expected_token) {

    if (current_token == expected_token) {

        current_token = getNextToken();
    }
    else {

        error();
    }
}

/*
Grammar:

S  -> E S'
S' -> epsilon
S' -> + S
E  -> num
E  -> ( S )
*/

/* Forward Declaration */
void parse_S();

/* =========================
   parse_S_prime
   S' -> epsilon | + S
   ========================= */

void parse_S_prime() {

    if (current_token == PLUS_TOKEN) {

        print_indent();
        printf("S' -> + S\n");

        match(PLUS_TOKEN);

        depth++;

        parse_S();

        depth--;
    }
    else {

        print_indent();
        printf("S' -> epsilon\n");
    }
}

/* =========================
   parse_E
   E -> num | ( S )
   ========================= */

void parse_E() {

    if (current_token == NUMBER_TOKEN) {

        print_indent();
        printf("E -> num\n");

        match(NUMBER_TOKEN);
    }

    else if (current_token == LEFTPAREN_TOKEN) {

        print_indent();
        printf("E -> ( S )\n");

        match(LEFTPAREN_TOKEN);

        depth++;

        parse_S();

        depth--;

        match(RIGHTPAREN_TOKEN);
    }

    else {

        error();
    }
}

/* =========================
   parse_S
   S -> E S'
   ========================= */

void parse_S() {

    print_indent();
    printf("S -> E S'\n");

    depth++;

    parse_E();

    parse_S_prime();

    depth--;
}

/* =========================
   Main
   ========================= */

int main() {

    /* Open test file */
    fp = fopen("test.txt", "r");

    if (fp == NULL) {

        printf("File Open Error\n");
        return 1;
    }

    /* Get first token */
    current_token = getNextToken();

    /* Start Parsing */
    parse_S();

    /* Check End */
    if (current_token == EOF_TOKEN) {

        printf("\nParsing Success\n");
    }
    else {

        printf("\nSyntax Error\n");
    }

    fclose(fp);

    return 0;
}
