// UST
// Last Edited: May 20, 2020

#include "ust.hpp"
#include "tip.hpp"
#include "ess.hpp"
#include "profileGraph.hpp"

using namespace std;

int main(int argc, char** argv) {
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#else
    cout<<"ESS-Compress does not support this OS."<<endl;
    return EXIT_FAILURE; //exit(2);
#endif
    
    bool PROFILE_AND_STAT = false;
    int IS_TIP = 0;
    int K = 0;
    
    char * nvalue;
    string graph_file_name;
    
    char c;
    while( ( c = getopt (argc, argv, "i:k:t:p:") ) != -1 )
    {
        switch(c)
        {
            case 't':
                if(optarg) {
                    if(strcmp(optarg, "0")==0 || strcmp(optarg, "1")==0 ||  strcmp(optarg, "2")==0){
                        IS_TIP = static_cast<int>(std::atoi(optarg));
                    }else{
                        fprintf(stderr, "Error: use either -t 0 or -t 1 \n");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 'p':
                if(optarg) {
                    if(strcmp(optarg, "0")==0 || strcmp(optarg, "1")==0){
                        PROFILE_AND_STAT = static_cast<bool>(std::atoi(optarg));
                    }else{
                        fprintf(stderr, "Error: use either -p 0 or -p 1 \n");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 'i':
                if(optarg) nvalue = optarg;
                break;
            case 'k':
                if(optarg) {
                    K = std::atoi(optarg) ;
                    if(K<=0){
                        fprintf(stderr, "Error: Specify a positive k value.\n");
                        exit(EXIT_FAILURE);
                    }
                }else{
                    fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name> [-t <0: for no tip, 1: tip fast, default = 0>]\n",
                            argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name> [-t <0: for no tip, 1: tip fast, default = 0>]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    if(K==0 || strcmp(nvalue, "")==0){
        fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name> [-t <0: for no tip, 1: tip fast, default = 0>]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
    graph_file_name = string(nvalue);
    
    
    if(PROFILE_AND_STAT){
        ProfileGraph().run(graph_file_name, K);
    }else if(IS_TIP == 2){
        UST ust;
        ust.run(graph_file_name, K, 0);
    }else if(IS_TIP == 1){
        ESSTip esstip;
        esstip.run(graph_file_name, K);
    }else{
        AbsorbGraph ess;
        ess.run(graph_file_name, K);
    }
}
