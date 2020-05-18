// --- VERSION 8 :  absorption bug fixed -- optimal to bound ----
// - jan 22, 2020

#include "absorbGraph.hpp"
#include "global.hpp"
#include "decoder.hpp"
#include <ctime>
#include <random>
#include <sys/time.h>
#include <sys/resource.h>
#define MEM_ENV_VAR 1024


//void setmemlimit();

void tipSpell(vector<MyTypes::fourtuple>& sorter){

    ofstream uidSequence;
    string uidSeqFilename = "uidSeq.usttemp"; //"uidSeq"+ mapmode[ALGOMODE] +".txt"
   uidSequence.open(uidSeqFilename);

    int finalUnitigSerial = 0;
    for(MyTypes::fourtuple& n : sorter){
        int uid = get<0>(n);
        int bcalmid = unitigs.at(uid).serial;
//                        int finalWalkId = get<1>(n);
//                        int pos_in_walk = get<2>(n);
//                        int isTip = get<3>(n);
//                        cout<<uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<" "<<oldToNew[uid].isWalkEnd<< " was merged: "<< merged[oldToNew[uid].finalWalkId]<< endl;
        uidSequence << finalUnitigSerial <<" "<< bcalmid << endl;
        finalUnitigSerial++;
    }
    uidSequence.close();


    //system();

    //keep the sequences only
    system(("awk '!(NR%2)' "+UNITIG_FILE+" > seq.usttemp").c_str());
    system("sort -n -k 2 -o uidSeq.usttemp uidSeq.usttemp");
    system("paste -d' ' uidSeq.usttemp seq.usttemp > merged.usttemp ");
    system("mkdir -p tmpUSTdir");
    system("sort -T tmpUSTdir/ -n -k 1 -o merged.usttemp merged.usttemp");

    system("cat  merged.usttemp  | cut -d' ' -f3 >  seq.usttemp");


    //string walkString = "";
    ofstream tipFile("ust_ess_tip.txt");


    ifstream sequenceStringFile ("seq.usttemp");

    //for(int si = 0; si<sorter.size(); si++){
        int startWalkIndex = 0;
    int prevwalk = -1;
        while(true){
            assert(startWalkIndex<sorter.size());
            MyTypes::fourtuple &n = sorter[startWalkIndex];
            int finalWalkId = get<1>(n);

            int uid = get<0>(n);
            int isTip = get<3>(n);
            int pos_in_walk =get<2>(n);
            //cout<<uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;

//            string unitigString;
//            if(nodeSign[uid] == false){
//                unitigString =  reverseComplement(unitigs.at(uid).sequence);
//            }else{
//                unitigString =  (unitigs.at(uid).sequence);
//            }


//
            string unitigString;
            string sequenceFromFile = "";//getline
            getline (sequenceStringFile,sequenceFromFile);
            if(nodeSign[uid] == false){
                unitigString =  reverseComplement(sequenceFromFile);
            }else{
                unitigString =  sequenceFromFile;
            }



            if(prevwalk!=finalWalkId){
                tipFile<<">\n";
            }
            prevwalk = finalWalkId;

//            if( MODE_ABSORPTION_TIP && isTip == 0){//
//                walkString = plus_strings(walkString, unitigString, K);
//
            if(isTip == 0 ){

                if(startWalkIndex==0)
                    tipFile<<unitigString;
                else if(finalWalkId != get<1>(sorter[startWalkIndex-1]))
                    tipFile<<unitigString;
                else
                    tipFile<<unitigString.substr(K - 1, unitigString.length() - (K - 1));
                    //tipFile<<cutPref(unitigString, K);


            }else if(isTip==1){ //right R   R    ]   ]   ]   ]
                //cut prefix: correct
                if(0==0){
                    unitigString = unitigString.substr(K - 1, unitigString.length() - (K - 1));
//                    if(walkString.length()<K){
//                        cout<<"pos: "<<walkString.length()<<endl;
//                    }
                    tipFile<<"(";
                    tipFile<<unitigString;
                    tipFile<<")";
                }
                if(1==0){
                    tipFile<<">pref\n"<<unitigString<<endl;
                }

            }else if(isTip==2 && MODE_ABSORPTION_TIP){ //left L   L    [ [ [
                //cut suffix: correct
                if(0==0){
                    unitigString = unitigString.substr(0, unitigString.length() - (K - 1));
//                    if(walkString.length()<K){
//                        cout<<"pos: "<<walkString.length()<<endl;
//                    }
                    tipFile<<"{";
                    tipFile<<unitigString;
                    tipFile<<"}";
                }
                if(1==0){
                    tipFile<<">suf\n"<<unitigString<<endl;
                }
            }

            if(startWalkIndex+1 == sorter.size()) {
//                int brackets1 = std::count(walkString.begin(), walkString.end(), '(');
//                int brackets2 = std::count(walkString.begin(), walkString.end(), ')');
//                int stringPlus = std::count(walkString.begin(), walkString.end(), '{');
//                int stringMinus = std::count(walkString.begin(), walkString.end(), '}');
//
//                C_tip_special += brackets1- brackets2 -stringPlus-stringMinus;
//                C_tip_ustitch += walkString.length();
//
                //tipFile<<">\n"<<walkString<<endl;
                tipFile<<endl;
                V_tip_ustitch++;
                break;
            }else if(get<1>(sorter[startWalkIndex+1]) != finalWalkId){
                //tipFile<<">\n"<<walkString<<endl;
                tipFile<<endl;

                V_tip_ustitch++;
                //walkString = "";
                //walkString.shrink_to_fit();
                //break;
            }
            startWalkIndex++;
        }
    //}

    sequenceStringFile.close();
    tipFile.close();
}
void setmemlimit()
{
        struct rlimit memlimit;
        long bytes;

//        if(MEM_ENV_VAR!=NULL)
//        {
                bytes = (60.0)*(1024*1024*1024);
                memlimit.rlim_cur = bytes;
                memlimit.rlim_max = bytes;
                setrlimit(RLIMIT_AS, &memlimit);
//        }
}

class GroupMerger {
public:
    map<int, bool> fwdVisited;
    map<int, bool> bwdVisited;
    map<int, int> bwdWalkId;
    map<int, int> fwdWalkId;
    GroupMerger() {
    }
    void connectGroups(int from, int to){
        fwdVisited[from] = false;
        bwdVisited[to] = false;
        fwdWalkId[from] = to;
        bwdWalkId[to] = from;
    }
    ~GroupMerger() {
    }
};

class DisjointSet {
    //unordered_map<int, int> parent;
    unordered_map<int, int> parent;
public:
    DisjointSet() {
    }
    void make_set(int id) {
        this->parent[id] = -1;
    }
    
    void Union(int xId, int yId) {
        int xset = find_set(xId);
        int yset = find_set(yId);
        
        if(xset != yset)
        {
            parent[xset] = yset;
        }
    }
    
    int find_set(int id) {
        if (parent[id] == -1)
            return id;
        return find_set(parent[id]);
    }
    ~DisjointSet(){
    }
    
};


class Graph {
public:
    size_t V = adjList.size();
    int time = 0;
    
    int* p_dfs;
    bool* saturated;
    struct node_sorter * sortStruct;
    bool* countedForLowerBound;
    DisjointSet disSet;
    GroupMerger gmerge;
    
    Graph() {
        color = new char[V];
        p_dfs = new int[V];
        nodeSign = new bool[V];
        oldToNew = new new_node_info_t[V];
        saturated = new bool[V];
        sortStruct = new struct node_sorter[V];
        global_indegree = new int[V];
        global_outdegree = new int[V];
        global_plusindegree = new int[V];
        global_plusoutdegree = new int[V];
        global_issinksource = new int[V];
        global_priority = new int[V];
        countedForLowerBound = new bool[V];
        
        for (int i = 0; i < V; i++) {
            
            if(MODE_WALK_UNION || MODE_ABSORPTION_NOTIP  || MODE_ABSORPTION_TIP){
                disSet.make_set(i);
            }
            
            oldToNew[i].serial = -1;
            saturated[i] = false;
            sortStruct[i].sortkey = 0;
            sortStruct[i].node = i;
            global_indegree[i] = 0;
            global_outdegree[i] = 0;
            global_plusindegree[i] = 0;
            global_plusoutdegree[i] = 0;
            global_issinksource[i] = 0;
            global_priority[i] = 0;
            countedForLowerBound[i] = false;
        }
    }
    
    inline bool sRight(edge_t plusminusedge){
        return !(plusminusedge.right == true);
    }
    
    inline bool sLeft(edge_t plusminusedge){
        return (plusminusedge.left == true);
    }
    
    void indegreePopulate(){
        int xc = 0;
        for(auto elist: adjList){
            for(edge_t e: elist){
                global_indegree[e.toNode] += 1;
                sortStruct[e.toNode].sortkey = sortStruct[e.toNode].sortkey + 1;
                if(e.right == true){
                    global_plusindegree[e.toNode] += 1;
                }
                if(e.left == true){
                    global_plusoutdegree[xc] += 1;
                }
                
            }
            global_outdegree[xc] = elist.size();
            xc++;
        }
        
        for(int i = 0; i<5; i++){
            for(int j = 0; j<5; j++){
                inOutCombo[make_pair(i,j)] = 0;
            }
        }
        for(int i = 0; i<V; i++){
            pair<int, int> a;
            a = make_pair(global_plusindegree[i], global_plusoutdegree[i]);
            inOutCombo[a] = (inOutCombo.count(a)  ? inOutCombo[a] + 1 : 1  );
            
            
            if(DBGFLAG == SINKSOURCE){
                cout<<i<<"is ";
            }
            
            if(global_plusoutdegree[i] == 0 && global_plusindegree[i] != 0){
                sink_count++;
                global_issinksource[i] = 1;
                global_priority[i] = 5;
                countedForLowerBound[i] = true;
                
                if(DBGFLAG == SINKSOURCE){
                    cout<<"sink, ";
                }
                
            }
            if(global_plusindegree[i] == 0 && global_plusoutdegree[i] != 0){
                source_count++;
                global_issinksource[i] = 1;
                global_priority[i] = 5;
                countedForLowerBound[i] = true;
                
                if(DBGFLAG == SINKSOURCE){
                    cout<<"source, ";
                }
            }
            
            if(global_indegree[i] == 0){
                global_issinksource[i] = 1;
                isolated_node_count++;
                
                if(DBGFLAG == SINKSOURCE){
                    cout<<"isolated, ";
                }
            }
            if(global_indegree[i] == 1){
                onecount++;
            }
            
            if(DBGFLAG == SINKSOURCE){
                cout<<endl;
            }
            
        }
        
        xc = 0; // current vertex while traversing the adjacency list
        for(auto elist: adjList){
            int neighborCount = 0;
            int spNeighborCount[2];
            spNeighborCount[0]=0;
            spNeighborCount[1]=0;
            stack<int> countedNodes;
            set<pair<int, bool> > countedSides;
            //if(true){
            if(FLG_NEWUB == true){
                //ENDPOINT SIDE UPPER BOUND - improved
                    for(edge_t e_xy: elist){    //central node: all neighbors of x
                        int y = e_xy.toNode;
                        auto adjY = adjList[y];
                        bool eligibleSp = true;

                        //pair<int, bool> pairr;
                        for(edge_t e_from_y : adjY){    // check if this neighbor is speacial
                            //pairr =make_pair(e_from_y.toNode, sRight(e_xy) );
                            if(e_from_y.toNode!=xc){

                                if(sRight(e_xy) == sLeft(e_from_y)){
                                    eligibleSp = false;
                                    break;
                                }
                            }
                        }

                        if(eligibleSp){
                            spNeighborCount[sLeft(e_xy)]++;
                        }
                    }
                    if(spNeighborCount[0]>1){
                        sharedparent_count += spNeighborCount[0] - 1 ;
                    }
                    if(spNeighborCount[1]>1){
                        sharedparent_count += spNeighborCount[1] - 1 ;
                    }
            }
            if(FLG_NEWUB == false){
                //ENDPOINT SIDE UPPER BOUND
                for(edge_t e_xy: elist){
                    int y = e_xy.toNode;
                    auto adjY = adjList[y];
                    bool eligible = true;
                    pair<int, bool> pairr;
                    for(edge_t e_from_y : adjY){
                        pairr =make_pair(e_from_y.toNode, sRight(e_xy) );
                        if(e_from_y.toNode!=xc){

                            if(sRight(e_xy) == sLeft(e_from_y)){
                                eligible = false;
                                break;
                            }

                        }

                    }

                    if(eligible){
                        neighborCount++;
                    }
                }

                if(global_issinksource[xc] == 1){
                    if(neighborCount>1){
                        sharedparent_count += neighborCount - 1 ;
                    }
                }else{
                    if(neighborCount>2){
                        sharedparent_count += neighborCount - 2 ;
                    }
                }
            }
            //sharedparent_count_wrong =sharedparent_count;

            xc++;
        }
        
        //check if not ALGOMODE == INDEGREE_DFS_INVERTED
        delete [] global_indegree;
        delete [] global_outdegree;
        delete [] global_plusindegree;
        delete [] global_plusoutdegree;
    }
    
    
    void DFS_visit(int u) {
        if(MODE_ABSORPTION_TIP || BOTHTIPABSORB || BOTHTIPABSORB_V2){
            if(global_issinksource[u]==1){
                auto adju = adjList.at(u);
                vector<edge_t> myvector;
                for (edge_t e : adju) {
                    myvector.push_back(e);
                }
                sinkSrcEdges[u] = myvector;
                return;
            }
        }
        
        stack<edge_t> s;
        edge_t uEdge;
        uEdge.toNode = u;
        s.push(uEdge);
        
        while (!s.empty()) {
            edge_t xEdge = s.top();
            
            int x = xEdge.toNode;
            s.pop();
            
            if (color[x] == 'w') {
                //Original DFS code
                time = time + 1;
                color[x] = 'g';
                s.push(xEdge);
                auto adjx = adjList.at(x);
                if(ALGOMODE == RANDOM_DFS || 1==1){
                    random_shuffle ( adjx.begin(), adjx.end(), myrandom);
                }
                
                if(ALGOMODE == EPPRIOR){
                    sort( adjx.begin( ), adjx.end( ), [ ]( const edge_t& lhs, const edge_t& rhs )
                         {
                             return global_priority[lhs.toNode]   <  global_priority[rhs.toNode]  ;
                         });
                }
                
                if(ALGOMODE == INDEGREE_DFS){
                    sort( adjx.begin( ), adjx.end( ), [ ]( const edge_t& lhs, const edge_t& rhs )
                         {
                             return global_indegree[lhs.toNode] < global_indegree[rhs.toNode];
                         });
                }
                
                if(ALGOMODE == PLUS_INDEGREE_DFS){
                    sort( adjx.begin( ), adjx.end( ), [ ]( const edge_t& lhs, const edge_t& rhs )
                         {
                             return global_indegree[lhs.toNode]  - global_plusindegree[lhs.toNode] > global_indegree[lhs.toNode]  - global_plusindegree[rhs.toNode];
                         });
                }
                
                if(ALGOMODE == INDEGREE_DFS_INVERTED){
                    sort( adjx.begin( ), adjx.end( ), [ ]( const edge_t& lhs, const edge_t& rhs )
                         {
                             return global_indegree[lhs.toNode] > global_indegree[rhs.toNode];
                         });
                }
                if (ALGOMODE == OUTDEGREE_DFS){
                    if(p_dfs[x] == -1){
                        sort( adjx.begin( ), adjx.end( ), [ ]( const edge_t& lhs, const edge_t& rhs )
                             {
                                 return global_outdegree[lhs.toNode] < global_outdegree[rhs.toNode];
                             });
                    }else if (nodeSign[x] == false){
                        sort( adjx.begin( ), adjx.end( ), [ ]( const edge_t& lhs, const edge_t& rhs )
                             {
                                 return global_outdegree[lhs.toNode] - global_plusoutdegree[lhs.toNode] < global_outdegree[rhs.toNode] - global_plusoutdegree[rhs.toNode];
                             });
                    }else if (nodeSign[x] == true){
                        sort( adjx.begin( ), adjx.end( ), [ ]( const edge_t& lhs, const edge_t& rhs )
                             {
                                 return global_plusoutdegree[lhs.toNode] < global_plusoutdegree[rhs.toNode];
                             });
                    }
                }
                
                
                // Now our branching code ::
                // For a white x
                // Consider 2 case:
                // Case 1. p[x] = -1, it can happen in two way, x is the first one ever in this connected component, or no one wanted to take x
                // either way, if p[x] = -1, i can be representative of a new node in new graph
                // Case 2. p[x] != -1, so x won't be the representative/head of a newHome. x just gets added to its parent's newHome.
                int u = unitigs.at(x).ln; //unitig length
                
                if (p_dfs[x] == -1) {
                    
                    list<int> xxx;
                    xxx.push_back(x);
                    newToOld.push_back(xxx);
                    oldToNew[x].serial = countNewNode++; // countNewNode starts at 0, then keeps increasing
                    oldToNew[x].finalWalkId = oldToNew[x].serial;
                    
                    
                    //added while doing bracket comp
                    walkFirstNode.push_back(x);
                    
                    
                    //make the sequence
                    //NOT CORRECT? I am not sure
                    if(nodeSign[x]==false){
                        //newSequences[oldToNew[x].serial] = reverseComplement(unitigs.at(x).sequence);
                        
                        
                        //newNewSequences[x] = reverseComplement(unitigs.at(x).sequence);
                    }else{
                        //newSequences[oldToNew[x].serial] = (unitigs.at(x).sequence);
                        
                        //newNewSequences[x] = (unitigs.at(x).sequence);
                    }
                    
                    oldToNew[x].pos_in_walk = 1;
                    //oldToNew[x].startPosWithKOverlap = 1;
                    if (u < K) {
                        //oldToNew[x].endPosWithKOVerlap = 1; // do we actually see this? yes
                        if(DBGFLAG == UKDEBUG){
                            cout<< "node: "<< x<<"u< k ***** u = "<<u<<endl;
                        }
                    } else {
                        //oldToNew[x].endPosWithKOVerlap = u - K + 1;
                    }
                    
                } else {
                    
                    newToOld[oldToNew[p_dfs[x]].serial].push_back(x);
                    oldToNew[x].serial = oldToNew[p_dfs[x]].serial;
                    oldToNew[x].finalWalkId = oldToNew[x].serial;
                    
                    
                    if(MODE_WALK_UNION || MODE_ABSORPTION_TIP || MODE_ABSORPTION_NOTIP ){
                        disSet.Union(x, p_dfs[x]);
                    }
                    
                    
                    //oldToNew[x].startPosWithKOverlap = oldToNew[p_dfs[x]].endPosWithKOVerlap + 1;
                    oldToNew[x].pos_in_walk = oldToNew[p_dfs[x]].pos_in_walk + 1;
                    if (u < K) {
                        //oldToNew[x].endPosWithKOVerlap = oldToNew[x].startPosWithKOverlap + 1; // do we actually see this? yes
                        if(DBGFLAG == UKDEBUG){
                            cout<< "node: "<< x<<"u< k ***** u = "<<u<<endl;
                        }
                    } else {
                        //oldToNew[x].endPosWithKOVerlap = u - K + (oldToNew[x].startPosWithKOverlap); //check correctness
                    }
                    
                    // x says: Now that I know where my newHome is: I can extend my parent's sequence
                    // Is it more complicated than this?
                    //string parentSeq = newSequences[oldToNew[x].serial];
                    //string childSeq = unitigs.at(x).sequence;
                    
                    // Is it CORRECT? just for testing now
                    if(nodeSign[x]==false){
                        //childSeq = reverseComplement(childSeq);
                    }
                    //newSequences[oldToNew[x].serial] = plus_strings(parentSeq, childSeq, K);
                    //newNewSequences[x] = newSequences[oldToNew[x].serial] ;
                }
                
                // x->y is the edge, x is the parent we are extending
                for (edge_t yEdge : adjx) { //edge_t yEdge = adjx.at(i);
                    int y = yEdge.toNode;
                    
                    if(MODE_ABSORPTION_TIP || BOTHTIPABSORB || BOTHTIPABSORB_V2){
                        if(global_issinksource[y] == true){
                            continue;
                        }
                    }
                    
                    // cout << "Edge " << x << "->" << y << " "<<global_indegree[y] << endl;
                    if (DBGFLAG == DFSDEBUGG) {
                        cout << "Edge " << x << "->" << y << endl;
                    }
                    
                    //Normal DFS
                    if (color[y] == 'w') {
                        s.push(yEdge);
                    }
                    
                    if(DBGFLAG == PARTICULAR){
                        // DEBUGGGING a particular edge
                        if (y == 2 && x == 0) {
                            cout << "Saturated? " << saturated[x] << endl;
                        }
                    }
                    
                    //handle self-loop, self-loop will always be an extra edge
                    if (y == x) {
                        edge_both_t e;
                        e.edge = yEdge;
                        e.fromNode = x;
                    } else if (saturated[x]) {
                        // Since x is saturated, we only add resolveLater edges
                        // no need to check for consistency
                        if (y != p_dfs[x]) {
                            edge_both_t e;
                            e.edge = yEdge;
                            e.fromNode = x;
                        }
                    } else {
                        // If x has space to take a child, meaning x is not saturated
                        // hunting for potential child
                        
                        if (color[y] == 'w' && p_dfs[y] == -1) {
                            // y has white color & got no parent => means it's homeless, so let's see if we can take it as a child of x
                            //But just see if it is eligible to be a child, i.e. is it consistent (sign check)?
                            
                            //2 case, Does x's child have grandparent?
                            // No.
                            if (p_dfs[x] == -1 && ALGOMODE != NODEASSIGN) {
                                // case 1: child has no grandparent
                                // so extend path without checking any sign
                                
                                nodeSign[x] = yEdge.left;
                                nodeSign[y] = yEdge.right;
                                p_dfs[y] = x;
                                saturated[x] = true; //found a child
                                
                                
                                //TESTED NOT YET
                                //                                if (nodeSign[y] == false) {
                                //                                    unitigs.at(y).sequence = reverseComplement(unitigs.at(y).sequence);
                                //                                }
                                
                                //Yes.
                            } else if (nodeSign[x] == yEdge.left) {
                                // case 2: child (=y) has grandparent, i.e. x's parent exists
                                nodeSign[y] = yEdge.right;
                                p_dfs[y] = x;
                                saturated[x] = true; //found a child
                                
                                //TESTED NOT YET
                                //                                if (nodeSign[y] == false) {
                                //                                    unitigs.at(y).sequence = reverseComplement(unitigs.at(y).sequence);
                                //                                }
                                
                            } else {
                                // do we reach this case?
                                edge_both_t e;
                                e.edge = yEdge;
                                e.fromNode = x;
                            }
                            
                        } else {
                            
                            //merger
                            if(MODE_WALK_UNION || MODE_ABSORPTION_NOTIP || MODE_ABSORPTION_TIP){
                                // y is not white
                                bool consistentEdge = (nodeSign[y] == yEdge.right && (p_dfs[x]==-1 || (p_dfs[x]!=-1&& nodeSign[x] == yEdge.left)) );
                                if(p_dfs[y]==-1 && consistentEdge && oldToNew[x].serial != oldToNew[y].serial){
                                    
                                    //cout<<"x: "<<x<<":" <<disSet.find_set(x)<<" ";
                                    //cout<<"y: "<<y<<":" <<disSet.find_set(y) <<endl;
                                    
                                    //not in same group already, prevent cycle
                                    if(disSet.find_set(x)!=disSet.find_set(y)){
                                        nodeSign[x] = yEdge.left;
                                        nodeSign[y] = yEdge.right;
                                        p_dfs[y] = x;
                                        saturated[x] = true; //found a child
                                        // oldToNew[y].serial
                                        
                                        disSet.Union(x, y);
                                        //cout<<"x: "<<disSet.find_set(x);
                                        //cout<<"y: "<<disSet.find_set(y);
                                        //cout<<endl;
                                        //cout<<oldToNew[x].serial<<"+"<<oldToNew[y].serial<<endl;
                                        gmerge.connectGroups(oldToNew[x].serial,oldToNew[y].serial );
                                        
                                    }
                                }
                            }
                            
                            if (y != p_dfs[x]) {
                                edge_both_t e;
                                e.edge = yEdge;
                                e.fromNode = x;
                            }
                        }
                    }
                }
            } else if (color[x] == 'g') {
                time = time + 1;
                color[x] = 'b';
            }
        }
    }
    
    
    
    void DFS() {
        
        for (int i=0; i<V; i++) {
            nodeSign[i] = true;
        }
        if(ALGOMODE == NODEASSIGN){
            for (int i=0; i<V; i++) {
                nodeSign[i] = true;
                if(global_plusindegree[i]< global_indegree[i] - global_plusindegree[i]){
                    nodeSign[i] = true;
                }
            }
        }
        
        if(ALGOMODE == SOURCEFIRST){
            for (int i = 0; i < V; i++) {
                sortStruct[i].node = i;
                sortStruct[i].sortkey = global_issinksource[i];
            }
            vector<struct node_sorter> myvector (sortStruct, sortStruct+V);
            sort (myvector.begin(), myvector.end(), sort_by_key_inverted);
            copy(myvector.begin(), myvector.end(), sortStruct);
        }
        
        if(ALGOMODE == EPPRIOR){
            for (int i = 0; i < V; i++) {
                sortStruct[i].node = i;
                sortStruct[i].sortkey = global_priority[i];
            }
            vector<struct node_sorter> myvector (sortStruct, sortStruct+V);
            sort (myvector.begin(), myvector.end(), sort_by_key_inverted);
            copy(myvector.begin(), myvector.end(), sortStruct);
        }
        
        if(ALGOMODE == INDEGREE_DFS_INVERTED){
            for (int i = 0; i < V; i++) {
                sortStruct[i].node = i;
                sortStruct[i].sortkey = global_indegree[i];
            }
            vector<struct node_sorter> myvector (sortStruct, sortStruct+V);
            sort (myvector.begin(), myvector.end(), sort_by_key_inverted);
            //random_shuffle ( myvector.begin(), myvector.end() );
            copy(myvector.begin(), myvector.end(), sortStruct);
            
        }
        
        if (ALGOMODE == INDEGREE_DFS || ALGOMODE == INDEGREE_DFS_1 ){
            for (int i = 0; i < V; i++) {
                sortStruct[i].node = i;
                sortStruct[i].sortkey = global_indegree[i];
            }
            vector<struct node_sorter> myvector (sortStruct, sortStruct+V);
            sort (myvector.begin(), myvector.end(), sort_by_key);
            copy(myvector.begin(), myvector.end(), sortStruct);
            
            if(DBGFLAG == INDEGREEPRINT){
                cout<<"print in degrees"<<endl;
                for(int i = 0; i<V; i++){
                    cout<<sortStruct[i].node<<"->"<<sortStruct[i].sortkey<<endl;
                }
            }
        }
        
        
        if(ALGOMODE == RANDOM_DFS || 1==1){
             for (int i = 0; i < V; i++) {
                 sortStruct[i].node = i;
                 sortStruct[i].sortkey = i;
             }
            vector<struct node_sorter> myvector (sortStruct, sortStruct+V);
            sort (myvector.begin(), myvector.end(), sort_by_key);
            random_shuffle ( myvector.begin(), myvector.end(), myrandom );
            copy(myvector.begin(), myvector.end(), sortStruct);
            
        }
        
        
        if (ALGOMODE == OUTDEGREE_DFS || ALGOMODE == OUTDEGREE_DFS_1){
            for (int i = 0; i < V; i++) {
                sortStruct[i].node = i;
                sortStruct[i].sortkey = global_outdegree[i];
            }
            vector<struct node_sorter> myvector (sortStruct, sortStruct+V);
            sort (myvector.begin(), myvector.end(), sort_by_key);
            copy(myvector.begin(), myvector.end(), sortStruct);
        }
        
        double time_a = readTimer();
        for (int i = 0; i < V; i++) {
            color[i] = 'w';
            p_dfs[i] = -1;
        }
        cout<<"[3.1.0] Basic V loop time: "<<readTimer() - time_a<<" sec"<<endl;
        
        
        time_a = readTimer();
        for (int j = 0; j < V; j++) {
            int i;
            if(ALGOMODE == OUTDEGREE_DFS || ALGOMODE == OUTDEGREE_DFS_1 || ALGOMODE == INDEGREE_DFS || ALGOMODE == INDEGREE_DFS_1 || ALGOMODE == SOURCEFIRST || 1==1){
                i = sortStruct[j].node;
            }else{
                i = j;
            }
            
            
            
            if (color[i] == 'w') {
                if(DBGFLAG == DFSDEBUGG ){
                    cout<<"visit start of node: "<<i<<endl;
                }
                DFS_visit(i);
            }
        }
        cout<<"[3.1.1] DFS and node assignment time: "<<readTimer() - time_a<<" sec"<<endl;
        
        
        
        
        cout<<"[3.1.2] Stitching strings from DFS output... \n";
        time_a = readTimer();
        
        //fix sequences
//        for(int i = 0; i< countNewNode; i++){
//            string s = "";
//            for(int x: newToOld[i]){
//                if(color[x] != 'l' && color[x] != 'r' ){ // useless in normal code
//                    if(nodeSign[x] == false){
//                        s = plus_strings(s, reverseComplement(unitigs.at(x).sequence), K);
//                    }else{
//                        s = plus_strings(s, (unitigs.at(x).sequence), K);
//                    }
//                }
//            }
//
//            newSequences[i] = s;
//
//            for(int x: newToOld[i]){
//                newNewSequences[x] = s;
//            }
//
//            C_ustitch += s.length();
//        }
        cout<<"[3.1.2] TIME to stitch: "<<readTimer() - time_a<<" sec."<<endl;
        ofstream globalStatFile("global_stat", std::fstream::out | std::fstream::app);
        globalStatFile << "TIME_STITCH_SEC" <<  "=" << readTimer() - time_a << endl;
        globalStatFile.close();
        
        
        cout<<"[3.2] Doing union...."<<endl;
        time_a = readTimer();
        bool* merged = new bool[countNewNode];
        //obsoleteWalkId = new bool[countNewNode];
        for (int i = 0; i<countNewNode; i++) {
            merged[i] = false;
            //obsoleteWalkId[i] = false;
            obsoleteWalkId.push_back(false);
        }
        
        
        /***MERGE START***/
        if(MODE_WALK_UNION || MODE_ABSORPTION_NOTIP || MODE_ABSORPTION_TIP){
            //ofstream betterfile;
//            betterfile.open("stitchedUnitigs"+modename[ALGOMODE]+".fa");
//
//            ofstream betterfilePlain;
//            betterfilePlain.open("plainOutput"+modename[ALGOMODE]+".txt");
//
            
            for ( const auto& p: gmerge.fwdWalkId)
            {
                if(gmerge.fwdVisited[p.first] == false){
                    
                    int fromnode =p.first;
                    int tonode = p.second;
                    deque<int> lst;
                    
                    lst.push_back(fromnode);
                    lst.push_back(tonode);
                    
                    gmerge.fwdVisited[fromnode] = true;
                    gmerge.bwdVisited[tonode] = true;
                    
                    
                    if(gmerge.fwdVisited.count(tonode)>0){
                        while(gmerge.fwdVisited[tonode] == false){
                            gmerge.fwdVisited[tonode] = true;
                            tonode = gmerge.fwdWalkId[tonode];
                            gmerge.bwdVisited[tonode] = true;
                            
                            lst.push_back(tonode);
                            if(gmerge.fwdVisited.count(tonode)==0)
                                break;
                        }
                    }
                    if(gmerge.bwdVisited.count(fromnode)>0){
                        while(gmerge.bwdVisited[fromnode] == false){
                            gmerge.bwdVisited[fromnode] = true;
                            fromnode = gmerge.bwdWalkId[fromnode];
                            gmerge.fwdVisited[fromnode] = true;
                            
                            lst.push_front(fromnode);
                            if(gmerge.bwdVisited.count(fromnode)==0)
                                break;
                        }
                    }
                    
                    string mergeString = "";
                    
                    int headOfThisWalk = walkFirstNode[lst.at(0)]; //CHECK AGAIN
                    assert(!lst.empty());
                    int commonWalkId = lst.at(0);
                    
                    int posOffset = 1;
                    
                    int lastWalk = -1;
                    for(auto i: lst){
                        // i is new walk id before merging
                        merged[i] = true;
                        
                        //POTENTIAL BUG
                        if(i!=commonWalkId){
                            obsoleteWalkId[i] = true;
                        }
                            
                            
                        //mergeString = plus_strings(mergeString, newSequences[i], K);
                        walkFirstNode[i] = headOfThisWalk;
                        
                        // travesing the walk list of walk ID i
                        for(int uid: newToOld[i]){
                            oldToNew[uid].serial = commonWalkId;
                            oldToNew[uid].finalWalkId = commonWalkId;
                            oldToNew[uid].pos_in_walk = posOffset++;
                        }
                    }
                    oldToNew[newToOld[lst.back()].back()].isWalkEnd = true;
                    
                    
                    //newNewSequences[headOfThisWalk] = mergeString;
                    
                    
                    //cout<<endl;
                    V_twoway_ustitch ++;
                    C_twoway_ustitch+=mergeString.length();
                    //betterfile << '>' << commonWalkId <<" LN:i:"<<mergeString.length()<<" ";
//                    betterfile << '>' ;
//                    betterfile<<endl;
//
//                    betterfile<<mergeString;
//                    betterfilePlain<<mergeString;
//
//                    betterfile<<endl;
//                    betterfilePlain<<endl;
                    
                    
                }
            }
            for (int newNodeNum = 0; newNodeNum<countNewNode; newNodeNum++){
                
                
                if(merged[newNodeNum] == false){
                    oldToNew[newToOld[newNodeNum].back()].isWalkEnd = true;
                    
//                    betterfile<<'>';
//                    //betterfile << '>' << newNodeNum <<" LN:i:"<<newSequences[newNodeNum].length()<<" ";
//                    betterfile<<endl;
                    
                    //betterfile<<newSequences[newNodeNum];
                    //betterfilePlain<<newSequences[newNodeNum];
                    
//                    betterfile<<endl;
//                    betterfilePlain<<endl;
                    
                    //C_twoway_ustitch+=newSequences[newNodeNum].length();
                    V_twoway_ustitch++;
                }
            }
            //betterfile.close();
        }
        cout<<"[3.2] UST union done. TIME: "<<readTimer()-time_a<<"sec.\n";
        
         cout<<"N. walk: "<<V_twoway_ustitch<<endl;
        
        //BRACKETCOMP encoder and printer::::
        //        if(0==0){
        //            vector<mytuple> sorter;
        //            for(int uid = 0 ; uid< V; uid++){
        //
        //                new_node_info_t nd = oldToNew[uid];
        //
        //
        //                //if(!global_issinksource[uid]){
        //                sorter.push_back(make_tuple(uid, nd.finalWalkId, nd.pos_in_walk, nd.isTip));
        //                //}
        //
        //            }
        //            stable_sort(sorter.begin(),sorter.end(),sort_by_tipstatus);
        //            stable_sort(sorter.begin(),sorter.end(),sort_by_pos);
        //            stable_sort(sorter.begin(),sorter.end(),sort_by_walkId);
        //
        //            for(mytuple n : sorter){
        //                int uid = get<0>(n);
        //                int finalWalkId = get<1>(n);
        //                int pos_in_walk = get<2>(n);
        //                int isTip = get<3>(n);
        //                cout<<uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<" "<<oldToNew[uid].isWalkEnd<< " was merged: "<< merged[oldToNew[uid].finalWalkId]<< endl;
        //            }
        //        }
        
        
        // clean up
        delete [] merged;
        
        /// TWOWAYEXT DONE: NOW LET"S DO BRACK COMP
        
        
        bool* hasStartTip = new bool[V];
        bool* hasEndTip = new bool[V];
        for (int i = 0; i<V; i++) {
            hasStartTip[i] = false;
            hasEndTip[i] = false;
        }
        //@@@@@ BRACKETED
        
         // @@DBG_BLOCK
        /*
        if(2 == 0){
            for (int sinksrc = 0; sinksrc<V; sinksrc++) {
                if(global_issinksource[sinksrc] == 1){
                    list<int> xxx;
                    xxx.push_back(sinksrc);
                    newToOld.push_back(xxx);
                    oldToNew[sinksrc].serial = countNewNode++;
                    oldToNew[sinksrc].finalWalkId = oldToNew[sinksrc].serial;
                    oldToNew[sinksrc].pos_in_walk = 1;
                    oldToNew[sinksrc].isTip = 0;
                    // error resolved in sept 14
                    color[sinksrc] = 'b';
                }
            }
        }
         */
        
        
        if(MODE_ABSORPTION_TIP || BOTHTIPABSORB_V2){
            if(2==2){
                for (auto const& x : sinkSrcEdges)
                {
                    int sinksrc = x.first;
                    if(sinksrc == 3997){    // @@DBG_BLOCK
                        // || sinksrc == 3997
                    }
                    for(edge_t e: x.second){
                        
                        // when can this occur? it does occur
                        if(color[sinksrc] != 'w'){
                            break;
                        }
                        
                        //there are 3 cases
                        //if consistent this way [[[if(nodeSign[e.toNode] == e.right)]]]
                        //case fwd1: sinksrc -> contig start
                        //case fwd2. sinksrc -> contig middle/end -> ... (say that sinksrc is LEFT)
                        //case fwd3. sinksrc -> sinksrc_other (i'd say ignore this for now)
                        //
                        
                        //case bwd1. contig end -> sinksrc
                        //case bwd2. .... -> contig middle/start -> sinksrc (say that sinksrc is RIGHT)
                        //case bwd3. sinksrc_other -> sinksrc  (i'd say ignore this for now)
                        
                        // 3 fwd cases
                        if(nodeSign[e.toNode] == e.right){  //ensure it is a fwd case
                            if(color[e.toNode]!='w' && color[e.toNode]!='r' && color[e.toNode]!='l'){//  this ensures that this to vertex is NOT sinksrc_other
                                //case 1, 2
                                int whichwalk = oldToNew[e.toNode].finalWalkId;
                                //*** case fwd1 : sinksrc -> contigStart
                                //case fwd2. sinksrc -> contig middle/end -> ... (say that sinksrc is LEFT)
                                //let's merge case fwd1 & fwd2
                                //color[sinksrc] = 'b';
                                
                                nodeSign[sinksrc] = e.left;
                                color[sinksrc] = 'l';
                                oldToNew[sinksrc].serial = whichwalk;
                                oldToNew[sinksrc].finalWalkId = whichwalk;
                                oldToNew[sinksrc].pos_in_walk = oldToNew[e.toNode].pos_in_walk - 1;
                                assert(oldToNew[e.toNode].pos_in_walk != -1);
                                
                                 // @@DBG_BLOCK int k = oldToNew[e.toNode].pos_in_walk;
                                 // @@DBG_BLOCK bool jjjj = hasStartTip[e.toNode];
                                if(oldToNew[e.toNode].pos_in_walk == 1 && hasStartTip[e.toNode] == false ){
                                    oldToNew[sinksrc].isTip = 0;
                                    hasStartTip[e.toNode] = true;
                                    oldToNew[sinksrc].pos_in_walk = oldToNew[e.toNode].pos_in_walk - 1 ;
                                    int j = oldToNew[sinksrc].pos_in_walk;
                                    
                                }else{
                                    oldToNew[sinksrc].isTip = 2;
                                }
                                
                            }
                            
                        }else{
                            // 3 bwd cases
                            
                            if((color[e.toNode]!='w' && color[e.toNode]!='r' && color[e.toNode]!='l')){
                                int whichwalk = oldToNew[e.toNode].finalWalkId;
                                
                                //*** case bwd1: contigend --> sinksrc
                                //*** case bwd2: contigmiddle--> sinksrc
                                
                                
                                nodeSign[sinksrc] = !e.left;
                                //color[sinksrc] = 'b';
                                color[sinksrc] = 'r';
                                oldToNew[sinksrc].serial = whichwalk;
                                oldToNew[sinksrc].finalWalkId = whichwalk;
                                oldToNew[sinksrc].pos_in_walk = oldToNew[e.toNode].pos_in_walk ;
                                assert(oldToNew[e.toNode].pos_in_walk != -1);
                                
                                if(oldToNew[e.toNode].isWalkEnd == true && !hasEndTip[e.toNode] ){
                                    oldToNew[sinksrc].isTip = 0;
                                    hasEndTip[e.toNode] = true;
                                    oldToNew[sinksrc].pos_in_walk = oldToNew[e.toNode].pos_in_walk + 1;
                                }else{
                                    oldToNew[sinksrc].isTip = 1;
                                }                            }
                        }
                    }
                }
            }
            delete [] hasStartTip;
            delete [] hasEndTip;
            // now take care of all the remaining edges
            //            for (auto const& x : sinkSrcEdges)
            //            {
            //                int sinksrc = x.first;
            //                if(color[sinksrc] == 'w'){  //still white, that means it goes isolated now
            //                    list<int> xxx;
            //                    xxx.push_back(sinksrc);
            //                    newToOld.push_back(xxx);
            //                    oldToNew[sinksrc].serial = countNewNode++;
            //                    oldToNew[sinksrc].finalWalkId = oldToNew[sinksrc].serial;
            //                    oldToNew[sinksrc].pos_in_walk = 1;
            //                    oldToNew[sinksrc].isTip = 0;
            //                    // error resolved in sept 14
            //                    color[sinksrc] = 'b';
            //                }
            //            }
            
            for (int sinksrc = 0; sinksrc<V; sinksrc++) {
                bool istipit = (oldToNew[sinksrc].isTip==1 || oldToNew[sinksrc].isTip==2);
                if((global_issinksource[sinksrc] == 1 && color[sinksrc] == 'w') || (BOTHTIPABSORB_V2 && istipit)){
                    list<int> xxx;
                    xxx.push_back(sinksrc);
                    newToOld.push_back(xxx);
                    
                    if(istipit)  {
                        
                        oldToNew[sinksrc].serial = countNewNode++;
                        obsoleteWalkId.push_back(false);
                        oldToNew[sinksrc].finalWalkId = oldToNew[sinksrc].serial;
                        oldToNew[sinksrc].pos_in_walk = 1;
                    }else if(color[sinksrc] == 'r' || color[sinksrc] == 'l'  ){
                        oldToNew[sinksrc].isTip = 0;
                        
                    }else{
                        oldToNew[sinksrc].serial = countNewNode++;
                        obsoleteWalkId.push_back(false);
                        oldToNew[sinksrc].finalWalkId = oldToNew[sinksrc].serial;
                        oldToNew[sinksrc].pos_in_walk = 1;
                        oldToNew[sinksrc].isTip = 0;
                    }
                    // error resolved in sept 14
                    color[sinksrc] = 'b';
                }
            }
            
            
            
            // make the absorb graph
            //BRACKETCOMP encoder and printer::::
            vector<MyTypes::fourtuple> sorter;
            for(int uid = 0 ; uid< V; uid++){
                new_node_info_t nd = oldToNew[uid];
                sorter.push_back(make_tuple(uid, nd.finalWalkId, nd.pos_in_walk, nd.isTip));
            }
            
            
            stable_sort(sorter.begin(),sorter.end(),sort_by_tipstatus);
            stable_sort(sorter.begin(),sorter.end(),sort_by_pos);
            stable_sort(sorter.begin(),sorter.end(),sort_by_walkId);
            //sorter.push_back(make_tuple(0, 9999999999, 0, 0)); //dummy entry for making coding logic easier
            
            
            


            //START absorbGraph
            //if you are doing both end absorption, do it here: adding **
            // by adding entries to **
            // count the number of * divided by two... lets say, cnt=3, then for next cnt walks, cut k-1 prefix, and k-1 suffix, make changes in recursive absorption
            
            ofstream globalStatFile;
            globalStatFile.open("global_stat", std::fstream::out | std::fstream::app);
            
            double memvmKB,memrssKB;
            //process_mem_usage(memvmKB, memrssKB);
            //cout<<"memory (virtual) to do ONLY UST DFS (KB)"<<memvmKB<<endl;
            //cout<<"memory (rss) to do ONLY UST DFS (KB)"<<memrssKB<<endl;
            //globalStatFile << "MEM_VM_ONLY_UST_KB" <<  "=" << memvmKB<< endl;
            //globalStatFile << "MEM_RSS_ONLY_UST_KB" <<  "=" << memrssKB << endl;
            
            double time1 = readTimer();
            
            //
            tipSpell(sorter);
            time1 = readTimer() - time1;
            globalStatFile << "TIME_ABSORB_SEC" <<  "=" << time1 << endl;
            globalStatFile.close();
            // end of absorb graph
        }
        
        if(MODE_ABSORPTION_NOTIP &&!BOTHTIPABSORB_V2 ){
            if(BOTHTIPABSORB || BOTHTIPABSORB){
               for (int sinksrc = 0; sinksrc<adjList.size(); sinksrc++) {
                   //if(global_issinksource[sinksrc] == 1 && color[sinksrc] == 'w' ){
                   if(global_issinksource[sinksrc] == 1 ){
                       list<int> xxx;
                       xxx.push_back(sinksrc);
                       newToOld.push_back(xxx);
                       oldToNew[sinksrc].serial = countNewNode++;
                       oldToNew[sinksrc].finalWalkId = oldToNew[sinksrc].serial;
                       oldToNew[sinksrc].pos_in_walk = 1;
                       oldToNew[sinksrc].isTip = 0;
                       // error resolved in sept 14
                       color[sinksrc] = 'b';
                   }
               }
           }
            
            
            // make the absorb graph
            //BRACKETCOMP encoder and printer::::
            vector<MyTypes::fourtuple> sorter;
            for(int uid = 0 ; uid< V; uid++){
                //if( global_issinksource[uid]==0 || !BOTHTIPABSORB ){
                    new_node_info_t nd = oldToNew[uid];
                                   sorter.push_back(make_tuple(uid, nd.finalWalkId, nd.pos_in_walk, nd.isTip));
                //}
               
            }
            
            //stable_sort(sorter.begin(),sorter.end(),sort_by_tipstatus);
            stable_sort(sorter.begin(),sorter.end(),sort_by_pos);
            stable_sort(sorter.begin(),sorter.end(),sort_by_walkId);
            //sorter.push_back(make_tuple(0, 9999999999, 0, 0)); //dummy entry for making coding logic easier
            
        //     for(auto i: sorter ){
        //     cout<<"walk "<<get<1>(i)<<":";
        //     cout<<"("<<get<2>(i)<<")";
        //      cout<<get<0>(i)<<" ";
        //     cout<<get<3>(i)<<"";
        //     cout<<endl;
        // }
        // cout<<"\n\n";
            bool TESTING_CONNECT=false;
            if(TESTING_CONNECT){
                int maxv = 32; 

                cout<<"HERE I AM, absorb in unitig graph::: "<<maxv<<"\n";
                    for(int x=0; x<sorter.size(); x++){
                        int uid = get<0>(sorter[x]);
                        int old_walk = get<1>(sorter[x]);
                        int old_pos = get<2>(sorter[x]) - 1;
                        int istip = get<3>(sorter[x]);

                        if(old_pos/maxv > 0){
                            //modify its walkid
                             int new_walk;
                             int new_pos = old_pos%maxv + 1;
                             if(new_pos == 1){
                                   new_walk = countNewNode++;
                                   obsoleteWalkId.push_back(false);
                             }else{
                                 new_walk = countNewNode;
                             }
                              
                             oldToNew[uid].finalWalkId = new_walk;
                             oldToNew[uid].pos_in_walk = new_pos;
                             sorter[x] = make_tuple(uid, new_walk, new_pos, istip);
                        }   
                    }
              //      stable_sort(sorter.begin(),sorter.end(),sort_by_pos);
            //stable_sort(sorter.begin(),sorter.end(),sort_by_walkId);
            }
        // for(auto i: sorter ){
        //     cout<<"walk "<<get<1>(i)<<":";
        //     cout<<"("<<get<2>(i)<<")";
        //      cout<<get<0>(i)<<" ";
        //     cout<<get<3>(i)<<"";
        //     cout<<endl;
        // }
            //START absorbGraph
            //if you are doing both end absorption, do it here: adding **
            // by adding entries to **
            // count the number of * divided by two... lets say, cnt=3, then for next cnt walks, cut k-1 prefix, and k-1 suffix, make changes in recursive absorption
            
            
            ofstream globalStatFile;
            globalStatFile.open("global_stat", std::fstream::out | std::fstream::app);
            //double memvmKB,memrssKB;
            //process_mem_usage(memvmKB, memrssKB);
            //cout<<"memory (virtual) to do ONLY UST DFS (KB)"<<memvmKB<<endl;
            //cout<<"memory (rss) to do ONLY UST DFS (KB)"<<memrssKB<<endl;
            //globalStatFile << "MEM_VM_ONLY_UST_KB" <<  "=" << memvmKB<< endl;
            //globalStatFile << "MEM_RSS_ONLY_UST_KB" <<  "=" << memrssKB << endl;

            double time1 = readTimer();
            if(ALGOMODE!=15)
                absorptionManager(sorter);
            time1 = readTimer() - time1;
            globalStatFile << "TIME_ABSORB_SEC" <<  "=" << time1 << endl;
            globalStatFile.close();
            
            // end of absorb graph
        }
        
        delete []  global_issinksource;
        delete []  global_priority;
        freeDFS();
    }
    
    inline void freeDFS(){
        delete [] saturated;
        delete [] p_dfs;
    }
    
    ~Graph() {
        delete [] color;
        delete [] nodeSign;
        delete [] oldToNew;
        
        delete [] sortStruct;
        delete [] countedForLowerBound;
        
        //delete [] obsoleteWalkId;
    }
};


void printNewGraph(Graph &G){
    list<int> *newToOld = new list<int>[countNewNode];
    
    for (int i = 0; i < G.V; i++) {
        newToOld[oldToNew[i].serial].push_back(i);
        cout << "old " << i << "-> new" << oldToNew[i].serial << endl;
    }
    for (int i = 0; i < countNewNode; i++) {
        list<int> adj = newToOld[i];
        
        cout<<"new " << i<<": old (index) ";
        for(int val : adj){
            cout<<val<<" ";
        }
        cout<<endl;
    }
    delete [] newToOld;
}


void formattedOutputForwardExt(Graph &G){
    string plainOutput = "plainOutput"+modename[ALGOMODE]+".txt";
    ofstream plainfile;
    plainfile.open(plainOutput);
    
    string stitchedUnitigs = "stitchedUnitigs"+modename[ALGOMODE]+".fa";
    ofstream myfile;
    myfile.open (stitchedUnitigs);
    //>0 LN:i:13 KC:i:12 km:f:1.3  L:-:0:- L:-:2:-  L:+:0:+ L:+:1:-
    for (int newNodeNum = 0; newNodeNum<countNewNode; newNodeNum++){
        //myfile << '>' << newNodeNum <<" LN:i:"<<newSequences[newNodeNum].length()<<" ";
        myfile<<endl;
        
        //plainfile<<newSequences[newNodeNum];
        //myfile<<newSequences[newNodeNum];
        
        plainfile<<endl;
        myfile<<endl;
    }
    //myfile << '>' << newNodeNum <<">0 LN:i:13 KC:i:12 km:f:1.3  L:-:0:- L:-:2:-  L:+:0:+ L:+:1:- " ;
    myfile.close();
    plainfile.close();
}




int main(int argc, char** argv) {
    //setmemlimit();
    //unsigned int SEED =unsigned ( std::time(0) );
    unsigned int SEED = 1;
    std::srand (  SEED );
    //processEncodedFile();
    //return 0;
    
    FILE * statFile;
    statFile = fopen (("stats"+modename[ALGOMODE]+".txt").c_str(),"w");
    
    ofstream globalStatFile;
    globalStatFile.open("global_stat", std::fstream::out | std::fstream::app);
    //globalStatFile.open("global_stat", std::fstream::out);

    const char* nvalue = "" ;
    int c ;
    
    ///*
    bool TIP_MODE_BY_USER = true;
    if(BENCHMARK){
        while( ( c = getopt (argc, argv, "i:k:t:") ) != -1 )
        {
            switch(c)
            {
                case 'i':
                    if(optarg) {
                        nvalue = optarg;
                    }else{
                        fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name>\n",
                                argv[0]);
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 't':
                    if(optarg) {
                        TIP_MODE_BY_USER = static_cast<bool>(std::atoi(optarg));
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
                    fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name>\n",
                            argv[0]);
                    exit(EXIT_FAILURE);
            }
        }
    }else   if(DEBUGMODE==false){
        while( ( c = getopt (argc, argv, "i:k:m:d:f:p:s:") ) != -1 )
        {
            switch(c)
            {
                case 'i':
                    if(optarg) nvalue = optarg;
                    break;
                case 'f':
                    if(optarg) {
                        FLG_NEWUB = static_cast<bool>(std::atoi(optarg));
                    }
                    break;
                case 'm':
                    if(optarg) {
                        ALGOMODE = static_cast<ALGOMODE_T>(std::atoi(optarg));
                    }
                    break;
                case 's':
                    if(optarg) {
                        SEED = static_cast<unsigned int>(std::atoi(optarg));
                    }
                    break;
                case 'd':
                    if(optarg) {
                        DBGFLAG = static_cast<DEBUGFLAG_T>(std::atoi(optarg));
                    }
                    break;
                case 'p':
                    if(optarg) {
                        SMALLK = static_cast<DEBUGFLAG_T>(std::atoi(optarg));
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
                    fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name>\n",
                            argv[0]);
                    exit(EXIT_FAILURE);
                    
            }
        }
        
        if(K==0 || strcmp(nvalue, "")==0){
            fprintf(stderr, "Usage: %s -k <kmer size> -i <input-file-name>\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }

        
    }
    //*/
    
    if(TIP_MODE_BY_USER){
        ALGOMODE = static_cast<ALGOMODE_T>(15);
    }else{
        ALGOMODE = static_cast<ALGOMODE_T>(16);
    }
    UNITIG_FILE = string(nvalue);
    
    ifstream infile(UNITIG_FILE);
    if(!infile.good()){
        fprintf(stderr, "Error: File named \"%s\" cannot be opened.\n", UNITIG_FILE.c_str());
        exit(EXIT_FAILURE);
    }

    double startTime = readTimer();
    cout << "## [1] Reading file... " << UNITIG_FILE << ": K = "<<K<<endl;
    if(ALGOMODE==15){
        if (EXIT_FAILURE == read_unitig_file(UNITIG_FILE, unitigs)) {
            return EXIT_FAILURE;
        }
            
    }else{
        read_unitig_file_abs(UNITIG_FILE, unitigs);
    }
    
    infile.close();
    double TIME_READ_SEC = readTimer() - startTime;
    cout<<"[COMPLETE][1] TIME to read file "<<TIME_READ_SEC<<" sec.\n\n";
    
    {
        
        Graph G;
        
        //count total number of edges
        int E_bcalm = 0;
        for (int i = 0; i < G.V; i++) {
            E_bcalm += adjList[i].size();
        }
        int V_bcalm = G.V;
        int numKmers = 0;
        int C_bcalm = 0;
        
        for (unitig_struct_t unitig : unitigs) {
            C_bcalm += unitig.ln;
            numKmers +=  unitig.ln - K + 1;
        }
        
//        double memvmKB, memrssKB;
//        process_mem_usage(memvmKB, memrssKB);
        //cout<<"memory (virtual) to load bcalm file (KB)"<<memvmKB<<endl;
        //cout<<"memory (rss) to load bcalm file (KB)"<<memrssKB<<endl;
        //globalStatFile << "MEM_VM_LOAD_KB" <<  "=" << memvmKB<< endl;
        //globalStatFile << "MEM_RSS_LOAD_KB" <<  "=" << memrssKB << endl;
        
        
        cout<<"## [2] Gathering info about upper bound... \n";
        double time_a = readTimer();
        G.indegreePopulate();
        
//        for(int i = 0; i<adjList.size(); i++){
//            cout<<i<<" in:"<<global_indegree[i]<<" out: "<<global_outdegree[i]<<endl;
//        }
//        exit(1);
        
        
        cout<<"[COMPLETE][2] TIME for information gather: "<<readTimer() - time_a<<" sec.\n\n";
        
        
        
        
        if(ALGOMODE == GRAPHPRINT){
            char sss[1000];
            cout<<"input the nodes to include in printing separated by space (i.e. 20 19 18): "<<endl;
            while(true){
                //string ipstr = "20 19 18";
                gets(sss);
                string ipstr(sss);
                if(ipstr=="stop"){
                    break;
                }
                makeGraphDot(ipstr);
                cout<<"done print, say again:"<<endl;
            }
        }
        
        
        int walkstarting_node_count = ceil((sharedparent_count + sink_count + source_count)/2.0) + isolated_node_count;
        
        
        
        int charLowerbound = C_bcalm-(K-1)*(G.V - walkstarting_node_count*1.0);
        float upperbound = (1-((C_bcalm-(K-1)*(G.V - walkstarting_node_count*1.0))/C_bcalm))*100.0;
        
//        printf( "%d\t\
//               %d\t\
//               %d\t\
//               %d\t\
//               %d\t\
//               %d\t\
//               %.2f\t\
//               %.2f%%\t\
//               %d\t\
//               %d\t\
//               %d\t\
//               %d\t\
//               %.2f%%\t",
//               K,
//               numKmers,
//               V_bcalm,
//               E_bcalm,
//               C_bcalm,
//               charLowerbound,
//               (charLowerbound*2.0)/numKmers,
//               upperbound,
//               isolated_node_count,
//               sink_count,
//               source_count,
//               sharedparent_count,
//               sharedparent_count*100.0/V_bcalm
//               );
        
        
        globalStatFile << "SEED" <<  "=" << SEED << endl;
        
        globalStatFile << "K" <<  "=" << K << endl;
        globalStatFile << "N_KMER" <<  "=" << numKmers << endl;
        globalStatFile << "V_BCALM" <<  "=" << V_bcalm << endl;
        globalStatFile << "E_BCALM" <<  "=" << E_bcalm << endl;
        globalStatFile << "C_BCALM" <<  "=" << C_bcalm << endl;
        globalStatFile << "C_LB" <<  "=" << charLowerbound << endl;
        globalStatFile << "V_LB" <<  "=" << walkstarting_node_count << endl;
        globalStatFile << "N_ISOLATED" <<  "=" << isolated_node_count << endl;
        globalStatFile << "N_SINK" <<  "=" << sink_count << endl;
        globalStatFile << "N_SOURCE" <<  "=" << source_count << endl;
        globalStatFile << "N_SPECIAL" <<  "=" << sharedparent_count << endl;
        
        //OPTIONAL;DERIVABLE
        globalStatFile << "BITSKMER_LB" <<  "=" << (charLowerbound*2.0)/numKmers << endl;
        globalStatFile << "PERCENT_UB" <<  "=" << upperbound <<  "%" << endl;
        globalStatFile << "PERCENT_N_SPECIAL" <<  "=" << sharedparent_count*100.0/V_bcalm <<"%" << endl;
        globalStatFile << "WALK_LB" <<  "=" << walkstarting_node_count << endl;
        
        
        for (auto i = inOutCombo.begin(); i != inOutCombo.end(); i++) {
            //printf("%.2f%%\t", (i->second)*100.0/V_bcalm);
            globalStatFile << "PERCENT_DEGREE_"<<(i->first).first << "_" << (i->first).second <<  "=" << (i->second)*100.0/V_bcalm <<"%" << endl;
        }
        
        
        printf("%.2f%%\t\
               %.2f%%\t",
               isolated_node_count*100.0/V_bcalm,
               (sink_count+source_count)*100.0/V_bcalm);
        //OPTIONAL; derivable
        globalStatFile << "PERCENT_N_ISOLATED" <<  "=" << isolated_node_count*100.0/V_bcalm <<"%" << endl;
        globalStatFile << "PERCENT_N_DEADEND" <<  "=" << (sink_count+source_count)*100.0/V_bcalm <<"%" << endl;
        
        // Iterating the map and printing ordered values
        //    for (auto i = inOutCombo.begin(); i != inOutCombo.end(); i++) {
        //        cout << "(" << i->first.first<< ", "<< i->first.second << ")" << " := " << i->second << '\n';
        //    }
        
        if(!NAIVEVARI){
            ncc = pullOutConnectedComponent();
            globalStatFile << "NUM_CC_UNITIG_GRAPH" <<  "=" << ncc << endl;
        }
            
        cout<<"NCC="<< ncc << endl;
        cout<<"WALK_LB="<<walkstarting_node_count<<endl;
        cout<<"ESS_CHAR_KMER_LB="<<(charLowerbound-(walkstarting_node_count-ncc)*(K-4)*1.0)/numKmers<<endl;
        globalStatFile << "ESS_CHAR_KMER_LB" <<(charLowerbound-(walkstarting_node_count-ncc)*(K-4)*1.0)/numKmers<<endl;
        if(ALGOMODE == PROFILE_ONLY){
            globalStatFile.close();
            printf("\n");
            return 0;
        }
        if(ALGOMODE == PROFILE_ONLY_ESS){
            exit(1);
        }
        
        MODE_WALK_UNION = (ALGOMODE == TWOWAYEXT);
        MODE_ABSORPTION_TIP = (ALGOMODE == BRACKETCOMP);
        MODE_ABSORPTION_NOTIP = (ALGOMODE == ONEWAYABSORPTION || ALGOMODE == ONEWAYABSORPTION_UNTESTED || ALGOMODE == PROFILE_ONLY_ABS);

        if (MODE_ABSORPTION_TIP){
            ofileTipOutput ="ust_ess_tip.txt";
        }
        if(ALGOMODE == TIPANDAB){
            MODE_ABSORPTION_NOTIP = true;
            MODE_ABSORPTION_TIP = false;
            BOTHTIPABSORB_V2 = true;
        }
        if (ALGOMODE == TIPANDAB_TIPLATER)
        {
            MODE_ABSORPTION_NOTIP = true;
            MODE_ABSORPTION_TIP = false;
            BOTHTIPABSORB_V2 = true;
        }
        if(ALGOMODE == PROFILE_ONLY_ABS){
             MODE_ABSORPTION_NOTIP = true;
            MODE_ABSORPTION_TIP = false;
            BOTHTIPABSORB_V2 = false;
        }
        if (ALGOMODE == ESS)
        {
            MODE_ABSORPTION_NOTIP = true;
            MODE_ABSORPTION_TIP = false;
            BOTHTIPABSORB_V2 = true;
        }

        //pullOutConnectedComponent();
        //exit(1);
        
        //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
        //##################################################//
        //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
        //##################################################//
        
        
        
        printf("\n\n Running in mode: %s\t \n\n",  modename[ALGOMODE].c_str());
        
        
        //STARTING DFS
        cout<<"## [3] Running UST algorithm... "<<endl;
        G.DFS();
        
        //process_mem_usage(memvmKB, memrssKB);
        //cout<<"memory (virtual) to do initial DFS (KB)"<<memvmKB<<endl;
        //cout<<"memory (rss) to do initial DFS (KB)"<<memrssKB<<endl;
        //globalStatFile << "MEM_VM_DFS_KB" <<  "=" << memvmKB<< endl;
        //globalStatFile << "MEM_RSS_DFS_KB" <<  "=" << memrssKB << endl;
        
        if(DBGFLAG == PRINTER){
            //printBCALMGraph(adjList);
            printNewGraph(G);
            for(int i = 0; i< countNewNode; i++){
                cout<<"new ->" <<i<<" ";
                for(int x: newToOld[i]){
                    cout<<nodeSign[x]<<"."<<x<<" ";
                }
                cout<<endl;
            }
        }
        
        double TIME_TOTAL_SEC = readTimer() - startTime;
        
        time_a = readTimer();
        
        if(ALGOMODE==BRACKETCOMP){
            C_ustitch = C_tip_ustitch;
            V_ustitch  = V_tip_ustitch;
        }else if(ALGOMODE == TWOWAYEXT){
            V_ustitch = V_twoway_ustitch;
            C_ustitch = C_twoway_ustitch;
        }else if(ALGOMODE == BASIC){
            formattedOutputForwardExt(G);
            V_ustitch = countNewNode;
        }else if(ALGOMODE == ONEWAYABSORPTION || ALGOMODE == TIPANDAB_TIPLATER || ALGOMODE == TIPANDAB){
            C_ustitch = C_oneabsorb;
            V_ustitch  = V_oneabsorb;
        }else{
            formattedOutputForwardExt(G);
            V_ustitch = countNewNode;
        }
        
        //
        //if vari new mode
        if(!BENCHMARK){
            if(NAIVEVARI){
                V_oneabsorb = absorbGraphNumCC_endAbosrb;
                V_ustitch =absorbGraphNumCC_endAbosrb;
                int csp = (V_twoway_ustitch-V_oneabsorb)*4;
                C_oneabsorb = C_twoway_ustitch + C_abs_calc;
                C_ustitch = C_oneabsorb;
                cout<<"c_ust="<<C_twoway_ustitch<<endl;
                cout<<"c_abs_calc="<<C_ustitch<<endl;
                cout<<"c_special_calc="<<csp<<endl;
                cout<<"v_ust="<<V_twoway_ustitch<<endl;
                cout<<"v_abs="<<V_ustitch<<endl;
                cout<<"n_abs="<<(V_twoway_ustitch-V_oneabsorb)<<endl;
                cout<<"c_per_imp="<<(1.0-(C_ustitch*1.0)/(C_twoway_ustitch*1.0))*100<<"%"<<endl;
                ncc=pullOutConnectedComponent();
                globalStatFile << "NUM_CC_UNITIG_GRAPH" <<  "=" <<  ncc<< endl;
                cout << "n_CC_UNITIG_GRAPH" <<  "=" << ncc << endl;
                
                C_oneabsorb_ACGT = C_ustitch - csp;
                C_oneabsorb_plusminus = C_ustitch - csp/4;
                C_oneabsorb_brackets = C_ustitch - csp/2;
            }else{
                int csp = (V_twoway_ustitch-V_oneabsorb)*3;
                if(MODE_ABSORPTION_TIP){
                    csp = C_tip_special;
                     cout<<"c_usttip="<<C_tip_ustitch<<endl;
                    
                }
                
                 cout<<"c_ust="<<C_twoway_ustitch<<endl;
                cout<<"c_abs="<<C_oneabsorb<<endl;
                cout<<"c_special_calc="<<csp<<endl;
                cout<<"v_ust="<<V_twoway_ustitch<<endl;
                cout<<"v_abs="<<V_oneabsorb<<endl;
                cout<<"n_abs="<<(V_twoway_ustitch-V_oneabsorb)<<endl;
                cout<<"c_per_imp="<<(1.0-(C_ustitch*1.0)/(C_twoway_ustitch*1.0))*100<<"%"<<endl;
                
                
            }
            cout<<"WALK_LB="<<walkstarting_node_count<<endl;
            cout<<"N_CC="<<ncc<<endl;
            
        }
        
        
        
        cout<<"[3.5] TIME to output: "<<readTimer() - time_a<<" sec. \n\n";
        cout<<"[COMPLETE][3] Total TIME: "<<TIME_TOTAL_SEC<<" sec. \n\n";
        
        
        float percent_saved_c = (1-(C_ustitch*1.0/C_bcalm))*100.0;
        float ustitchBitsPerKmer;
        if(MODE_ABSORPTION_NOTIP || MODE_ABSORPTION_TIP){
            ustitchBitsPerKmer =C_ustitch*2.0/numKmers;
        }else{
            ustitchBitsPerKmer =C_ustitch*2.0/numKmers;
        }
        
        
        printf("%s\t",  modename[ALGOMODE].c_str());
        printf("%d\t\
               %.2f%%\t\
               %.2f%%\t\
               %d\t\
               %.2f\t",
               V_ustitch,
               percent_saved_c,
               upperbound - percent_saved_c,
               C_ustitch,
               ustitchBitsPerKmer
               );
        printf("%.2f\t\
               %.2f\t",
               TIME_READ_SEC,
               TIME_TOTAL_SEC
               );
        printf("\n");
        printf("\n");
        
        globalStatFile << "TIME_TOTAL_ENCODE_SEC" <<  "=" << TIME_TOTAL_SEC << endl;
        
        //globalStatFile << "USTITCH_MODE" <<  "=" << mapmode[ALGOMODE].c_str() << endl;
        globalStatFile << "V_USTITCH" << mapmode[ALGOMODE].c_str() <<  "=" << V_ustitch << endl;
        globalStatFile << "PERCENT_C_SAVED" << mapmode[ALGOMODE].c_str() <<  "=" << percent_saved_c << "%" << endl;
        globalStatFile << "PERCENT_C_GAP_WITH_UB" << mapmode[ALGOMODE].c_str() <<  "=" << upperbound - percent_saved_c << "%" << endl;
        globalStatFile << "C_USTITCH" << mapmode[ALGOMODE].c_str() <<   "=" <<C_ustitch << endl;
        globalStatFile << "BITSKMER_USTITCH" << mapmode[ALGOMODE].c_str() <<  "=" <<ustitchBitsPerKmer << endl;
        globalStatFile << "TIME_READINPUT_SEC" << mapmode[ALGOMODE].c_str() <<  "=" <<TIME_READ_SEC << endl;
        globalStatFile << "TIME_ENCODE_TOTAL_SEC" << mapmode[ALGOMODE].c_str() <<  "=" <<TIME_TOTAL_SEC << endl;
        
        
        if(ALGOMODE == ONEWAYABSORPTION || BOTHTIPABSORB_V2){
            globalStatFile << "C_ONEABSORB_ACGT" << mapmode[ALGOMODE].c_str() <<  "=" <<C_oneabsorb_ACGT << endl;
            globalStatFile << "C_ONEABSORB_PLUSMINUS" << mapmode[ALGOMODE].c_str() <<  "=" <<C_oneabsorb_plusminus << endl;
            
            globalStatFile << "C_ONEABSORB_BRACKETS" << mapmode[ALGOMODE].c_str() <<  "=" <<C_oneabsorb_brackets << endl;
            
            globalStatFile << "ABSORB_GRAPH_NUM_CC" <<  "=" <<absorbGraphNumCC_endAbosrb << endl;
            //connectedCC_allAbsorb();
            globalStatFile << "ABSORB_GRAPH_NUM_CC_ALL_ABSORB" <<  "=" <<absorbGraphNumCC_allAbosrb << endl;
        }
        
        globalStatFile.close();
        fclose(statFile);
        
        //process_mem_usage(memvmKB, memrssKB);
        //cout<<"before clearing memory (virtual) to do ONLY UST DFS (KB)"<<memvmKB<<endl;
        //cout<<"before memory (rss) to do ONLY UST DFS (KB)"<<memrssKB<<endl;
        
        
        if(VARIMODE){
            ofstream boolfile("vari_isabsorbed.txt");
            for(int i = 0; i<G.V; i++){
                boolfile<<int(oldToNew[i].pos_in_walk!=1)<<endl;
            }
            boolfile.close();
        }
        
    }
    
    
    unitigs.clear();
    unitigs.shrink_to_fit();
    

    inOutCombo.clear();
      
    //adjList.clear();
    //adjList.shrink_to_fit();
    
    //reverseAdjList.clear();
    //reverseAdjList.shrink_to_fit();
    
    //newSequences.clear();
     
    //newNewSequences.clear(); //int is the unitig id (old id)
    
    //newNewMarker.clear();
    

    newToOld.clear();
    newToOld.shrink_to_fit();
    
    walkFirstNode.clear(); //given a walk id, what's the first node of that walk
    walkFirstNode.shrink_to_fit();
    
    sinkSrcEdges.clear(); //int is the unitig id (old id)
    
   
    double memvmKB, memrssKB;
    //process_mem_usage(memvmKB, memrssKB);
    
    //cout<<"CLEARED memory (virtual) to do ONLY UST DFS (KB)"<<memvmKB<<endl;
    //cout<<"CLEARED memory (rss) to do ONLY UST DFS (KB)"<<memrssKB<<endl;
    //globalStatFile << "MEM_VM_ONLY_UST_KB" <<  "=" << memvmKB<< endl;
    //globalStatFile << "MEM_RSS_ONLY_UST_KB" <<  "=" << memrssKB << endl;
    
    cout<<"## [4] Decoding...\n";
    double decodetime = readTimer();
    if(MODE_ABSORPTION_TIP){
        if(NAIVEVARI || BENCHMARK ){
            cout<<"## [4] Skipped. \n";
        }else{
           decodeTip(K,ofileTipOutput);
        }
            
    }
    if(MODE_ABSORPTION_NOTIP){
        if(NAIVEVARI || BENCHMARK ){
            cout<<"## [4] Skipped. \n";
        }else{
            decodeOneAbsorb(K,ofileTipOutput);
        }
    }
    globalStatFile.open("global_stat", std::fstream::out | std::fstream::app);
    decodetime = readTimer() - decodetime;
    globalStatFile << "TIME_DECODE_SEC" <<  "=" <<decodetime << endl;
    globalStatFile << "METHOD" <<  "=" << ALGOMODE << endl;
    
    
    cout<<"[COMPLETE][4] Decoding done. TIME: "<<decodetime<<"sec. \n";
    globalStatFile.close();

    
   
    return EXIT_SUCCESS;
}
