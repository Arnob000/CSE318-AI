#include<bits/stdc++.h>

using namespace std;

//Adversial search
#define BINS 6
#define MANCALASIZE 2*BINS+2
#define GEMS 4
#define Hn 8

//extrainfo
#define stncap1 0
#define additionalmove1 1
#define stncap2 2
#define additionalmove2 3
#define stninpath1 4
#define stninpath2 5

//heuristic weights
#define h2w1 2
#define h2w2 3

#define h3w1 2
#define h3w2 3
#define h3w3 3

#define h4w1 3
#define h4w2 4
#define h4w3 4
#define h4w4 5

#define h5w1 4
#define h5w2 5
#define h5w3 6

#define h6w1 4
#define h6w2 5
#define h6w3 4
#define h6w4 3

#define h7w1 3
#define h7w2 2
#define h7w3 4

#define h8w1 3
#define h8w2 4
#define h8w3 3
#define h8w4 4


class Board{
    vector<int> mancala;   //0-bin for player1, (bin+1)-(mancalasize-1) for player2
    Board* parent;
    bool nextPlayer; //true is plr1, start from bottom row
    long extraInfo[6];
    

public:
    Board(){
        for(int i=0; i<MANCALASIZE; i++){
                mancala.push_back(GEMS);
        }
        mancala[BINS]=0;
        mancala[2*BINS+1]=0;
        parent=NULL;
        nextPlayer=true;
        for(int i=0; i<6; i++){
            extraInfo[i]=0;
        }

    //    mancala[BINS]=7;
    //    mancala[2]=0;
    //    mancala[4]=1;
    //    printBoard();
    //    move(1,true);
    //    printBoard();
    //    move(11,false);
    //    printBoard();
      
        // cout<<getValueOfBins()<<" "<<getValueOfStorage()<<" "<<getTotalValue()<<endl;
        // cout<<getValueOfBins(false)<<" "<<getValueOfStorage(false)<<" "<<getTotalValue(false)<<endl;
    }

    void setParent(Board* b){
        parent=b;
    }

    Board* getParent(){
        return parent;
    }

    int getTotalValue(bool player1=true){
       if(player1){
        return getValueOfBins()+getValueOfStorage();
       }
       else
        return getValueOfBins(false)+getValueOfStorage(false);
    }

    int getValueOfBins(bool player1=true){
        int sum=0;
        if(player1){
            for(int i=0; i<BINS; i++){
                sum+=mancala[i];
            }
        }
        else{
            for(int i=BINS+1; i<MANCALASIZE-1; i++){
                sum+=mancala[i];
            }
        }
        return sum;
    }

    int getValueOfStorage(bool player1=true){
        if(player1)
            return mancala[BINS];
        else
            return mancala[MANCALASIZE-1];
    }

    void setMancala(vector<int> m){
        mancala=m;
    }

    vector<int> getMancala(){
        return mancala;
    }

    void clearExtraInfo(){
       for(int i=0; i<6; i++){
            extraInfo[i]=0;
        }
    }

    void copyExtraInfo(long scap, long amv, long spth, bool player){
        if(player){
            extraInfo[stncap1]=scap;
            extraInfo[additionalmove1]=amv;
            extraInfo[stninpath1]=spth;
        }
        else {
            extraInfo[stncap2]=scap;
            extraInfo[additionalmove2]=amv;
            extraInfo[stninpath2]=spth;
        }
    }

    long getStoneCapture(bool player1=true){
        if(player1)
            return extraInfo[stncap1];
        else
            return extraInfo[stncap2];
    }

    long getAdditionalMoves(bool player1=true){
        if(player1)
            return extraInfo[additionalmove1];
        else
            return extraInfo[additionalmove2];
    }

     long getStoneInPath(bool player1=true){
        if(player1)
            return extraInfo[stninpath1];
        else
            return extraInfo[stninpath2];
    }

    void move(int pos, bool player){ //player is curr player
        bool f1=false;

        if(pos==BINS || pos==(MANCALASIZE-1))
            return;

        int gems=mancala[pos];
        mancala[pos]=0;
        pos++;
        while(true){ 
            if(gems<=0){
                if(player && pos-1==BINS){
                    nextPlayer=true;
                    extraInfo[additionalmove1]++;  
                }
                else if(player)
                    nextPlayer=false;
                else if(!player && pos==0){
                    nextPlayer=false;
                    extraInfo[additionalmove2]++;  
                }
                else if(!player)
                    nextPlayer=true;
                break;
            }

            //stn capture
            if(player && mancala[pos]==0 && mancala[2*BINS-pos]!=0 && gems==1 && pos>=0 && pos<BINS){
                f1=true;
            }
            else if(!player && mancala[pos]==0 && mancala[2*BINS-pos]!=0 && gems==1 && pos>BINS && pos<(MANCALASIZE-1)){
                f1=true;
            }

            mancala[pos]++;
            gems--;
            if(player && pos==(MANCALASIZE-1)){
                mancala[pos]--;
                gems++;
            }
            else if(!player && pos==BINS){
                mancala[pos]--;
                gems++;
            }


            //stn capture
            if(f1){
                if(player){
                    extraInfo[stncap1]+=mancala[pos];
                    extraInfo[stncap1]+=mancala[2*BINS-pos];

                    mancala[BINS]+=mancala[pos];
                    mancala[pos]=0;
                    mancala[BINS]+=mancala[2*BINS-pos];
                    mancala[2*BINS-pos]=0;
                }
                else{
                    extraInfo[stncap2]+=mancala[pos];
                    extraInfo[stncap2]+=mancala[2*BINS-pos];

                    mancala[MANCALASIZE-1]+=mancala[pos];
                    mancala[pos]=0;
                    mancala[MANCALASIZE-1]+=mancala[2*BINS-pos];
                    mancala[2*BINS-pos]=0;
                }
            }

            //stn in path
            if(pos==BINS)
                extraInfo[stninpath1]++;
            else if(pos==MANCALASIZE-1)
                extraInfo[stninpath2]++;

            pos++;
            if(pos==MANCALASIZE)
                pos=0;
        }
    }

    bool nxtPlr(){
        return nextPlayer;
    }

    void Rotate(){
        int t;
        for(int i=0; i<=BINS; i++){
            t=mancala[i];
            mancala[i]=mancala[BINS+1+i];
            mancala[BINS+1+i]=t;
        }
    }

    void printBoard(){
        cout<<mancala[MANCALASIZE-1]<<" | ";
        for(int i=MANCALASIZE-2; i>BINS; i--){
            cout<<mancala[i]<<" ";
        }
        cout<<"|"<<endl<<"  | ";
        for(int i=0; i<BINS; i++){
            cout<<mancala[i]<<" ";
        }
        cout<<"| "<<mancala[BINS]<<endl<<endl;
    }

    ~Board(){
        mancala.clear();
        // if(parent!=NULL)
        //     delete parent;
        parent=NULL;
    }
};


//down true, up false, ekta player holei hoy may be
long h1(Board* b, bool player){
    return (b->getValueOfStorage(player)-b->getValueOfStorage(!player));
}

long h2(Board* b, bool player){
    int r=0,s=0;
    r=h2w1*(b->getValueOfStorage(player)-b->getValueOfStorage(!player));
    s=h2w2*(b->getValueOfBins(player)-b->getValueOfBins(!player));
    r+=s;
    return r;
}

long h3(Board* b, bool player){
    int r=0,s=0;
    r=h3w1*(b->getValueOfStorage(player)-b->getValueOfStorage(!player));
    s=h3w2*(b->getValueOfBins(player)-b->getValueOfBins(!player));
    r+=s;
    s=h3w3*(b->getAdditionalMoves(player));
    r+=s;
    return r;
}

long h4(Board* b, bool player){
    int r=0,s=0;
    r=h4w1*(b->getValueOfStorage(player)-b->getValueOfStorage(!player));
    s=h4w2*(b->getValueOfBins(player)-b->getValueOfBins(!player));
    r+=s;
    s=h4w3*(b->getAdditionalMoves(player));
    r+=s;
    s=h4w4*(b->getStoneCapture(player));
    r+=s;
    return r;
}

//W1* (stones_on_my_side –stones_on_opponents_side) + W2 * (additional_move_earned) + W3 * (stones_captured)
long h5(Board* b, bool player){
    int r=0,s=0;
    r=h5w1*(b->getValueOfBins(player)-b->getValueOfBins(!player));
    s=h5w2*(b->getAdditionalMoves(player)-b->getAdditionalMoves(!player));
    r+=s;
    s=h5w3*(b->getStoneCapture(player)-b->getStoneCapture(!player));
    r+=s;
    return r;
}

//W1* (stones_on_my_side –stones_on_opponents_side) + W2 * (additional_move_earned) + W3 * (stones_captured) +W4*(stn in path)
long h6(Board* b, bool player){
    int r=0,s=0;
    r=h6w1*(b->getValueOfBins(player)-b->getValueOfBins(!player));
    s=h6w2*(b->getAdditionalMoves(player)-b->getAdditionalMoves(!player));
    r+=s;
    s=h6w3*(b->getStoneCapture(player)-b->getStoneCapture(!player));
    r+=s;
    s=h6w4*(b->getStoneInPath(player)-b->getStoneInPath(!player));
    r+=s;
    return r;
}

//W1 * (stones_in_my_storage – stones_in_opponents_storage) + W2 * (stones_on_my_side – stones_on_opponents_side) + W3 * (my_additional_moves - opponents_additional_moves)
long h7(Board* b, bool player){
    int r=0,s=0;
    r=h7w1*(b->getValueOfStorage(player)-b->getValueOfStorage(!player));
    s=h7w2*(b->getValueOfBins(player)-b->getValueOfBins(!player));
    r+=s;
    s=h7w3*(b->getAdditionalMoves(player)-b->getAdditionalMoves(!player));
    r+=s;
    return r;
}


//W1 * (stones_in_my_storage – stones_in_opponents_storage) + W2 * (stones_on_my_side – stones_on_opponents_side) + W3 * (my_additional_moves - opponents_additional_moves) + W4 * (my_stones_captured - opponents_stons_captured)
long h8(Board* b, bool player){
    int r=0,s=0;
    r=h8w1*(b->getValueOfStorage(player)-b->getValueOfStorage(!player));
    s=h8w2*(b->getValueOfBins(player)-b->getValueOfBins(!player));
    r+=s;
    s=h8w3*(b->getAdditionalMoves(player)-b->getAdditionalMoves(!player));
    r+=s;
    s=h8w4*(b->getStoneCapture(player)-b->getStoneCapture(!player));
    r+=s;
    return r;
}

long Heuristic(Board* b, bool player, int choise=1){
    if (choise==1){
        return h1(b,player);
    }
    else if (choise==2){
        return h2(b,player);
    }
    else if (choise==3){
        return h3(b,player);
    }
    else if (choise==4){
        return h4(b,player);
    }
    else if (choise==5){
        return h5(b,player);
    }
    else if (choise==6){
        return h6(b,player);
    }
    else if (choise==7){
        return h5(b,player);
    }
    else if (choise==8){
        return h6(b,player);
    }
    else{
        return h1(b,player);
    }  
}

Board* createBoard(Board* parent, int pos, bool player){ //curr player
    Board* b=new Board();
    b->setParent(parent);
    b->setMancala(parent->getMancala());
    b->copyExtraInfo(parent->getStoneCapture(player), parent->getAdditionalMoves(player), parent->getStoneInPath(player), player);
    b->copyExtraInfo(parent->getStoneCapture(!player), parent->getAdditionalMoves(!player), parent->getStoneInPath(!player), !player);
    
    b->move(pos, player); //pos returned by game play
    //printBoard(b->getParent()->getNode(),grid);
    return b;
}


pair<int, int>  minimax(Board* b, int depth, bool player, int alpha, int beta, int level, bool prune=true){
    pair<int, int> pr; //{pos, pruneVal}
    int pos;
    int val;
    int zeros=0;
    Board* newBoard;
    vector<pair<int, int> > prs;
    //gen BIN num of diff movement...

    if(depth==0){
        val=Heuristic(b,player, level);
       // cout<<"H(n) "<<val<<endl;
        pr=make_pair(-1, val);
        return pr;
    }
    
    if(player){ //white
        val=INT_MIN;
        for(int i=0; i<BINS; i++){
            if(b->getMancala()[i]==0){
                zeros++; //all empty?
                continue;
            }
            newBoard=createBoard(b,i,player);
            pr=minimax(newBoard, depth-1, newBoard->nxtPlr(), alpha, beta, level, prune);
            if (newBoard!=NULL)
                delete newBoard;

            if(val<pr.second){ 
                pos=i;
                val=pr.second;

                // prs.clear();            //randomize select for multiple same h(n)
                // pr=make_pair(pos,val);
                // prs.push_back(pr);
            }

            // if(val==pr.second){
            //     pr=make_pair(i,val);
            //     prs.push_back(pr);
            // }

            if(prune){                 //minimax with prune
                alpha=max(alpha,val); 
                if(alpha>=beta)
                    break;
            }
        }
        if(zeros==BINS){ //end game
            pos=-1;
            val=Heuristic(b,player,level);
            
            // prs.clear();
            // pr=make_pair(pos,val);
            // prs.push_back(pr);
        }

        // val=rand()%prs.size();
        // pr=prs[val];
        pr=make_pair(pos,val);
        return pr;
    }
    else{ //black
        val=INT_MAX;
        for(int i=BINS+1; i<MANCALASIZE-1; i++){
            if(b->getMancala()[i]==0){
                zeros++; //all empty?
                continue;
            }
            newBoard=createBoard(b,i,player);
            pr=minimax(newBoard, depth-1, newBoard->nxtPlr(), alpha, beta, level, prune);
            if (newBoard!=NULL)
                delete newBoard;

            if(val>pr.second){
                pos=i;
                val=pr.second;

                // prs.clear();            //randomize select for multiple same h(n)
                // pr=make_pair(pos,val);
                // prs.push_back(pr);
            }
            
            // if(val==pr.second){
            //     pr=make_pair(i,val);
            //     prs.push_back(pr);
            // }

            if(prune){
                beta=min(beta,val);  //prune
                if(alpha>=beta)
                    break;
            }
        }
        if(zeros==BINS){ //end game
            pos=-1;
            val=Heuristic(b,player,level);

            // prs.clear();
            // pr=make_pair(pos,val);
            // prs.push_back(pr);
        }

        // val=rand()%prs.size();
        // pr=prs[val];
        pr=make_pair(pos,val);
        return pr;
    }
}

// memfree
void clear(Board* newBoard){
    Board* p=NULL;
    if(newBoard!=NULL)
        p=newBoard->getParent();
    while(p!=NULL){
        delete newBoard;
        newBoard=p;
        p=newBoard->getParent();
    }
    if(newBoard!=NULL)
        delete newBoard;
}

int playAI(int ai1Level, int ai1Depth, int ai2Level, int ai2Depth, bool algo1=true, bool algo2=true, bool single=true){
    int pos;
    pair<int, int> miniPair;
    Board* newBoard=new Board();
    if(single)
        newBoard->printBoard();
    while(true){
        if(newBoard->nxtPlr()==true){
            //plays AI1
            newBoard->clearExtraInfo();
            miniPair=minimax(newBoard, ai1Depth, true, INT_MIN, INT_MAX, ai1Level, algo1);
            pos=miniPair.first;
            newBoard=createBoard(newBoard,pos,true); //1st player
            if(single){
              cout<<"AI1 POSITION: "<<pos<<endl;
              newBoard->printBoard();
            }
        }
        else {
            //plays AI2
            newBoard->clearExtraInfo();
            newBoard->Rotate(); //2nd er jonno
            miniPair=minimax(newBoard, ai2Depth, true, INT_MIN, INT_MAX, ai2Level, algo2);
            pos=miniPair.first;
            newBoard->Rotate(); //1st er jonno original board
            pos=pos+BINS+1;
            newBoard=createBoard(newBoard,pos,false);
            if(single){
              cout<<"AI2 POSITION: "<<pos<<endl;
              newBoard->printBoard();
            }
        }

        if(newBoard->getValueOfBins()==0 || newBoard->getValueOfBins(false)==0){ //game end
            if(single){
                cout<<"AI1's point: "<<newBoard->getTotalValue()<<endl;
                cout<<"AI2's point: "<<newBoard->getTotalValue(false)<<endl;
            }
          
            if(newBoard->getTotalValue() > newBoard->getTotalValue(false)){
                cout<<"AI1 WINs\n";
                clear(newBoard);
                return 1;
            }
            else if(newBoard->getTotalValue() < newBoard->getTotalValue(false)){
                cout<<"AI2 WINs\n";
                clear(newBoard);
                return -1;
            }
            else{
                cout<<"MATCH DRAW\n";
                clear(newBoard);
                return 0;
            }
        }
    }

}

void playhuman(int level, bool algo=true){
    int pos;
    int depth=level+2;
    pair<int, int> miniPair;
    Board* newBoard=new Board();
    newBoard->printBoard();
    while(true){
        if(newBoard->nxtPlr()==true){
            cout<<"ENTER POSITION: ";
            cin>>pos;
            if(newBoard->getMancala()[pos]==0)
                continue;
            newBoard=createBoard(newBoard,pos,true); //1st player
            newBoard->printBoard();
        }
        else {
            //plays AI
            newBoard->clearExtraInfo();
            newBoard->Rotate(); //2nd er jonno
            miniPair=minimax(newBoard, depth, true, INT_MIN, INT_MAX, level, algo);
            pos=miniPair.first;
            newBoard->Rotate(); //1st er jonno original board
            pos=pos+BINS+1;
            newBoard=createBoard(newBoard,pos,false);
        
            cout<<"AI POSITION: "<<pos<<endl;
            newBoard->printBoard();
        }

        if(newBoard->getValueOfBins()==0 || newBoard->getValueOfBins(false)==0){ //game end
            cout<<"Your point: "<<newBoard->getTotalValue()<<endl;
            cout<<"AI's point: "<<newBoard->getTotalValue(false)<<endl;
            if(newBoard->getTotalValue() > newBoard->getTotalValue(false)){
                cout<<"YOU WIN\n";
            }
            else if(newBoard->getTotalValue() < newBoard->getTotalValue(false)){
                cout<<"AI WINs\n";
            }
            else{
                cout<<"MATCH DRAW\n";
            }
            clear(newBoard);
            break;
        }
    }
}

int main(){
//    while(true){
    // srand(time(0));
    int val;
    int win[Hn];
    int play[Hn];
    long p=0,d=0,w1=0,w2=0;
    int ai1Level, ai2Level, ai1Depth, ai2Depth, algo; 
    bool algorithm1, algorithm2;
    cout<<"SELECT MODE: 1. AI-AI 2.AI-HUMAN 3. AI_AI(100)\nENTER: ";
    cin>>val;
    if(val==1){
        cout<<"AI1's ALGORITHM: 1.MINIMAX 2.MINIMAX WITH PRUNING\nENTER: ";
        cin>>algo;
        if(algo==1){
            algorithm1=false;
        }
        else{
            algorithm1=true;
        }
        cout<<"AI2's ALGORITHM: 1.MINIMAX 2.MINIMAX WITH PRUNING\nENTER: ";
        cin>>algo;
        if(algo==1){
            algorithm2=false;
        }
        else{
            algorithm2=true;
        }
        cout<<"AI1's HEURISTIC: 1. 2. 3. 4. 5. 6. 7. 8.\nENTER: ";
        cin>>ai1Level;
        cout<<"AI2's HEURISTIC: 1. 2. 3. 4. 5. 6. 7. 8.\nENTER: ";
        cin>>ai2Level;
        cout<<"AI1's Depth: 1 - 10\nENTER: ";
        cin>>ai1Depth;
        cout<<"AI2's Depth: 1 - 10\nENTER: ";
        cin>>ai2Depth;
        playAI(ai1Level, ai1Depth, ai2Level, ai2Depth, algorithm1, algorithm2);
    }
    else if(val==2){
        cout<<"1.BEGINNER 2.PRO\nENTER: ";
        cin>>algo;
        if(algo==1){
            algorithm1=false;
        }
        else{
            algorithm1=true;
        }
        cout<<"CHOOSE LEVEL: 1. 2. 3. 4. 5. 6. 7. 8.\nENTER: ";
        cin>>val;
        playhuman(val,algorithm1); //human player 1,
    }
    else{
        for(int i=0; i<Hn; i++){
            play[i]=0;
            win[i]=0;
        }

        for(int i=1; i<=Hn; i++){
            for(int j=1; j<=Hn; j++){
                for(int d1=3; d1<=5; d1++){
                    for(int d2=3; d2<=5; d2++){
                        val=0;
                        play[i-1]++;
                        play[j-1]++;
                        p++;
                        val = playAI(i,d1,j,d2,true,true,false);
                        if(val==1){
                            win[i-1]++;
                            w1++;
                        }
                        else if(val==-1){
                            win[j-1]++;
                            w2++;
                        }
                        else if(val==0){
                            d++;
                        }
                    }
                }
            }
        }

        cout<<"\nWin-loss Ratio-\n";
        for(int i=0; i<Hn; i++){
            cout<<"H"<<(i+1)<<"'s win rate "<<((win[i]*100.0)/play[i])<<"%\n";
        }

        cout<<endl<<"Player statistic-\n";
        cout<<"Player1 win rate "<<(w1*100.0)/p<<"%\n";
        cout<<"Player2 win rate "<<(w2*100.0)/p<<"%\n";
        cout<<"Match draw rate "<<(d*100.0)/p<<"%\n"; 
        cout<<"Match played "<<p<<"\n"; 
    }

//    }
}


