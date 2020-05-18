//
//  decoder.h
//  bcl
//
//  Created by Amatur Rahman on 28/11/19.
//  Copyright © 2019 psu. All rights reserved.
//
//

#ifndef decoder_h
#define decoder_h

#include <cmath>
#include<cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <stdint.h>
#include <unistd.h>
using namespace std;


int decodeTip(int K, string UNITIG_FILE="ust_ess_tip.txt"){
    ifstream unitigFile;
    unitigFile.open(UNITIG_FILE);

    ofstream outFile;
    outFile.open("tip.fa");

    string line;
    bool startPrefCut = false;
    bool startSufCut = false;
    string pref;
    string suf;
    int walkid = 0;

    while (getline(unitigFile, line)) {
        //cout<<line<<endl;
        if (line.empty() || line.substr(0, 1).compare(">") == 0) {

        } else {
            walkid++;

            startPrefCut = false;
            startSufCut = false;

            string tip = "";
            string sbuf = "";
            string lastk = "";
            string pref = "";
            string suf = "";

            //cout<<walkid-1<<" "<<line[0]<<endl;
            for(int i = 0; i<line.length(); i++){
                if(line[i]=='A'|| line[i]=='C' || line[i]=='G' || line[i]=='T'){
                    if(startPrefCut){
                        tip += line[i];
                    }else if(startSufCut){
                        tip += line[i];
                    }else{
                        sbuf +=   line[i];
                        if(lastk.length() < K - 1){
                            lastk += line[i];
                        }else{
                            lastk = lastk.substr(1, K-2) + line[i];
                        }
                    }
                }else if(line[i]=='(' || line[i]==')'){
                    if(startPrefCut){ //already has one
                        startPrefCut = false;
                        //outFile<< "> pref: " << " "  << endl; //for debug
                        outFile<< ">"<< endl;
                        outFile<< (pref + tip) << endl;
                        tip = "";
                    }else if(!startPrefCut){ //prefix is cut starts
                        startPrefCut = true;
                        pref = lastk;
                    }
                }else if(line[i]=='{' || line[i]=='}'){ //suffix is cut
                    if(startSufCut){ //already has one
                        startSufCut = false;
                        //outFile<< ">suf: " << " " << endl; //for Debug
                        outFile<< ">" << endl;
                        outFile<< (tip + suf) << endl;
                        tip = "";
                    }else if(!startSufCut){
                        startSufCut = true;
                        suf = lastk;
                    }
                }
            }
            //print the tips before printing the contig
            //outFile<<">"<<walkid-1<<" : "<<"\n"; for Debug
            outFile<<">\n";
            outFile<<sbuf << endl;
        }
    }

    unitigFile.close();
    outFile.close();

    cout << "Complete conversion." << endl;
    return EXIT_SUCCESS;
}


void updateCurrOverlap(char c, string& lastk1, int K){
    if(lastk1.length() < K - 1){
        lastk1 += c;
        //printf("baje case");
    }else{
        lastk1 = lastk1.substr(1, K-2) + c;
    }
}

vector<string> decompressEnclosed(string& s, int& i, string overlapFromParent, int K, ofstream & FOUT){
    vector<string> S;
    string sOut;
    string currOverlap;

    while(i < s.length()){
        char c = s[i++];
        if(c=='['){
            vector<string> S1 = decompressEnclosed(s, i,currOverlap, K, FOUT);
            for(int j = 0; j < S1.size(); j++){
                //S.push_back(S1.at(j));
            }
        }else if(c==']'){
            //S.push_back(sOut);
            FOUT<<">\n"<<sOut<<endl;
            return S;
        }else if(c=='+'){
            sOut += overlapFromParent;
            //update currOverlap
            currOverlap = overlapFromParent;
        }else if(c=='-'){
            sOut += reverseComplement(overlapFromParent);
            //update currOverlap
            currOverlap = reverseComplement(overlapFromParent);
        }else{
            sOut += c;
            updateCurrOverlap(c,currOverlap, K);
            //update currOverlap
        }

    }
    return S;
}

vector<string> decompress(string& s, int K, ofstream & FOUT){
    vector<string> S;
    string sOut;
    string currOverlap;

    int i = 0;
    while(i < s.length()){
        char c = s[i++];
           if(c=='['){
               vector<string> S1 = decompressEnclosed(s, i,currOverlap, K, FOUT);
               for(int j = 0; j < S1.size(); j++){
                   //S.push_back(S1.at(j));
               }
           }else{
               sOut += c;
               updateCurrOverlap(c, currOverlap, K);
               //update currOverlap
           }
    }


   // cout<< "Complete conversion of: " <<s << endl;

    //S.push_back(sOut);
    FOUT<<">\n"<<sOut<<endl;
    return S;
}


///Users/Sherlock/Library/Developer/Xcode/DerivedData/bcl-awuuvnjbkkmukneqtvxvqashsbtu/Build/Products/Debug/tipOutput.txt
///Users/Sherlock/amaturWS/testEncoded.txt
void decodeOneAbsorb(int K, string ENCODED_FILE= "ust_ess_abs.txt", string OUT_FA_FILE="absorbDecompressed.fa"){
    ifstream encodedFile;
    encodedFile.open(ENCODED_FILE);

    ofstream outFile;
    outFile.open(OUT_FA_FILE);
    string line;
    int nWalks = 0;

    vector<int> nWalkNo;
    vector<string> collectedStrings;
    while (getline(encodedFile, line)) {
        //cout<<line<<endl;
        if (line.empty() || line.substr(0, 1).compare(">") == 0) {

        } else {
            nWalks++;
            vector<string> S1 = decompress(line, K, outFile);
            for(int j = 0; j < S1.size(); j++){
                //collectedStrings.push_back(S1.at(j));
                nWalkNo.push_back(nWalks);
            }

        }
    }
    encodedFile.close();

    int i =0;

    //double memvmKB,memrssKB;
    //process_mem_usage(memvmKB, memrssKB);
    //   cout<<"inside decoder memory (virtual) to do ONLY UST DFS (KB)"<<memvmKB<<endl;
    //   cout<<"inside decoder  memory (rss) to do ONLY UST DFS (KB)"<<memrssKB<<endl;
    for(string s: collectedStrings){
        //outFile<<">"<<nWalkNo[i++]<<"\n"<<s<<endl;
        //cout<<s<<endl;
    }
    outFile.close();
}



#endif /* decoder_h */
