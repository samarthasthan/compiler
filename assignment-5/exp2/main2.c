#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPR_LEN 100
#define MAX_VARS 26

typedef struct {
  char name;
  int value;
  int is_initialized;
} Variable;

Variable variables[MAX_VARS]; // Array to store variables

// Function to find a variable by name
int find_variable(char name) {
  for (int i = 0; i < MAX_VARS; i++) {
    if (variables[i].name == name) {
      return i;
    }
  }
  return -1;
}

// Function to initialize a variable with a given name and value
void initialize_variable(char name, int value) {
  int index = find_variable(name);
  if (index == -1) {
    // Find an empty spot in the variables array
    for (int i = 0; i < MAX_VARS; i++) {
      if (variables[i].name == '\0') {
        variables[i].name = name;
        variables[i].value = value;
        variables[i].is_initialized = 1;
        break;
      }
    }
  } else {
    variables[index].value = value;
    variables[index].is_initialized = 1;
  }
}

// Function to evaluate an expression and return the result
int evaluate_expression(const char *expression) {
  int result = 0;
  int number = 0;
  char op = '+';
  int i = 0;

  while (expression[i] != '\0') {
    char c = expression[i];

    if (isdigit(c)) {
      number = number * 10 + (c - '0');
    } else if (isalpha(c)) {
      // Retrieve the variable's value
      int index = find_variable(c);
      if (index != -1 && variables[index].is_initialized) {
        number = variables[index].value;
      } else {
        printf("Error: Variable '%c' is not initialized.\n", c);
        exit(1);
      }
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      // Apply the previous operator
      switch (op) {
      case '+':
        result += number;
        break;
      case '-':
        result -= number;
        break;
      case '*':
        result *= number;
        break;
      case '/':
        if (number == 0) {
          printf("Error: Division by zero.\n");
          exit(1);
        }
        result /= number;
        break;
      }
      // Reset number and update operator
      number = 0;
      op = c;
    }

    i++;
  }

  // Apply the last operator and number
  switch (op) {
  case '+':
    result += number;
    break;
  case '-':
    result -= number;
    break;
  case '*':
    result *= number;
    break;
  case '/':
    if (number == 0) {
      printf("Error: Division by zero.\n");
      exit(1);
    }
    result /= number;
    break;
  }

  return result;
}

// Function to optimize and process expressions
void optimize_and_process_expressions() {
  int num_expressions;
  printf("Enter the number of expressions: ");
  scanf("%d", &num_expressions);
  getchar(); // Consume newline character

  char expression[MAX_EXPR_LEN];
  for (int i = 0; i < num_expressions; i++) {
    printf("Enter expression %d: ", i + 1);
    fgets(expression, MAX_EXPR_LEN, stdin);
    expression[strcspn(expression, "\n")] = '\0'; // Remove newline

    // Parse the expression
    char *equal_sign = strchr(expression, '=');
    if (equal_sign == NULL) {
      printf("Error: Invalid expression format.\n");
      exit(1);
    }

    char var_name = expression[0]; // Variable name
    char *exp = equal_sign + 1;    // Expression part

    // Evaluate the expression
    int value = evaluate_expression(exp);
    initialize_variable(var_name, value);
  }

  // Print variable values
  for (int i = 0; i < MAX_VARS; i++) {
    if (variables[i].is_initialized) {
      printf("%c = %d\n", variables[i].name, variables[i].value);
    }
  }
}

int main() {
  optimize_and_process_expressions();
  return 0;
}
