#include "huff-tree.hpp"
#include "bit_io.hpp"
#include "common.hpp"
#include <iostream>
#include <iomanip>
#include <string>

using namespace eecs214;
using namespace std;

void frequencyBuilder(const char* in_file, int frequencies[]) {
    bifstream in(in_file); // create bifstream
    
    int curr; // initialize?
    
    for(int i = 0; i < 256; i++) {
        frequencies[i] = 0; // set freq array to all zeros
    }
    
    ///// FREQUENCY TABLE /////
    while(in) {
        // frequency table here
        in.read_bits(curr, 7); // SEVEN OR EIGHT? ***
        frequencies[curr]++;
    }
    
//    for (int i = 0; i < 256; i++) {
//        cout << frequencies[i] << endl;
//    }
    
    return;
}

void outputFileSize(const char* in_file, bofstream& out, int size) {
    bifstream in(in_file);
    
    out.write_bits(size, 32);
    cout << "WORKING" << endl;
    
    return;
}

void outputFreq(const char* in_file, bofstream& out, int frequencies[]) {
    bifstream in(in_file);
    
    for (int i = 0; i < 256; i++) {
        out.write_bits(frequencies[i], 32);
    }
    
    return;
}

void outputter(const char* in_file, bofstream& out, string HuffCode[]) {
    bifstream in(in_file);
    
    ///// OUTPUTTING METADATA TO FILE: FILE SIZE & FREQUENCY TABLE /////
    int curr; // initialize?
    string code = "";
    
    while(in) {
        //cout << "WORKING" << endl;
        in.read_bits(curr, 8);
        code = HuffCode[curr];
        
        for (int i = 0; i < code.length(); i++) {
            if (code[i] == '0') {
//                cout << code[i] << endl;
                out.write(0);
            }
            else {
//                cout << code[i] << endl;
                out.write(1);
            }
        }
        
        code = "";
    }
    
    return;
}

void huff (const char* inFile, bofstream& out)
{
    int frequencies[256];
    frequencyBuilder(inFile, frequencies);
    
    int FILE_SIZE = 0;
    
    for (int i = 0; i < 256; i++) {
        FILE_SIZE += frequencies[i];
    }
    
    cout << FILE_SIZE << endl;
    
    
    ///// BUILDING TREE /////
    
    Forest* f = new Forest();
    
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            HuffTree* t = new HuffTree;
            t->setChar(i);
            t->setFreq(frequencies[i]);
            cout << "For i=" << i << " CHAR "<< t->getChar() << " FREQ " << t->getFreq() << endl;
            
            f->AddTree(t, f);
            //cout << f->getNumTrees() << endl; // increases!
        }
    }
    
    ///// MERGE TREES /////
    
    cout << "TOTAL TREES: " << f->getNumTrees() << endl;

    while (f->getNumTrees() > 1) {
        HuffTree* mega = new HuffTree;
        HuffTree* l = new HuffTree;
        HuffTree* r = new HuffTree;
        
        if (f != NULL) {
            l = f->Min();
//            cout << "*LEFT FREQ: " << l->getFreq() << endl;
            
            if (f != NULL) {
                r = f->Min();
//                cout << "*RIGHT FREQ: " << r->getFreq() << endl;
                
                mega = mega->MergeTrees(l, r); // change number of trees?
//                cout << "**MEGA:" << mega->getFreq() << endl;
                
                f->AddTree(mega, f);
                
                if (f->getNumTrees() == 1) {
                    cout << "BREAK THYME" << endl;
                    break;
                }
            }
        }
    }

    cout << "Number of trees: " << f->getNumTrees() << endl;
    // end result – forest with one tree!

    
    ///// ASSIGNING HUFFMAN CODES /////
    // get single tree in TreeList
    HuffTree* complete = new HuffTree;
    complete = f->getTree();
    
    HuffTree* newTree = new HuffTree;
    
    // store huffCodes in array
    string huffCode = "";
    string bitstring[256];
    newTree->HuffCodeGenerator(complete, huffCode, bitstring);
    
    // print array
    for (int i = 0; i < 256; i++) {
        if (bitstring[i] != "") {
            cout << "INDEX: " << i << " ENCODED AS: " << bitstring[i] << endl;
        }
    }
    
///// OUTPUTTING METADATA TO FILE: FILE SIZE & FREQUENCY TABLE /////
    outputFileSize(inFile, out, FILE_SIZE);
    outputFreq(inFile, out, frequencies);
    outputter(inFile, out, bitstring);
    
    return;
    
}

int main(int argc, const char **argv)
{
    const char *in_file, *out_file; // files to open for reading and writing
    
    if (argc != 3) {
        //for huff:
        in_file = "merchant.txt";
//        in_file = "kjv.txt";
//        in_file = "bartleby.txt";
//        in_file = "hamlet.txt";
        out_file = "output.txt";

    }
    
    else {
        in_file = argv[1];
        out_file = argv[2];
    }

    bifstream in(in_file);
    assert_good(in, argv);

    bofstream out(out_file);
    assert_good(out, argv);

    huff(in_file,out);
    return 0;
}
