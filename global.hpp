//
//  global.h
//  bcl
//
//  Created by Amatur Rahman on 24/11/19.
//  Copyright © 2019 psu. All rights reserved.

#ifndef global_h
#define global_h

bool DEBUGMODE = false;
bool ALLABSORBING = false;
bool GETLOWERBOUND_CC = true;
bool VARIMODE = false;
bool VARIMODE2 = false;
bool BOTHTIPABSORB = false;
bool BOTHTIPABSORB_V2 = false;
bool ABSORBONLYTWO = false;
bool SCCORDER = true;
int C_abs_calc = 0;
int ncc = 0;

bool BENCHMARK=true;


bool NAIVEVARI = false;

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <stdint.h>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <list>
#include <stack>
#include <unordered_map>
#include <utility>
#include <queue>
#include <deque>
#include <unistd.h>
#include <tuple>
#include <set>
#include <map>

using namespace std;

//int K = 11;
//string UNITIG_FILE = "/Volumes/exFAT/data2019/staphsub/11/list_reads.unitigs.fa";

//int K = 11;
//string UNITIG_FILE = "/Volumes/exFAT/data2019/phi/11/list_reads.unitigs.fa";

//
//int K = 31;
//string UNITIG_FILE = "/Volumes/exFAT/data2019/chol/31/list_reads.unitigs.fa";

//int SMALLK = 27;
//int K = 47;
//string UNITIG_FILE = "/Volumes/exFAT/data2019/chol/47/list_reads.unitigs.fa";

//int K = 11;
//string UNITIG_FILE = "/Users/Sherlock/amaturWS/data2019/subgraph/phi/11/list_reads.unitigs.fa";

int K = 11;
string UNITIG_FILE = "/Users/Sherlock/amaturWS/data2019/subgraph/chol/11/list_reads.unitigs.fa";
int SMALLK = K;

//int K = 11;
//string UNITIG_FILE = "/Users/Sherlock/amaturWS/data2019/staphsub/"+to_string(K)+"/list_reads.unitigs.fa";
//string UNITIG_FILE = "/Users/Sherlock/amaturWS/data2019/staphsub/11/list_reads.unitigs.fa";

ofstream abfile;

enum DEBUGFLAG_T
{
    NONE = 0,
    UKDEBUG = 0,
    VERIFYINPUT = 1,
    INDEGREEPRINT = 2,
    DFSDEBUGG = 3,
    PARTICULAR = 4,
    NODENUMBER_DBG = 5,
    OLDNEWMAP = 9,
    PRINTER = 10,
    SINKSOURCE = 12
};

enum ALGOMODE_T
{
    BASIC = 0,
    INDEGREE_DFS = 1,
    INDEGREE_DFS_1 = 2,
    OUTDEGREE_DFS = 3,
    OUTDEGREE_DFS_1 = 4,
    INDEGREE_DFS_INVERTED = 5,
    PLUS_INDEGREE_DFS = 6,
    RANDOM_DFS = 7,
    NODEASSIGN = 8,
    SOURCEFIRST = 9,
    TWOWAYEXT = 10,
    PROFILE_ONLY = 11,
    EPPRIOR = 12,
    GRAPHPRINT = 13,
    TIGHTUB = 14,
    BRACKETCOMP = 15,
    ONEWAYABSORPTION = 16,
    ONEWAYABSORPTION_UNTESTED = 17,
    TIPANDAB = 18,
    TIPANDAB_TIPLATER = 19,
    PROFILE_ONLY_ABS = 20,
    SCC_ABS=21,
    ESS=22,PROFILE_ONLY_ESS=23
};

bool FLG_NEWUB = true;

DEBUGFLAG_T DBGFLAG = NONE; //NODENUMBER_DBG
ALGOMODE_T ALGOMODE = ONEWAYABSORPTION;

bool MODE_WALK_UNION = (ALGOMODE == TWOWAYEXT);
bool MODE_ABSORPTION_TIP = (ALGOMODE == BRACKETCOMP);
bool MODE_ABSORPTION_NOTIP = (ALGOMODE == ONEWAYABSORPTION || ALGOMODE == ONEWAYABSORPTION_UNTESTED);

//string mapmode[] = {"basic", "indegree_dfs", "indegree_dfs_initial_sort_only", "outdegree_dfs", "outdegree_dfs_initial_sort_only", "inverted_indegree_dfs", "plus_indegree_dfs", "random_dfs", "node_assign", "source_first", "twoway", "profile_only", "endpoint_priority", "graph_print", "tight_ub", "tip", "one_way_absorption", "one_way_absorption_not_tested", "tip_and_ab", "tip_and_ab_tip_later"
//};

string mapmode[] = {"basic", "indegree_dfs", "indegree_dfs_initial_sort_only", "outdegree_dfs", "outdegree_dfs_initial_sort_only", "inverted_indegree_dfs", "plus_indegree_dfs", "random_dfs", "node_assign", "source_first", "", "profile_only", "endpoint_priority", "graph_print", "tight_ub", "", "", "", "", "", "", "", ""};
string modename[] = {"Fwd", "indegree_dfs", "indegree_dfs_initial_sort_only", "outdegree_dfs", "outdegree_dfs_initial_sort_only", "inverted_indegree_dfs", "plus_indegree_dfs", "random_dfs", "node_assign", "source_first", "", "profile_only", "endpoint_priority", "graph_print", "tight_ub", "ess-tip-compress", "ess-compress", "one_way_absorption_not_tested", "tip_and_ab", "tip_and_ab_tip_later", "profile_only_abs", "scc_abs", "ess", "profile_only_ess"};

/*
 FILENAMES
 */
string ofileTipOutput = "ust_ess_abs.txt";
string ofileTipDebug = "tipDebug.txt";

namespace MyTypes
{
typedef tuple<int, int, int, int> fourtuple; // uid, walkid, pos, isTip
// etc.
} // namespace MyTypes

typedef struct
{
    int serial = -1;
    int sccid = -1;
    bool isWalkEnd = false;
    int pos_in_walk = -100;
    int finalWalkId = -1; // renders some walkId as invalid
    int isTip = 0;
} new_node_info_t;

typedef struct
{
    int serial;
    string sequence;
    int ln;
    //int kc;
    //float km;
} unitig_struct_t;
vector<unitig_struct_t> unitigs;

typedef struct
{
    //1 means +, 0 means -
    bool left;
    bool right;
    char isSmallK = 'n';
    int smallk = 0;
    int toNode;
} edge_t;

typedef struct
{
    edge_t edge;
    int fromNode;
} edge_both_t;

typedef struct
{
    edge_t edge;
    int kmerStartIndex;
    int kmerEndIndex;
} newEdge_t;

bool sort_by_walkId(const MyTypes::fourtuple &lhs, const MyTypes::fourtuple &rhs)
{
    return get<1>(lhs) < get<1>(rhs);
}
bool sort_by_pos(const MyTypes::fourtuple &lhs, const MyTypes::fourtuple &rhs)
{
    return get<2>(lhs) < get<2>(rhs);
}
bool sort_by_tipstatus(const MyTypes::fourtuple &lhs, const MyTypes::fourtuple &rhs)
{
    return get<3>(lhs) < get<3>(rhs);
}

struct node_sorter
{
    int node;
    int sortkey;
    //bool operator() (struct node_sorter  i, struct node_sorter  j) { return (i.sortkey<j.sortkey);}
};
bool sort_by_key(struct node_sorter i, struct node_sorter j) { return (i.sortkey < j.sortkey); }
bool sort_by_key_inverted(struct node_sorter i, struct node_sorter j) { return (i.sortkey > j.sortkey); }

int *global_indegree;
int *global_outdegree;
int *global_plusindegree;
int *global_plusoutdegree;
int *global_issinksource;
int *global_priority;
new_node_info_t *oldToNew;
bool *nodeSign;
char *color;
//absorber-global
vector<bool> obsoleteWalkId;
int countNewNode = 0; // number of walks by ust-onewaymerge

map<pair<int, int>, int> inOutCombo;
vector<vector<edge_t>> adjList;
//vector<vector<edge_t>> reverseAdjList;

vector<list<int>> newToOld;
vector<int> walkFirstNode;                       //given a walk id, what's the first node of that walk
unordered_map<int, vector<edge_t>> sinkSrcEdges; //int is the unitig id (old id)

//connected component count graph
int absorbGraphNumCC_endAbosrb = 0;
int absorbGraphNumCC_allAbosrb = 0;

int C_ustitch = 0;
int C_twoway_ustitch = 0;
int C_tip_ustitch = 0;
int C_oneabsorb = 0;
int C_oneabsorb_ACGT = 0;
int C_oneabsorb_brackets = 0;
int C_oneabsorb_plusminus = 0;
int THEDEPTH = 999999;

int C_tip_special=0;

int V_ustitch = 0;
int V_twoway_ustitch = 0;
int V_tip_ustitch = 0;
int V_oneabsorb = 0;

int isolated_node_count = 0;
int sink_count = 0;
int source_count = 0;
int sharedparent_count = 0;
int sharparentCntRefined = 0;
int onecount = 0;

std::map<std::string, set<int>> plusMapPre;
std::map<std::string, set<int>> minusMapPre;
std::map<std::string, set<int>> plusMapSuf;
std::map<std::string, set<int>> minusMapSuf;

int myrandom(int i) { return std::rand() % i; }

string getStdoutFromCommand(string cmd)
{

    string data;
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL)
                data.append(buffer);
        pclose(stream);
    }
    return data;
}

void process_mem_usage(double &vm_usage, double &resident_set)
{
    return;
    //in KB?
    vm_usage = 0.0;
    resident_set = 0.0;

#ifdef __APPLE__
    return;
#endif

    //    int pid = (int) getpid();
    //    string virt_top = getStdoutFromCommand("top -p "+ std::to_string(pid)+" -b -n1 | tail -1 |  awk '{ print $5}'");
    //    string rss_top = getStdoutFromCommand("top -p "+ std::to_string(pid)+" -b -n1 | tail -1 |  awk '{ print $6}'");
    //
    //    sscanf(virt_top.c_str(), "%lf", &vm_usage);
    //    sscanf(rss_top.c_str(), "%lf", &resident_set);

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

inline string plus_strings(const string &a, const string &b, size_t kmersize)
{
    if (a == "")
        return b;
    if (b == "")
        return a;
    string ret = a + b.substr(kmersize - 1, b.length() - (kmersize - 1));
    return ret;
}

inline string pref(const string &b, size_t kmersize)
{
    return b.substr(0, (kmersize - 1));
}
inline string cutPref(const string &b, size_t kmersize)
{
    //    if(b.length()<kmersize){
    //        return "";
    //    }
    return b.substr(kmersize - 1, b.length() - (kmersize - 1));
}
inline string suf(const string &b, size_t kmersize)
{
    //    if(b.length()<kmersize){
    //        return "";
    //    }
    return b.substr(b.length() - (kmersize - 1), (kmersize - 1));
}
inline string cutSuf(const string &b, size_t kmersize)
{
    //    if(b.length()<kmersize){
    //        return "";
    //    }
    return b.substr(0, b.length() - (kmersize - 1));
}

inline string delSpaces(string &str)
{
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

inline bool charToBool(char c)
{
    if (c == '+')
    {
        return true;
    }
    else
    {
        if (c != '-')
            cout << "ERRRRRROOR!" << endl;
        return false;
    }
}

inline string reverseComplement(string base)
{
    size_t len = base.length();
    char *out = new char[len + 1];
    out[len] = '\0';
    for (int i = 0; i < len; i++)
    {
        if (base[i] == 'A')
            out[len - i - 1] = 'T';
        else if (base[i] == 'C')
            out[len - i - 1] = 'G';
        else if (base[i] == 'G')
            out[len - i - 1] = 'C';
        else if (base[i] == 'T')
            out[len - i - 1] = 'A';
    }
    string outString(out);
    free(out);
    return outString;
}

inline double readTimer()
{
    return clock() / (double)CLOCKS_PER_SEC;
}

inline string currentDateTime()
{
    // Get current date/time, format is YYYY-MM-DD HH:mm:ss
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X\n", &tstruct);
    return buf;
}

inline int countOutArcs(int node)
{
    return (adjList.at(node)).size();
}

inline char boolToCharSign(bool sign)
{
    return (sign == true) ? '+' : '-';
}

int maximumUnitigLength()
{
    int m = 0;
    for (unitig_struct_t u : unitigs)
    {
        if (u.ln > m)
        {
            m = u.ln;
        }
    }
    return m;
}

int read_unitig_file_abs(const string &unitigFileName, vector<unitig_struct_t> &unitigs)
{
    ifstream unitigFile;
    unitigFile.open(unitigFileName);

    string line;

    int nodeNum;
    char lnline[20];
    char kcline[20];
    char kmline[20];
    char edgesline[100000];
    bool doCont = false;

    //    set<string> variAllStrings;
    //    ofstream variAllStringsFile("variKMC.fa");
    //    ofstream variPreFile("vari_pre.txt");
    //    ofstream variSufFile("vari_suf.txt");
    //    ofstream variPreRCFile("vari_prerc.txt");
    //    ofstream variSufRCFile("vari_sufrc.txt");

    //ofstream variIntFile("vari_int.txt");

    getline(unitigFile, line);

    do
    {
        unitig_struct_t unitig_struct;
        edgesline[0] = '\0';
        sscanf(line.c_str(), "%*c %d %s  %s  %s %[^\n]s", &unitig_struct.serial, lnline, kcline, kmline, edgesline);
        //cout<<line<<endl;
        // @@DEBUG
        //        if(unitig_struct.serial == 1241914){
        //            cout<<line<<endl;
        //            cout<<edgesline<<endl;
        //        }
        //>0 LN:i:13 KC:i:12 km:f:1.3  L:-:0:- L:-:2:-  L:+:0:+ L:+:1:-

        sscanf(lnline, "%*5c %d", &unitig_struct.ln);
        int kc;
        float km;
        sscanf(kcline, "%*5c %d", &kc);
        sscanf(kmline, "%*5c %f", &km);

        char c1, c2;
        stringstream ss(edgesline);

        vector<edge_t> edges;
        while (getline(ss, line, ' '))
        {
            if (delSpaces(line).length() != 0)
            {
                if (DBGFLAG == VERIFYINPUT)
                {
                    cout << line << endl;
                }
                //                if(unitig_struct.serial == 1241914){
                //                    cout<<line<<endl;
                //                }
                sscanf(line.c_str(), "%*2c %c %*c %d  %*c  %c", &c1, &nodeNum, &c2); //L:-:0:-
                edge_t newEdge;

                bool DELSELFLOOP = true;
                if (DELSELFLOOP)
                {
                    if ((unitig_struct.serial) != nodeNum)
                    {
                        newEdge.left = charToBool(c1);
                        newEdge.right = charToBool(c2);
                        newEdge.toNode = nodeNum;
                        edges.push_back(newEdge);
                    }
                }
                else
                {
                    newEdge.left = charToBool(c1);
                    newEdge.right = charToBool(c2);
                    newEdge.toNode = nodeNum;
                    edges.push_back(newEdge);
                }
            }
        }
        adjList.push_back(edges);
        if(ALGOMODE == 15){
            unitigs.push_back(unitig_struct);
            unitigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
        }



        doCont = false;
        while (1==1 && getline(unitigFile, line))
        {
            if (line.substr(0, 1).compare(">"))
            {
                unitig_struct.sequence = unitig_struct.sequence + line;
                //cout<<unitig_struct.sequence<<endl;

                //ADDED VARI
                if (VARIMODE)
                {
                    //if(unitig_struct.ln >= 2*(K-1)){
                    //}
                    //string thepref = pref(unitig_struct.sequence, K);
                    //string thesuf =suf(unitig_struct.sequence, K);
                    //variAllStrings.insert(thepref);
                    //variAllStrings.insert(thesuf);

                    //variPreFile<<thepref<<endl;
                    //variPreRCFile<<reverseComplement(thepref)<<endl;

                    //variSufFile<<thesuf<<endl;
                    //variSufRCFile<<reverseComplement(thesuf)<<endl;

                    //variAllStringsFile<<">\n"<<thepref<<endl;
                    //variAllStringsFile<<">\n"<<thesuf<<endl;
                    //variIntFile<<unitig_struct.serial<<endl;
                }

                unitigs.push_back(unitig_struct);
            }
            else
            {
                doCont = true;
                break;
            }
        }
    } while (doCont);

    unitigFile.close();

    //    variPreFile.close();
    //    variSufFile.close();
    //    variPreRCFile.close();
    //    variSufRCFile.close();
    //variIntFile.close();
    if (VARIMODE)
    {
        //         for(string s: variAllStrings){
        //             variAllStringsFile<<">\n"<<s<<endl;
        //         }
        //         variAllStrings.clear();
    }
    // variAllStringsFile.close();

    cout << "Finished reading input unitig file (bcalm2 file)." << endl;
    //exit(1);
    return EXIT_SUCCESS;
}



// @@ --- ALL PRINTING CODE --- //
void printBCALMGraph(vector<vector<edge_t>> adjList)
{
    for (int i = 0; i < adjList.size(); i++)
    {
        cout << i << "# ";
        for (edge_t edge : adjList.at(i))
        {
            cout << boolToCharSign(edge.left) << ":" << edge.toNode << ":" << boolToCharSign(edge.right) << ", ";
        }
        cout << endl;
    }
}

//void printAllBCALMSequences(vector<unitig_struct_t> unitigs)
//{
//    for (unitig_struct_t unitig : unitigs)
//    {
//        cout << unitig.serial << ": " << unitig.ln << " " << unitig.sequence.length() << endl;
//    }
//}

// might be useful for doing some visualization.
bool canReachSinkSource(int v, bool visited[], bool sign)
{
    // Mark the current node as visited and
    // print it

    visited[v] = true;
    //cout << v << " ";
    bool reachable = false;

    if (global_plusoutdegree[v] == 0 && global_plusindegree[v] != 0)
    {
        //cout<<v<<"is sink.";
        return true; //sink
    }
    if (global_plusindegree[v] == 0 && global_plusoutdegree[v] != 0)
    {
        //cout<<v<<"is source.";
        return true; //source
    }
    if (global_indegree[v] == 0)
    {
        //cout<<v<<"is isolated.";
        return true; //isolated
    }

    // Recur for all the vertices adjacent
    // to this vertex
    vector<edge_t>::iterator i;
    for (i = adjList[v].begin(); i != adjList[v].end(); ++i)
    {

        if (!visited[(*i).toNode] && sign == (*i).left)
        {
            reachable = canReachSinkSource((*i).toNode, visited, (*i).right);
            if (reachable == true)
            {
                return true;
            }
        }
    }
    return reachable;
}

set<int> extractIntegerWords(string str)
{
    set<int> retset;
    stringstream ss;

    /* Storing the whole string into string stream */
    ss << str;

    /* Running loop till the end of the stream */
    string temp;
    int found;
    while (!ss.eof())
    {

        /* extracting word by word from stream */
        ss >> temp;

        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found)
            retset.insert(found);

        /* To save from space at the end of string */
        temp = "";
    }
    return retset;
}

void makeGraphDot(string ipstr)
{
    FILE *fp;

    fp = fopen("/Users/Sherlock/Downloads/graphviz-2.40.1/graph.gv", "w+");

    fprintf(fp, "digraph d {\n");
    //string ipstr = "20 19 18";
    set<int> verticesMarked;
    set<int> vertices = extractIntegerWords(ipstr);
    set<int> vMarked(vertices.begin(), vertices.end());
    //set<pair<int, int> > edges;

    for (int x : vertices)
    {
        if (x >= adjList.size())
        {
            cout << "wrong, do again" << endl;
            return;
        }
        auto adjX = adjList[x];
        for (edge_t ex : adjX)
        {
            vertices.insert(ex.toNode);
            fprintf(fp, "%d -> %d[taillabel=\"%d\", headlabel=\"%d\", arrowhead=\"none\"]\n", x, ex.toNode, ex.left, !ex.right);

            //            pair<int, int> p;
            //            if(x < ex.toNode){
            //                p.first = x;
            //                p.second = ex.toNode;
            //            }else{
            //                p.second = x;
            //                p.first = ex.toNode;
            //            }
            //            edges.insert(p);
        }
    }
    for (int x : vertices)
    {
        if (vMarked.count(x) > 0)
        {
            fprintf(fp, "%d [label=\"%d\", color=\"red\"]\n", x, x);
        }
        else
        {
            fprintf(fp, "%d [label=\"%d\"]\n", x, x);
        }
    }

    //for all int in list
    //make a list of neighbors add them

    fprintf(fp, "}\n");

    fclose(fp);
}



vector<int> nodes;

void pullOutCCDFS(int start, bool *ccVisited, int &count, bool pullit=true)
{
    ccVisited[start] = true;
    count++;

    for (edge_t e : adjList[start])
    {
        int elem = e.toNode;
        if (ccVisited[elem] == false)
        {
            if (pullit)
                nodes.push_back(elem);
            pullOutCCDFS(elem, ccVisited, count);
        }
    }
}

int pullOutConnectedComponent(bool pullit = false)
{
    if(BENCHMARK){
        return -1;
    }
    if(ALGOMODE == 15){
        return -1;
    }
    //pullit = false;
    int numCC_bcalm_graph = 0;
    bool *ccVisited = new bool[adjList.size()];
    vector<int> verticesInOneCC;
    map<int, int> uidRemapper;

    for (int i = 0; i < adjList.size(); i++)
    {
        ccVisited[i] = (false);
    }

    stack<int> stack;
    for (int ii = 0; ii < adjList.size(); ii++)
    {
        if (ccVisited[ii] == false)
        {
            stack.push(ii);
            numCC_bcalm_graph++;
            while (!stack.empty())
            {
                int s = stack.top();
                stack.pop();
                if (!ccVisited[s])
                {
                    //cout << s << " ";
                    ccVisited[s] = true;
                }

                for (auto i = adjList[s].begin(); i != adjList[s].end(); ++i){
                    int tonode = (*i).toNode;
                    if (!ccVisited[tonode])
                        stack.push(tonode);
                }
                    
            }
        }
    }

    if (pullit)
    {
        for (int i = 0; i < adjList.size(); i++)
        {
            if (ccVisited[i] == false)
            {
                vector<int> nodes;
                nodes.push_back(i);
                numCC_bcalm_graph++;
                int count = 0;
                pullOutCCDFS(i, ccVisited, count);
                //cout<<"This component has "<<count<<" nodes"<<"\n";

                int serial = 0;
                if (pullit)
                {
                    for (int nn : nodes)
                    {
                        verticesInOneCC.push_back(nn);
                        uidRemapper[nn] = serial++;
                        //cout<<nn<<",";
                    }
                }

                //cout<<endl;
            }
            if (pullit)
                break;
        }
    }
    

    if (pullit)
    {
        //>1 LN:i:41 KC:i:178 km:f:16.2  L:+:2:-  L:-:1112198:+ L:-:1331912:+
        //ATCCTGAATATGGTTTTGAAAAAAACGCGGGTTACGGTACC
        ofstream subgraphFile;
        subgraphFile.open("subgraph.fa");
        for (int uid : verticesInOneCC)
        {

            subgraphFile << ">" << uidRemapper[uid] << " "
                         << "LN:i:" << unitigs.at(uid).ln << " KC:i:178 km:f:16.2 ";
            for (edge_t e : adjList[uid])
            {
                if (uidRemapper.count(e.toNode) > 0)
                {
                    subgraphFile << "L:" << boolToCharSign(e.left) << ":" << uidRemapper[e.toNode] << ":" << boolToCharSign(e.right) << " ";
                }
            }
            subgraphFile << endl;
//            subgraphFile << unitigs.at(uid).sequence << endl;
        }

        subgraphFile.close();
    }

    cout << "number of connected components in bcalm graph: " << numCC_bcalm_graph << endl;
    delete[] ccVisited;

    return numCC_bcalm_graph;
}

int read_unitig_file(const string& unitigFileName, vector<unitig_struct_t>& unitigs) {
    ifstream unitigFile;
    unitigFile.open(unitigFileName);

    string line;

    int nodeNum;
    char lnline[20];
    char kcline[20];
    char kmline[20];
    char edgesline[100000];
    bool doCont = false;

    int smallestK = 9999999;
    getline(unitigFile, line);

    do {
        unitig_struct_t unitig_struct;

        if(1==1){
            edgesline[0] = '\0';
            sscanf(line.c_str(), "%*c %d %s  %s  %s %[^\n]s", &unitig_struct.serial, lnline, kcline, kmline, edgesline);

            if(    line.find("KC") == string::npos){
                cout<<"Incorrect input format. Try using flag -a 1."<<endl;
                exit(3);
            }

            //>0 LN:i:13 KC:i:12 km:f:1.3  L:-:0:- L:-:2:-  L:+:0:+ L:+:1:-
            sscanf(lnline, "%*5c %d", &unitig_struct.ln);


            if(unitig_struct.ln < smallestK){
                           smallestK = unitig_struct.ln ;
                       }
            if(unitig_struct.ln < K){
                printf("Wrong k! Try again with correct k value. \n");
                exit(2);
            }

        }



        char c1, c2;
        stringstream ss(edgesline);

        vector<edge_t> edges;
        while (getline(ss, line, ' ')) {
            if (delSpaces(line).length() != 0) {
                if(DBGFLAG==VERIFYINPUT){
                    cout<<line<<endl;
                }

                sscanf(line.c_str(), "%*2c %c %*c %d  %*c  %c", &c1, &nodeNum, &c2); //L:-:0:-
                edge_t newEdge;

                bool DELSELFLOOP=true;
                if(DELSELFLOOP){
                    if((unitig_struct.serial)!= nodeNum){
                        newEdge.left = charToBool(c1);
                        newEdge.right = charToBool(c2);
                        newEdge.toNode = nodeNum;
                        edges.push_back(newEdge);
                    }
                }else{
                    newEdge.left = charToBool(c1);
                    newEdge.right = charToBool(c2);
                    newEdge.toNode = nodeNum;
                    edges.push_back(newEdge);
                }

            }

        }
        adjList.push_back(edges);



        doCont = false;
        while (getline(unitigFile, line)) {
            if (line.substr(0, 1).compare(">")) {
                //unitig_struct.sequence = unitig_struct.sequence + line;
                unitigs.push_back(unitig_struct);
            } else {
                doCont = true;
                break;
            }
        }
    } while (doCont);


    unitigFile.close();

    if(smallestK > K ){
        cout<<"\n :::: :::: :::: :::: !!!!!!!!! WARNING !!!!!!!!!!! :::: :::: :::: ::::"<<endl;
        cout<<"The length of the smallest string we found was " << smallestK << ". Please make sure you are using the correct value of 'k' to ensure correctness of output."<<endl;
         cout << "------------------------------------------------------"<<endl;
    }

    //cout << "Complete reading input unitig file (bcalm2 file)." << endl;
    return EXIT_SUCCESS;
}
 
ofstream tipFile;
ofstream tipDebugFile;

#endif /* global_h */
