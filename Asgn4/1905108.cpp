#include<bits/stdc++.h>

using namespace std;

#define RUN 20
#define PERCENT 80
#define GRAPH 0

class Example{
public:
    map<string,string> attr;
    string result;

    void print(void){
        cout<<attr["buying"]<<" "<<attr["maint"]<<" "<<attr["doors"]<<" "<<attr["persons"]<<" "<<attr["lug_boot"]<<" "<<attr["safety"]<<" "<<result<<endl;
    }
};

class Attribute{
public:
    string name;
    int vno;
    string vals[4];

    Attribute(){
        name="";
        vno=0;
        vals[0]="";
        vals[1]="";
        vals[2]="";
        vals[3]="";
    }
};

class AtNode{
    string name;
    bool isleaf;
    string forVal;
    int childno;
    vector<AtNode*> childs;
    AtNode* parent;
public:
    AtNode(){
        name="";
        isleaf=false;
        forVal="";
        childno=0;
        parent=NULL;
    }

    AtNode(string n, bool l){
        name=n;
        isleaf=l;
        forVal="";
        childno=0;
        parent=NULL;
    }

    void setParent(AtNode* p){
        parent=p;
    }

    void setChildno(int c){
        childno=c;
    }

    AtNode* getParent(){
        return parent;
    }

    int getChildno(){
        return childno;
    }

    void setName(string n){
        name=n;
    }

    string getName(){
        return name;
    }

    void forValue(string v){
        forVal=v;
    }

    string forValue(){
        return forVal;
    }

    void setChild(AtNode* c){
        childs.push_back(c);
    }

    AtNode* getChild(int i){
        if(i<childs.size())
            return childs[i];
        else
            return NULL;
    }

    ~AtNode(){
        for(int i=0; i<childs.size(); i++){
            // cout<<"Deleting: "<<childs[i]->getName()<<endl;
            delete childs[i];
        }
        childs.clear();
        parent=NULL;
        // cout<<"Deleting: "<<name<<endl;
    }

};

double entpy(double p){
    double en;
    if(p==0)
        return 0.0;
    en=(-1.0)*p*log10(p)/log10(2);
    return en;
}

double Entropy(double a=0, double b=0, double c=0, double d=0){
    double sum=a+b+c+d;
    double p=1.0*a/sum;
    double en=entpy(p);
    p=1.0*b/sum;
    en+=entpy(p);
    p=1.0*c/sum;
    en+=entpy(p);
    p=1.0*d/sum;
    en+=entpy(p);
    return en;
}

double setEntropy(vector<Example> &exmpl){
    int unacc=0, acc=0, good=0, vgood=0;

    for(int i=0; i<exmpl.size(); i++){
        if(exmpl[i].result=="unacc"){
            unacc++;
        }
        else if(exmpl[i].result=="acc"){
            acc++;
        }
        else if(exmpl[i].result=="good"){
            good++;
        }
        else if(exmpl[i].result=="vgood"){
            vgood++;
        }
    }
    return Entropy(unacc,acc,good,vgood);
}


string Plurality(vector<Example> &exmpl){
    int unacc=0, acc=0, good=0, vgood=0;
    string result;
    int i=0;
    for(i=0; i<exmpl.size(); i++){
        if(exmpl[i].result=="unacc"){
            unacc++;
        }
        else if(exmpl[i].result=="acc"){
            acc++;
        }
        else if(exmpl[i].result=="good"){
            good++;
        }
        else if(exmpl[i].result=="vgood"){
            vgood++;
        }
    }

    result="unacc";
    int maxx=unacc;
    if(maxx<acc){
        result="acc";
        maxx=acc;
    }
    if(maxx<good){
        result="good";
        maxx=good;
    }
    if(maxx<vgood){
        result="vgood";
        maxx=vgood;
    }

    return result;
}

bool Classification(vector<Example> &exmpl){
    string result=exmpl[0].result;
    for(int i=0; i<exmpl.size(); i++){
        if(result!=exmpl[i].result){
            return false;
        }
    }
    return true;
}


Attribute Important(vector<Example> &example, vector<Attribute> &attribute){
    Attribute a;
    double gain=-1.0, ebs=0.0, eas=0.0, currgain=0.0;

    //entropy before-- same for all
    ebs=setEntropy(example);
    // cout<<"Entropy before: "<<ebs<<endl;

    //entropy after
    vector<Example> exmpl1;
    vector<Example> exmpl2;
    vector<Example> exmpl3;
    vector<Example> exmpl4;
    for(int i=0; i<attribute.size(); i++){
        exmpl1.clear();
        exmpl2.clear();
        exmpl3.clear();
        exmpl4.clear();

        for(int j=0; j<example.size(); j++){
            if(example[j].attr[attribute[i].name]==attribute[i].vals[0]){
                exmpl1.push_back(example[j]);
            }
            else if(example[j].attr[attribute[i].name]==attribute[i].vals[1]){
                exmpl2.push_back(example[j]);
            }
            else if(example[j].attr[attribute[i].name]==attribute[i].vals[2]){
                exmpl3.push_back(example[j]);
            }
            else if(example[j].attr[attribute[i].name]==attribute[i].vals[3]){
                exmpl4.push_back(example[j]);
            }            
        }

        eas=0.0;
        if(exmpl1.size()!=0){
            eas+=setEntropy(exmpl1)*exmpl1.size()/example.size();
        }
        if(exmpl2.size()!=0){
            eas+=setEntropy(exmpl2)*exmpl2.size()/example.size();
        }
        if(exmpl3.size()!=0){
            eas+=setEntropy(exmpl3)*exmpl3.size()/example.size();
        }
        if(exmpl4.size()!=0){
            eas+=setEntropy(exmpl4)*exmpl4.size()/example.size();
        }

        currgain=ebs-eas;
        // cout<<"Example size: "<<example.size()<<endl;
        // cout<<"Entropy after: "<<eas<<endl; //entropy after
        // cout<<attribute[i].name<<" "<<currgain<<endl;
        if(currgain>gain){
            gain=currgain;
            a=attribute[i];
        }
    }

    return a;
}

AtNode* root;

AtNode* decisiontree(vector<Example> example, vector<Attribute> attribute, vector<Example> &parentExample){
    AtNode* leaf;
    AtNode* tree;
    AtNode* subtree;
    string plu;
    Attribute a;
    vector<Example> exmpl;
    vector<Attribute> atr;

    if(example.size()==0){
        plu=Plurality(parentExample);
        leaf=new AtNode(plu,true);
        return leaf;
    }
    else if(Classification(example)){
        plu=example[0].result;
        leaf=new AtNode(plu,true);
        return leaf;
    }
    else if(attribute.size()==0){
        plu=Plurality(example);
        leaf=new AtNode(plu,true);
        return leaf;
    }
    // else if(parentExample.size()==0){
    //     leaf=new AtNode("noNode",true);
    //     return leaf;
    // }
    else{
        a=Important(example, attribute);
        tree=new AtNode(a.name,false);
        tree->setChildno(a.vno);

        // cout<<"\n[";
        // for(int i=0; i<attribute.size(); i++){
        //    cout<<attribute[i].name<<" ";
        // }
        // cout<<"]\nImportant: "<<a.name<<endl;

        atr.clear();
        for(int j=0; j<attribute.size(); j++){
            if(attribute[j].name!=a.name){
                atr.push_back(attribute[j]);
            }
        }
   
        for(int i=0; i<a.vno; i++){
            exmpl.clear();
            for(int j=0; j<example.size(); j++){
                if(example[j].attr[a.name]==a.vals[i]){
                    exmpl.push_back(example[j]);
                }
            }
            
            subtree=decisiontree(exmpl,atr,example);
            subtree->setParent(tree);
            subtree->forValue(a.vals[i]);   //edge value

            tree->setChild(subtree);
        }
        return tree;
    }

}

void printTree(AtNode* root, int level=0){
    cout<<root->getName()<<endl;
    
    if(root->getChildno()==0){
        return;
    }
    else{
        level++;
        for(int i=0; i<root->getChildno(); i++){
            for(int j=0; j<level; j++){
                cout<<"  ";
            }
            cout<<root->getChild(i)->forValue()<<"--";
            printTree(root->getChild(i),level);
        }
    }
}


double testTree(AtNode* root, vector<Example> &test){
    AtNode* node;
    string result;
    int count=0, i=0;
    double accuracy=0.0;

    for(i=0; i<test.size(); i++){
        node=root;
        while(node->getChildno()!=0){
            for(int j=0; j<node->getChildno(); j++){
                if(test[i].attr[node->getName()]==node->getChild(j)->forValue()){
                    node=node->getChild(j);
                    break;
                }
            }
        }
        if(node->getName()==test[i].result){
            // cout<<test[i].result<<" "<<node->getName()<<endl;
            count++;
        }
    }

    accuracy=100.0*count/test.size();
    return accuracy;
}


int main(){
    vector<Example> e;
    vector<Example> example;
    vector<Example> test;
    vector<Attribute> a;
    Attribute atr;
    Example ex;
    string s="z";
    string token[7];
    mt19937 generator (time(NULL)); 
    int init=0;
    int c=0;
    int len=0;
    int upto=0,r=0;
    double accuracy=0.0, sd=0.0, mean=0.0;
    double accu[RUN];
    double graph[2000];
        
    freopen("car.data","r",stdin);
    
    while(getline(cin,s)){
        // cin>>s;    
        c=0;
        len=0;
        init=0;
        // cout<<s<<endl;
        for(int i=0; i<s.length(); i++){
            len++;
            if(s[i]==','){
                token[c]=s.substr(init,len-1);
                c++;
                init=i+1;
                len=0;
                // cout<<" "<<token[c-1]<<endl;
            }
        }
        token[c]=s.substr(init,len);
        // cout<<" "<<token[c]<<endl;
        ex.attr["buying"]=token[0];
        ex.attr["maint"]=token[1];
        ex.attr["doors"]=token[2];
        ex.attr["persons"]=token[3];
        ex.attr["lug_boot"]=token[4];
        ex.attr["safety"]=token[5];
        ex.result=token[6];
        e.push_back(ex); 
        
        token[0]="";
        token[1]="";
        token[2]="";
        token[3]="";
        token[4]="";
        token[5]="";
        token[6]="";        
    }

    atr.name="buying";
    atr.vno=4;
    atr.vals[0]="vhigh";
    atr.vals[1]="high";
    atr.vals[2]="med";
    atr.vals[3]="low";
    a.push_back(atr);

    atr.name="maint";	
    atr.vno=4;
    atr.vals[0]="vhigh";
    atr.vals[1]="high";
    atr.vals[2]="med";
    atr.vals[3]="low";
    a.push_back(atr);

    atr.name="doors";
    atr.vno=4;
    atr.vals[0]="2";
    atr.vals[1]="3";
    atr.vals[2]="4";
    atr.vals[3]="5more";
    a.push_back(atr);

    atr.name="persons";
    atr.vno=3;
    atr.vals[0]="2";
    atr.vals[1]="4";
    atr.vals[2]="more";
    atr.vals[3]="";
    a.push_back(atr);

    atr.name="lug_boot";
    atr.vno=3;
    atr.vals[0]="small";
    atr.vals[1]="med";
    atr.vals[2]="big";
    atr.vals[3]="";
    a.push_back(atr);

    atr.name="safety";
    atr.vno=3;
    atr.vals[0]="low";
    atr.vals[1]="med";
    atr.vals[2]="high";
    atr.vals[3]="";
    a.push_back(atr);

    upto=PERCENT*e.size()/100;
      
    for(r=0; r<RUN; r++){
        shuffle(e.begin(),e.end(), generator);
        example.clear();
        test.clear();

        for(int i=0; i<upto; i++){
            example.push_back(e[i]);
        }
        for(int i=upto; i<e.size(); i++){
            test.push_back(e[i]);
        }
        
        root=decisiontree(example,a,example);
        // printTree(root);

        accuracy=testTree(root,test);
        accu[r]=accuracy;
    
        cout<<"Accuracy: "<<accuracy<<endl;
        // cout<<"Example size: "<<example.size()<<endl;
        // cout<<"Test size: "<<test.size()<<endl;
        if(root!=NULL)
            delete root;
        root=NULL;
    }
    cout<<endl;
    
    //mean
    mean=0.0;
    for(r=0; r<RUN; r++){
        mean+=accu[r];
    }
    mean=mean/RUN;

    //standard deviation
    sd=0.0;
    for(r=0; r<RUN; r++){
        sd+=(accu[r]-mean)*(accu[r]-mean);
    }
    sd=sd/(RUN-1);
    sd=sqrt(sd);

    cout<<"Mean: "<<mean<<endl;
    cout<<"Standard Deviation: "<<sd<<endl;


    //graph
    if(GRAPH==1){   
        freopen("graph.csv","w",stdout);
        cout<<"Traning Set Size,Accuracy"<<endl;
        for(r=1; r<e.size(); r++){
            shuffle(e.begin(),e.end(), generator);
            example.clear();
            test.clear();

            for(int i=0; i<r; i++){
                example.push_back(e[i]);
            }
            for(int i=r; i<e.size(); i++){
                test.push_back(e[i]);
            }
            
            root=decisiontree(example,a,example);
            accuracy=testTree(root,test);
            graph[r]=accuracy;
        
            cout<<r<<","<<accuracy<<endl;
            if(root!=NULL)
                delete root;
            root=NULL;
        }
    }
}