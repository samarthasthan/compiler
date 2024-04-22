#include <iostream>

#include <string>

using namespace std;

class DFA {
private:
  enum State { Q0, Q1, Q2 };
  State currentState;

public:
  DFA() : currentState(Q0) {}

  void processInput(char symbol) {
    if (currentState == Q0) {
      if (symbol == '0')
        currentState = Q0;
      else if (symbol == '1')
        currentState = Q1;
      else
        currentState = Q0;
    } else if (currentState == Q1) {
      if (symbol == '0')
        currentState = Q2;
      else if (symbol == '1')
        currentState = Q1;
      else
        currentState = Q0;
    } else if (currentState == Q2) {
      currentState = Q2;
    }
  }

  bool isAccepted() const { return currentState == Q2; }
};

int main() {
  DFA dfa;

  string input;
  cout << "Enter a string:";
  cin >> input;

  for (char symbol : input) {
    dfa.processInput(symbol);
  }

  if (dfa.isAccepted()) {
    cout << "Accepted" << endl;
  } else {
    cout << "Rejected" << endl;
  }

  return 0;
}