#include<bits/stdc++.h>
#include<random>


using namespace std;

#define ITR 20

int vertex;
//vector<pair<long long, long long> > graph[500000];
vector< vector< pair< long long , long long > > > graph;
map<pair< long long , long long >, long long > graphMap;
set<long long> ver;
set<long long> s1;
set<long long> s2;
mt19937 genRandom(time(NULL));
long long avgLocalItr;
long long avgLocalWt;

void printSet(set<long long> &s){
    cout<<"{";
    for(auto i: s){
        cout<<i<<" ";
    }
    cout<<"}\n";
}


long long sigmaWeight(long long i, set<long long> &s2){ //hash map e fast hobe: (v1,v2)--wt
    long long v2,wt=0;

    for(long long j=0; j<graph[i].size(); j++){
        v2=graph[i][j].first;
        if(s2.find(v2) != s2.end()){
          //  cout<<i<<"-"<<graph[i][j].first<<" : "<<graph[i][j].second<<endl;
            wt+=graph[i][j].second;
        }
    }
//
//    for(auto j:s2){
//        //cout<<i<<"-"<<j<<" : "<<graphMap[make_pair(i,j)]<<endl;
//        wt+=graphMap[make_pair(i,j)];
//    }
    return wt;
}

long long maxCutWeight(set<long long> &s1, set<long long> &s2){
    long long wt=0;
    for(auto i:s1){
        wt+=sigmaWeight(i,s2);
    }
return wt;
}

void random(){
    s1.clear();
    s2.clear();

    for(auto i: ver){
        int ran=genRandom()%2;
//        cout<<ran<<endl;
        if(ran==1){
            s1.insert(i);
        }
        else{
            s2.insert(i);
        }
    }
}


void findMaxCut(double alpha){
//    cout<<"alpha "<<alpha<<endl;
    s1.clear();
    s2.clear();
    long long minedge=LONG_MAX,maxedge=LONG_MIN;
    long long minSigma=LONG_MAX,maxSigma=LONG_MIN;
    double mu;
    vector<pair<long long, long long> > RCL;
    vector<long long > RCLv;
    set<long long > remaining;
    pair<long long,long long> xy;
    map<long long, long long> d1,d2;
    long long sx;

    for(int i=1; i<=vertex; i++){
        for(int j=0; j<graph[i].size(); j++){
            if(minedge>=graph[i][j].second){
                minedge=graph[i][j].second;
            }
            if(maxedge<=graph[i][j].second){
                maxedge=graph[i][j].second;
            }
        }
    }
    mu=1.0*minedge+1.0*alpha*(maxedge-minedge);

    for(int i=1; i<=vertex; i++){
        for(int j=0; j<graph[i].size(); j++){
            if(graph[i][j].second>=mu){
                RCL.push_back(make_pair(i,graph[i][j].first));
                //cout<<i<<"-"<<graph[i][j].first<<endl;
            }
        }
    }

    int ran=genRandom()%RCL.size();
    xy=RCL[ran];
    s1.insert(xy.first);
    s2.insert(xy.second);

//    debug
//    printSet(s1);
//    printSet(s2);

    while(s1.size()+s2.size()!=vertex){
        RCLv.clear();
        remaining.clear();
        d1.clear();
        d2.clear();
        minSigma=LONG_MAX;
        maxSigma=LONG_MIN;

        for(auto i: ver){
            remaining.insert(i);
        }
        for(auto i: s1){
            remaining.erase(i);
        }
        for(auto i: s2){
            remaining.erase(i);
        }

//        cout<<"rem: ";
//        printSet(remaining);

        for(auto v: remaining){
            sx=sigmaWeight(v,s1);
            d2[v]=sx;
            if(minSigma>=sx)
                minSigma=sx;
            if(maxSigma<=sx)
                maxSigma=sx;

            sx=sigmaWeight(v,s2);
            d1[v]=sx;
            if(minSigma>=sx)
                minSigma=sx;
            if(maxSigma<=sx)
                maxSigma=sx;

        }

        mu=1.0*minSigma+1.0*alpha*(maxSigma-minSigma);

       for(auto v: remaining){
            if(d1[v]>=mu || d2[v]>=mu)
                RCLv.push_back(v);
       }

       ran=genRandom()%RCLv.size();
       long long vv=RCLv[ran];

       if(d1[vv]>d2[vv]){
            s1.insert(vv);
       }
       else{
            s2.insert(vv);
       }


//    //debug
//    printSet(s1);
//    printSet(s2);

    }

}

void random2(){
    findMaxCut(0);
}

void greedy(){
    findMaxCut(1);
}

void semiGreedy(){
    long long ran=genRandom();
    double alpha=1.0*ran/(ran+genRandom());
    findMaxCut(alpha);
}

void addEdge(long long a, long long b, long long w){
    graph[a].push_back(make_pair(b,w));
    graph[b].push_back(make_pair(a,w));
    graphMap[make_pair(a,b)]=w;
    graphMap[make_pair(b,a)]=w;
}

void printGraph(){
    for(int i=1; i<=vertex; i++){
        for(int j=0; j<graph[i].size(); j++)
            cout<<i<<"-"<<graph[i][j].first<<" : "<<graph[i][j].second<<endl;
    }
}


long long localSearch(){
    bool change=true;
    long long countItr=0;
    while(change){
        change=false;
        countItr++;
        for(auto v: ver){
            if(!change){
                if(s1.find(v)!=s1.end()){
                    s1.erase(v);
                    if(sigmaWeight(v,s1)>sigmaWeight(v,s2)){
                        s2.insert(v);
                        change=true;
                    }
                    else{
                        s1.insert(v);
                    }
                }
                else if(s2.find(v)!=s2.end()){
                    s2.erase(v);
                    if(sigmaWeight(v,s2)>sigmaWeight(v,s1)){
                        s1.insert(v);
                        change=true;
                    }
                    else{
                        s2.insert(v);
                    }
                }
            }
        }
    }
    return countItr;
}

long long Grasp(long long maxItr, int choise=2){
    long long wt=LONG_MIN;
    long long newwt=0;
    long long localItr=0;
    long long localWt=0;
    avgLocalItr=0;
    avgLocalWt=0;

    for(int i =0; i<maxItr; i++){
        if(choise==2){
            semiGreedy();
        }
        else if(choise==1){
            greedy();
        }
        else if(choise==0){
            random();
        }
        else{
            random2();
        }
        localItr+=localSearch();
        newwt=maxCutWeight(s1,s2);
        localWt+=newwt;
        //cout<<newwt<<endl;
        if(wt<newwt){
         wt=newwt;
        }
    }
    avgLocalItr=1.0*localItr/maxItr;
    avgLocalWt=1.0*localWt/maxItr;
    return wt;
}


int main(){
    string fname;
    long long e,a,b,w;
    int n;
    int test=1;

    freopen("1905108.csv","a",stdout);
    cout<<"Problem,,,Constructive algorithm,,,,Local search,,,,,,,,GRASP,,,,,,,,Known best solution or upper bound"<<endl;
    cout<<"Name,|V| or n,|E| or m,Randomized-1,Randomized-2,Simple Greedy,Semi-greedy,Local-1(Random1),,Local-1(Random2),,Local-3(semi-greedy),,Local-4(Greedy),,GRASP-1(Random1),,GRASP-2(Random2),,GRASP-3(semi-greedy),,GRASP-4(Greedy),"<<endl;
    cout<<",,,,,,,No of iterations,Avg value,No of iterations,Avg value,No of iterations,Avg value,No of iterations,Avg value,No of iterations,Best value,No of iterations,Best value,No of iterations,Best value,No of iterations,Best value"<<endl;
    while(test<=54){
    fname="set1/g"+to_string(test)+".rud";
    //cout<<"T"<<test<<endl;
    test++;
    freopen(fname.c_str(),"r",stdin);
    cin>>n>>e;
    graph = vector< vector< pair< long long , long long > > > (n + 1);
    vertex=n;
    for(long long i=0; i<e; i++){
        cin>>a>>b>>w;
        addEdge(a,b,w);
        ver.insert(a);
        ver.insert(b);
    }


    //csv
    cout<<"G"<<(test-1)<<","<<n<<","<<e<<",";
    //random
    long long maxcut=0;
    for(int i=0; i<ITR; i++){
        random();
        maxcut+=maxCutWeight(s1,s2);
    }
    cout<<(1.0*maxcut/ITR)<<",";
    //random2
    maxcut=0;
    for(int i=0; i<ITR; i++){
        random2();
        maxcut+=maxCutWeight(s1,s2);
    }
    cout<<(1.0*maxcut/ITR)<<",";
    //greedy
    maxcut=0;
    for(int i=0; i<ITR; i++){
        greedy();
        maxcut+=maxCutWeight(s1,s2);
    }
    cout<<(1.0*maxcut/ITR)<<",";
    //semiGreedy
    maxcut=0;
    for(int i=0; i<ITR; i++){
        semiGreedy();
        maxcut+=maxCutWeight(s1,s2);
    }
    cout<<(1.0*maxcut/ITR)<<",";

    long long maxcutR1,maxcutR2,maxcutSemi;
    //random
    maxcutR1=Grasp(ITR,0);
    cout<<avgLocalItr<<","<<avgLocalWt<<",";
    //random2
    maxcutR2=Grasp(ITR,3);
    cout<<avgLocalItr<<","<<avgLocalWt<<",";
    //semi-greedy
    maxcutSemi=Grasp(ITR,2);
    cout<<avgLocalItr<<","<<avgLocalWt<<",";
    //greedy
    maxcut=Grasp(ITR,1);
    cout<<avgLocalItr<<","<<avgLocalWt<<",";

    cout<<ITR<<","<<maxcutR1<<","<<ITR<<","<<maxcutR2<<","<<ITR<<","<<maxcutSemi<<","<<ITR<<","<<maxcut<<endl;

    for(int i=0; i<=n; i++){
        graph[i].clear();
    }
    graph.clear();
    ver.clear();
    s1.clear();
    s2.clear();
    }
}

/*
6 7
1 2 1
3 4 2
5 6 3
1 3 4
3 5 4
2 4 4
4 6 4
*/

/*
5 7
1 2 5
1 5 6
2 3 9
2 5 10
3 4 15
3 5 2
4 5 10
*/

