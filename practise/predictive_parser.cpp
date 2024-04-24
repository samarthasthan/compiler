#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// Enum for parsing symbols
enum SymbolType { TERMINAL, NON_TERMINAL };

// Represents a parsing symbol (either terminal or non-terminal)
struct Symbol {
  string name;
  SymbolType type;

  Symbol(string name, SymbolType type) : name(name), type(type) {}

  bool operator<(const Symbol &other) const { return name < other.name; }

  bool operator==(const Symbol &other) const {
    return name == other.name && type == other.type;
  }
};

// Represents a production rule
struct ProductionRule {
  Symbol non_terminal;       // Non-terminal symbol
  vector<Symbol> production; // Production (a sequence of symbols)

  ProductionRule(Symbol non_terminal, vector<Symbol> production)
      : non_terminal(non_terminal), production(production) {}
};

// Predictive parser class
class PredictiveParser {
private:
  map<Symbol, map<Symbol, vector<Symbol>>> parsing_table;
  vector<ProductionRule> rules;

  // Generate parsing table from given production rules
  void generateParsingTable() {
    map<Symbol, set<Symbol>> firstSets = computeFirstSets();
    map<Symbol, set<Symbol>> followSets = computeFollowSets(firstSets);

    for (const ProductionRule &rule : rules) {
      Symbol nonTerminal = rule.non_terminal;
      vector<Symbol> production = rule.production;

      set<Symbol> firstSet = computeFirstOfProduction(production, firstSets);

      for (const Symbol &symbol : firstSet) {
        if (symbol.name != "ε") { // ε represents epsilon
          parsing_table[nonTerminal][symbol] = production;
        } else {
          for (const Symbol &followSymbol : followSets[nonTerminal]) {
            parsing_table[nonTerminal][followSymbol] = production;
          }
        }
      }
    }
  }

  // Compute the FIRST sets for each non-terminal symbol
  map<Symbol, set<Symbol>> computeFirstSets() {
    map<Symbol, set<Symbol>> firstSets;

    for (const ProductionRule &rule : rules) {
      firstSets[rule.non_terminal]; // Initialize the set for the non-terminal
    }

    bool updated = true;
    while (updated) {
      updated = false;

      for (const ProductionRule &rule : rules) {
        Symbol nonTerminal = rule.non_terminal;
        vector<Symbol> production = rule.production;

        size_t i = 0;
        while (i < production.size()) {
          Symbol symbol = production[i];

          if (symbol.type == TERMINAL) {
            updated |= firstSets[nonTerminal].insert(symbol).second;
            break;
          } else {
            // Non-terminal
            size_t initialSize = firstSets[nonTerminal].size();
            firstSets[nonTerminal].insert(firstSets[symbol].begin(),
                                          firstSets[symbol].end());

            if (firstSets[symbol].find(Symbol("ε", TERMINAL)) ==
                firstSets[symbol].end()) {
              break;
            }

            updated |= firstSets[nonTerminal].size() > initialSize;
          }

          i++;
        }

        // If all symbols in the production have ε in their FIRST set, add ε to
        // the FIRST set of the non-terminal
        if (i == production.size()) {
          updated |=
              firstSets[nonTerminal].insert(Symbol("ε", TERMINAL)).second;
        }
      }
    }

    return firstSets;
  }

  // Compute the FOLLOW sets for each non-terminal symbol
  map<Symbol, set<Symbol>>
  computeFollowSets(const map<Symbol, set<Symbol>> &firstSets) {
    map<Symbol, set<Symbol>> followSets;

    // Initialize the FOLLOW set for each non-terminal symbol
    for (const ProductionRule &rule : rules) {
      followSets[rule.non_terminal]; // Initialize the set for the non-terminal
    }

    // Start symbol has the end-of-input (EOF) symbol in its FOLLOW set
    followSets[rules[0].non_terminal].insert(
        Symbol("$", TERMINAL)); // EOF symbol

    bool updated = true;
    while (updated) {
      updated = false;

      for (const ProductionRule &rule : rules) {
        Symbol nonTerminal = rule.non_terminal;
        vector<Symbol> production = rule.production;

        for (size_t i = 0; i < production.size(); i++) {
          Symbol symbol = production[i];
          if (symbol.type == NON_TERMINAL) {
            // Compute the FOLLOW set for the current non-terminal symbol
            size_t j = i + 1;
            while (j < production.size()) {
              Symbol nextSymbol = production[j];

              // Get the FIRST set of the next symbol
              set<Symbol> firstOfNextSymbol;
              if (nextSymbol.type == TERMINAL) {
                firstOfNextSymbol.insert(nextSymbol);
              } else {
                firstOfNextSymbol = firstSets.at(nextSymbol);
              }

              // Add the FIRST set of the next symbol to the FOLLOW set of the
              // current non-terminal symbol
              size_t initialSize = followSets[symbol].size();
              followSets[symbol].insert(firstOfNextSymbol.begin(),
                                        firstOfNextSymbol.end());

              // If ε is not in the FIRST set of the next symbol, break
              if (firstOfNextSymbol.find(Symbol("ε", TERMINAL)) ==
                  firstOfNextSymbol.end()) {
                break;
              }

              updated |= followSets[symbol].size() > initialSize;
              j++;
            }

            // Add the FOLLOW set of the current non-terminal to the FOLLOW set
            // of the non-terminal symbol
            if (j == production.size()) {
              size_t initialSize = followSets[symbol].size();
              followSets[symbol].insert(followSets[nonTerminal].begin(),
                                        followSets[nonTerminal].end());
              updated |= followSets[symbol].size() > initialSize;
            }
          }
        }
      }
    }

    return followSets;
  }

  // Compute the FIRST set of a production
  set<Symbol>
  computeFirstOfProduction(const vector<Symbol> &production,
                           const map<Symbol, set<Symbol>> &firstSets) {
    set<Symbol> firstSet;

    for (const Symbol &symbol : production) {
      if (symbol.type == TERMINAL) {
        firstSet.insert(symbol);
        break;
      } else {
        const set<Symbol> &nonTerminalFirstSet = firstSets.at(symbol);
        firstSet.insert(nonTerminalFirstSet.begin(), nonTerminalFirstSet.end());

        if (nonTerminalFirstSet.find(Symbol("ε", TERMINAL)) ==
            nonTerminalFirstSet.end()) {
          break;
        }
      }
    }

    return firstSet;
  }

public:
  PredictiveParser(const vector<ProductionRule> &rules) : rules(rules) {
    generateParsingTable();
  }

  // Parse an input string using the predictive parser
  bool parse(const string &input) {
    stack<Symbol> parseStack;
    parseStack.push(Symbol("$", TERMINAL)); // End-of-input (EOF) symbol
    parseStack.push(rules[0].non_terminal); // Start symbol

    size_t inputIndex = 0;
    while (!parseStack.empty()) {
      Symbol topSymbol = parseStack.top();
      parseStack.pop();

      // If top symbol is a terminal
      if (topSymbol.type == TERMINAL) {
        if (inputIndex < input.size() &&
            string(1, input[inputIndex]) == topSymbol.name) {
          inputIndex++;
        } else if (topSymbol.name == "$" && inputIndex == input.size()) {
          return true;
        } else {
          return false;
        }
      } else {
        // Non-terminal symbol
        if (inputIndex < input.size()) {
          Symbol inputSymbol(string(1, input[inputIndex]), TERMINAL);
          if (parsing_table.find(topSymbol) != parsing_table.end() &&
              parsing_table[topSymbol].find(inputSymbol) !=
                  parsing_table[topSymbol].end()) {
            vector<Symbol> production = parsing_table[topSymbol][inputSymbol];
            for (auto it = production.rbegin(); it != production.rend(); ++it) {
              parseStack.push(*it);
            }
          } else {
            return false;
          }
        } else {
          Symbol inputSymbol("$", TERMINAL);
          if (parsing_table.find(topSymbol) != parsing_table.end() &&
              parsing_table[topSymbol].find(inputSymbol) !=
                  parsing_table[topSymbol].end()) {
            vector<Symbol> production = parsing_table[topSymbol][inputSymbol];
            for (auto it = production.rbegin(); it != production.rend(); ++it) {
              parseStack.push(*it);
            }
          } else {
            return false;
          }
        }
      }
    }

    // If the parsing stack is empty and the input is fully consumed, the input
    // is successfully parsed
    return inputIndex == input.size();
  }
};

// Example code using the PredictiveParser
int main() {
  cout << "Hello world";
  // Define the production rules for the grammar
  vector<ProductionRule> rules1 = {
      {Symbol("L", NON_TERMINAL),
       {Symbol("R", NON_TERMINAL), Symbol("a", TERMINAL)}},
      {Symbol("L", NON_TERMINAL),
       {Symbol("Q", NON_TERMINAL), Symbol("b", TERMINAL),
        Symbol("a", TERMINAL)}},
      {Symbol("R", NON_TERMINAL),
       {Symbol("a", TERMINAL), Symbol("b", TERMINAL), Symbol("a", TERMINAL)}},
      {Symbol("R", NON_TERMINAL),
       {Symbol("c", TERMINAL), Symbol("a", TERMINAL), Symbol("b", TERMINAL),
        Symbol("a", TERMINAL)}},
      {Symbol("R", NON_TERMINAL),
       {Symbol("R", NON_TERMINAL), Symbol("b", TERMINAL),
        Symbol("c", TERMINAL)}},
      {Symbol("Q", NON_TERMINAL),
       {Symbol("b", TERMINAL), Symbol("b", TERMINAL), Symbol("c", TERMINAL)}},
      {Symbol("Q", NON_TERMINAL),
       {Symbol("b", TERMINAL), Symbol("c", TERMINAL)}}};

  vector<ProductionRule> rules2 = {
      {Symbol("A", NON_TERMINAL),
       {Symbol("B", NON_TERMINAL), Symbol("a", TERMINAL)}},
      {Symbol("B", NON_TERMINAL),
       {Symbol("dab", TERMINAL), Symbol("b", TERMINAL)}},
      {Symbol("B", NON_TERMINAL),
       {Symbol("C", NON_TERMINAL), Symbol("b", TERMINAL)}},
      {Symbol("C", NON_TERMINAL),
       {Symbol("c", TERMINAL), Symbol("B", NON_TERMINAL)}},
      {Symbol("C", NON_TERMINAL),
       {Symbol("A", NON_TERMINAL), Symbol("c", TERMINAL)}}};

  vector<ProductionRule> rules3 = {
      {Symbol("Goal", NON_TERMINAL),
       {Symbol("Aim", NON_TERMINAL), Symbol("a", TERMINAL)}},
      {Symbol("Goal", NON_TERMINAL), {Symbol("b", TERMINAL)}},
      {Symbol("Aim", NON_TERMINAL),
       {Symbol("Aim", NON_TERMINAL), Symbol("c", TERMINAL)}},
      {Symbol("Aim", NON_TERMINAL),
       {Symbol("Goal", NON_TERMINAL), Symbol("d", TERMINAL)}},
      {Symbol("Aim", NON_TERMINAL), {}}};

  // Create PredictiveParser objects for each set of production rules
  PredictiveParser parser1(rules1);
  PredictiveParser parser2(rules2);
  PredictiveParser parser3(rules3);

  // Test input strings for each parser
  string input1 = "ababacaba";
  string input2 = "dabba";
  string input3 = "bcdd";

  // Parse input strings using the respective parsers
  cout << "Parsing input 1: " << input1
       << " - Result: " << (parser1.parse(input1) ? "Accepted" : "Rejected")
       << endl;
  cout << "Parsing input 2: " << input2
       << " - Result: " << (parser2.parse(input2) ? "Accepted" : "Rejected")
       << endl;
  cout << "Parsing input 3: " << input3
       << " - Result: " << (parser3.parse(input3) ? "Accepted" : "Rejected")
       << endl;

  return 0;
}
