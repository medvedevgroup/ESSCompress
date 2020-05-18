//
//  decoder.h
//
//  Created by Amatur Rahman on 28/11/19.
//  Copyright © 2019 Penn State. All rights reserved.
//

//#ifndef decoder_h
//#define decoder_h
#include <ctype.h>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//

#include "decoder.hpp"
using namespace std;

string remove_extension(const string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot);
}


string basename(const string& s) {

   char sep = '/';

#ifdef _WIN32
   sep = '\\';
#endif

   size_t i = s.rfind(sep, s.length());
   if (i != string::npos) {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}

bool isFileExist(string& fileName) {
    FILE *fp = fopen(fileName.c_str(), "r");
    if (fp) {
        fclose(fp);
        return true;
    }
    return errno != ENOENT;
}

int main(int argc, char** argv) {

    const char* nvalue = "" ;
    int K=0;
    string filename = "ust_ess_abs.txt";
    bool tip_mode = 0;

        int c ;

            while( ( c = getopt (argc, argv, "i:k:t:") ) != -1 )
            {
                switch(c)
                {
                    case 'i':
                        if(optarg) nvalue = optarg;
                        break;
                    case 't':
                        if(optarg) {
                            tip_mode = static_cast<bool>(std::atoi(optarg));
                        }
                        break;
                    case 'k':
                        if(optarg) {
                            K = std::atoi(optarg) ;
                            if(K<=0){
                                fprintf(stderr, "Error: Specify a positive k value.\n");
                                exit(EXIT_FAILURE);
                            }
                        }else{
                            fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name>\n",
                                    argv[0]);
                            exit(EXIT_FAILURE);
                        }
                        break;
                    default: //
                        fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-path>\n",
                                argv[0]);
                        exit(EXIT_FAILURE);

                }
            }


    if(K==0 || strcmp(nvalue, "")==0){
        fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // if(tip_mode){
    //     filename = "ust_ess_tip.txt";
    // }


    string pathname = string(nvalue);
    if(!isFileExist(pathname)){
        cout<<"File \""<<pathname<<"\""<<"does not exist."<<endl;
        exit(EXIT_FAILURE);
    }

    //pathname+="/"+filename;
    //int K=31, string ENCODED_FILE= "tipOutput.txt"

    string outputFilename = remove_extension(basename(pathname));

    if(tip_mode){
        decodeTip(K, pathname, pathname+".spss.fa");
        cout<<"ESS-Tip-Compress decoding done!"<<endl;
        cout<<"Output SPSS is in file \""<<outputFilename+".spss.fa"<<"\""<<endl;
    }else{
        decodeOneAbsorb(K, pathname, outputFilename+".spss.fa");
        cout<<"ESS-Compress decoding done!"<<endl;
        cout<<"Output SPSS is in file \""<<outputFilename+".spss.fa"<<"\""<<endl;
    }


    // cout<<GetStdoutFromCommand("top -p "+ std::to_string(pid)+" -b -n1 | tail -1 |  awk '{ print $5}'")<<endl;
    // cout<<"printed memory requirement"<<endl;
    //

    return 0;
}



//#endif /* decoder_h */
