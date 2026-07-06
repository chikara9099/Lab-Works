#include<iostream>
#include<string>
using namespace std;

struct ComponentInfo {
    string type;
    string name;
    ComponentInfo* next;

    ComponentInfo(string t, string n) : type(t), name(n), next(nullptr) {}
};

class SymbolInfo{
    private:
        string name;
        string type;
        SymbolInfo* next;
    public:
        SymbolInfo(string name, string type){
            this->name = name;
            this->type = type;
            next = nullptr;
        }

        string getName(){
            return name;
        }

        void setName(string name){
            this->name = name;
        }

        string getType(){
            return type;
        }

        void setType(string type){
            this->type = type;
        }

        SymbolInfo* getNext(){
            return next;
        }

        void setNext(SymbolInfo* next){
            this->next = next;
        }
};

unsigned int SDBMHash(string str, unsigned int num_buckets){
    unsigned int hash = 0;
    unsigned int len = str.length();
    for(unsigned int i = 0; i < len; i++){
        hash = (str[i] + (hash<<6) + (hash<<16) - hash) % num_buckets;
    }
    return hash;
}

class ScopeTable{
    private:
        unsigned int num_buckets;
        SymbolInfo** buckets;
        ScopeTable* parent_scope;
        unsigned int scope_id;
    public:
        ScopeTable(unsigned int num_buckets, ScopeTable* parent, unsigned int id){
            this->num_buckets = num_buckets;
            parent_scope = parent;
            scope_id = id;
            buckets = new SymbolInfo*[num_buckets];
            for(unsigned int i = 0; i < num_buckets; i++){
                *(buckets + i) = nullptr;
            }
            cout<<"\tScopeTable# "<<scope_id<<" created"<<endl;
        }

        ~ScopeTable(){
            for(unsigned int i = 0; i < num_buckets; i++){
                SymbolInfo* current = *(buckets+i);
                while(current){
                    SymbolInfo* temp = current->getNext();
                    delete current;
                    current = temp;
                }
            }
            delete[] buckets;
            cout<<"\tScopeTable# "<<scope_id<< " removed"<<endl;
        }

        bool insert(string name, string type){
            unsigned int index = SDBMHash(name,num_buckets);
            unsigned int pos = 0;
            SymbolInfo* current = buckets[index];
            if(current == nullptr){
                buckets[index] = new SymbolInfo(name, type);
                cout<<"\tInserted in ScopeTable# "<<scope_id<< " at position "<<index+1<<", "<<pos+1<<endl;
                return true;
            }
            else{
                while(current->getNext() != nullptr && current->getName() != name){
                    current = current->getNext();
                    pos++;
                }
                if(current->getName() == name){
                    cout<<"\t\'"<<name<<"\'"<< " already exists in the current ScopeTable"<<endl;
                    return false;
                }
                else{
                    current->setNext(new SymbolInfo(name,type));
                    pos++;
                    cout<<"\tInserted in ScopeTable# "<<scope_id<< " at position "<<index+1<<", "<<pos+1<<endl;
                    return true;
                }
            }
        }

        SymbolInfo* lookUp(string name){
            unsigned int index = SDBMHash(name,num_buckets);
            unsigned int pos = 0;
            SymbolInfo* current = buckets[index];
            if(current == nullptr){
                return nullptr;
            }
            else{
                while(current != nullptr){
                    if(current->getName() == name){
                        cout<<"\t\'"<<name<<"\' found in ScopeTable# "<<scope_id<<" at position "<<index+1<<", "<<pos+1<<endl;
                        return current;
                    }
                    else{
                        pos++;
                        current = current->getNext();
                    }
                }
            }
            return current;
        }

        bool remove(string name){
            unsigned int index = SDBMHash(name, num_buckets);
            unsigned int pos = 0;
            SymbolInfo* current = buckets[index];
            if(current == nullptr){
                cout<<"\tNot found in the current ScopeTable"<<endl;
                return false;
            }
            if(current->getName() == name){
                buckets[index] = current->getNext();
                delete current;
                cout<<"\tDeleted "<<"\'"<<name<<"\' from ScopeTable# "<<scope_id<<" at position "<<index+1<<", "<<pos+1<<endl;
                return true;
            }
            while(current->getNext() != nullptr){
                if(current->getNext()->getName() == name){
                    SymbolInfo* temp = current->getNext();
                    current->setNext(temp->getNext());
                    delete temp;
                    pos++;
                    cout<<"\tDeleted"<<"\'"<<name<<"\' from ScopeTable# "<<scope_id<<" at position "<<index+1<<", "<<pos+1<<endl;
                    return true;
                }
                current = current->getNext();
                pos++;
            }
            return false;
        }

        void print(int count=0){
            for(int i = 0;i < count;i++) cout<<"\t";
            cout<< "ScopeTable# " << scope_id << endl;
            for(unsigned int i = 0;i < num_buckets;i++){
                for(int j = 0;j < count;j++) cout<<"\t";
                cout<<i+1<<"-->";
                SymbolInfo* current = buckets[i];
                while(current != nullptr){
                    cout<<" <"<<current->getName()<<","<<current->getType()<<">";
                    current = current->getNext();
                }
                cout<<endl;
            }
        }

        ScopeTable* getParentScope(){
            return parent_scope;
        }

        unsigned int getScopeId(){
            return scope_id;
        }
};

class SymbolTable{
    private:
        ScopeTable* current_scope;
        unsigned int total_buckets;
        unsigned int scope_counter = 1;
    public:
        SymbolTable(unsigned int n){
            total_buckets = n;
            current_scope = new ScopeTable(n,nullptr,scope_counter);
        }

        ~SymbolTable(){
            while(current_scope != nullptr){
                ScopeTable* temp = current_scope;
                current_scope = current_scope->getParentScope();
                delete temp;
            }
        }

        void enterScope(){
            scope_counter++;
            ScopeTable* parent = current_scope;
            current_scope = new ScopeTable(total_buckets,parent,scope_counter);
        }

        bool exitScope(){
            if(current_scope == nullptr) return false;
            if(current_scope->getParentScope() == nullptr) return false;
            ScopeTable* temp = current_scope;
            current_scope = current_scope->getParentScope();
            delete temp;
            return true;
        }

        bool insert(string name,string type){
            return current_scope->insert(name,type);
        }

        SymbolInfo* lookUp(string name){
            ScopeTable* temp = current_scope;
            while(temp != nullptr){
                SymbolInfo* res = temp->lookUp(name);
                if(res == nullptr) temp = temp->getParentScope();
                else return res;
            }
            cout<<"\t\'"<<name<<"\'"<<" not found in any of the ScopeTables"<<endl;
            return nullptr;
        }

        bool remove(string name){
            return current_scope->remove(name);
        }

        void print(){
            current_scope->print();
        }
        
        void printAll(){
            int level = 0;
            ScopeTable* temp = current_scope;
            while(temp != nullptr){
                temp->print(level);
                temp = temp->getParentScope();
                level++;
            }
        }
        ScopeTable* getCurrentScope(){
            return current_scope;
        }
};