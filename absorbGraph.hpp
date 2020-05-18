//
//  absorbGraph.hpp

#ifndef absorbGraph_hpp
#define absorbGraph_hpp

#include "global.hpp"
#include <algorithm>
#include <sstream>


class SCCGraph
{
public:
    int V;    // No. of vertices
    list<int> *adj;    // An array of adjacency lists
    
    // Fills Stack with vertices (in increasing order of finishing
    // times). The top element of stack has the maximum finishing
    // time
    void fillOrder(int v, bool visited[], stack<int> &Stack);
    
    // A recursive function to print DFS starting from v
    void DFSUtil(int v, bool visited[], map<int, int>& vToMetagraphV, map<int, set<int> >& sccIdToWalks, int & countSCC);
    
    SCCGraph(int V);
    SCCGraph();
    
    void addEdge(int v, int w);
    
    // The main function that finds and prints strongly connected
    // components
    //queue<int> printSCCs(AbsorbGraph* abs);
    
    // Function that returns reverse (or transpose) of this graph
    SCCGraph getTranspose();
};


class AbsorbGraph
{
public:
    ofstream depthfile;
    ofstream smallkfile, segfaultfile;
    
    vector<MyTypes::fourtuple> sorter;
    
    map<int, int > sorterIndexMap;
    vector<int> *ccAdjList;
    vector<int> earliestAbsorberPos;
    map<int, vector<edge_t> > absorbGraph;
    map<int, stack<edge_t> > absorbGraphCycleRemoved;
    
    
    bool EARLYABSORB = true;
    
    SCCGraph* g;
    bool* absorbed;
    char* absorbedCategory;
    
    queue<int> orderOfUnitigs;
    queue<int> reservedStringSize;
    
    queue<int> printSCCs();
    
    /*vari mode*/
    void hasherSmallerK(int);
    void addSmallerKEdge(int);
    
    /*helpers*/
    /*string*/
    string splitA(string &s, int);
    string splitX(string &s, int);
    string splitB(string &s, int);
    
    void print_absorb_graph();
    bool isItWalkStarting(int uid);
    vector<int> getAllUidsInWalk(int walkId);
    
    void lowerboundcc();
    void earlyAbsorb(const vector<int>&, bool*);
    void sorterIndexAndAbsorbGraphMaker();    //make the absorb graph with cycle, populates sorterIndexMap AND absorbGraph
    
    void absorbGraphIsCyclicUtil(int uid, vector<char>& visited, int depth);
    void removeCycleFromAbsorbGraphRec();
    void removeCycleFromAbsorbGraph();
    
    bool* isItAPrintedWalk;
    void recursiveWalkStringMaker(int startWalkIndex, int depth, int Kpass);
    void iterSpellSlowDisk(int startWalkIndex2, int depth, int Kpass, vector<char>& color);
    void iterSpellTested(int startWalkIndex2, int depth, int Kpass, vector<char>& color, int);
    void tipSpell();
    void recursiveWalkStringMakerOld(int startWalkIndex, int depth, int Kpass, ofstream &);

    
    void tipAbsorbedOutputter();
    
    void postorder_master();
    void postorder(int i, map<int, vector<int > >& walkGraph, vector<bool>& visited, queue<int>& q);
    string* constructedStrings;
    
    AbsorbGraph(vector<MyTypes::fourtuple>& sorter){
        
        
        
        this->sorter = sorter;
        if(ALGOMODE!=15){
            ccAdjList = new vector<int>[countNewNode];
            g = new SCCGraph(countNewNode);
            absorbedCategory = new char[adjList.size()];
            absorbed = new bool[countNewNode];
            isItAPrintedWalk = new bool[countNewNode];
            constructedStrings = new string[countNewNode];
            
            for(int i=0; i< adjList.size(); i++){
                absorbedCategory[i] = '0';
                if(NAIVEVARI){
                    hasherSmallerK(i);
                }
            }
            
            for(int i=0; i<countNewNode; i++){
                isItAPrintedWalk[i]=false;
                constructedStrings[i] = "";
                absorbed[i] = false;
            }
            if(NAIVEVARI){
                for(int i=0; i<  adjList.size(); i++){
                    addSmallerKEdge(i);
                }
            }
            depthfile.open("depthfile.txt");
            smallkfile.open("smallk.txt");
            segfaultfile.open("segfaultfile.txt");
        }
        
        
    }
    ~AbsorbGraph(){
        if(ALGOMODE!=15){
            delete [] ccAdjList;
            delete [] absorbedCategory;
            delete [] absorbed;
            delete [] isItAPrintedWalk;
            delete [] constructedStrings;
        }
        depthfile.close();
        smallkfile.close();
        segfaultfile.close();
    }
};

string AbsorbGraph::splitA(string &s, int K){
    int ol = K-1;
    return s.substr(0, ol);
}
string AbsorbGraph::splitB(string &s, int K){
    int ol = K-1;
    int pref_end_index = ol - 1;
    int suf_start_index = s.length() - ol;
    if(pref_end_index >= suf_start_index){
        return cutPref(s, K);
        //return s.substr(pref_end_index+1, s.length()- pref_end_index - 1); //cutPref
    }
    return suf(s,K);
    //s.substr(suf_start_index, ol);
}

string AbsorbGraph::splitX(string &s, int K){
    int ol = K-1;
    int pref_end_index = ol - 1;
    int suf_start_index = s.length() - ol;
    string X = "";
    if(pref_end_index < suf_start_index){
        X = s.substr(pref_end_index+1,suf_start_index-pref_end_index-1);
        //X=cutPref(s, K);
        //X = cutSuf(X, K);
    }
    return X;
}


void AbsorbGraph::lowerboundcc(){
    bool* ccVisited =  new bool[countNewNode];
    for(int i = 0; i<countNewNode; i++){
        ccVisited[i] = false;
    }
    
    stack<int> stack;
    for(int ii=0;ii<countNewNode;ii++)
    {
        if(ccVisited[ii] == false &&  !obsoleteWalkId[ii])
        {
            //cout<<endl;
            stack.push(ii);
            //vector<int>nodes;
            //nodes.push_back(i);
            absorbGraphNumCC_endAbosrb++;
            int count=0;
            
            while (!stack.empty())
            {
                int s = stack.top();
                stack.pop();
                
                if (!ccVisited[s])
                {
                    //cout << s << " ";
                    ccVisited[s] = true;
                }
                
                // Get all adjacent vertices of the popped vertex s
                // If a adjacent has not been visited, then push it
                // to the stack.
                for (auto i = ccAdjList[s].begin(); i != ccAdjList[s].end(); ++i)
                    if (!ccVisited[*i])
                        stack.push(*i);
            }
            //cout<<"This component has "<<count<<" nodes"<<"\n";
            //          for(int nn:nodes){
            //              cout<<nn<<",";
            //          }
            //cout<<endl;
        }
    }
    cout<<"number of connected components (end): "<<absorbGraphNumCC_endAbosrb<<endl;
    delete [] ccVisited;
}




void AbsorbGraph::earlyAbsorb(const vector<int>& earliestAbsorberPos,bool* isAnAbsorbedWalk){
    for(int wid=0; wid<countNewNode; wid++){
        //
        if(!isAnAbsorbedWalk[wid] && !obsoleteWalkId[wid]){
            //earliestAbsorberPos[wid];
            int it = sorterIndexMap[wid];
            for(int x = it+1; x<sorter.size(); x++){
                int walkId = get<1>(sorter[x]);
                int pos = get<2>(sorter[x]);
                int uid=get<0>(sorter[x]);
                if(walkId!=wid || pos >= earliestAbsorberPos[wid]){
                    break;
                }
                
                for(edge_t e : adjList[uid]){
                    int absorber_uid = e.toNode;
                    int absorberWalk = oldToNew[absorber_uid].finalWalkId;
                    int absorber_pos = oldToNew[absorber_uid].pos_in_walk;
                    if(!(absorber_pos<earliestAbsorberPos[absorberWalk]) && !isItWalkStarting(absorber_uid)){
                        //add it
                        if(absorberWalk != wid  ){
                            edge_t enew;
                            if(e.left==e.right){
                                enew.left = (e.left?false:true);
                                enew.right = (e.right?false:true);
                            }else{
                                enew.left = e.left;
                                enew.right = e.right;
                            }
                            enew.toNode = uid;
                            //                            bool is1 =(enew.left == nodeSign[e.toNode] && enew.right == nodeSign[enew.toNode] );
                            //                            bool is4 =(enew.left != nodeSign[e.toNode] && enew.right == nodeSign[enew.toNode] );
                            //                            bool is3 =(enew.left != nodeSign[e.toNode] && enew.right != nodeSign[enew.toNode] );
                            //                            bool is2 =(enew.left == nodeSign[e.toNode] && enew.right != nodeSign[enew.toNode] );
                            
                            absorbGraph[e.toNode].push_back(enew);
                            isAnAbsorbedWalk[wid]=true;
                            g->addEdge(absorberWalk, wid);
                            if(GETLOWERBOUND_CC){
                                ccAdjList[absorberWalk].push_back(wid);
                                ccAdjList[wid].push_back(absorberWalk);
                            }
                            
                        }
                    }
                }
            }
        }
    }
}


void AbsorbGraph::hasherSmallerK(int node){
//    for(int smallOverlap = K-1; smallOverlap >= SMALLK; smallOverlap--) {
//        string seq = unitigs.at(node).sequence;
//        //int SMALLK = 20;
//        string pref = seq.substr(0, smallOverlap-1);
//        string suf = seq.substr(seq.length()-(smallOverlap-1), smallOverlap-1);
//
//        plusMapPre[pref].insert(node);
//        //plusMapSuf[suf].insert(node);
//
//        string rseq = reverseComplement(unitigs.at(node).sequence);
//        string rpref = rseq.substr(0, smallOverlap-1);
//        string rsuf = rseq.substr(rseq.length()-(smallOverlap-1), smallOverlap-1);
//
//        minusMapPre[rpref].insert(node);
//        //minusMapSuf[rsuf].insert(node);
//
//    }
}
void AbsorbGraph::addSmallerKEdge(int node){
    //int SMALLK= 20;
    //int SMALLK = 6;
//    for(int smallOverlap = SMALLK; smallOverlap <= K-1 ; smallOverlap++) {
//        string seq = unitigs.at(node).sequence;
//        string pref = seq.substr(0, smallOverlap-1);
//        string suf = seq.substr(seq.length()-(smallOverlap-1), smallOverlap-1);
//        
//        string rseq = reverseComplement(unitigs.at(node).sequence);
//        string rpref = rseq.substr(0, smallOverlap-1);
//        string rsuf = rseq.substr(rseq.length()-(smallOverlap-1), smallOverlap-1);
//        
//        
//        if (plusMapPre.find(suf) != plusMapPre.end()){
//            set<int> x = plusMapPre[suf];
//            for( int i : x){
//                if(i!=node){
//                    edge_t e, e2;
//                    e.toNode = node;
//                    e.left = false;
//                    e.right = false;
//                    e.isSmallK = 'n';
//                    e.smallk = smallOverlap;
//                    adjList[i].push_front(e);
//                    
//                    e2.toNode = i;
//                    e2.left = true;
//                    e2.right = true;
//                    e2.isSmallK = 'n';
//                    e2.smallk = smallOverlap;
//                    adjList[node].push_front(e2);
//                }
//            }
//            
//        }
//        
//        
//        if (minusMapPre.find(suf) != minusMapPre.end()){
//            set<int> x = minusMapPre[suf];
//            for( int i : x){
//                if(i!=node){
//                    edge_t e, e2;
//                    e.toNode = node;
//                    e.left = true;
//                    e.right = false;
//                    e.isSmallK = 'n';
//                    e.smallk = smallOverlap;
//                    adjList[i].push_front(e);
//                    
//                    e2.toNode = i;
//                    e2.left = true;
//                    e2.right = false;
//                    e2.isSmallK = 'n';
//                    e2.smallk = smallOverlap;
//                    adjList[node].push_front(e2);
//                }
//            }
//            
//        }
//        
//        if (minusMapPre.find(rsuf) != minusMapPre.end()){
//            set<int> x = minusMapPre[rsuf];
//            for( int i : x){
//                if(i!=node){
//                    edge_t e, e2;
//                    e.toNode = node;
//                    e.left = true  ;
//                    e.right = true;
//                    e.isSmallK = 'n';
//                    e.smallk = smallOverlap;
//                    adjList[i].push_front(e);
//                    
//                    e2.toNode = i;
//                    e2.left = false;
//                    e2.right = false;
//                    e2.isSmallK = 'n';
//                    e2.smallk = smallOverlap;
//                    adjList[node].push_front(e2);
//                }
//            }
//            
//        }
//        
//        if (plusMapPre.find(rsuf) != plusMapPre.end()){
//            set<int> x = plusMapPre[rsuf];
//            for( int i : x){
//                if(i!=node){
//                    edge_t e, e2;
//                    e.toNode = node;
//                    e.left = false;
//                    e.right = true;
//                    e.isSmallK = 'n';
//                    e.smallk = smallOverlap;
//                    adjList[i].push_front(e);
//                    
//                    e2.toNode = i;
//                    e2.left = false;
//                    e2.right = true;
//                    e2.isSmallK = 'n';
//                    e2.smallk = smallOverlap;
//                    adjList[node].push_front(e2);
//                }
//            }
//            
//        }
//    }
    
}


void AbsorbGraph::postorder(int i, map<int, vector<int > >& walkGraph, vector<bool>& visited, queue<int>& q){
    //return uids
    vector<int> v = walkGraph[i];
    for(int e : v){
        if(!visited[e])
            postorder(e, walkGraph, visited, q);
    }
    q.push(i);
    visited[i] = true;
}


void AbsorbGraph::postorder_master(){
    map<int, vector<int > > walkGraph;
    int V_walkgraph = 0 ;
    int V_isolated_walkgraph  = 0;
    int E_walkgraph = 0;
    int V_root_walkgraph = 0;
    int V_noniso = 0;
    vector<int> isolated;
    vector<int> indegree(countNewNode, 0);
    set<int> noniso;
    
    for(int uid = 0; uid<adjList.size(); uid++){
        stack<edge_t> adj = absorbGraphCycleRemoved[uid];
        while (!adj.empty()) {
            edge_t e = adj.top();
            adj.pop();
            walkGraph[oldToNew[uid].finalWalkId].push_back(oldToNew[e.toNode].finalWalkId);
            noniso.insert(oldToNew[uid].finalWalkId);
            noniso.insert(oldToNew[e.toNode].finalWalkId);
        }
    }
    
    for(map<int, vector<int > >::iterator it = walkGraph.begin(); it != walkGraph.end(); ++it) {
        vector<int> adj = it->second;
        E_walkgraph+= adj.size();
        for(auto w: adj){
            indegree[w] = indegree[w] + 1;
        }
        
    }
    V_noniso=noniso.size();
    for(int i =0 ; i<countNewNode; i++){
        if(!obsoleteWalkId[i]){
            V_walkgraph++;
            if(noniso.count(i)==0){
                V_isolated_walkgraph++;
                isolated.push_back(i);
            }
            if(indegree[i]==0){
                V_root_walkgraph++;
            }
        }
    }
    for(int i =0 ; i<countNewNode; i++){
        if(!obsoleteWalkId[i]){
            int id =indegree[i] ;
            assert(id < 2);
        }
        
    }
    
    assert(V_walkgraph == V_isolated_walkgraph + V_noniso);
    assert(E_walkgraph == V_walkgraph - ( V_root_walkgraph)  );
    
    queue<int> q;
    vector<bool> visited(countNewNode, false);
    for(int wid = 0; wid<countNewNode; wid++){
        if(!visited[wid] && !obsoleteWalkId[wid])
            postorder(wid, walkGraph, visited, q);
    }
    cout<<q.size();
    assert(V_walkgraph == V_isolated_walkgraph + V_noniso);
    assert(E_walkgraph == V_walkgraph - ( V_root_walkgraph)  );
    
    //
    //    while(!q.empty()){
    //        int aa = q.front();
    //        cout<<aa<<" ";
    //        q.pop();
    //    }
    //
    //exit(1);
}

bool AbsorbGraph::isItWalkStarting(int uid){
    int pos_in_walk = oldToNew[uid].pos_in_walk;
    if(pos_in_walk > 1) return false;
    if(pos_in_walk == 0) return true;
    if(sorterIndexMap.count(oldToNew[uid].finalWalkId)==0){
        auto adjL = adjList[uid];
        for(edge_t ee: adjL){
            if(oldToNew[ee.toNode].pos_in_walk==0 && oldToNew[ee.toNode].finalWalkId == oldToNew[uid].finalWalkId ){
                return false;
            }
        }
        return true;
    }
    assert(!obsoleteWalkId[oldToNew[uid].finalWalkId]);
    assert(sorterIndexMap.count(oldToNew[uid].finalWalkId)>0);
    int it = sorterIndexMap[oldToNew[uid].finalWalkId];
    assert(it<sorter.size());
    int ituid = get<0>(sorter[it]);
    return (uid==ituid);
}

void connectedCC_allAbsorb(){
    return;
    absorbGraphNumCC_allAbosrb = 0;
    
    vector<vector<int> > ccAllAbsorbAdjList(countNewNode);
    bool* ccVisited =  new bool[countNewNode];
    for(int i = 0; i<countNewNode; i++){
        ccVisited[i] = (false);
    }
    
    
    for(int uid = 0; uid <adjList.size(); uid++){
        auto adju = adjList.at(uid);
        for (edge_t e : adju) {
            int absorberWalk = oldToNew[e.toNode].finalWalkId;
            int absorbedWalk = oldToNew[uid].finalWalkId;
            
            if(absorberWalk != absorbedWalk){
                ccAllAbsorbAdjList[absorberWalk].push_back(absorbedWalk);
                ccAllAbsorbAdjList[absorbedWalk].push_back(absorberWalk);
            }
        }
    }
    
    if(1==1){
        stack<int> stack;
        for(int ii=0;ii<countNewNode;ii++)
        {
            if(ccVisited[ii] == false &&  !obsoleteWalkId[ii])
            {
                //cout<<endl;
                stack.push(ii);
                //vector<int>nodes;
                //nodes.push_back(i);
                absorbGraphNumCC_allAbosrb++;
                
                while (!stack.empty())
                {
                    // Pop a vertex from stack and print it
                    int s = stack.top();
                    stack.pop();
                    
                    // Stack may contain same vertex twice. So
                    // we need to print the popped item only
                    // if it is not visited.
                    if (!ccVisited[s])
                    {
                        //cout << s << " ";
                        ccVisited[s] = true;
                    }
                    
                    // Get all adjacent vertices of the popped vertex s
                    // If a adjacent has not been visited, then push it
                    // to the stack.
                    for (auto i = ccAllAbsorbAdjList[s].begin(); i != ccAllAbsorbAdjList[s].end(); ++i)
                        if (!ccVisited[*i])
                            stack.push(*i);
                }
            }
            
        }
        cout<<"number of connected components (all): "<<absorbGraphNumCC_allAbosrb<<endl;
        delete [] ccVisited;
        
    }
}

void AbsorbGraph::sorterIndexAndAbsorbGraphMaker(){
    
    ofstream variPreFile;
    ofstream variSufFile;
    ofstream variIntFile;
    ofstream variWalkIdFile;
    
    //    if(VARIMODENEW){
    //
    //        map<string, set<pair<int, char>> > ws;
    //        map<string, set<pair<int, char>> > nws;
    //        int K_prime = K-1;
    //
    //           for(int uid =0 ; uid<adjList.size(); uid++){
    //               string uu=unitigs[uid].sequence;
    //               string rc = reverseComplement(uu);
    //
    //               if(isItWalkStarting(sorter, sorterIndexMap, uid)){
    //                   ws[suf(rc, K_prime)].insert(make_pair(uid,'1'));
    //                   ws[suf(uu, K_prime)].insert(make_pair(uid,'2'));
    //                   ws[pref(rc, K_prime)].insert(make_pair(uid,'3'));
    //                   ws[pref(uu, K_prime)].insert(make_pair(uid,'4'));
    //               }else{
    //                   nws[suf(rc, K_prime)].insert(make_pair(uid,'1'));
    //                   nws[suf(uu, K_prime)].insert(make_pair(uid,'2'));
    //                   nws[pref(rc, K_prime)].insert(make_pair(uid,'3'));
    //                   nws[pref(uu, K_prime)].insert(make_pair(uid,'4'));
    //               }
    //               variSufFile<<suf(uu, K-1)<<" "<<endl;
    //               variWalkIdFile<<oldToNew[uid].finalWalkId<<endl;
    //           }
    //
    //       }
    
    if(VARIMODE){
//        variPreFile.open("vari_pre.txt");
//        variSufFile.open("vari_suf.txt");
//        variIntFile.open("vari_int.txt");
//        variWalkIdFile.open("vari_walkid.txt");
//
//        for(int uid =0 ; uid<adjList.size(); uid++){
//            string uu=unitigs[uid].sequence;
//            if (nodeSign[uid]==false ){
//                uu=reverseComplement(uu);
//            }
//            variSufFile<<suf(uu, K-1)<<" "<<endl;
//            variWalkIdFile<<oldToNew[uid].finalWalkId<<endl;
//        }
    }
    
    
    int prevWalkId = -1;
    int lastWalkStartingIndex = -1;
    
    //list<int> *adj
    //countNewNode=countNewNode+1;
    //obsoleteWalkId.push_back(false);
    
    bool* isAnAbsorbedWalk = new bool[countNewNode];
    for(int i = 0; i<countNewNode; i++){
        isAnAbsorbedWalk[i]=false;
    }
    
    if(BOTHTIPABSORB && 1 == 0){
        for (auto const& x : sinkSrcEdges)
        {
            int sinksrc = x.first;
            for(edge_t e: x.second){
                if(color[sinksrc] != 'w'){
                    break;
                }
                if(nodeSign[e.toNode] == e.right){  //ensure it is a fwd case
                    if(color[e.toNode]!='w' && color[e.toNode]!='r' && color[e.toNode]!='l'){//  this ensures that this to vertex is
                        nodeSign[sinksrc] = e.left;
                        color[sinksrc] = 'l';
                        oldToNew[sinksrc].pos_in_walk = 1;
                        assert(oldToNew[e.toNode].pos_in_walk != -1);
                
                        oldToNew[sinksrc].isTip = 2;
                        absorbedCategory[sinksrc] = '1';
                        
                        edge_t enew;
                        if(e.left==e.right){
                            enew.left = (e.left?false:true);
                            enew.right = (e.right?false:true);
                        }else{
                            enew.left = e.left;
                            enew.right = e.right;
                        }
                        
                        enew.toNode = sinksrc;
                        
                        absorbGraph[e.toNode].push_back(enew);
                        isAnAbsorbedWalk[oldToNew[sinksrc].finalWalkId]=true;
                    }
                    
                }else{
                    // 3 bwd cases
                    if((color[e.toNode]!='w' && color[e.toNode]!='r' && color[e.toNode]!='l')){
                        nodeSign[sinksrc] = !e.left;
                        color[sinksrc] = 'r';
                        oldToNew[sinksrc].pos_in_walk = 1;
                        assert(oldToNew[e.toNode].pos_in_walk != -1);
                        
                        oldToNew[sinksrc].isTip = 1;
                        absorbedCategory[sinksrc] = '4';
                        
                        edge_t enew;
                        if(e.left==e.right){
                            enew.left = (e.left?false:true);
                            enew.right = (e.right?false:true);
                        }else{
                            enew.left = e.left;
                            enew.right = e.right;
                        }
                        
                        enew.toNode = sinksrc;
                        absorbGraph[e.toNode].push_back(enew);
                        isAnAbsorbedWalk[oldToNew[sinksrc].finalWalkId]=true;
                        
                    }
                }
            }
        }
    }
    
    int MAX_POS = adjList.size()+1;
    vector<int> earliestAbsorberPos(countNewNode, MAX_POS);
    
    for(int tup_i = 0; tup_i < sorter.size(); tup_i++){
        MyTypes::fourtuple tup = sorter[tup_i];
        int uid = get<0>(tup);
        new_node_info_t nd = oldToNew[uid];
        int finalWalkId = get<1>(tup);
        
        //for end abosrbing
        if(prevWalkId !=finalWalkId ){  //walk starting: 2 cases-> a) this is not walk ender b) ender
            if(prevWalkId!=-1){
                int prev_uid = get<0>(sorter[tup_i - 1]);
                int prev_wid = get<1>(sorter[tup_i - 1]);
                int prev_pos = get<2>(sorter[tup_i - 1]);
                //earliestAbsorberPos[prev_wid] =2;
                //                if(prev_pos/2>1){
                //                    earliestAbsorberPos[prev_wid] =prev_pos/2;
                //                }else{
                //                    earliestAbsorberPos[prev_wid] = 2;
                //                }
            }
            
            lastWalkStartingIndex = tup_i;
            sorterIndexMap[finalWalkId] = lastWalkStartingIndex;
            
            auto adju = adjList.at(uid);
            for (edge_t e : adju) {
                if(BOTHTIPABSORB && global_issinksource[e.toNode] ==1){
                    continue;
                }
                
                int absorberWalk = oldToNew[e.toNode].finalWalkId;
                int absorbedWalk = nd.finalWalkId;
                
                if (ALGOMODE == PROFILE_ONLY_ABS)
                {
                    if (absorberWalk != absorbedWalk && isAnAbsorbedWalk[oldToNew[uid].finalWalkId] == true)
                    {
                        ccAdjList[absorberWalk].push_back(absorbedWalk);
                        ccAdjList[absorbedWalk].push_back(absorberWalk);
                    }
                }
                
                if(absorberWalk != absorbedWalk  ){
                    
                    // && isAnAbsorbedWalk[oldToNew[uid].finalWalkId] == false
                    //if(absorberWalk != absorbedWalk){
                    //the edge "enew" is from absorber uid to absorbed uid
                    assert(absorberWalk<countNewNode);
                    assert(absorbedWalk<countNewNode);
                    
                    edge_t enew;
                    if(e.left==e.right){
                        enew.left = (e.left?false:true);
                        enew.right = (e.right?false:true);
                    }else{
                        enew.left = e.left;
                        enew.right = e.right;
                    }
                    enew.toNode = uid;
                    if(NAIVEVARI){
                        enew.smallk = e.smallk;
                    }
                    bool is1 =(enew.left == nodeSign[e.toNode] && enew.right == nodeSign[enew.toNode] );
                    bool is4 =(enew.left != nodeSign[e.toNode] && enew.right == nodeSign[enew.toNode] );
                    bool is3 =(enew.left != nodeSign[e.toNode] && enew.right != nodeSign[enew.toNode] );
                    bool is2 =(enew.left == nodeSign[e.toNode] && enew.right != nodeSign[enew.toNode] );
                    // || is1 || is2  || (unitigs.at(e.toNode).sequence.length() >= 2*(K-1))
                    if(!isItWalkStarting(e.toNode) || is1 || is2  || (unitigs.at(e.toNode).ln >= 2*(K-1))) {
                        //|| (unitigs.at(e.toNode).sequence.length() >= 2*(K-1))
                        //  or (is1 && unitigs.at(uid).sequence.length() > 2*(K-1)) or (is2 && unitigs.at(uid).sequence.length() > 2*(K-1))
                        //|| is1 || is2
                         if( is1 || is2 || is3 || is4){
                        //if( ((is1 or is4) or (!ABSORBONLYTWO)) ){
                            absorbGraph[e.toNode].push_back(enew);
                             
                             if(EARLYABSORB){
                                 if(oldToNew[e.toNode].pos_in_walk < earliestAbsorberPos[absorberWalk]){
                                     earliestAbsorberPos[absorberWalk] =oldToNew[e.toNode].pos_in_walk ;
                                     assert(earliestAbsorberPos[absorberWalk]>0);
                                 }
                                 
                             }
                            
                            isAnAbsorbedWalk[oldToNew[uid].finalWalkId]=true;
                            g->addEdge(absorberWalk, absorbedWalk);
                            if(GETLOWERBOUND_CC){
                                ccAdjList[absorberWalk].push_back(absorbedWalk);
                                ccAdjList[absorbedWalk].push_back(absorberWalk);
                            }
                        }
                    }
                }
            }
            if(VARIMODE){
                if(isAnAbsorbedWalk[oldToNew[uid].finalWalkId]==false){
                    string uu=unitigs[uid].sequence;
                    if (nodeSign[uid]==false ){
                        uu=reverseComplement(uu);
                    }
                    
                    variPreFile<<pref(uu, K-1)<<endl;
                    variIntFile<<uid<<endl;
                }
            }
        }else{
            //non-walk starting vertex
        }
        prevWalkId = finalWalkId;
    }
    
    if(EARLYABSORB)
        earlyAbsorb(earliestAbsorberPos, isAnAbsorbedWalk);
    if(GETLOWERBOUND_CC){
        lowerboundcc();
    }
    
    if(VARIMODE){
        variPreFile.close();
        variSufFile.close();
        variIntFile.close();
        variWalkIdFile.close();
        
    }
    if(VARIMODE2){
        ifstream variout("variout.txt");
        string line;
        int a, b, smallk;
        while(getline(variout,line)){
            sscanf(line.c_str(), "%d %d %d", &a, &b, &smallk);
            //for each pair a->b->k
            edge_t enew;
            enew.left = true;
            enew.right = true;
            enew.smallk = (smallk+1);
            enew.toNode = b;
            absorbGraph[a].push_back(enew);
            isAnAbsorbedWalk[oldToNew[b].finalWalkId]=true;
        }
        variout.close();
    }
    delete [] isAnAbsorbedWalk;
    if(ALGOMODE==PROFILE_ONLY_ABS){
        absorbGraphNumCC_endAbosrb;
        ofstream globalStatFile("global_stat", std::fstream::out | std::fstream::app);
        globalStatFile << "ABSORB_GRAPH_NUM_CC" <<  "=" <<absorbGraphNumCC_endAbosrb << endl;
        globalStatFile.close();
        return;
    }
}

//not  tested
vector<int> AbsorbGraph::getAllUidsInWalk(int walkId){
    vector<int> uids;
    int it = sorterIndexMap[walkId];
    for(int i=it; i<sorter.size(); i++){
        int wid = get<1>(sorter[i]);
        if(wid!=walkId){
            break;
        }
        
        int uid = get<0>(sorter[i]);
        uids.push_back(uid);
    }
    return uids;
}




void AbsorbGraph::absorbGraphIsCyclicUtil(int uid, vector<char>& visited, int depth)
{
    visited[oldToNew[uid].finalWalkId] = 'g';
    
    //    if(depth>THEDEPTH){
    //        visited[oldToNew[uid].finalWalkId] = 'b';
    //        return;
    //    }
    
    vector<int> uidsNeighbors = getAllUidsInWalk(oldToNew[uid].finalWalkId);
    
    for(int alluid:uidsNeighbors){
        //vector<edge_t> &adjv = ;
        
        
        for(const auto & e : absorbGraph[alluid]){
            int i = e.toNode;
            if (visited[oldToNew[i].finalWalkId] == 'g'){
                //removed edge
            }else if (visited[oldToNew[i].finalWalkId] == 'w'){
                //add to the list                //find absorption category
                //call the absorber v[i]
                //.....orderOfUnitigs.push(i);
                
                //cout<<"adding edge: "<<alluid<<"->"<<e.toNode<<"["<<oldToNew[alluid].finalWalkId<<","<<oldToNew[e.toNode].finalWalkId<<"]"<<"scc:"<<oldToNew[alluid].sccid<<","<<oldToNew[e.toNode].sccid<<endl;
                absorbGraphCycleRemoved[alluid].push(e);
                
                if(NAIVEVARI){
                    if(e.smallk!=0){
                        smallkfile<<e.smallk-1<<endl;
                        C_abs_calc -= e.smallk-1 - 4;
                    }else{
                        smallkfile<<K-1<<endl;
                        C_abs_calc -= K-1 - 4;
                    }
                    visited[oldToNew[alluid].finalWalkId] = 'g';
                }
                
                if(e.left == nodeSign[alluid] && e.right == nodeSign[e.toNode] )
                    absorbedCategory[e.toNode] = '1';
                if(e.left == nodeSign[alluid] && e.right != nodeSign[e.toNode] )
                    absorbedCategory[e.toNode] = '2';
                if(e.left != nodeSign[alluid] && e.right != nodeSign[e.toNode] )
                    absorbedCategory[e.toNode] = '3';
                if(e.left != nodeSign[alluid] && e.right == nodeSign[e.toNode] )
                    absorbedCategory[e.toNode] = '4';
                if(ABSORBONLYTWO)
                    assert(absorbedCategory[e.toNode] != '2' && absorbedCategory[e.toNode] != '3' );
                
                absorbGraphIsCyclicUtil(i, visited, depth+1);
            }
        }
        
    }
    visited[oldToNew[uid].finalWalkId]  = 'b';
    return;
}


void  AbsorbGraph::removeCycleFromAbsorbGraphRec()
{
    vector<pair<int, int> > indegree(countNewNode, make_pair(0,0));
    vector<int> oldnode(countNewNode, 0);
    for(int i = 0; i< indegree.size(); i++){
        indegree[i] = make_pair(i, 0);
    }
    
    for (std::map<int, vector<edge_t> >::iterator it=absorbGraph.begin(); it!=absorbGraph.end(); ++it){
        int x =  it->first ;
        vector<edge_t> adjx = it->second;
        for(edge_t e: adjx){
            int walk = oldToNew[e.toNode].finalWalkId;
            oldnode[walk] =e.toNode;
            indegree[walk] = make_pair(indegree[walk].first, indegree[walk].second+1);
        }
    }

    vector<char> visited(countNewNode);
    for(int i = 0; i < countNewNode; i++)
    {
        visited[i] = (obsoleteWalkId[i]?'b':'w');
    }
    
    deque<int> dq;
    stack<int> putLast;
    
    bool SCCORDER = true;
    if(SCCORDER){
        cout<<"scc"<<endl;
        queue<int> topoorder = printSCCs();
        //delete g;
        cout<<"scc odd"<<endl;
        while(!topoorder.empty()){
            int jj =topoorder.front();
            topoorder.pop();
            dq.push_front(jj);
        }
        cout<<endl;
    }else{
        for(int ii = 0; ii <adjList.size(); ii++){
            if(indegree[oldToNew[ii].finalWalkId].second==0){
                dq.push_front(ii);
            }else{
                if(ALGOMODE==TIPANDAB_TIPLATER && (oldToNew[ii].isTip == 1 || oldToNew[ii].isTip == 2)){
                    putLast.push(ii);
                }else{
                    dq.push_back(ii);
                }
                //            else if(indegree[oldToNew[ii].finalWalkId].second==1){
                //                putLast.push(ii);
                //                //cout<<"h\n";
                //            }
            }
        }
        while(!putLast.empty()){
            int jj =putLast.top();
            putLast.pop();
            dq.push_back(jj);
        }
    }
    
    while(!dq.empty()){
        int i = dq.front();
        dq.pop_front();
        //if(absorbGraph.count(i)>0){
        //cout<<adjList.size()<<endl;
        int watch = visited[oldToNew[i].finalWalkId] ;
        if(visited[oldToNew[i].finalWalkId] == 'w' ){
            orderOfUnitigs.push(oldToNew[i].finalWalkId);
            absorbGraphIsCyclicUtil(i, visited, 0);
        }
    }
    //absorbGraph.clear();
}

void AbsorbGraph::removeCycleFromAbsorbGraph()
{
    //removeCycleFromAbsorbGraphRec();
    //postorder_master();
    //return;
    //
    char *visited = new char[countNewNode];
    int *parent= new int[adjList.size()];
    for(int i = 0; i < countNewNode; i++)
    {
        visited[i] = (obsoleteWalkId[i]?'b':'w');        // Initially mark all walks as not visited ('w') (except invalid walks)
    }
    for(int i = 0; i < adjList.size(); i++)
    {
        parent[i] = -1;
    }
    map<int, edge_t> edmap;
    stack<int> stak;
    
    
    
    vector<pair<int, int> > indegree(countNewNode, make_pair(0,0));
    vector<int> oldnode(countNewNode, 0);
    for(int i = 0; i< indegree.size(); i++){
        indegree[i] = make_pair(i, 0);
    }
    
    for (std::map<int, vector<edge_t> >::iterator it=absorbGraph.begin(); it!=absorbGraph.end(); ++it){
        int x =  it->first ;
        vector<edge_t> adjx = it->second;
        for(edge_t e: adjx){
            int walk = oldToNew[e.toNode].finalWalkId;
            oldnode[walk] =e.toNode;
            indegree[walk] = make_pair(indegree[walk].first, indegree[walk].second+1);
        }
    }
    
    deque<int> dq;
    stack<int> putLast;
    
    bool SCCORDER = true;
    if(SCCORDER){
        cout<<"scc"<<endl;
        queue<int> topoorder = printSCCs();
        while(!topoorder.empty()){
            int jj =topoorder.front();
            topoorder.pop();
            dq.push_front(jj);
        }
    }else{
        for(int ii = 0; ii <adjList.size(); ii++){
            if(indegree[oldToNew[ii].finalWalkId].second==0){
                dq.push_front(ii);
            }else{
                if(ALGOMODE==TIPANDAB_TIPLATER && (oldToNew[ii].isTip == 1 || oldToNew[ii].isTip == 2)){
                    putLast.push(ii);
                }else{
                    dq.push_back(ii);
                }
                //            else if(indegree[oldToNew[ii].finalWalkId].second==1){
                //                putLast.push(ii);
                //                //cout<<"h\n";
                //            }
            }
        }
        while(!putLast.empty()){
            int jj =putLast.top();
            putLast.pop();
            dq.push_back(jj);
        }
    }
    
    delete g;
    cout<<"[3.3.0] SCC done."<<endl;
    
    while(!dq.empty()){
        int qtop = dq.front();
        dq.pop_front();
        if(visited[oldToNew[qtop].finalWalkId] == 'w' ){
            //visited[oldToNew[qtop].finalWalkId] = 'g';
            stak.push(qtop);
            // Push the current source node.
            orderOfUnitigs.push(oldToNew[qtop].finalWalkId);
            
            
            int stringSize = 0;
            while (!stak.empty())
            {
                int uid = stak.top();
                stak.pop();
                
                if(visited[oldToNew[uid].finalWalkId] == 'w'){
                    visited[oldToNew[uid].finalWalkId] = 'g';
                    if(parent[uid]!=-1){
                        edge_t e = edmap[uid];
                        int alluid = parent[uid];
                        absorbGraphCycleRemoved[parent[uid]].push(e);
                        absorbed[oldToNew[e.toNode].finalWalkId] = true;
                        if(e.left == nodeSign[alluid] && e.right == nodeSign[e.toNode] )
                            absorbedCategory[e.toNode] = '1';
                        if(e.left == nodeSign[alluid] && e.right != nodeSign[e.toNode] )
                            absorbedCategory[e.toNode] = '2';
                        if(e.left != nodeSign[alluid] && e.right != nodeSign[e.toNode] )
                            absorbedCategory[e.toNode] = '3';
                        if(e.left != nodeSign[alluid] && e.right == nodeSign[e.toNode] )
                            absorbedCategory[e.toNode] = '4';
                        if(ABSORBONLYTWO)
                            assert(absorbedCategory[e.toNode] != '2' && absorbedCategory[e.toNode] != '3' );
                    }
                    
                    vector<int> uidsNeighbors = getAllUidsInWalk( oldToNew[uid].finalWalkId);
                    for(int alluid:uidsNeighbors){
                        stringSize+=unitigs.at(alluid).sequence.length();
                        vector<edge_t> adjv = absorbGraph[alluid];
                        for(edge_t e : adjv){
                            int i = e.toNode;
                            if (visited[oldToNew[i].finalWalkId] == 'g'){
                                //removed edge
                                //edmap[e.toNode] = e;
                                //parent[e.toNode] = alluid;
                            }else if (visited[oldToNew[i].finalWalkId] == 'w'){
                                //cout<<"adding edge: "<<alluid<<"->"<<e.toNode<<"["<<oldToNew[alluid].finalWalkId<<","<<oldToNew[e.toNode].finalWalkId<<"]"<<"scc:"<<oldToNew[alluid].sccid<<","<<oldToNew[e.toNode].sccid<<endl;
                                //visited[oldToNew[alluid].finalWalkId] = 'g';
                                edmap[e.toNode] = e;
                                parent[e.toNode] = alluid;
                                stak.push(e.toNode);
                            }
                        }
                    }
                }
                
                visited[oldToNew[uid].finalWalkId]  = 'b';
            }
            //reservedStringSize.push(stringSize);
        }
    }
    delete[] visited;
    delete[] parent;
    absorbGraph.clear();
    postorder_master();
}


void AbsorbGraph::iterSpellTested(int startWalkIndex2, int depth, int Kpass, vector<char>& color, int stringSize){
    segfaultfile<<startWalkIndex2<<" walk: "<<get<1>(sorter[startWalkIndex2])<<endl;
    
    //if(isItAPrintedWalk[get<1>(sorter[startWalkIndex2])]) return "";
    //vector<char> color(adjList.size(), 'w');
    stack<int> recurStak;//startWalkIndex
    recurStak.push(startWalkIndex2);
    
    while(!recurStak.empty()){
        
        segfaultfile<<"stack: "<<recurStak.size()<<" top:"<<get<1>(sorter[recurStak.top()])<<endl;
        //cout<<recurStak.top()<<endl;
        //int thestart =  recurStak.top();
        int currSorterIndex = recurStak.top();
        recurStak.pop();
        
        bool isThisAbsorbedWalk = false;;
        char walkAbsorbedCategory = '0';
        string unitigString = "";
        
        string& walkString = constructedStrings[get<1>(sorter[currSorterIndex])];
        //walkString.reserve(stringSize);
        
        while(true){
            
            //cout<<walkString<<endl;
            assert(currSorterIndex<sorter.size());
            MyTypes::fourtuple n = sorter[currSorterIndex];
            int finalWalkId = get<1>(n);
            
            depthfile<<get<0>(n)<<" "<<depth<<endl;
            
            if (isItAPrintedWalk[get<1>(sorter[currSorterIndex])]){
                //color[currSorterIndex] = 'b';
                //recurStak.pop();
                return;
            }
//            if(color[currSorterIndex] == 'b'){
//                return;
//            }
            int uid = get<0>(n);
            
            if(color[currSorterIndex]=='w'){
                recurStak.push(currSorterIndex);
                if(absorbGraphCycleRemoved[uid].size() > 0){         /*populate two types of stacks*/
                    stack<edge_t> st = absorbGraphCycleRemoved[uid];
                    while(!st.empty()){
                        edge_t st_top = st.top();
                        if(absorbedCategory[st_top.toNode]=='3' or absorbedCategory[st_top.toNode]=='4'  ){
                            recurStak.push(sorterIndexMap[oldToNew[st_top.toNode].finalWalkId]);
                        }else if(absorbedCategory[st_top.toNode]=='1' or absorbedCategory[st_top.toNode]=='2' ){
                            recurStak.push(sorterIndexMap[oldToNew[st_top.toNode].finalWalkId]);
                        }else{
                            assert(false);
                        }
                        st.pop();
                    }
                }
                color[currSorterIndex] = 'g';
                break;
            }
//            if(color[currSorterIndex]=='b'){
//                cout<<"WARNING: "<<currSorterIndex<<" uid:" << get<0>(sorter[currSorterIndex])<<" duplicate."<<endl;
//                break;
//            }
            assert(color[currSorterIndex]=='g');
            // past this mean color is grey
            if(nodeSign[uid] == false){
                unitigString =  reverseComplement(unitigs.at(uid).sequence);
            }else{
                unitigString =  (unitigs.at(uid).sequence);
            }
            
            stack<edge_t> stType12;
            stack<edge_t> stType34;
            if(absorbGraphCycleRemoved[uid].size() > 0){         /*populate two types of stacks*/
                stack<edge_t> st = absorbGraphCycleRemoved[uid];
                while(!st.empty()){
                    edge_t st_top = st.top();
                    if(absorbedCategory[st_top.toNode]=='3' or absorbedCategory[st_top.toNode]=='4'  ){
                        stType34.push(st_top);
                    }else if(absorbedCategory[st_top.toNode]=='1' or absorbedCategory[st_top.toNode]=='2' ){
                        stType12.push(st_top);
                    }else{
                        assert(false);
                    }
                    st.pop();
                }
            }
            
            
            //walkstarting ####
            if( isItWalkStarting(uid)){
                if(absorbedCategory[uid]=='0'){ //not absorbed
                    if(unitigString.length()<2*(K-1)){
                        walkString += unitigString;
                    }else{
                        walkString += splitA(unitigString, Kpass);
                        while(!stType34.empty()){
                            int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                            stType34.pop();
                            assert(constructedStrings[get<1>(sorter[sindex])]!="");
                            walkString += constructedStrings[get<1>(sorter[sindex])];
                            constructedStrings[get<1>(sorter[sindex])] = "";
                            constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                        }
                        walkString += splitX(unitigString, Kpass);
                        walkString += splitB(unitigString, Kpass);
                    }
                }else{
                    isThisAbsorbedWalk=true;
                    walkAbsorbedCategory =absorbedCategory[uid];
                    
                    if(unitigString.length()>=2*(K-1)){
                        string sign = (absorbedCategory[uid]=='2' or absorbedCategory[uid]=='4')?"-":"+";
                        if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3'){
                            //walkString += splitA(unitigString, Kpass);
                            //walkString += splitX(unitigString, Kpass);
                            string apart = cutSuf(unitigString, Kpass);
                            walkString += pref(apart, Kpass);
                            while(!stType34.empty()){
                                int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                                stType34.pop();
                                assert(constructedStrings[get<1>(sorter[sindex])]!="");
                                walkString += constructedStrings[get<1>(sorter[sindex])];
                                constructedStrings[get<1>(sorter[sindex])] = "";
                                constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                            }
                            walkString += cutPref(apart, Kpass);
                            walkString += sign;
                            
                        }else{
                            walkString += sign;
                            while(!stType34.empty()){
                                int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                                stType34.pop();
                                assert(constructedStrings[get<1>(sorter[sindex])]!="");
                                walkString += constructedStrings[get<1>(sorter[sindex])];
                                constructedStrings[get<1>(sorter[sindex])] = "";
                                constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                            }
                            walkString += splitX(unitigString, Kpass);
                            walkString += splitB(unitigString, Kpass);
                        }
                    }else{
                        string sign = (absorbedCategory[uid]=='2' or absorbedCategory[uid]=='4')?"-":"+";
                        if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3'){
                            //walkString += splitA(unitigString, Kpass);
                            //swalkString += splitX(unitigString, Kpass);
                            walkString += cutSuf(unitigString, Kpass);
                            walkString += sign;
                            
                        }else{
                            walkString += sign;
                            walkString += splitX(unitigString, Kpass);
                            walkString += splitB(unitigString, Kpass);
                        }
                    }
                    
                }
                while(!stType12.empty()){
                    //assert(false);
                    int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                    stType12.pop();
                    assert(constructedStrings[get<1>(sorter[sindex])]!="");
                    walkString += constructedStrings[get<1>(sorter[sindex])];
                    constructedStrings[get<1>(sorter[sindex])] = "";
                    constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                }
            }else{ //not walk starting
                if(absorbedCategory[uid]=='0'){
                    //34
                    while(!stType34.empty()){
                        int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                        stType34.pop();
                        assert(constructedStrings[get<1>(sorter[sindex])]!="");
                        walkString += constructedStrings[get<1>(sorter[sindex])];
                        constructedStrings[get<1>(sorter[sindex])] = "";
                        constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                    }
                    walkString += splitX(unitigString, Kpass);
                    walkString += splitB(unitigString, Kpass);
                    //12
                    while(!stType12.empty()){
                        int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                        stType12.pop();
                        walkString += constructedStrings[get<1>(sorter[sindex])];
                        constructedStrings[get<1>(sorter[sindex])] = "";
                        constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                    }
                }
                //earlyAbsorb
                if(absorbedCategory[uid]!='0'){
                    isThisAbsorbedWalk=true;
                    walkAbsorbedCategory =absorbedCategory[uid];
                    assert(stType34.empty());
                    assert(stType12.empty());
                    
                    if(absorbedCategory[uid]=='1' or absorbedCategory[uid]=='4' ){
                        string signs;
                        if(absorbedCategory[uid]=='1') signs="+";
                        else if(absorbedCategory[uid]=='4') signs="-";
                        walkString = cutSuf(walkString, Kpass) + signs + cutPref(unitigString, Kpass);
                    }
                                            
                    if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3' ){
                        string signs;
                        if(absorbedCategory[uid]=='3') signs="+";
                        else if(absorbedCategory[uid]=='2') signs="-";
                        walkString = cutSuf(walkString, Kpass) + unitigString;
                        walkString = cutSuf(walkString, Kpass) + signs;
                    }
                }
                
            }
            
            
            
            if(color[currSorterIndex]=='g'){
                color[currSorterIndex] = 'b';
                //recurStak.pop();
            }else{
                assert(false);
            }
            
            
            
            if(currSorterIndex+1 == sorter.size() or get<1>(sorter[currSorterIndex+1]) != finalWalkId){
                if(ABSORBONLYTWO){
                    if(walkAbsorbedCategory=='1') walkString="["+walkString+"]";
                    else if(walkAbsorbedCategory=='4') walkString="("+walkString+")";
                }else{
                    //if(absorbedCategory[get<0>(sorter[sorterIndexMap[finalWalkId]])] != '0') walkString="["+walkString+"]";
                    if(absorbed[finalWalkId] == true) walkString="["+walkString+"]";
                }
                isItAPrintedWalk[finalWalkId] = true;
                break;
            }
            currSorterIndex++;
        }
    }
}





inline bool file_exists_test (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}


string IntToString(int a)
{
    ostringstream temp;
    temp << a;
    return temp.str();
}



//void deleteWalkFile(int wid){
//    string filename = "ustwalk" + IntToString(wid);
//    if(!file_exists_test(filename)){
//        cout<<filename + " File does not exist"<<endl;
//        exit(3);
//    }
//    system(("rm -rf "+filename).c_str());
//}

string readAndDeleteWalkFile(int wid){
    string filename = "ustwalk" + IntToString(wid);
    if(!file_exists_test(filename)){
        cout<<"\n"<<filename + " File does not exist"<<endl;
        exit(3);
    }
    ifstream f(filename); //taking file as inputstream
    string str;
    if(f) {
       ostringstream ss;
       ss << f.rdbuf(); // reading data
       str = ss.str();
    }
    f.close();
    system(("rm -rf "+filename).c_str());
    return str;
}



void AbsorbGraph::iterSpellSlowDisk(int startWalkIndex2, int depth, int Kpass, vector<char>& color){
   
    fstream FOUT;
    //("ust_unitiglabel.txt");
    
    segfaultfile<<startWalkIndex2<<" walk: "<<get<1>(sorter[startWalkIndex2])<<endl;
    
    //if(isItAPrintedWalk[get<1>(sorter[startWalkIndex2])]) return "";
    //vector<char> color(adjList.size(), 'w');
    stack<int> recurStak;//startWalkIndex
    recurStak.push(startWalkIndex2);
    
    while(!recurStak.empty()){
        
        segfaultfile<<"stack: "<<recurStak.size()<<" top:"<<get<1>(sorter[recurStak.top()])<<endl;
        //cout<<recurStak.top()<<endl;
        //int thestart =  recurStak.top();
        int currSorterIndex = recurStak.top();
        recurStak.pop();
        
        bool isThisAbsorbedWalk = false;;
        char walkAbsorbedCategory = '0';
        string unitigString = "";
        
        string& walkString = constructedStrings[get<1>(sorter[currSorterIndex])];
        FOUT.open("ustwalk"+IntToString(get<1>(sorter[currSorterIndex])),std::fstream::app);
        while(true){
            
            //cout<<walkString<<endl;
            assert(currSorterIndex<sorter.size());
            MyTypes::fourtuple n = sorter[currSorterIndex];
            int finalWalkId = get<1>(n);
            
            depthfile<<get<0>(n)<<" "<<depth<<endl;
            
            if (isItAPrintedWalk[get<1>(sorter[currSorterIndex])]){
                //color[currSorterIndex] = 'b';
                //recurStak.pop();
                return;
            }
//            if(color[currSorterIndex] == 'b'){
//                return;
//            }
            int uid = get<0>(n);
            
            if(color[currSorterIndex]=='w'){
                int wid = get<1>(n);
                recurStak.push(currSorterIndex);
                if(absorbGraphCycleRemoved[uid].size() > 0){         /*populate two types of stacks*/
                    stack<edge_t> st = absorbGraphCycleRemoved[uid];
                    while(!st.empty()){
                        edge_t st_top = st.top();
                        if(absorbedCategory[st_top.toNode]=='3' or absorbedCategory[st_top.toNode]=='4'  ){
                            recurStak.push(sorterIndexMap[oldToNew[st_top.toNode].finalWalkId]);
                        }else if(absorbedCategory[st_top.toNode]=='1' or absorbedCategory[st_top.toNode]=='2' ){
                            recurStak.push(sorterIndexMap[oldToNew[st_top.toNode].finalWalkId]);
                        }else{
                            assert(false);
                        }
                        st.pop();
                    }
                }
                
                color[currSorterIndex] = 'g';
                break;
            }
//            if(color[currSorterIndex]=='b'){
//                cout<<"WARNING: "<<currSorterIndex<<" uid:" << get<0>(sorter[currSorterIndex])<<" duplicate."<<endl;
//                break;
//            }
            assert(color[currSorterIndex]=='g');
            // past this mean color is grey
            if(nodeSign[uid] == false){
                unitigString =  reverseComplement(unitigs.at(uid).sequence);
            }else{
                unitigString =  (unitigs.at(uid).sequence);
            }
            
            stack<edge_t> stType12;
            stack<edge_t> stType34;
            if(absorbGraphCycleRemoved[uid].size() > 0){         /*populate two types of stacks*/
                stack<edge_t> st = absorbGraphCycleRemoved[uid];
                while(!st.empty()){
                    edge_t st_top = st.top();
                    if(absorbedCategory[st_top.toNode]=='3' or absorbedCategory[st_top.toNode]=='4'  ){
                        stType34.push(st_top);
                    }else if(absorbedCategory[st_top.toNode]=='1' or absorbedCategory[st_top.toNode]=='2' ){
                        stType12.push(st_top);
                    }else{
                        assert(false);
                    }
                    st.pop();
                }
            }
            
            int walkFileId = oldToNew[uid].finalWalkId;
            
            //walkstarting ####
            
            
            //cout<<walkFileId<<endl;
            
            
            if( isItWalkStarting(uid)){
                
                if(absorbedCategory[uid]=='0'){ //not absorbed
                    if(unitigString.length()<2*(K-1)){
                        
                        //walkString += unitigString;
                        FOUT<<unitigString;
                        
                    }else{
                        //walkString += splitA(unitigString, Kpass);
                        FOUT<<splitA(unitigString, Kpass);
                        
                        while(!stType34.empty()){
                            int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                            stType34.pop();
                            assert(constructedStrings[get<1>(sorter[sindex])]!="");
                            
                            //walkString += constructedStrings[get<1>(sorter[sindex])];
                            FOUT<<"["<<readAndDeleteWalkFile(get<1>(sorter[sindex]))<<"]";
                            
                            constructedStrings[get<1>(sorter[sindex])] = "";
                            constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                           
                            
                        }
                        //walkString += splitX(unitigString, Kpass);
                        FOUT<<splitX(unitigString, Kpass);
                        
                        //walkString += splitB(unitigString, Kpass);
                        FOUT<<splitB(unitigString, Kpass);
                    }
                }else{
                    isThisAbsorbedWalk=true;
                    walkAbsorbedCategory =absorbedCategory[uid];
                    
                    if(unitigString.length()>=2*(K-1)){
                        string sign = (absorbedCategory[uid]=='2' or absorbedCategory[uid]=='4')?"-":"+";
                        if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3'){
                            string apart = cutSuf(unitigString, Kpass);
                            
                            //walkString += pref(apart, Kpass);
                            FOUT<< pref(apart, Kpass);
                            
                            while(!stType34.empty()){
                                int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                                stType34.pop();
                                assert(constructedStrings[get<1>(sorter[sindex])]!="");
                                
                                //walkString += constructedStrings[get<1>(sorter[sindex])];
                                FOUT<<"["<<readAndDeleteWalkFile(get<1>(sorter[sindex]))<<"]";
                                
                                constructedStrings[get<1>(sorter[sindex])] = "";
                                constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                            }
                            //walkString += cutPref(apart, Kpass);
                            FOUT<<cutPref(apart, Kpass);
                            
                            //walkString += sign;
                            FOUT<<sign;
                            
                        }else{
                            //walkString += sign;
                            FOUT<<sign;
                            
                            while(!stType34.empty()){
                                int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                                stType34.pop();
                                assert(constructedStrings[get<1>(sorter[sindex])]!="");
                                
                                //walkString += constructedStrings[get<1>(sorter[sindex])];
                                FOUT<<"["<<readAndDeleteWalkFile(get<1>(sorter[sindex]))<<"]";
                                
                                constructedStrings[get<1>(sorter[sindex])] = "";
                                constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                            }
                            //walkString += splitX(unitigString, Kpass);
                            FOUT<<splitX(unitigString, Kpass);
                            
                            //walkString += splitB(unitigString, Kpass);
                            FOUT<<splitB(unitigString, Kpass);
                        }
                    }else{
                        string sign = (absorbedCategory[uid]=='2' or absorbedCategory[uid]=='4')?"-":"+";
                        if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3'){
                            //walkString += splitA(unitigString, Kpass);
                            //swalkString += splitX(unitigString, Kpass);
                            //walkString += cutSuf(unitigString, Kpass);
                            FOUT<<cutSuf(unitigString, Kpass);
                            
                            //walkString += sign;
                            FOUT<<sign;
                            
                        }else{
                            //walkString += sign;
                            FOUT<<sign;
                            
                            //walkString += splitX(unitigString, Kpass);
                            FOUT<<splitX(unitigString, Kpass);
                            
                            //walkString += splitB(unitigString, Kpass);
                            FOUT<<splitB(unitigString, Kpass);
                        }
                    }
                    
                }
                while(!stType12.empty()){
                    //assert(false);
                    int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                    stType12.pop();
                    assert(constructedStrings[get<1>(sorter[sindex])]!="");
                    
                    //walkString += constructedStrings[get<1>(sorter[sindex])];
                    FOUT<<"["<<readAndDeleteWalkFile(get<1>(sorter[sindex]))<<"]";
                    
                    constructedStrings[get<1>(sorter[sindex])] = "";
                    constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                }
            }else{ //not walk starting
                if(absorbedCategory[uid]=='0'){
                    //34
                    while(!stType34.empty()){
                        int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                        stType34.pop();
                        assert(constructedStrings[get<1>(sorter[sindex])]!="");
                        
                        //walkString += constructedStrings[get<1>(sorter[sindex])];
                        FOUT<<"["<<readAndDeleteWalkFile(get<1>(sorter[sindex]))<<"]";
                        
                        constructedStrings[get<1>(sorter[sindex])] = "";
                        constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                    }
                    //walkString += splitX(unitigString, Kpass);
                    FOUT<<splitX(unitigString, Kpass);
                    
                    //walkString += splitB(unitigString, Kpass);
                    FOUT<<splitB(unitigString, Kpass);
                    
                    //12
                    while(!stType12.empty()){
                        int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                        stType12.pop();
                        
                        //walkString += constructedStrings[get<1>(sorter[sindex])];
                        FOUT<<"["<<readAndDeleteWalkFile(get<1>(sorter[sindex]))<<"]";
                        
                        constructedStrings[get<1>(sorter[sindex])] = "";
                        constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
                    }
                }
                //earlyAbsorb
                if(absorbedCategory[uid]!='0'){
                    assert(false);
                    isThisAbsorbedWalk=true;
                    walkAbsorbedCategory =absorbedCategory[uid];
                    assert(stType34.empty());
                    assert(stType12.empty());
                    
                    if(absorbedCategory[uid]=='1' or absorbedCategory[uid]=='4' ){
                        string signs;
                        if(absorbedCategory[uid]=='1') signs="+";
                        else if(absorbedCategory[uid]=='4') signs="-";
                        walkString = cutSuf(walkString, Kpass) + signs + cutPref(unitigString, Kpass);
                        
                        
                    }
                                            
                    if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3' ){
                        string signs;
                        if(absorbedCategory[uid]=='3') signs="+";
                        else if(absorbedCategory[uid]=='2') signs="-";
                        walkString = cutSuf(walkString, Kpass) + unitigString;
                        walkString = cutSuf(walkString, Kpass) + signs;
                    }
                }
                
            }
            
            
            
            if(color[currSorterIndex]=='g'){
                color[currSorterIndex] = 'b';
                //recurStak.pop();
            }else{
                assert(false);
            }
            
            
            
            if(currSorterIndex+1 == sorter.size() or get<1>(sorter[currSorterIndex+1]) != finalWalkId){
                if(ABSORBONLYTWO){
                    if(walkAbsorbedCategory=='1') walkString="["+walkString+"]";
                    else if(walkAbsorbedCategory=='4') walkString="("+walkString+")";
                }else{
                    //if(absorbedCategory[get<0>(sorter[sorterIndexMap[finalWalkId]])] != '0') walkString="["+walkString+"]";
                    if(absorbed[finalWalkId] == true) walkString="["+walkString+"]";
                }
                isItAPrintedWalk[finalWalkId] = true;
                break;
            }
            currSorterIndex++;
        }
        FOUT.close();
    }
    
}



//
//void AbsorbGraph::tipSpell(){
//    return;
//    ofstream uidSequence;
//    string uidSeqFilename = "uidSeq.usttemp"; //"uidSeq"+ mapmode[ALGOMODE] +".txt"
//   uidSequence.open(uidSeqFilename);
//
//    int finalUnitigSerial = 0;
//    for(MyTypes::fourtuple n : sorter){
//        int uid = get<0>(n);
//        int bcalmid = unitigs.at(uid).serial;
////                        int finalWalkId = get<1>(n);
////                        int pos_in_walk = get<2>(n);
////                        int isTip = get<3>(n);
////                        cout<<uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<" "<<oldToNew[uid].isWalkEnd<< " was merged: "<< merged[oldToNew[uid].finalWalkId]<< endl;
//        uidSequence << finalUnitigSerial <<" "<< bcalmid << endl;
//        finalUnitigSerial++;
//    }
//    uidSequence.close();
//
//
//    //system();
//
//    //keep the sequences only
//    system(("awk '!(NR%2)' "+UNITIG_FILE+" > seq.usttemp").c_str());
//    system("sort -n -k 2 -o uidSeq.usttemp uidSeq.usttemp");
//    system("paste -d' ' uidSeq.usttemp seq.usttemp > merged.usttemp ");
//    system("sort -n -k 1 -o merged.usttemp merged.usttemp");
//
//    system("cat  merged.usttemp  | cut -d' ' -f3 >  seq.usttemp");
//
//
//    //string walkString = "";
//    ofstream tipFile("ust_ess_tip.txt");
//
//
//    ifstream sequenceStringFile ("seq.usttemp");
//
//    //for(int si = 0; si<sorter.size(); si++){
//        int startWalkIndex = 0;
//    int prevwalk = -1;
//        while(true){
//            assert(startWalkIndex<sorter.size());
//            MyTypes::fourtuple &n = sorter[startWalkIndex];
//            int finalWalkId = get<1>(n);
//
//            int uid = get<0>(n);
//            int isTip = get<3>(n);
//            int pos_in_walk =get<2>(n);
//            //cout<<uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;
//
////            string unitigString;
////            if(nodeSign[uid] == false){
////                unitigString =  reverseComplement(unitigs.at(uid).sequence);
////            }else{
////                unitigString =  (unitigs.at(uid).sequence);
////            }
//
//
////
//            string unitigString;
//            string sequenceFromFile = "";//getline
//            getline (sequenceStringFile,sequenceFromFile);
//            if(nodeSign[uid] == false){
//                unitigString =  reverseComplement(sequenceFromFile);
//            }else{
//                unitigString =  sequenceFromFile;
//            }
//
//
//
//            if(prevwalk!=finalWalkId){
//                tipFile<<">\n";
//            }
//            prevwalk = finalWalkId;
//
////            if( MODE_ABSORPTION_TIP && isTip == 0){//
////                walkString = plus_strings(walkString, unitigString, K);
////
//            if(isTip == 0 ){
//
//                if(startWalkIndex==0)
//                    tipFile<<unitigString;
//                else if(finalWalkId != get<1>(sorter[startWalkIndex-1]))
//                    tipFile<<unitigString;
//                else
//                    tipFile<<unitigString.substr(K - 1, unitigString.length() - (K - 1));
//                    //tipFile<<cutPref(unitigString, K);
//
//
//            }else if(isTip==1){ //right R   R    ]   ]   ]   ]
//                //cut prefix: correct
//                if(0==0){
//                    unitigString = unitigString.substr(K - 1, unitigString.length() - (K - 1));
////                    if(walkString.length()<K){
////                        cout<<"pos: "<<walkString.length()<<endl;
////                    }
//                    tipFile<<"(";
//                    tipFile<<unitigString;
//                    tipFile<<")";
//                }
//                if(1==0){
//                    tipFile<<">pref\n"<<unitigString<<endl;
//                }
//
//            }else if(isTip==2 && MODE_ABSORPTION_TIP){ //left L   L    [ [ [
//                //cut suffix: correct
//                if(0==0){
//                    unitigString = unitigString.substr(0, unitigString.length() - (K - 1));
////                    if(walkString.length()<K){
////                        cout<<"pos: "<<walkString.length()<<endl;
////                    }
//                    tipFile<<"{";
//                    tipFile<<unitigString;
//                    tipFile<<"}";
//                }
//                if(1==0){
//                    tipFile<<">suf\n"<<unitigString<<endl;
//                }
//            }
//
//            if(startWalkIndex+1 == sorter.size()) {
////                int brackets1 = std::count(walkString.begin(), walkString.end(), '(');
////                int brackets2 = std::count(walkString.begin(), walkString.end(), ')');
////                int stringPlus = std::count(walkString.begin(), walkString.end(), '{');
////                int stringMinus = std::count(walkString.begin(), walkString.end(), '}');
////
////                C_tip_special += brackets1- brackets2 -stringPlus-stringMinus;
////                C_tip_ustitch += walkString.length();
////
//                //tipFile<<">\n"<<walkString<<endl;
//                tipFile<<endl;
//                V_tip_ustitch++;
//                break;
//            }else if(get<1>(sorter[startWalkIndex+1]) != finalWalkId){
//                //tipFile<<">\n"<<walkString<<endl;
//                tipFile<<endl;
//
//                V_tip_ustitch++;
//                //walkString = "";
//                //walkString.shrink_to_fit();
//                //break;
//            }
//            startWalkIndex++;
//        }
//    //}
//
//    sequenceStringFile.close();
//    tipFile.close();
//}




void AbsorbGraph::recursiveWalkStringMaker(int startWalkIndex, int depth, int Kpass){
    //iterWalkStringMaker(startWalkIndex, depth, Kpass);
    //return;
    
    //recursivePrinter(startWalkIndex, isItAPrintedWalk, sorter, sorterIndexMap,  absorbGraphCycleRemoved, orderOfUnitigs, absorbedCategory, depth, K);
    //return "";
    //return iterWalkStringMaker(startWalkIndex, isItAPrintedWalk, sorter, sorterIndexMap,  absorbGraphCycleRemoved, orderOfUnitigs, absorbedCategory, depth, Kpass);
    
    segfaultfile<<startWalkIndex<<endl;
    assert(startWalkIndex<adjList.size());
    if(isItAPrintedWalk[get<1>(sorter[startWalkIndex])]) return;
    //cout<<depth<<endl;
    bool isThisAbsorbedWalk = false;;
    char walkAbsorbedCategory = '0';
    string unitigString = "";
    string& walkString = constructedStrings[get<1>(sorter[startWalkIndex])];
    
    while(true){
        assert(startWalkIndex<sorter.size());
        MyTypes::fourtuple &n = sorter[startWalkIndex];
        int finalWalkId = get<1>(n);
        
        //depthfile<<get<0>(n)<<" "<<depth<<endl;
        
        //@ABSORB
        if( MODE_ABSORPTION_NOTIP ){
            if (isItAPrintedWalk[finalWalkId]) return;
        }
        
        int uid = get<0>(n);
        int isTip = get<3>(n);
        //cout<<uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;
        
        if(nodeSign[uid] == false){
            unitigString =  reverseComplement(unitigs.at(uid).sequence);
        }else{
            unitigString =  (unitigs.at(uid).sequence);
        }
        
        stack<edge_t> stType12;
        stack<edge_t> stType34;
        if(absorbGraphCycleRemoved[uid].size() > 0){         //populate two types of stacks
            stack<edge_t> & st = absorbGraphCycleRemoved[uid];
            while(!st.empty()){
                edge_t st_top = st.top();
                assert(isItAPrintedWalk[oldToNew[st_top.toNode].finalWalkId]==false);
                if(absorbedCategory[st_top.toNode]=='3' or absorbedCategory[st_top.toNode]=='4'  ){
                    stType34.push(st_top);
                }else if(absorbedCategory[st_top.toNode]=='1' or absorbedCategory[st_top.toNode]=='2' ){
                    stType12.push(st_top);
                }else{
                    cout<<"errorrrrrrrrrrr"<<endl;
                    assert(false);
                }
                st.pop();
            }
        }
        
        //walk starting AND not absorbed
        if( isItWalkStarting(uid) && absorbedCategory[uid]=='0' && MODE_ABSORPTION_NOTIP){
            walkString+= cutSuf(unitigString, Kpass);
            
            while(!stType34.empty()){
                int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                char isSmallK =stType34.top().isSmallK;
                
                if(isItAPrintedWalk[oldToNew[stType34.top().toNode].finalWalkId]==true){
                    stType34.pop();
                    continue;
                }
                
                stType34.pop();
                recursiveWalkStringMaker(sindex, depth+1, K);
                walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
                constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
            }
            walkString+= suf(unitigString, Kpass);
            
            while(!stType12.empty()){
                int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                char isSmallK =stType12.top().isSmallK;
                if(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId]==true){
                    stType12.pop();
                    continue;
                }
                
                stType12.pop();
                recursiveWalkStringMaker(sindex, depth+1, K);
                walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
            }
        }
        
        //not walk starting AND not absorbed
        if(!isItWalkStarting(uid) && absorbedCategory[uid]=='0' && MODE_ABSORPTION_NOTIP){
            while(!stType34.empty()){
                int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                char isSmallK =stType34.top().isSmallK;
                
                if(isItAPrintedWalk[oldToNew[stType34.top().toNode].finalWalkId]==true){
                    stType34.pop();
                    continue;
                }
                stType34.pop();
                
                assert(sindex<adjList.size());
                recursiveWalkStringMaker(sindex, depth+1, K);
                walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
            }
            walkString+= cutPref(unitigString, Kpass);
            
            while(!stType12.empty()){
                int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                
                char isSmallK =stType12.top().isSmallK;
                if(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId]==true){
                    stType12.pop();
                    continue;
                }
                
                stType12.pop();
                recursiveWalkStringMaker(sindex, depth+1, K);
                walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
            }
        }
        
        //not walk starting AND absorbed
        //        if(pos_in_walk != 1 && absorbedCategory[uid]!='0' && MODE_ABSORPTION_NOTIP){
        //            assert(false);
        //        }
        
        //ERROR ONE
        if(isItWalkStarting(uid) && absorbedCategory[uid]!='0' && MODE_ABSORPTION_NOTIP){

            isThisAbsorbedWalk=true;
            walkAbsorbedCategory =absorbedCategory[uid];
            
            if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3'){
                walkString+=cutSuf(unitigString, Kpass);
            }
            if(!ABSORBONLYTWO){
                if(absorbedCategory[uid]=='1') walkString+="+";
                else if(absorbedCategory[uid]=='4') walkString+="-";
            }
            
            if(absorbedCategory[uid]=='1' or absorbedCategory[uid]=='4' ){
                walkString+= cutPref(unitigString, Kpass);
                while(!stType12.empty()){
                    //assert(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId] == false);
                    int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                    char isSmallK =stType12.top().isSmallK;
                    
                    if(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId]==true){
                        stType12.pop();
                        continue;
                    }
                    
                    stType12.pop();
                    recursiveWalkStringMaker(sindex, depth+1, K);
                    walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
                }
            }

            if(!ABSORBONLYTWO){
                if(absorbedCategory[uid]=='3') walkString+="+";
                else if(absorbedCategory[uid]=='2') walkString+="-";
                
                while(!stType12.empty()){
                    int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                    char isSmallK =stType12.top().isSmallK;
                    
                    if(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId]==true){
                        stType12.pop();
                        continue;
                    }
                    stType12.pop();
                    recursiveWalkStringMaker(sindex, depth+1, K);
                    walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
                }
            }
        }
        
        //earlyAbsorb
        if(!isItWalkStarting(uid) && absorbedCategory[uid]!='0' && MODE_ABSORPTION_NOTIP){
            isThisAbsorbedWalk=true;
            walkAbsorbedCategory =absorbedCategory[uid];
            assert(stType34.empty());
            assert(stType12.empty());
            
            if(absorbedCategory[uid]=='1' or absorbedCategory[uid]=='4' ){
                string signs;
                if(absorbedCategory[uid]=='1') signs="+";
                else if(absorbedCategory[uid]=='4') signs="-";
                
                walkString = cutSuf(walkString, Kpass) + signs + cutPref(unitigString, Kpass);
            }
            
            if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3' ){
                //if(!ABSORBONLYTWO){
                string signs;
                if(absorbedCategory[uid]=='3') signs="+";
                else if(absorbedCategory[uid]=='2') signs="-";
                walkString = cutSuf(walkString, Kpass) + unitigString;
                walkString = cutSuf(walkString, Kpass) + signs;
                // }
            }
        }
        
        if( MODE_ABSORPTION_TIP && isTip == 0){//
            //walkString+= cutPref(unitigString, K);
            walkString = plus_strings(walkString, unitigString, K);
            
        }else if(isTip==1 && MODE_ABSORPTION_TIP){ //right R   R    ]   ]   ]   ]
            //cut prefix: correct
            if(0==0){
                unitigString = unitigString.substr(K - 1, unitigString.length() - (K - 1));
                if(walkString.length()<K){
                    cout<<"pos: "<<walkString.length()<<endl;
                }
                walkString += "(" + unitigString + ")";
            }
            if(1==0){
                tipFile<<">pref\n"<<unitigString<<endl;
            }
            
        }else if(isTip==2 && MODE_ABSORPTION_TIP){ //left L   L    [ [ [
            //cut suffix: correct
            if(0==0){
                unitigString = unitigString.substr(0, unitigString.length() - (K - 1));
                if(walkString.length()<K){
                    cout<<"pos: "<<walkString.length()<<endl;
                }
                walkString += "{" + unitigString + "}";
            }
            if(1==0){
                tipFile<<">suf\n"<<unitigString<<endl;
            }
        }
        
        if(startWalkIndex+1 == sorter.size()) {
            //print previous walk
            // tipDebugFile<<">"<<finalWalkId << " " << uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;
            // V_tip_ustitch++;
            // C_tip_ustitch+=walkString.length();
            //
            //  tipDebugFile<<walkString<<endl;
            //  tipFile<< walkString<<endl;
            
            if(MODE_ABSORPTION_NOTIP){
                isItAPrintedWalk[finalWalkId] = true;
            }
            
            break;
        }else if(get<1>(sorter[startWalkIndex+1]) != finalWalkId){
            //print previous walk
            //  tipDebugFile<<">"<<finalWalkId << " " << uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;
            //           V_tip_ustitch++;
            //            C_tip_ustitch+=walkString.length();
            //
            //   tipDebugFile<<walkString<<endl;
            //           tipFile<< walkString<<endl;
            
            isItAPrintedWalk[finalWalkId] = true;
            break;
        }
        startWalkIndex++;
    }
    if(ABSORBONLYTWO){
        if(walkAbsorbedCategory=='1') walkString="["+walkString+"]";
        else if(walkAbsorbedCategory=='4') walkString="("+walkString+")";
    }else{
        if(isThisAbsorbedWalk) walkString="["+walkString+"]";
    }

    constructedStrings[get<1>(sorter[startWalkIndex])] = walkString;
}



void AbsorbGraph::recursiveWalkStringMakerOld(int startWalkIndex, int depth, int Kpass, ofstream &FOUT){
    //iterWalkStringMaker(startWalkIndex, depth, Kpass);
    //return;
    
    //recursivePrinter(startWalkIndex, isItAPrintedWalk, sorter, sorterIndexMap,  absorbGraphCycleRemoved, orderOfUnitigs, absorbedCategory, depth, K);
    //return "";
    //return iterWalkStringMaker(startWalkIndex, isItAPrintedWalk, sorter, sorterIndexMap,  absorbGraphCycleRemoved, orderOfUnitigs, absorbedCategory, depth, Kpass);
    
    segfaultfile<<startWalkIndex<<endl;
    assert(startWalkIndex<adjList.size());
    if(isItAPrintedWalk[get<1>(sorter[startWalkIndex])]) return;
    //cout<<depth<<endl;
    bool isThisAbsorbedWalk = false;;
    char walkAbsorbedCategory = '0';
    string unitigString = "";
    
    
    //string& walkString = constructedStrings[get<1>(sorter[startWalkIndex])];
    
    while(true){
        assert(startWalkIndex<sorter.size());
        MyTypes::fourtuple &n = sorter[startWalkIndex];
        int finalWalkId = get<1>(n);
        
        //depthfile<<get<0>(n)<<" "<<depth<<endl;
        
        //@ABSORB
        if( MODE_ABSORPTION_NOTIP ){
            if (isItAPrintedWalk[finalWalkId]) return;
        }
        
        int uid = get<0>(n);
        int isTip = get<3>(n);
        //cout<<uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;
        
        if(nodeSign[uid] == false){
            unitigString =  reverseComplement(unitigs.at(uid).sequence);
        }else{
            unitigString =  (unitigs.at(uid).sequence);
        }
        
        stack<edge_t> stType12;
        stack<edge_t> stType34;
        if(absorbGraphCycleRemoved[uid].size() > 0){         //populate two types of stacks
            stack<edge_t> & st = absorbGraphCycleRemoved[uid];
            while(!st.empty()){
                edge_t st_top = st.top();
                assert(isItAPrintedWalk[oldToNew[st_top.toNode].finalWalkId]==false);
                if(absorbedCategory[st_top.toNode]=='3' or absorbedCategory[st_top.toNode]=='4'  ){
                    stType34.push(st_top);
                }else if(absorbedCategory[st_top.toNode]=='1' or absorbedCategory[st_top.toNode]=='2' ){
                    stType12.push(st_top);
                }else{
                    cout<<"errorrrrrrrrrrr"<<endl;
                    assert(false);
                }
                st.pop();
            }
        }
        
        //walk starting AND not absorbed
        if( isItWalkStarting(uid) && absorbedCategory[uid]=='0' && MODE_ABSORPTION_NOTIP){
            //walkString+= cutSuf(unitigString, Kpass);
            FOUT<<cutSuf(unitigString, Kpass);
            
            while(!stType34.empty()){
                int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                char isSmallK =stType34.top().isSmallK;
                
                if(isItAPrintedWalk[oldToNew[stType34.top().toNode].finalWalkId]==true){
                    stType34.pop();
                    continue;
                }
                
                stType34.pop();
                
                FOUT<<"[";
                recursiveWalkStringMakerOld(sindex, depth+1, K, FOUT);
                FOUT<<"]";
                //walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
                //constructedStrings[get<1>(sorter[sindex])].shrink_to_fit();
            }
            
            //walkString+= suf(unitigString, Kpass);
            FOUT<<suf(unitigString, Kpass);
            
            while(!stType12.empty()){
                int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                char isSmallK =stType12.top().isSmallK;
                if(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId]==true){
                    stType12.pop();
                    continue;
                }
                
                stType12.pop();
                
                FOUT<<"[";
                recursiveWalkStringMakerOld(sindex, depth+1, K, FOUT);
                FOUT<<"]";
            
                //walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
            }
        }
        
        //not walk starting AND not absorbed
        if(!isItWalkStarting(uid) && absorbedCategory[uid]=='0' && MODE_ABSORPTION_NOTIP){
            while(!stType34.empty()){
                int sindex = sorterIndexMap[oldToNew[stType34.top().toNode].finalWalkId];
                char isSmallK =stType34.top().isSmallK;
                
                if(isItAPrintedWalk[oldToNew[stType34.top().toNode].finalWalkId]==true){
                    stType34.pop();
                    continue;
                }
                stType34.pop();
                
                assert(sindex<adjList.size());
                
                FOUT<<"[";
                recursiveWalkStringMakerOld(sindex, depth+1, K, FOUT);
                FOUT<<"]";
                //recursiveWalkStringMaker(sindex, depth+1, K);
                //walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
            }
            //walkString+= cutPref(unitigString, Kpass);
            FOUT<<cutPref(unitigString, Kpass);
            
            while(!stType12.empty()){
                int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                
                char isSmallK =stType12.top().isSmallK;
                if(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId]==true){
                    stType12.pop();
                    continue;
                }
                
                stType12.pop();
                
                FOUT<<"[";
                recursiveWalkStringMakerOld(sindex, depth+1, K, FOUT);
                FOUT<<"]";
                //recursiveWalkStringMaker(sindex, depth+1, K);
                //walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
            }
        }
        
        //not walk starting AND absorbed
        //        if(pos_in_walk != 1 && absorbedCategory[uid]!='0' && MODE_ABSORPTION_NOTIP){
        //            assert(false);
        //        }
        
        //ERROR ONE
        if(isItWalkStarting(uid) && absorbedCategory[uid]!='0' && MODE_ABSORPTION_NOTIP){

            isThisAbsorbedWalk=true;
            walkAbsorbedCategory =absorbedCategory[uid];
            
            if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3'){
                //walkString+=cutSuf(unitigString, Kpass);
                FOUT<<cutSuf(unitigString, Kpass);
                
            }
            if(!ABSORBONLYTWO){
                if(absorbedCategory[uid]=='1') {
                    //walkString+="+";
                    FOUT<<"+";
                }
                else if(absorbedCategory[uid]=='4'){
                    //walkString+="-";
                    FOUT<<"-";
                }
            }
            
            if(absorbedCategory[uid]=='1' or absorbedCategory[uid]=='4' ){
                FOUT<<cutPref(unitigString, Kpass);
                //walkString+= cutPref(unitigString, Kpass);
                while(!stType12.empty()){
                    //assert(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId] == false);
                    int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                    char isSmallK =stType12.top().isSmallK;
                    
                    if(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId]==true){
                        stType12.pop();
                        continue;
                    }
                    
                    stType12.pop();
                    
                    FOUT<<"[";
                    recursiveWalkStringMakerOld(sindex, depth+1, K, FOUT);
                    FOUT<<"]";
                    //recursiveWalkStringMaker(sindex, depth+1, K);
                    //walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
                }
            }

            if(!ABSORBONLYTWO){
                if(absorbedCategory[uid]=='3') {
                    FOUT<<"+";
                    //walkString+="+";
                }else if(absorbedCategory[uid]=='2'){
                    FOUT<<"-";
                    //walkString+="-";
                }
                
                while(!stType12.empty()){
                    int sindex = sorterIndexMap[oldToNew[stType12.top().toNode].finalWalkId];
                    char isSmallK =stType12.top().isSmallK;
                    
                    if(isItAPrintedWalk[oldToNew[stType12.top().toNode].finalWalkId]==true){
                        stType12.pop();
                        continue;
                    }
                    stType12.pop();
                    
                    FOUT<<"[";
                    recursiveWalkStringMakerOld(sindex, depth+1, K, FOUT);
                    FOUT<<"]";
                    //recursiveWalkStringMaker(sindex, depth+1, K);
                    //walkString = walkString + constructedStrings[get<1>(sorter[sindex])];
                }
            }
        }
        
        //earlyAbsorb
        /*
        if(!isItWalkStarting(uid) && absorbedCategory[uid]!='0' && MODE_ABSORPTION_NOTIP){
            isThisAbsorbedWalk=true;
            walkAbsorbedCategory =absorbedCategory[uid];
            assert(stType34.empty());
            assert(stType12.empty());
            
            if(absorbedCategory[uid]=='1' or absorbedCategory[uid]=='4' ){
                string signs;
                if(absorbedCategory[uid]=='1') signs="+";
                else if(absorbedCategory[uid]=='4') signs="-";
                
                walkString = cutSuf(walkString, Kpass) + signs + cutPref(unitigString, Kpass);
            }
            
            if(absorbedCategory[uid]=='2' or absorbedCategory[uid]=='3' ){
                //if(!ABSORBONLYTWO){
                string signs;
                if(absorbedCategory[uid]=='3') signs="+";
                else if(absorbedCategory[uid]=='2') signs="-";
                walkString = cutSuf(walkString, Kpass) + unitigString;
                walkString = cutSuf(walkString, Kpass) + signs;
                // }
            }
        }
         
        
       
        if( MODE_ABSORPTION_TIP && isTip == 0){//
            //walkString+= cutPref(unitigString, K);
            walkString = plus_strings(walkString, unitigString, K);
            
        }else if(isTip==1 && MODE_ABSORPTION_TIP){ //right R   R    ]   ]   ]   ]
            //cut prefix: correct
            if(0==0){
                unitigString = unitigString.substr(K - 1, unitigString.length() - (K - 1));
                if(walkString.length()<K){
                    cout<<"pos: "<<walkString.length()<<endl;
                }
                walkString += "(" + unitigString + ")";
            }
            if(1==0){
                tipFile<<">pref\n"<<unitigString<<endl;
            }
            
        }else if(isTip==2 && MODE_ABSORPTION_TIP){ //left L   L    [ [ [
            //cut suffix: correct
            if(0==0){
                unitigString = unitigString.substr(0, unitigString.length() - (K - 1));
                if(walkString.length()<K){
                    cout<<"pos: "<<walkString.length()<<endl;
                }
                walkString += "{" + unitigString + "}";
            }
            if(1==0){
                tipFile<<">suf\n"<<unitigString<<endl;
            }
        }
        */
        
        if(startWalkIndex+1 == sorter.size()) {
            //print previous walk
            // tipDebugFile<<">"<<finalWalkId << " " << uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;
            // V_tip_ustitch++;
            // C_tip_ustitch+=walkString.length();
            //
            //  tipDebugFile<<walkString<<endl;
            //  tipFile<< walkString<<endl;
            
            if(MODE_ABSORPTION_NOTIP){
                isItAPrintedWalk[finalWalkId] = true;
            }
            
            break;
        }else if(get<1>(sorter[startWalkIndex+1]) != finalWalkId){
            //print previous walk
            //  tipDebugFile<<">"<<finalWalkId << " " << uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;
            //           V_tip_ustitch++;
            //            C_tip_ustitch+=walkString.length();
            //
            //   tipDebugFile<<walkString<<endl;
            //           tipFile<< walkString<<endl;
            
            isItAPrintedWalk[finalWalkId] = true;
            break;
        }
        startWalkIndex++;
    }
//    if(ABSORBONLYTWO){
//        if(walkAbsorbedCategory=='1') walkString="["+walkString+"]";
//        else if(walkAbsorbedCategory=='4') walkString="("+walkString+")";
//    }else{
//        if(isThisAbsorbedWalk) walkString="["+walkString+"]";
//    }

    //constructedStrings[get<1>(sorter[startWalkIndex])] = walkString;
}




void AbsorbGraph::tipAbsorbedOutputter(){
    //abfile.open("tipOutput.txt");
    /// START OUTPUTTING
    
    if(NAIVEVARI){
        return;
    }
    
    //vector<bool> isItAPrintedWalk(countNewNode, false);
    
    tipFile.open(ofileTipOutput);
    tipDebugFile.open(ofileTipDebug);
    
    
    vector<char> color(sorter.size(), 'w');
    
    for(int i = 0; i<countNewNode; i++){
        isItAPrintedWalk[i] = false;
    }
    
    int depth = 0;
    while(!orderOfUnitigs.empty()){
        int it = sorterIndexMap[orderOfUnitigs.front()];
        
        if(DBGFLAG==PRINTER){
            cout<<"order: "<<orderOfUnitigs.front()<<endl;
        }
        
        
        orderOfUnitigs.pop();
        int strsize=0;
        //int strsize = reservedStringSize.front();
        //reservedStringSize.pop();
        
        depth = 0;
        //recursivePrinter(it, isItAPrintedWalk, sorter, sorterIndexMap,  absorbGraphCycleRemoved, orderOfUnitigs, absorbedCategory, depth, K);
        
        bool RECUROLD = false;
        bool RECURLOW2 = false;
        bool ITERDISK = false;
        
        string walkString = "";
        
        if(RECUROLD){
            //walkString = recursiveWalkStringMakerOld(it, depth, K, tipFile);
        }else if(RECURLOW2){
            tipFile<< ">\n";
            recursiveWalkStringMakerOld(it, depth, K, tipFile);
            tipFile<< "\n";
        }else if(ITERDISK){
            walkString = readAndDeleteWalkFile(get<1>(sorter[it]));
        }else{
            iterSpellTested(it, depth, K, color, 0);
            walkString = constructedStrings[get<1>(sorter[it])];
        }
        
        
        if(walkString!="" && MODE_ABSORPTION_NOTIP){
            V_tip_ustitch++;
            C_tip_ustitch+=walkString.length();
            
            // tipDebugFile<<walkString<<endl;
            tipFile<< ">\n"<< walkString<<endl;
            C_oneabsorb+=walkString.length();
            V_oneabsorb++;
            constructedStrings[get<1>(sorter[it])] = "";
            
            int brackets1 = std::count(walkString.begin(), walkString.end(), '[');
            int brackets2 = std::count(walkString.begin(), walkString.end(), ']');
            int stringPlus = std::count(walkString.begin(), walkString.end(), '+');
            int stringMinus = std::count(walkString.begin(), walkString.end(), '-');
            
            C_oneabsorb_ACGT +=walkString.length() -brackets1- brackets2 -stringPlus-stringMinus;
            C_oneabsorb_plusminus += stringPlus +stringMinus;
            C_oneabsorb_brackets +=brackets1+brackets2;
            
            walkString = "";
            walkString.shrink_to_fit();
        }
    }

    int it = -1;
    while(true){
        if(orderOfUnitigs.empty()){
            it++;
            if(it>=sorter.size()) break;
        }else{
            assert(false);
            it = sorterIndexMap[orderOfUnitigs.front()];
            orderOfUnitigs.pop();
        }
        if(MODE_ABSORPTION_NOTIP)
            break;
        
        //recursive call
        depth = 0;
        string walkString="";
        
        if(MODE_ABSORPTION_TIP){
            //walkString = recursiveWalkStringMakerOld(it, depth, K, tipFile);
        }else{
            assert(false);
        }

        //tipDebugFile<<">"<<finalWalkId << " " << uid<<" " <<finalWalkId<<" "<<pos_in_walk<<" "<<isTip<<endl;
        
        if(walkString!=""){
            V_tip_ustitch++;
            C_tip_ustitch+=walkString.length();
            
            // tipDebugFile<<walkString<<endl;
            tipFile<<">\n"<< walkString<<endl;
            
            C_oneabsorb+=walkString.length();
            
            
            int brackets1 = std::count(walkString.begin(), walkString.end(), '[');
            int brackets2 = std::count(walkString.begin(), walkString.end(), ']');
            int stringPlus = std::count(walkString.begin(), walkString.end(), '+');
            int stringMinus = std::count(walkString.begin(), walkString.end(), '-');
            
            C_oneabsorb_ACGT +=walkString.length() -brackets1- brackets2 -stringPlus-stringMinus;
            C_oneabsorb_plusminus += stringPlus +stringMinus;
            C_oneabsorb_brackets +=brackets1+brackets2;
            V_oneabsorb++;
        }
        //
    }
    //delete [] isItAnAbsorbedWalk;
    tipDebugFile.close();
    tipFile.close();
}

//map<int, vector<edge_t> >
void AbsorbGraph::print_absorb_graph()
{
    map<int,vector<edge_t> > const &m = absorbGraph;
    cout<<"ABSORB GRAPH---------------"<<endl;
    for (auto const& pair: m) {
        std::cout << pair.first << ":  ";
        vector<edge_t> vec = pair.second;
        for(int i=0; i<vec.size(); i++){
            edge_t e = vec[i];
            cout<< e.toNode  << ", ";
        }
        cout<<"\n";
    }
    cout<<"ABSORB GRAPH END---------------"<<endl;
}

//map<int, vector<edge_t> >
//void print_absorb_graph_acyclic(vector<stack<edge_t> > const &m)
//{
//    cout<<"ABSORB GRAPH ACYCLIC---------------"<<endl;
//    for (auto const& pair: m) {
//        std::cout << pair.first << ":  ";
//        stack<edge_t> vec = pair.second;
//        while(!vec.empty()){
//            edge_t e = vec.top();
//            vec.pop();
//            cout<< e.toNode  << ", ";
//        }
//        cout<<"\n";
//    }
//    cout<<"ABSORB GRAPH ACYCLIC END---------------"<<endl;
//}



void makeGraphDotAbsorb(map<int, vector<edge_t> > adjList){
    FILE * fp;
    
    fp = fopen ("/Users/Sherlock/dot-visualize/graph.gv", "w+");
    
    fprintf(fp, "digraph d {\n");
    set<int> vertices;
    for (std::map<int, vector<edge_t> >::iterator it=adjList.begin(); it!=adjList.end(); ++it){
        int x =  it->first ;
        vector<edge_t> adjX = it->second;
        for(edge_t ex: adjX){
            vertices.insert(oldToNew[x].finalWalkId);
            vertices.insert(oldToNew[ex.toNode].finalWalkId);
            fprintf(fp, "%d -> %d[taillabel=\"%d\", headlabel=\"%d\"]\n", oldToNew[x].finalWalkId, oldToNew[ex.toNode].finalWalkId, ex.left, !ex.right);
            cout<<oldToNew[x].finalWalkId<<"->"<<oldToNew[ex.toNode].finalWalkId<<endl;
            
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
    for(int i = 0 ; i<countNewNode; i++){
        if(!obsoleteWalkId[i] && vertices.count(i)==0 ){
            vertices.insert(i);
        }
    }
    for(int x: vertices){
        if(false){
            fprintf(fp, "%d [label=\"%d\", color=\"red\"]\n", x, x);
        }else{
            fprintf(fp, "%d [label=\"%d\"]\n", x, x);
        }
        
    }
    
    //for all int in list
    //make a list of neighbors add them
    
    
    fprintf(fp, "}\n");
    
    fclose(fp);
}


void absorptionManager(vector<MyTypes::fourtuple>& sorter) {
    
    if(DBGFLAG==PRINTER){
        for(auto i: sorter ){
            cout<<"walk "<<get<1>(i)<<":";  //    int finalWalkId = get<1>(n);
            cout<<"("<<get<2>(i)<<")";  //    int pos_in_walk = get<2>(n);
            cout<<get<0>(i)<<" ";   //    int uid = get<0>(n);
            cout<<get<3>(i)<<"";
            cout<<endl;
        }
    }
    
    
    AbsorbGraph abs(sorter); //initialize all the graphs and other data structures
    
    
    if(MODE_ABSORPTION_NOTIP ){

        
        if(MODE_ABSORPTION_NOTIP){
            abs.sorterIndexAndAbsorbGraphMaker();//sorter, sorterIndexMap, absorbGraph, absorbedCategory
        }
        
        //makeGraphDotAbsorb(absorbGraph);
        
        if(DBGFLAG==PRINTER){
            abs.print_absorb_graph();
        }
        
        abs.removeCycleFromAbsorbGraph();//sorter,  sorterIndexMap, absorbGraph, absorbGraphCycleRemoved,  orderOfUnitigs,  absorbedCategory, last two are output
        cout<<"[3.3] cycle removed from absorption graph (forest construction done!)"<<endl;
        if(DBGFLAG==PRINTER){
               for (int i=0; i<adjList.size(); i++) {
                   cout<<i<<"->"<<abs.absorbedCategory[i]<<endl;
               }
               //print_absorb_graph_acyclic(absorbGraphCycleRemoved);
           }
           
           abs.tipAbsorbedOutputter(); //sorter, sorterIndexMap, absorbGraphCycleRemoved, orderOfUnitigs, absorbedCategory
           cout<<"[3.4] path spelling done!"<<endl;
           
    }
    
   
    if(MODE_ABSORPTION_TIP ){
        //abs.tipSpell();
    }
    
}





SCCGraph::SCCGraph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}
SCCGraph::SCCGraph()
{
    
}

// A recursive function to print DFS starting from v
void SCCGraph::DFSUtil(int v, bool visited[], map<int, int>& vToMetagraphV, map<int, set<int> >& sccIdToWalks, int & countSCC)
{
    // Mark the current node as visited and print it
    visited[v] = true;
    //cout << v << " ";
    vToMetagraphV[v] = countSCC;
    sccIdToWalks[countSCC].insert(v);
    
    // Recur for all the vertices adjacent to this vertex
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFSUtil(*i, visited, vToMetagraphV, sccIdToWalks, countSCC);
}

SCCGraph SCCGraph::getTranspose()
{
    SCCGraph g(V);
    for (int v = 0; v < V; v++)
    {
        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            g.adj[*i].push_back(v);
        }
    }
    return g;
}

void SCCGraph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}

void SCCGraph::fillOrder(int v, bool visited[], stack<int> &Stack)
{
    // Mark the current node as visited and print it
    visited[v] = true;
    
    // Recur for all the vertices adjacent to this vertex
    list<int>::iterator i;
    for(i = adj[v].begin(); i != adj[v].end(); ++i)
        if(!visited[*i])
            fillOrder(*i, visited, Stack);
    
    // All vertices reachable from v are processed by now, push v
    Stack.push(v);
}

// The main function that finds and prints all strongly connected
// components
queue<int> AbsorbGraph::printSCCs()
{
    //vector<MyTypes::fourtuple>& sorter, map<int, int >& sorterIndexMap
    
    int countSCC = 0;
    map<int, int> vToMetagraphV;
    map<int, set<int> > adjSCC;
    map<int, set<int> > sccIdToWalks;
    stack<int> Stack;
    
    // Mark all the vertices as not visited (For first DFS)
    bool *visited = new bool[g->V];
    for(int i = 0; i < g->V; i++)
        visited[i] = false;
    
    // Fill vertices in stack according to their finishing times
    for(int i = 0; i < g->V; i++)
        if(visited[i] == false)
            g->fillOrder(i, visited, Stack);
    
    // Create a reversed graph
    SCCGraph gr = g->getTranspose();
    
    // Mark all the vertices as not visited (For second DFS)
    for(int i = 0; i < g->V; i++)
        visited[i] = false;
    countSCC=0;
    // Now process all vertices in order defined by Stack
    while (Stack.empty() == false)
    {
        // Pop a vertex from stack
        int v = Stack.top();
        Stack.pop();
        
        // Print Strongly connected component of the popped vertex
        if (visited[v] == false && !obsoleteWalkId[v])
        {
            gr.DFSUtil(v, visited, vToMetagraphV, sccIdToWalks, countSCC);
            //cout << endl;
            countSCC++;
        }
    }
    queue<int> uidorder;
    if(1==1){
        int* outdeg = new int[countSCC];
        for(int u = 0 ; u<countSCC; u++){
            outdeg[u] = 0;
        }
        for(int u = 0 ; u<g->V; u++){
            for (auto i = g->adj[u].begin(); i != g->adj[u].end(); ++i)
            {
                int v = *i;
                //cout<<vToMetagraphV[v]<<endl;
                if(vToMetagraphV[v] != vToMetagraphV[u]){
                    adjSCC[vToMetagraphV[v]].insert(vToMetagraphV[u]); //reverse graph
                    //cout<<"scc edge: "<<vToMetagraphV[u]<<"->"<<vToMetagraphV[v]<<endl;
                    outdeg[vToMetagraphV[u]]++;
                }
                
            }
        }
        int lowerbound = 0;
        queue<int> later;
        
        for(int sccid = 0 ; sccid<countSCC; sccid++){
            
            int indegree = adjSCC[sccid].size();
            
            //cout<<sccid<<" " << indegree<< " ,out=" << outdeg[sccid]<<endl;
            if(indegree==0){   //these are the sources
                set<int> walks = sccIdToWalks[sccid];
                //cout<<"walk in scc "<<sccid<<":";
                for(int ww: walks){
                    //cout<<ww<<" ";
                    vector<int> uidsNeighbors = this->getAllUidsInWalk(ww);
                    for(int i = 0; i<uidsNeighbors.size(); i++){
                        // uidorder.push(uidsNeighbors[i]);
                        oldToNew[uidsNeighbors[i]].sccid = sccid;
                    }
                    if(uidsNeighbors.size()!=0){
                        //uidorder.push(uidsNeighbors[0]);
                    }
                    //break;
                }
                //cout<<endl;
                for(int ww: walks){
                    assert(!obsoleteWalkId[ww]);
                    if(!obsoleteWalkId[ww]){
                        vector<int> uidsNeighbors = getAllUidsInWalk(ww);
                        for(int i = 0; i<uidsNeighbors.size(); i++){
                            uidorder.push(uidsNeighbors[i]);
                            break;
                        }
                        //break;
                    }
                    
                }
                
                
                lowerbound++;
            }else{
                set<int> walks = sccIdToWalks[sccid];
                for(int ww: walks){
                    vector<int> uidsNeighbors = getAllUidsInWalk(ww);
                    for(int i = 0; i<uidsNeighbors.size(); i++){
                        later.push(uidsNeighbors[i]);
                        oldToNew[uidsNeighbors[i]].sccid = sccid;
                    }
                }
                
            }
            
        }
//        while(!uidorder.empty()){
//            int t =uidorder.front();
//            later.push(t);
//            uidorder.pop();
//        }
//        uidorder = later;
        map<int, int> checkdup;
        
        //cout<<"UIDORDER SIZE: "<<uidorder.size() <<"\n"<<endl;
        //assert(uidorder.size() == adjList.size());
        
        cout<<"number of SCC: "<<countSCC<<endl;
        cout<<"number of connected components: (SCC based) "<<lowerbound<<endl;
        absorbGraphNumCC_endAbosrb = lowerbound;
        delete[] outdeg;
    }
    return uidorder;
}



#endif /* absorbGraph_hpp */




