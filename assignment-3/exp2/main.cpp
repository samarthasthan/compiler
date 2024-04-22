#include <iostream>
#include <cstring>
#include <string>
 
using namespace std;
 
char production[7][10] = {"S->A", "A->Bb", "A->Cd", "B->aB", "B->@", "C->Cc", "C->@"};
char nonTerminals[4] = {'S', 'A', 'B', 'C'};
char terminals[5] = {'a', 'b', 'c', 'd', '$'};
string first[7] = {"abcd", "ab", "cd", "a@", "@", "c@", "@"};
string follow[7] = {"$", "$", "$", "a$", "b$", "c$", "d$"};
string table[4][5];
 
int numr(char c) {
  switch (c) {
      case 'S': return 0;
      case 'A': return 1;
      case 'B': return 2;
      case 'C': return 3;
      case 'a': return 0;
      case 'b': return 1;
      case 'c': return 2;
      case 'd': return 3;
      case '$': return 4;
      default: return -1;
  }
}
 
void printTable() {
  cout << "Predictive parsing table:\n";
  cout << "--------------------------\n";
  cout << "| |a|b|c|d|$|\n";
  cout << "--------------------------\n";
  for (int i = 0; i < 4; i++) {
      cout << "| " << nonTerminals[i] << " |";
      for (int j = 0; j < 5; j++) {
          cout << " " << table[i][j] << " |";
      }
      cout << "\n--------------------------\n";
  }
}
 
void initializeTable() {
  for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 5; j++) {
          table[i][j] = "";
      }
  }
}
 
void fillTable() {
  for (int i = 0; i < 7; i++) {
      int row = numr(production[i][0]);
      int index = 3;
      while (production[i][index] != '\0') {
          int col = numr(production[i][index]);
          if (col != -1) {
              table[row][col] = production[i];
              break;
          }
          index++;
      }
      if (production[i][3] == '@') {
          int row = numr(production[i][0]);
          for (int j = 0; j < follow[row].length(); j++) {
              int col = numr(follow[row][j]);
              table[row][col] = production[i];
          }
      }
  }
}
 
void predictiveParsing(char inputString[]) {
  int stack[100];
  int top = -1;
  stack[++top] = numr('$');
  stack[++top] = numr('S');
 
  cout << "Parsing Steps:\n";
  cout << "Stack\t\tInput String\t\tAction\n";
  cout << "-----------------------------------------\n";
 
  int ip = 0;
  while (stack[top] != numr('$')) {
      cout << nonTerminals[stack[top]] << "\t\t" << &inputString[ip] << "\t\t";
      if (stack[top] == numr(inputString[ip])) {
          if (inputString[ip] == '\0') {
              cout << "Error: Invalid input\n";
              return;
          }
          cout << "Match " << inputString[ip] << "\n";
          top--;
          ip++;
      } else {
          int row = stack[top];
          int col = numr(inputString[ip]);
          if (table[row][col] != "") {
              cout << "Apply Production: " << table[row][col] << "\n";
              top--;
              int len = table[row][col].length();
              for (int i = len - 1; i >= 3; i--) {
                  if (table[row][col][i] != '@') {
                      stack[++top] = numr(table[row][col][i]);
                  }
              }
          } else {
              cout << "ERROR!\n";
              cout << "Error: Invalid input\n";
              return;
          }
      }
  }
  cout << "$\t\t" << &inputString[ip] << "\t\tAccept\n";
  cout << "-----------------------------------------\n";
  cout << "Valid input\n";
}
 
int main() {
  initializeTable();
  fillTable();
  printTable();
 
  char inputString[100];
  cout << "\nEnter the input string: ";
  cin >> inputString;
  predictiveParsing(inputString);
  return 0;
}