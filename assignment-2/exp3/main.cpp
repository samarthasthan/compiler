#include <cstring>
#include <iostream>

using namespace std;

bool Inte(char *str) {
  int l1 = strlen(str);
  if (l1 == 0)
    return false;
  for (int i = 0; i < l1; i++) {
    if (str[i] < '0' || str[i] > '9')
      return false;
  }
  return true;
}

bool RealNum(char *str) {
  int l1 = strlen(str);
  bool hasDecimal = false;
  if (l1 == 0)
    return false;
  for (int i = 0; i < l1; i++) {
    if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
      return false;
    if (str[i] == '.')
      hasDecimal = true;
  }
  return hasDecimal;
}

bool Delim(char ch1) {
  const char delimiters[] = {' ', '+', '-', '*', '/',  ',', ';', '>',
                             '<', '=', '(', ')', '[',  ']', '{', '}',
                             '&', '|', '~', '%', '\n', '!'};
  for (char delimiter : delimiters) {
    if (ch1 == delimiter)
      return true;
  }
  return false;
}

bool Arith(char ch2) {
  const char arithmeticOperators[] = {'+', '-', '*', '/', '%'};
  for (char op : arithmeticOperators) {
    if (ch2 == op)
      return true;
  }
  return false;
}

bool RelOp(char ch3) {
  const char relationalOperators[] = {'>', '<', '!', '&', '|', '='};
  for (char op : relationalOperators) {
    if (ch3 == op)
      return true;
  }
  return false;
}

bool Op(char ch4) {
  const char operators[] = {'&', '|', '=', '~'};
  for (char op : operators) {
    if (ch4 == op)
      return true;
  }
  return false;
}

bool RelMul(char *str) {
  const char *relationalMultiples[] = {"&&", "||", "!=", "<=", ">=", "=="};
  for (const char *op : relationalMultiples) {
    if (strcmp(str, op) == 0)
      return true;
  }
  return false;
}

bool Identifier(char *str) {
  if (str[0] >= '0' && str[0] <= '9' || Delim(str[0]))
    return false;
  return true;
}

bool Keyword(char *str) {
  const char *keywords[] = {"if",       "else",   "while",    "do",    "break",
                            "continue", "int",    "double",   "float", "return",
                            "char",     "case",   "sizeof",   "long",  "short",
                            "typedef",  "switch", "unsigned", "void",  "static",
                            "struct",   "goto"};
  for (const char *keyword : keywords) {
    if (strcmp(str, keyword) == 0)
      return true;
  }
  return false;
}

char *subString(char *str, int left, int right) {
  char *subStr = new char[right - left + 2];

  for (int i = left; i <= right; i++)
    subStr[i - left] = str[i];
  subStr[right - left + 1] = '\0';
  return subStr;
}

void parse(char *str) {
  int l = 0, r = 0;
  int l1 = strlen(str);

  for (int i = 1; l <= r && r <= l1; i++) {
    if (Delim(str[r]) == false)
      r++;
    if (Delim(str[r]) == true && l == r) {
      if (Arith(str[r])) {
        cout << i << ". " << str[r] << " : Arithmetic Operator" << endl;
      } else if (RelOp(str[r])) {
        if (RelOp(str[r + 1])) {
          char *subStr = subString(str, l, r + 1);
          if (RelMul(subStr)) {
            cout << i << ". " << subStr << " : Rel. Operator" << endl;
          } else {
            cout << i << ". " << subStr << " : Invalid Rel. Operator" << endl;
          }
          r++;
        } else if (Op(str[r])) {
          cout << i << ". " << str[r] << " : Operator" << endl;
        } else {
          cout << i << ". " << str[r] << " : Rel. Operator" << endl;
        }
      } else if (str[r] != ' ' && str[r] != '\n') {
        if (str[r] == '(' || str[r] == ')')
          cout << i << ". " << str[r] << " : Parenthesis" << endl;
        else if (str[r] == '{' || str[r] == '}')
          cout << i << ". " << str[r] << " : Brackets" << endl;
        else
          cout << i << ". " << str[r] << " : Punctuations" << endl;
      }
      r++;
      l = r;
    } else if (Delim(str[r]) == true && l != r || (r == l1 && l != r)) {
      char *subStr = subString(str, l, r - 1);
      if (str[l] == '#') {
        while (str[r] != '>' && str[r] != '\n') {
          r++;
        }
        char *subStr = subString(str, l, r);
        cout << i << ". " << subStr << " :  Preprocessing" << endl;
        i++;
        r++;
      } else if (str[l] == '"') {
        while (str[r] != '"') {
          r++;
        }
        char *subStr = subString(str, l + 1, r - 1);
        cout << i << ". " << subStr << " : String" << endl;
        i++;
        r++;
      } else if (Keyword(subStr)) {
        cout << i << ". " << subStr << " : Keyword" << endl;
        i++;
      } else if (Inte(subStr)) {
        cout << i << ". " << subStr << " : Integer" << endl;
        i++;
      } else if (RealNum(subStr)) {
        cout << i << ". " << subStr << " : Real Number" << endl;
        i++;
      } else if (Identifier(subStr) == true && Delim(str[r - 1]) == false) {
        cout << i << ". " << subStr << " : Identifier" << endl;
        i++;
      } else if (Identifier(subStr) == false && Delim(str[r - 1]) == false) {
        cout << i << ". " << subStr << " : Invalid Identifier" << endl;
        i++;
      }
      l = r;
    }
  }
  return;
}

int main() {
  cout << "Input as a Linear Array" << endl;
  char str1[100];
  cin.getline(str1, 100);
  parse(str1);
  return 0;
}