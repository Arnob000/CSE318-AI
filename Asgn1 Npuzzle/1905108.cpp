#include<bits/stdc++.h>

using namespace std;

//n puzzle
vector<vector<long>> goal;
vector<vector<long>> src;
long grid=0;

class Board{
    long grid;
    long cost;
    vector<vector<long> > node;
    pair<long,long> blank;
    Board* parent;

public:
    Board(){
        grid=0;
        cost=0;
        blank=make_pair((long)0,(long)0);
        parent=NULL;
    }

    Board(long g, long c=0){
        grid=g;
        cost=c;
        blank=make_pair((long)0,(long)0);
        parent=NULL;
    }

    void setCost(long c){
        cost=c;
    }

    void setBlank(pair<long, long> b){
        blank=b;
    }

    void setNode(vector<vector<long> > arr){
        node=arr;
    }

    void setParent(Board* b){
        parent=b;
    }

    long getCost(){
        return cost;
    }

    pair<long, long> getBlank(){
        return blank;
    }

    vector<vector<long> > getNode(){
        return node;
    }

    Board* getParent(){
        return parent;
    }
    ~Board(){
        for(int i=0; i<node.size(); i++){
            node[i].clear();
        }
        node.clear();

        if(parent!=NULL)
            delete parent;

    }
};


long Hamming(vector<vector<long> > src, long grid){
    long count=0;
    long val=1;
     for(int i=0; i<grid; i++){
       for(int j=0; j<grid; j++){
           if(src[i][j]!=val && src[i][j]!=0)
            count++;
           val++;
        }
    }
    return count;
}

long Manhattan(vector<vector<long> > src, long grid){
    long count=0;
    long key;
    for(int k=0; k<grid; k++){
        for(int l=0; l<grid; l++){
            key=goal[k][l];
            if(key==0)
                continue;
            for(int i=0; i<grid; i++){
                for(int j=0; j<grid; j++){
                    if(src[i][j]==key){
                        //cout<<key<<"-"<<src[i][j]<<":"<<abs(k-i)+abs(l-j)<<endl;
                        count+=abs(k-i)+abs(l-j);
                    }
                }
            }
        }
    }
    return count;
}

long Heuristic(vector<vector<long> > src, long grid, bool choise=true){
    if (choise){
        return Manhattan(src, grid); //true to manhattan
    }
    else{
        return Hamming(src, grid);
    }
}

void printBoard(vector<vector<long> > arr, long v){
    for(int i=0; i<v; i++){
       for(int j=0; j<v; j++){
           cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}


void createGoal(){
    goal.clear();
    vector<long> d;
    for(int i=0;i<grid; i++){
        for(int j=0; j<grid; j++){
            d.push_back(i*grid+j+1);
        }
        goal.push_back(d);
        d.clear();
    }
    goal[grid-1][grid-1]=0;
}

long inversionCount(vector<vector<long> > src, long v){
    long count=0;
    long key;
    for(int k=0; k<v; k++){
        for(int l=0; l<v; l++){
            key=src[k][l];
            for(int i=k; i<v; i++){
                for(int j=0; j<v; j++){
                    if(i==k && j<l+1)
                        j=l+1;
                    if(j==v)
                        continue;
                    if(src[i][j]==0)
                        continue;
                    if(key>src[i][j]){
                        //cout<<key<<"-"<<src[i][j]<<" ";
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

bool solvable(long invCount, long grid, long blankRow){
    //odd grid
    //cout<<"Inv count:"<<invCount<<endl;
    if(grid%2==1){
        if(invCount%2==0)
            return true;
    }
    //even grid : row of blank+inversion count
    else {
        blankRow=grid-blankRow-1;
        //cout<<"Blank row:"<<blankRow<<endl;
        if((invCount+blankRow)%2==0)
            return true;
    }
    return false;
}

Board* createBoard(Board* parent, pair<long, long> newBlank){
    Board* b=new Board(grid);
    vector<vector<long> > node=parent->getNode();
    pair<long, long> blank=parent->getBlank();
    long temp;

    temp=node[blank.first][blank.second];
    node[blank.first][blank.second]=node[newBlank.first][newBlank.second];
    node[newBlank.first][newBlank.second]=temp;

    b->setBlank(newBlank);
    b->setCost(parent->getCost()+1);
    b->setNode(node);
    b->setParent(parent);
    //printBoard(b->getParent()->getNode(),grid);
    return b;
}


bool match(vector<vector<long> > aa, vector<vector<long> > bb){
    bool flag=true;

    for(int i=0; i<grid; i++){
        for(int j=0; j<grid; j++){
            if(aa[i][j]!=bb[i][j]){
                flag=false;
                break;
            }
        }
    }

    if(flag)
        return true;
    return false;
}

void printSolve(Board* src, Board* des){
        Board* dst=des->getParent();

        cout<<"Minimum number of moves: "<<des->getCost()<<endl;

        if(des->getCost()!=0){
            stack<vector<vector<long> > > sp;

            while(!match(dst->getNode(),src->getNode())){
                sp.push(dst->getNode());
                dst=dst->getParent();
            }

            printBoard(src->getNode(), grid);
            while(!sp.empty()){
                printBoard(sp.top(), grid);
                sp.pop();
            }
        }
        printBoard(des->getNode(), grid);
}


class findMin{
public:
    bool operator() (pair<long, Board*> p1, pair<long, Board*> p2){
        return (p1.first>=p2.first);
    }
};

void aStar(Board* src, long grid, bool choise=true){       //O(V+ElogV)

        Board* u;
        Board* v;
        long expand=0, explore=0;

        pair<long, long> blank, newBlank;
        long estimatedCost=0;
        priority_queue<pair<long,Board*> ,vector<pair<long, Board*> >, findMin> pq;

        estimatedCost=src->getCost()+Heuristic(src->getNode(), grid, choise);
        pq.push({estimatedCost,src});

        while(!pq.empty()){     //v times
            u=pq.top().second;      //O(1)
            pq.pop();
            expand++;

//(4 case for blank(a,b)): (a-1,b),(a+1,b),(a,b-1),(a,b+1)
            blank=u->getBlank();
            if(blank.first-1>=0){     //total E times
                newBlank=make_pair(blank.first-1, blank.second);
                v=createBoard(u, newBlank);
               // printBoard(v.getParent()->getNode(),grid);
                if(match(v->getNode(),goal))
                    break;
                estimatedCost=v->getCost()+Heuristic(v->getNode(),grid,choise);
                if(u->getParent()==NULL){
                    explore++;
                    pq.push({estimatedCost,v});
                }
                else if(!match(v->getNode(),u->getParent()->getNode())){
                    explore++;
                    pq.push({estimatedCost,v});      //O(logV)
                }
            }
            if(blank.first+1<grid){     //total E times
                newBlank=make_pair(blank.first+1, blank.second);
                v=createBoard(u, newBlank);
               // printBoard(v.getParent()->getNode(),grid);
                if(match(v->getNode(),goal))
                    break;
                estimatedCost=v->getCost()+Heuristic(v->getNode(),grid,choise);
                if(u->getParent()==NULL){
                    explore++;
                    pq.push({estimatedCost,v});
                }
                else if(!match(v->getNode(),u->getParent()->getNode())){
                    explore++;
                    pq.push({estimatedCost,v});      //O(logV)
                }
            }
            if(blank.second+1<grid){     //total E times
                newBlank=make_pair(blank.first, blank.second+1);
                v=createBoard(u, newBlank);
                //printBoard(v->getParent()->getNode(),grid);
                if(match(v->getNode(),goal))
                    break;
                estimatedCost=v->getCost()+Heuristic(v->getNode(),grid,choise);
                if(u->getParent()==NULL){
                    explore++;
                    pq.push({estimatedCost,v});
                }
                else if(!match(v->getNode(),u->getParent()->getNode())){
                    explore++;
                    pq.push({estimatedCost,v});      //O(logV)
                }
            }
            if(blank.second-1>=0){     //total E times
                newBlank=make_pair(blank.first, blank.second-1);
                v=createBoard(u, newBlank);
                //printBoard(v->getParent()->getNode(),grid);
                if(match(v->getNode(),goal))
                    break;
                estimatedCost=v->getCost()+Heuristic(v->getNode(),grid,choise);
                if(u->getParent()==NULL){
                    explore++;
                    pq.push({estimatedCost,v});
                }
                else if(!match(v->getNode(),u->getParent()->getNode())){
                    explore++;
                    pq.push({estimatedCost,v});      //O(logV)
                }
            }
        }
       // cout<<"Expanded: "<<expand<<endl;
       // cout<<"Explored: "<<explore<<endl;
        printSolve(src,v);

}


int main(){
//    while(true){
    long val;
    pair<long,long> blank=make_pair((long)0,(long)0);
    cin>>grid;

    vector<long> d;

    createGoal();

    src.clear();
    for(long i=0;i<grid; i++){
        for(long j=0; j<grid; j++){
            cin>>val;
            d.push_back(val);
            if(d[j]==0){
                blank=make_pair((long)i,(long)j);
            }
        }
        src.push_back(d);
        d.clear();
    }
//    cout<<blank.first<<" "<<blank.second;

    Board* srcBoard=new Board(grid);
    srcBoard->setNode(src);
    srcBoard->setBlank(blank);
    if(match(src,goal)){
         cout<<"Minimum number of moves: "<<0<<endl;
         printBoard(src,grid);
         return 0;
    }

    if(solvable(inversionCount(src,grid), grid, blank.first)){
        cout<<"Hamming-------------"<<endl;
        aStar(srcBoard,grid,false);
        cout<<"Manhattan-----------"<<endl;
        aStar(srcBoard,grid);
    }
    else{
        cout<<"\nUnsolvable Puzzle\n";
    }

//    }
}


