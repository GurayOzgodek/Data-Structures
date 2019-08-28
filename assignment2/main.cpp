#include<iostream>
#include<fstream>
using namespace std;

struct node{
int data;
node*next;
};

struct stackList{
node *head;
int counter; //it holds the number of players' card 
void push(int);
int pop();
void create();
bool isEmpty();
}firstPlayer,secondPlayer,table,bin;

void stackList::create(){
head = NULL;
}

void stackList::push(int data){
node *newNode = new node;
newNode->data = data;
if(!head){
create();
head = newNode;
newNode->next = NULL;
}
else{
newNode->next=head;
head = newNode;
}
counter++; 
}

int stackList::pop(){
node *temporary;
int temp;
temporary = head;
head = head->next;
temp = temporary->data;
counter--;
delete temporary;
return temp;
}

void readFile(char *text){
ifstream file(text);
int playerDeckCount,tableDeckCount,number;
file >> tableDeckCount >> playerDeckCount;
for(int i=0; i<tableDeckCount;i++){
file>>number;
table.push(number); 
}
for(int i=0; i<playerDeckCount;i++){
file>>number;
firstPlayer.push(number);
}
for(int i=0; i<playerDeckCount;i++){
file>>number;
secondPlayer.push(number);
}
}
bool gameOver(){
if(firstPlayer.counter == 0 || secondPlayer.counter == 0 || table.counter == 0) return 0;
else return 1;
}
void gaming(){
int term =0,popTable,popPlayer; //(term = 0,2,4,6... -> first player && term=1,3,5,7...->second player)
bool cont = gameOver();
while(cont){
popTable = table.pop();
if(term%2 != 0) popTable *= -1; // if second player's term, to reverse the process
if(popTable < 0){
if(-1*popTable > firstPlayer.counter) popTable = firstPlayer.counter * -1; //if player has not enough card to give 
for(int i=0;i<-1*popTable;i++){
if(firstPlayer.head->data > secondPlayer.head->data){
popPlayer = firstPlayer.pop();
secondPlayer.push(popPlayer);
}
else{
popPlayer = firstPlayer.pop();
bin.push(popPlayer);
}
}
}
else{ //if popTable > 0
if(popTable > secondPlayer.counter) popTable = secondPlayer.counter; //if player has not enough card to give 
for(int i=0;i<popTable;i++){
if(secondPlayer.head->data > firstPlayer.head->data){
popPlayer = secondPlayer.pop();
firstPlayer.push(popPlayer);
}
else{
popPlayer = secondPlayer.pop();
bin.push(popPlayer);
}
}
}
cont=gameOver();
term++;
}
}
int main(int argc , char *argv[]){
firstPlayer.counter=0;
secondPlayer.counter=0;
table.counter=0;
bin.counter=0;
readFile (argv[1]); 
gaming();
cout<<bin.counter<<endl;
}

