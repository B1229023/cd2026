#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 字元判斷函式
int is_space(char c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

int is_alpha(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'); 
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_alnum(char c) {
    return (is_alpha(c) || is_digit(c));
}

int main() {
	FILE *fp;
    //fp = stdin;
    char ch;
    //讀取test.txt測試檔案 
	fp = fopen("test.txt", "r"); 
    if (fp == NULL) {
        printf("file open error\n");
        return 1;
    }
    // 不斷讀取字元直到檔案結束 (EOF)
    while ((ch = fgetc(fp)) != EOF) {
        
        // 1. 忽略空白字元
        if (is_space(ch)) {
            continue;
        }
        
        // 2. 判斷是否為 ID 或 Keyword (以字母或底線開頭)
        else if (is_alpha(ch)) {
            char buffer[256];
            int i = 0;
            buffer[i++] = ch;
            
            // 繼續讀取直到不是字母、數字或底線
            while ((ch = fgetc(fp)) != EOF && is_alnum(ch)) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0'; // 字串結尾
            
            // 將多讀到但不屬於 ID 的字元塞回檔案流
            if (ch != EOF) {
                ungetc(ch, fp);
            }
            
            // 分辨是 Keyword 還是 ID
            if (strcmp(buffer, "int") == 0) {
                printf("%s: TYPE_TOKEN\n", buffer);
            } else if (strcmp(buffer, "main") == 0) {
                printf("%s: MAIN_TOKEN\n", buffer);
            } else if (strcmp(buffer, "if") == 0) {
                printf("%s: IF_TOKEN\n", buffer);
            } else if (strcmp(buffer, "else") == 0) {
                printf("%s: ELSE_TOKEN\n", buffer);
            } else if (strcmp(buffer, "while") == 0) {
                printf("%s: WHILE_TOKEN\n", buffer);
            } else {
                printf("%s: ID_TOKEN\n", buffer);
            }
        }
        
        // 3. 判斷是否為數字
        else if (is_digit(ch)) {
            char buffer[256];
            int i = 0;
            buffer[i++] = ch;
            
            while ((ch = fgetc(fp)) != EOF && is_digit(ch)) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            
            if (ch != EOF) {
                ungetc(ch, fp);
            }
            printf("%s: LITERAL_TOKEN\n", buffer);
        }
        
        // 4. 判斷 Relop 運算子 (=, ==, <, <=, >, >=)
        else if (ch == '=') {
            char next_ch = fgetc(fp);
            if (next_ch == '=') {
                printf("==: EQUAL_TOKEN\n");
            } else {
                if (next_ch != EOF) ungetc(next_ch, fp);
                printf("=: ASSIGN_TOKEN\n");
            }
        }
        else if (ch == '<') {
            char next_ch = fgetc(fp);
            if (next_ch == '=') {
                printf("<=: LESSEQUAL_TOKEN\n");
            } else {
                if (next_ch != EOF) ungetc(next_ch, fp);
                printf("<: LESS_TOKEN\n");
            }
        }
        else if (ch == '>') {
            char next_ch = fgetc(fp);
            if (next_ch == '=') {
                printf(">=: GREATEREQUAL_TOKEN\n");
            } else {
                if (next_ch != EOF) ungetc(next_ch, fp);
                printf(">: GREATER_TOKEN\n");
            }
        }
        
        // 5. 單一符號與數學運算子
        else if (ch == '(') printf("(: LEFTPAREN_TOKEN\n");
        else if (ch == ')') printf("): REFTPAREN_TOKEN\n"); 
        else if (ch == '{') printf("{: LEFTBRACE_TOKEN\n");
        else if (ch == '}') printf("}: REFTBRACE_TOKEN\n");
        else if (ch == ';') printf(";: SEMICOLON_TOKEN\n");
        else if (ch == '+') printf("+: PLUS_TOKEN\n");
        else if (ch == '-') printf("-: MINUS_TOKEN\n");
        
    }
    fclose(fp);
    return 0;
}
