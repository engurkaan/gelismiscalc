#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>

void parse(std::string& str);
double solve(std::string& eq);

std::map<int, double> cevaplar;

class unmatched_parantheses : public std::exception {
public:
    std::string getException;
    explicit unmatched_parantheses(std::string&& str) : getException{str} {}

    friend std::ostream& operator<<(std::ostream& os, const unmatched_parantheses& e) {
        return os<<e.getException;
    }
};

//OOP is unnecessary and could be replaced by functional programming
//also first converting to $id$ then replacing the $id$ is unnecessary, but hey, it works right?
class Parantez {
public:
    static inline int howMany{0};
    Parantez(std::string& stringToChange, int startingIndex): id{++howMany} {
        int counter{1};
        while(stringToChange[startingIndex+counter]!=')') {
            str+=stringToChange[startingIndex+counter];
            counter++;
        }
        stringToChange.replace(startingIndex,counter+1, '$' + std::to_string(id)+ '$');
    }

    void solveEq() {
        int prId{0};
        std::string endStr{};
        for(int counter{0}; counter<str.length(); counter++) {
            if (str[counter] == '$') {
                if(std::isdigit(str[counter-1])) {
                    endStr+="*";
                    //counter++;
                }
                while (str[++counter] != '$') {
                    prId = 10 * prId + int(str[counter] - '0');
                }
                endStr += std::to_string(cevaplar.find(prId)->second);
            } else {
                endStr += str[counter];
            }
            prId=0;
        }
        cevaplar.insert({id, solve(endStr)});
    }

private:
    std::string str;
    const int id;
};

int main() {
    std::cout<< "Enter the mathematical expression to solve: ";
    std::string str;
    std::cin>>str;
    str.insert(0, "("); str+=')';

    try {
        parse(str);
        std::cout<< "The answer: " << std::setprecision(8) << cevaplar[Parantez::howMany] <<'\n';
    } catch (unmatched_parantheses& e) {
        std::cout<< "Failed to compute. " << e <<'\n';
    }
}

void parse(std::string &str) {
    // @p1 = (, @p2  = )
    int p1{0}, p2{0};
    for(char c : str) {
        switch (c) {
            case '(':
                p1++;
                break;
            case ')':
                p2++;
                break;
            default:
                break;
        }
    }

    if(p1!=p2)
        throw ((p1>p2)? unmatched_parantheses(std::to_string(p1-p2)+" '(' unmatched")
                      : unmatched_parantheses(std::to_string(p2-p1) + " ')' unmatched"));
    if(str.find(')') < ((str.find('(',1)!= std::string::npos)? str.find('(', 1): 0 )) {
        throw unmatched_parantheses("First ')' came before any '('.");
    }

    for(int i=0; i<p1; i++) {
        Parantez(str, str.find_last_of('(')).solveEq();
    }
}

//TODO switch from bruteforce parsing process to caching numbers and operators as tokens
//TODO add log
double solve(std::string& eq) {
    unsigned long pos = eq.find('^');
    if(pos!=std::string::npos) {
        double leftSide, rightSide;
        int offsetr{1}, offsetl{1};
        while (std::isdigit(eq[pos - offsetl]) || eq[pos - offsetl] == '.') {
            offsetl++;
        }
        leftSide = std::stod(eq.substr(pos - offsetl + 1, offsetl - 1));
        while (std::isdigit(eq[pos + offsetr]) || eq[pos + offsetr] == '.') {
            offsetr++;
        }
        rightSide = std::stod(eq.substr(pos + 1, offsetr - 1));
        eq.replace(pos-offsetl+1, offsetr+offsetl-1, std::to_string(pow(leftSide, rightSide)));
        return solve(eq);
    }

    bool isMultip{true};
    pos = (eq.find('*')<eq.find('/') ?
            (eq.find('*')) : (isMultip=false, eq.find('/')));
    if(pos != std::string::npos) {
        double leftSide, rightSide;
        int offsetr{1}, offsetl{1};
        while (std::isdigit(eq[pos - offsetl]) || eq[pos - offsetl] == '.') {
            offsetl++;
        }
        leftSide = std::stod(eq.substr(pos - offsetl + 1, offsetl - 1));
        while (std::isdigit(eq[pos + offsetr]) || eq[pos + offsetr] == '.') {
            offsetr++;
        }
        rightSide = std::stod(eq.substr(pos + 1, offsetr - 1));
        if (isMultip) {
            eq.replace(pos-offsetl+1, offsetr+offsetl-1, std::to_string(leftSide*rightSide));
            return solve(eq);
        } else {
            eq.replace(pos-offsetl+1, offsetr+offsetl-1, std::to_string(leftSide/rightSide));
            return solve(eq);
        }
    }

    bool isAddi{true};
    pos = (eq.find('+')<eq.find('-') ?
           (eq.find('+')) : (isAddi=false, eq.find('-')));
    if(pos != std::string::npos) {
        double leftSide, rightSide;
        int offsetr{1}, offsetl{1};
        while (std::isdigit(eq[pos - offsetl]) || eq[pos - offsetl] == '.') {
            offsetl++;
        }
        leftSide = std::stod(eq.substr(pos - offsetl + 1, offsetl - 1));
        while (std::isdigit(eq[pos + offsetr]) || eq[pos + offsetr] == '.') {
            offsetr++;
        }
        rightSide = std::stod(eq.substr(pos + 1, offsetr - 1));
        if (isAddi) {
            eq.replace(pos-offsetl+1, offsetr+offsetl-1, std::to_string(leftSide+rightSide));
            return solve(eq);
        } else {
            eq.replace(pos-offsetl+1, offsetr+offsetl-1, std::to_string(leftSide-rightSide));
            return solve(eq);
        }
    }

    return std::stod(eq);
}