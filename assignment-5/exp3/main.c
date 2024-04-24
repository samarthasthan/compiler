#include <stdio.h>
#include <string.h>

#define MAX_EXPRS 10
#define MAX_EXPR_LEN 30
#define MAX_OPS 10

// Function to parse an arithmetic expression and generate assembly code
void generate_assembly_code(char exprs[][MAX_EXPR_LEN], int num_exprs) {
  char opr[MAX_OPS];
  int i;

  printf("\nAssembly Code:\n");
  printf("**************\n");

  for (i = 0; i < num_exprs; i++) {
    char expr[MAX_EXPR_LEN];
    strcpy(expr, exprs[i]);

    // Parse the expression
    char temp1, temp2, op, result_var;
    int temp_num;

    // Format: temp_num = temp1 op temp2
    if (sscanf(expr, "t%d = %c %c %c", &temp_num, &temp1, &op, &temp2) == 4) {
      // Determine the operation
      switch (op) {
      case '+':
        strcpy(opr, "ADD");
        break;
      case '-':
        strcpy(opr, "SUB");
        break;
      case '*':
        strcpy(opr, "MUL");
        break;
      case '/':
        strcpy(opr, "DIV");
        break;
      default:
        printf("Error: Unknown operator '%c'\n", op);
        return;
      }

      // Print assembly code
      printf("\nMOV %c, R0", temp1);
      printf("\n%s %c, R0", opr, temp2);
      printf("\nMOV R0, t%d\n", temp_num);
    }
    // Format: result_var = temp_num
    else if (sscanf(expr, "%c = t%d", &result_var, &temp_num) == 2) {
      printf("MOV t%d, R0\n", temp_num);
      printf("MOV R0, %c\n", result_var);
    }
    // Handle invalid expressions
    else {
      printf("Error: Invalid expression format '%s'\n", expr);
      return;
    }
  }
}

int main() {
  char exprs[MAX_EXPRS][MAX_EXPR_LEN];
  int num_exprs;
  int i;

  // Read the number of expressions
  printf("Enter the number of expressions: ");
  scanf("%d", &num_exprs);
  getchar(); // Consume newline character

  // Read each expression
  printf("Enter the expressions:\n");
  for (i = 0; i < num_exprs; i++) {
    fgets(exprs[i], MAX_EXPR_LEN, stdin);
    exprs[i][strcspn(exprs[i], "\n")] = '\0'; // Remove newline character
  }

  // Generate assembly code
  generate_assembly_code(exprs, num_exprs);

  return 0;
}
