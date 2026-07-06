#include <sstream>
#include "2205062_symbol_table.hpp"
using namespace std;

int main(int argc, char* argv[]){
    if(argc < 3){
        cerr<< "Usage: "<<argv[0]<<" <input_file> <output_file>"<<endl;
        return 1;
    }

    if(freopen(argv[1],"r",stdin) == nullptr){
        cerr << "Error: Cannot open input file " << argv[1] << endl;
        return 1;
    }

    if(freopen(argv[2],"w",stdout) == nullptr){
        cerr << "Error: Cannot open output file " << argv[2] << endl;
        return 1;
    }

    unsigned int num_buckets;
    if(!(cin>>num_buckets)){
        cerr << "Input file not in proper format " << argv[1] << endl;
        return 1;
    }

    SymbolTable st(num_buckets);
    string line;
    int cmd_count = 1;
    string dummy;
    getline(cin, dummy);

    while(getline(cin,line)){
        if(line.empty() || line.find_first_not_of("\t\r\n ") == string::npos) continue;
        stringstream ss(line);
        string command;
        ss>>command;
        if (command == "I") {
            string name, primary_type;
            if (ss >> name >> primary_type) {
                string full_type = primary_type;

                if (primary_type == "FUNCTION") {
                    string return_type;
                    if (ss >> return_type) {
                        full_type += "," + return_type + "<==(";
                        string param;
                        bool first = true;
                        while (ss >> param) {
                            if (!first) full_type += ",";
                            full_type += param;
                            first = false;
                        }
                        full_type += ")";
                    }
                }
                else if (primary_type == "STRUCT") {
                    string var_type, var_name;
                    bool first = true;
                    
                    full_type += ",{";
                    while (ss >> var_type >> var_name) {
                        if (!first) {
                            full_type += ",";
                        }
                        full_type += "(" + var_type + "," + var_name + ")";
                        first = false;
                    }
                    full_type += "}";
                }
                else if (primary_type == "UNION") {
                    string var_type, var_name;
                    bool first = true;
                    
                    full_type += ",{";
                    while (ss >> var_type >> var_name) {
                        if (!first) {
                            full_type += ",";
                        }
                        full_type += "(" + var_type + "," + var_name + ")";
                        first = false;
                    }
                    full_type += "}";
                }
                cout << "Cmd " << cmd_count++ << ": " << line << endl;
                st.insert(name, full_type);
            } else {
                cout << "\tNumber of parameters mismatch for the command I" << endl;
            }
        }
        else if (command == "L") {
            cout << "Cmd " << cmd_count++ << ": " << line << endl;
            string name, extra;
            if (!(ss >> name)) {
                cout << "\tNumber of parameters mismatch for the command L" << endl;
            } else if (ss >> extra) {
                cout << "\tNumber of parameters mismatch for the command L" << endl;
            } else {
                st.lookUp(name);
            }
        }
        else if (command == "D") {
            cout << "Cmd " << cmd_count++ << ": " << line << endl;
            string name, extra;
            if (!(ss >> name)) {
                cout << "\tNumber of parameters mismatch for the command D" << endl;
            } else if (ss >> extra) {
                cout << "\tNumber of parameters mismatch for the command D" << endl;
            } else {
                st.remove(name);
            }
        }
        else if (command == "P") {
            string mode;
            ss >> mode;
            if (mode == "C") {
                cout << "Cmd " << cmd_count++ << ": " << line << endl;
                st.print();
            } else if (mode == "A") {
                cout << "Cmd " << cmd_count++ << ": " << line << endl;
                st.printAll();
            }
        }
        else if (command == "S") {
            cout << "Cmd " << cmd_count++ << ": " << line << endl;
            st.enterScope();
        }
        else if (command == "E") {
            if(st.getCurrentScope()->getParentScope() != nullptr){
                cout << "Cmd " << cmd_count++ << ": " << line << endl;
                st.exitScope();
            }
        }
        else if (command == "Q") {
            cout << "Cmd " << cmd_count++ << ": " << line << endl;
            break;
        }
    }
    return 0;
}