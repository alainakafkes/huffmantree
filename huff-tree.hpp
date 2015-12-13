
#include <iostream>

using namespace std;

// Edit 11/28: Changed all HuffTree and Forest types to pointers.

class HuffTree {
public:
    HuffTree();
    int getFreq();
    char getChar();
    void setFreq(int f);
    void setChar(char d);
    void setLeft(HuffTree* l);
    void setRight(HuffTree* r);
    HuffTree* MergeTrees(HuffTree* l, HuffTree* r); // merges two trees into a bigger tree
    void HuffCodeGenerator(HuffTree* h, string code, string bitstring[]); // return type?

private:
    int freq;
    char c;
    HuffTree* Left;
    HuffTree* Right;
};

class Forest {
public:
    Forest();
    HuffTree* getTree();
    int getNumTrees();
    void setNumTrees(int x);
    int getWhere();
    Forest* AddTree(HuffTree* t, Forest* f);
    //HuffTree Min(Forest f);
    HuffTree* Min();
    
private:
    HuffTree* TreeList[256]; //array of pointers?
    int where;
    int numTrees;
};