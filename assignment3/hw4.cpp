/* @Author
Student Name: <Bahadır Güray Özgödek>
Student ID : <150150013>
Date: <13.12.2018> */

#include<iostream>
#include<fstream>
using namespace std;

struct node{
    int data;
    node *next;
};
struct queueAnt{
    node *head;
    node *tail;
    void create();
    void enqueue(int);
    int dequeue();
};

struct stackAnt {
    node *top;
    void create();
    void push(int);
    int pop();
};

struct Ants {
    queueAnt ants ;
    queueAnt holeDepths ;
    stackAnt hole ;
    void ReadFile ( char *);
    void ShowContents ( bool );
    void CrossRoad ();
};

void queueAnt::create(){
    head = NULL;
    tail = NULL;
}
void queueAnt::enqueue(int data){
    node *newNode = new node;
    newNode->data = data;
    newNode->next = NULL;
    if(!tail){
        tail = newNode;
        head = newNode;
    }
    else{
        tail->next = newNode;
        tail = newNode;
    }
}

int queueAnt::dequeue(){
    node *temporary;
    int temp;
    temporary = head;
    head = head->next;
    temp = temporary->data;
    delete temporary;
    return temp;
}

void stackAnt::create(){
    top = NULL;
}

void stackAnt::push(int data){
    node *newNode = new node;
    newNode->data = data;
    if(!top){
        top = newNode;
        top->next = NULL;
    }
    else{
        newNode->next = top;
        top = newNode;
    }
}

int stackAnt::pop(){
    node *temporary;
    int temp;
    temporary = top;
    top = top->next;
    temp = temporary->data;
    delete temporary;
    return temp;
}

void Ants::ReadFile(char *text){
    ants.create();
    holeDepths.create();
    hole.create();
    int antsNumber, hole;
    ifstream file (text);
    file >> antsNumber;     //number on the first line is holded by antsNumber
    for(int i=1;i<=antsNumber;i++){
        ants.enqueue(i);    
    }
    while(!file.eof()){     //numbers on the second line are pushed into the holeDepths
        file >> hole;
        if(!file.eof())
        holeDepths.enqueue(hole);
    }
}

void Ants::ShowContents(bool bl){
    if(bl == true){
        node *traverse;
        traverse = ants.head;
        while(traverse){
            cout << traverse->data<<" ";
            traverse = traverse->next;
        }
        cout<<endl;
    }
    else{
        node *traverse;
        traverse = holeDepths.head;
        while(traverse){
            cout << traverse->data<<" ";
            traverse = traverse->next;
        }
        cout<<endl;
    }
}

void Ants::CrossRoad(){
    int number;
    node *traverse;
    traverse = holeDepths.head;                     //holeDepths is traversed 
    while(traverse){                                //because while loop is executed 
        for(int i=0; i<traverse->data ; i++){       //as much as element number of holeDepths    
            number = ants.dequeue();                //then for loops are executed as much as   
            hole.push(number);                      //value of holeDepths's elements.
        }
        for(int i=0; i<traverse->data ; i++){
            number = hole.pop();
            ants.enqueue(number);
        }
        traverse = traverse->next;
    }
}

int main ( int argc , char ** argv ){
    Ants a ;
    a.ReadFile ( argv [1]); // store the number of ants and depths of holes
    cout << "The initial Ant sequence is: " ;
    a.ShowContents (1); // list ant sequence ( initially : 1 , 2 , ... , N )
    cout << "The depth of holes are: " ;
    a.ShowContents (0); // list depth of holes
    a.CrossRoad ();
    cout << "The final Ant sequence is: " ;
    a.ShowContents (1);
    return 0;
}
