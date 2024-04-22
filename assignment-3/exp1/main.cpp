#include <iostream>
#include <string>

using namespace std;

string prol[7] = {"S", "A", "A", "B", "B", "C", "C"};
string pror[7] = {"A", "Bb", "Cd", "aB", "@", "Cc", "@"};
string prod[7] = {"S->A", "A->Bb", "A->Cd", "B->aB", "B->@", "C->Cc", "C->@"};
string first[7] = {"abcd", "ab", "cd", "a@", "@", "c@", "@"};
string follow[7] = {"$", "$", "$", "a$", "b$", "c$", "d$"};
string table[5][6];

int numr(char c) {
  switch (c) {
  case 'S':
    return 0;
  case 'A':
    return 1;
  case 'B':
    return 2;
  case 'C':
    return 3;
  case 'a':
    return 0;
  case 'b':
    return 1;
  case 'c':
    return 2;
  case 'd':
    return 3;
  case '$':
    return 4;
  default:
    return 2;
  }
}

int main() {
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 6; j++)
      table[i][j] = " ";

  cout << "The following grammar is used for Parsing Table:" << endl;
  for (int i = 0; i < 7; i++)
    cout << prod[i] << endl;

  cout << "\nFirst sets:" << endl;
  for (int i = 0; i < 7; i++)
    cout << "First(" << prol[i][0] << ") = " << first[i] << endl;

  cout << "\nFollow sets:" << endl;
  for (int i = 0; i < 7; i++)
    cout << "Follow(" << prol[i][0] << ") = " << follow[i] << endl;

  cout << "\nPredictive parsing table:" << endl;

  for (int i = 0; i < 7; i++) {
    int k = first[i].length();
    for (int j = 0; j < 10; j++)
      if (first[i][j] != '@')
        table[numr(prol[i][0]) + 1][numr(first[i][j]) + 1] = prod[i];
  }

  for (int i = 0; i < 7; i++) {
    if (pror[i].length() == 1 && pror[i][0] == '@') {
      int k = follow[i].length();
      for (int j = 0; j < k; j++)
        table[numr(prol[i][0]) + 1][numr(follow[i][j]) + 1] = prod[i];
    }
  }

  table[0][0] = " ";
  table[0][1] = "a";
  table[0][2] = "b";
  table[0][3] = "c";
  table[0][4] = "d";
  table[0][5] = "$";

  table[1][0] = "S";
  table[2][0] = "A";
  table[3][0] = "B";
  table[4][0] = "C";

  cout << "\n--------------------------------------------------------" << endl;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 6; j++) {
      cout << table[i][j] << "      ";
      if (j == 5)
        cout << "\n--------------------------------------------------------"
             << endl;
    }
  }

  return 0;
}