/* @Author
Student Name: <Bahadır Güray Özgödek>
Student ID : <150150013>
Date: <25.12.2018> */

#include <iostream>
#include <fstream>
using namespace std;

 struct treeNode{   //ağacın düğümlerini oluşturmamı sağlayacak struct yapısı
     int number;
     treeNode *left;
     treeNode *right;
 };

 struct queueNode{  //programın içinde queue kullanılacak
    treeNode *data;
    queueNode *next;
 };

 struct stackNode{  // programın içinde stack kullanılacak
     treeNode *data;
     stackNode *next;
 };

struct stackList{   //stack fonksiyonları
    stackNode *head;
    void create();
    void push(treeNode *);
    treeNode* pop();
};

 struct queueList{  //queue fonksiyonları
    queueNode *head;
    queueNode *tail;
    void create();
    void clear();
    void enqueue(treeNode *);
    treeNode* dequeue();
 };

 struct treeList{   //ağacın fonksiyonları
    treeNode *root;
    queueList queueProcess;     // ilk başta ağacı oluştururken kullanılan queue
    queueList searchProcess;    //  path ararken kullanılan queue
    stackList stackProcess;     //  path ararken kullanılan stack 
    bool pathFound = true;  //path bulunup bulunamadığını döndüren boolean değer
    void create();
    void readFile(char *);
    void insert(int , treeNode*);
    void findPath(treeNode * , int);
 };

void stackList::create(){   //stack'i oluşturan fonksiyon
    head = NULL;
}

void stackList::push(treeNode *ptr){    //stack'in içine ağaç düğümü cinsinden eleman ekleyen fonksiyon
    stackNode *newNode = new stackNode;
    newNode->data = ptr;
    newNode->next = NULL;
    if(!head) head = newNode;
    else{
        newNode->next = head;
        head = newNode;
    }
}

treeNode * stackList::pop(){    //stack'ten eleman çekilmesini sağlayan fonksiyon
    stackNode *temp;
    treeNode *tp;
    temp = head;
    if(head->next){     //eğer stack'in içinde 1'den fazla eleman varsa
        tp = temp->data;
        head = head->next;
        delete temp;
        return tp;
    }
    else{   //eğer stack tek elemanlıysa
        tp = temp->data;
        head = NULL;
        delete temp;
        return tp;
    }
}

void queueList::create(){   //queue'yu oluşturan fonksiyon
    head = tail = NULL;
}

void queueList::clear(){    //queue'yu boşaltan fonksiyon
    queueNode *tail, *traverse;
    traverse = head;
    while(traverse){
      tail = traverse;
      traverse = traverse -> next;
      delete tail;
    }
    head = NULL;
}

void queueList::enqueue(treeNode *ptr){     //queue'ya ağaç düğümü cinsinden eleman eklenmesini sağlayan fonksiyon
    queueNode *newNode = new queueNode;  
    newNode->data = ptr;
    newNode->next = NULL;
    if(!head) head = tail = newNode;
    else{
        tail->next = newNode;
        tail = newNode;
    }
}

treeNode* queueList::dequeue(){     //queue'dan eleman çekilmesini sağlayan fonksiyon
    queueNode *temp;
    treeNode *tp;
    temp = head;
    head = head->next;
    tp = temp->data;
    delete temp;
    return tp;
}

void treeList::create(){    //tree'yi oluşturan fonksiyon
    root = NULL;
}

void treeList::insert(int data , treeNode *ptr){    //tree'te eleman ekleyen fonksiyon
    treeNode *newNode = new treeNode;   //yeni düğüm oluşturulur
    newNode->number = data;
    newNode->left = newNode->right = NULL;
    if(!ptr->left){     //ilk başta düğümün solunu kontrol eder
        ptr->left = newNode;
        for(int i=0;i<2;i++) queueProcess.enqueue(newNode);     //eklenen ağaç düğümünü iki defa queue'ya atar çünkü;
        return;                                                 //her ağaç düğümünün sağı ve solu dolmadan yeni düğüme geçilmez
    }
    if(!ptr->right){
        ptr->right = newNode;
        for(int i=0;i<2;i++) queueProcess.enqueue(newNode);     //ağaç düğümünün sağına eklendiği zaman da aynı işlem yapılır.
        return;                                                 //böylece ağaç doğru bir sırayla oluşturulmuş olur.
    }
    if(ptr->left && ptr->right){                                //insert fonksiyonuna parametre olarak sürekli root geldiğinden
        treeNode *temp = queueProcess.dequeue();                //hem solu hem sağı dolu olduğunda queue'dan eleman çekip insert
        insert(data , temp);                                    // fonksiyonunu onun için çağırır
    }
}

void treeList::readFile(char *text){
    create();                       
    queueProcess.create();
    searchProcess.create();             //ağaç, queue'lar ve stack oluşturulur
    stackProcess.create();
    ifstream file(text);
    int data,i=0,counter=0,target;
    int *arr = new int[100];        //ağacın elemanlarını tutan dinamik array
    file >> data;                   //dosyanın ilk elemanını alarak root oluşturulur.
    treeNode *newNode = new treeNode;
    newNode->number = data;
    newNode->left = newNode->right = NULL;
    root = newNode;
    while(file >> data){        //dosyadaki diğer elemanlar dinamik bir array'e atılır.
        *(arr+i) = data;
        i++;
        counter++;              //array'in eleman sayısını tutan değişken.
    }
    for(int i=0;i<counter-1;i++) insert(arr[i] , root);     //dosyanın sol elemanı target olduğu için döngü counter-1 kadar döner.
    target = arr[counter-1];    //target array'in sol elemanıdır.
    searchProcess.enqueue(root);    //path'i tutacak olan queue'ya root düğümü atılır.
    findPath(root->left , target);  //path bulma fonksiyonu left subtree için çağırılır
    if(pathFound)cout << "No Path Found\n"; //pathFound değişkeni başta da belirtildiği gibi yolun bulunup bulunamadığını tutar.
    searchProcess.clear();  //left subtree bittikten sonra path'i tutan queue sıfırlanır
    searchProcess.enqueue(root);    // ve right subtree kontrol edilmeden önce tekrar içinde root atılır.
    pathFound = true;   //path bulunduysa false olarak değiştirilen değer tekrar true yapılır.
    findPath(root->right , target);     //right subtree için fonksiyon tekrar çağırılır.
    if(pathFound)cout << "No Path Found\n";
}

void treeList::findPath(treeNode *ptr , int target){
    if(pathFound){  //path bulunduysa başka path'lere bakılmaması için şart koşulu
        if(ptr == NULL) return;
        searchProcess.enqueue(ptr);     //preorden olarak gezilen her düğümü path'e atar.
        if(ptr->left && ptr->right) stackProcess.push(ptr);     //eğer ağaç gezilirken iki tarafı da dolu olan bir düğüm varsa 
        queueNode *temp = searchProcess.head;                   //stack'e atılır ki sağ tarafı kontrol edilirken oradan devam edilsin.
        int sum = 0;
        while(temp){    //path'i tutan queue üstünde dolaşılması için temp değişkeni tanımlanır.
            sum += temp->data->number;  //toplam sürekli olarak kontrol edilir.
            if(sum == target){
                cout<<"Path Found: ";
                temp = searchProcess.head;
                while(temp){    //path bulunduysa yazdırılır.
                    cout<<temp->data->number<< " ";
                    temp = temp->next;
                }
                pathFound = false;  //eğer başka yollar da varsa onları yazdırmamak için boolean değer false yapılır.
                return ;
            }
            temp = temp->next;
        }
        if(ptr){    //recursive olarak çağırılan fonksiyonlar sayesinde ağaç preorder olarak dolaşılmış olur.
            findPath(ptr->left  , target);
            if(stackProcess.head != NULL){  //düğümün soluna gidecek yer yoksa ve path bulunamadıysa 
                ptr = stackProcess.pop();   //kalınan yer stack'ten alınır ve path'i tutan queue'nun
                temp = searchProcess.head;  //tail'i stack'ten alınan değere çekilir ki path'in sırası bozulmadan eklenebilsin
                while(temp->data != ptr) temp = temp->next;
                searchProcess.tail = temp;    
            }                
            findPath(ptr->right, target);   
        }
    }
}

int main(int , char **argv){
    treeList BT;    //ağaç objesi oluşturulur.
    BT.readFile(argv[1]);
}
