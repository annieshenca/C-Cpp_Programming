/*
 * Annie Shen (ashen7)
 * CMPS 109 Harrison - Spring 2018
 * Lab 1 - C Reverse Polish Notation (RPN) Calculator
 * Due Apr 8th, 2018
 *
 * The entire RPN expression is actually passed as a SINGLE argument!
 * Returns -1 on error, 0 otherwise.
 */

/* Resources:
 * String Parsing: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
 * Converting char to int using atoi: https://stackoverflow.com/questions/628761/convert-a-character-digit-to-the-corresponding-integer-in-c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(int argc, char *argv[]) {
  const char s[1] = " ";
  char *token;
  token = strtok(argv[1], s); // Get first token
  int operand[16];
  int j = -1;
  int num1 = 0;
  int num2 = 0;
  int result = 0;
  int dbool = 1;

  while (token != NULL) {
    //printf("here\n");
    /* Loop through the token itself due to isdigit limitations */
    for (int i = 0; i < strlen(token); i++) {
      //printf("token:%c ",token[i]);
      if (isdigit(token[i]) == 0) {
        dbool = 0;
      }
    }

    /* If dbool == 1 -> the current token is a number! Else, an operator. */
    if (dbool == 1) {
      operand[++j] = atoi(token);

    /* Must be a operator */
    } else {
      num2 = operand[j];
      num1 = operand[j-1];

      if (strcmp(token, "+") == 0) {
        result = num1 + num2;
      } else if (strcmp(token, "-") == 0) {
        result = num1 - num2;
      } else if (strcmp(token, "*") == 0) {
        result = num1 * num2;
      } else if (strcmp(token, "/") == 0) {
        result = num1 / num2;
      } else if (strcmp(token, "^") == 0) {
        result = pow(num1, num2);
      }

      operand[--j] = result;
    }

    dbool = 1; // Reset to 1
    token = strtok(NULL, s);
  }

  printf("%d\n", result);
  return 0;
}
