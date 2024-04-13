#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

string generateAssembly(const vector<string> &threeAddressCode) {
  stringstream assemblyCode;

  assemblyCode << "section .text\n"
               << "global _start\n"
               << "_start:\n";

  for (const string &line : threeAddressCode) {
    stringstream ss(line);
    string op, arg1, arg2, result;
    ss >> result >> op >> arg1 >> arg2;

    if (op == "+") {
      assemblyCode << "mov ax, " << arg1 << "\n"
                   << "add ax, " << arg2 << "\n"
                   << "mov " << result << ", ax\n";
    } else if (op == "-") {
      assemblyCode << "mov ax, " << arg1 << "\n"
                   << "sub ax, " << arg2 << "\n"
                   << "mov " << result << ", ax\n";
    } else if (op == "*") {
      assemblyCode << "mov ax, " << arg1 << "\n"
                   << "imul ax, " << arg2 << "\n"
                   << "mov " << result << ", ax\n";
    } else if (op == "/") {
      assemblyCode << "mov ax, " << arg1 << "\n"
                   << "mov bx, " << arg2 << "\n"
                   << "div bx\n"
                   << "mov " << result << ", ax\n";
    } else if (op == "=") {
      assemblyCode << "mov " << result << ", " << arg1 << "\n";
    } else {
      cerr << "Unknown operation: " << op << endl;
    }
  }

  assemblyCode << "mov ax, 4c00h\n"
               << "int 21h\n";

  return assemblyCode.str();
}

int main() {
  vector<string> threeAddressCode = {"t1 = a + b", "t2 = t1 * c", "d = t2 - a"};

  string assemblyCode = generateAssembly(threeAddressCode);

  ofstream outputFile("output.asm");
  if (outputFile.is_open()) {
    outputFile << assemblyCode;
    outputFile.close();
    cout << "Assembly code generated successfully." << endl;
  } else {
    cerr << "Unable to open output file." << endl;
  }

  return 0;
}
