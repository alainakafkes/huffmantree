#include "huff-tree.hpp"
#include <climits>
using namespace std;

int ERROR_CODE = 99999999;

HuffTree::HuffTree() {
    //cout << "HUFFMAN TREE MADE." << endl;
    freq = 0;
    c = 0;
    Left = NULL;
    Right = NULL;
}

int HuffTree::getFreq() {
    return freq;
}

char HuffTree::getChar() {
    return c;
}

void HuffTree::setFreq(int f) {
    freq = f;
}

void HuffTree::setChar(char d) {
    c = d;
}

void HuffTree::setLeft(HuffTree* l) {
    Left = l; // changed from &l
}

void HuffTree::setRight(HuffTree* r) {
    Right = r;
}

HuffTree* HuffTree::MergeTrees(HuffTree* l, HuffTree* r) {
    HuffTree* head = new HuffTree;
    
    // set data members of head
    head->Left = l;
    head->Right = r;
    head->setFreq(l->getFreq() + r->getFreq());
    head->c = '\0';
    
    return head;
}

void HuffTree::HuffCodeGenerator(HuffTree* h, string code, string bitstring[]) {

    if (h != NULL) { // had to account for case when there are no more trees to look at!
        if (h->Left == NULL and h->Right == NULL) {
            if (code[code.length() - 1] == '1') {
                code += "001";
            }
            
            else {
                code += "1011"; // maybe a fix?
            }

            bitstring[h->c] = code;
            cout << "Slot: " << h->c << " Value: " << bitstring[h->c] << endl;
        }
        
        if (h->Left != NULL or h->Right != NULL) {
            if (h != NULL and h->Left != NULL) {
                code = code + "0";
                HuffCodeGenerator(h->Left, code, bitstring);
            }
            
            if (h != NULL and h->Right != NULL) {
                code = code + "1";
                HuffCodeGenerator(h->Right, code, bitstring);
            }
        }
        
    }
    
    return;
}

/********************************************************************************/

Forest::Forest() {
    cout << "FOREST MADE." << endl;
    where = 0;
    numTrees = 0;
    
    for (int i = 0; i < 256; i++) {
        TreeList[i] = NULL;
    }
}

HuffTree* Forest::getTree() {
    int i = 1; // unclear why but this works
    
    while (TreeList[i] == NULL) {
        i++;
    }
    
    return TreeList[i];
}

int Forest::getNumTrees() {
    return numTrees;
}

void Forest::setNumTrees(int x) {
    numTrees = x;
}

int Forest::getWhere() {
    return where;
}

Forest* Forest::AddTree(HuffTree* t, Forest* f) {
    if (f->where < 256) {
        f->TreeList[where] = t;
        f->where++;
        f->numTrees++;
        return f;
    }
    else {
        return f;
    }
}

HuffTree* Forest::Min() {
    //int MinVal = TreeList[0]->getFreq();
    int MinVal = INT_MAX;
    int Mindex = 0;
    for(int i = 0; i < 256; i++) {
        if (TreeList[i] != NULL && TreeList[i]->getFreq() > 0) {
            if (TreeList[i]->getFreq() < MinVal) {
                MinVal = TreeList[i]->getFreq();
                Mindex = i;
                //cout << "MINDEX: " << Mindex << endl;
            }
        }
    }
    
    HuffTree* temp = new HuffTree;
    int copyFreq = TreeList[Mindex]->getFreq(); // need to reset frequency weights..?
    char copyChar = TreeList[Mindex]->getChar();
    
    temp = TreeList[Mindex];
    temp->setFreq(copyFreq);
    temp->setChar(copyChar);
    
    numTrees--; // take out two at a time?

    TreeList[Mindex] = TreeList[numTrees];
    //TreeList[Mindex]->setFreq(copyFreq);
    //TreeList[Mindex]->setFreq(copyChar);
    
    TreeList[numTrees] = NULL;
    return temp;
}

