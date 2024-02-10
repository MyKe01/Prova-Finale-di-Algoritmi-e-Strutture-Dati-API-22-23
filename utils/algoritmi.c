#include <stdio.h>
#include <stdlib.h>
#define N 10

typedef struct{
    int a[N];
    int length;
    int heap_size;
}heap_t;

//ordina in modo crescente
//--> T(n) = Θ(n^2)(caso pessimo) e Ω(n) (teta in caso ottimo)
void InsertionSort(int *a){
    int i, j, key;
    for (j=2; j< N; j++){
        key = a[j];
        //inserisce A[j] nella sequenza ordinata A[1..j-1]
        i=j-1;
        while (i>0 && a[i]>key){
            a[i+1] = a[i];
            i--;
        }
        a[i+1] = key;
    }
}


//Fusione di sotto-array. "n" è a.length
//Primo for : Θ(n1) = Θ(q-p+1) = Θ(n/2) = Θ(n).
//Secondo for : Θ(n2) = Θ(r-q) = Θ(n/2) = Θ(n).
// n1 + n2 = n, quindi Θ(n1 + n2)
//Il terzo for : Θ(n). 
//-->  T(n) = 3*Θ(n) = Θ(n).
void Merge(int *a, int p, int q, int f){
    int i, j, k, n1, n2, *l, *r; 
    n1 = q-p+1;
    n2 = f-q;
    l = malloc(sizeof(int)*(n1));
    r = malloc(sizeof(int)*(n2));
    for(i=1; i<n1; i++){
        l[i] = a[p+i-1];
    }
    for(i=1; i<n1; i++){
        r[i] = a[q+i];
    }
    l[n1+1] = 1000;
    r[n2+1] = 1000;
    i=1;
    j=1;
    for(k=p; k<f; k++){ 
        if(l[i]<=r[j]){
            a[k] = l[i];
            i++;
        }else{
            a[k] = r[j];
            j++;
        }
    }
    free(l);
    free(r);
}

//Per ordinare un array A invoco MergeSort(A, 1, A.length).
//Metodo "Divide et Impera", un array da 2 elementi è già ordinato.
//Divido l'array in sotto-array.
// T(n) = Θ(1) se n <2 . 
// T(n) = Θ(n) + 2T(n/2) else
//caso 2 del master th. : 
// --> T(n) =  Θ(nlog(n)) 
void MergeSort(int *a, int p, int r){
    int q;
    if (p<r){
        q = (int) (p+r)/2; 
        MergeSort(a, p, q);
        MergeSort(a, q+1, r);
        Merge(a, p, q, r);
    }
}


void MergeSortInPlace(int *a, int p, int r){
    
}
//operazioni sugli heap

//padre del nodo i
int Parent(int i){
    return (int)i/2;
}

//figlio sinistro del padre i
int Left(i){
    return 2*i;
}

//figlio destro del padre i
int Right(i){
    return 2*i +1;
}


// dato un elemento di un array tale che i suoi figli sinistro e 
// destro sono dei max-heap, ma in cui A[i] (la radice del sottoalbero) 
// potrebbe essere < dei suoi figli, modifica l'array in modo che 
// tutto l'albero di radice A[i] sia un max-heap.
// T(n) = O(log(n)) (h altezza dell'albero e T(n) = O(h), ma essendo quasi-completo h= logn)
void MaxHeapify(heap_t *h, int i){
    int l, r, max, temp;
    l = Left(i);
    r = Right(i);
    if (l<= h->heap_size && h->a[l]>h->a[i]){
        max = l;
    }else{
        max = i;
    }
    if(r<=  h->heap_size && h->a[r] > h->a[max]){
        max = r;
    }
    if(max!=i){
        temp = h->a[max];
        h->a[max] = h->a[i];
        h->a[i] = temp;
        MaxHeapify(h, max);
    }
}

//costruisco un maxheap a partire dalle foglie.
//altezza di un albero quasi completo di n nodi è log2(n)
//--> T(n) = O(n)
void BuildMaxHeap(heap_t *h){
    int i;
    h->length = N;
    h->heap_size = h->length;
    for (i= h->length/2; i>=0; i--){//ripetuto O(n)
        MaxHeapify(h, i);   //O(log(n))
    }
}

//A ogni ciclo piazziamo l'elemento più grande in fondo alla parte 
// di array ancora da ordinare.
//T(n) = O(nlog(n)).
void HeapSort(heap_t *h){
    int i, temp;
    BuildMaxHeap(h); //O(n)
    for (i=h->length; i>=1; i--){ //si ripete n volte
        temp = h->a[1]; //si ripete n-1 volte
        h->a[1] = h->a[i];
        h->a[i] = temp;
        h->heap_size --;
        MaxHeapify(h, 1); //O(log(n))
    }
}

//Nel caso pessimo T(n) = Θ(n^2).
//In media ha complessità Θ(nlog(n)).
//riorganizza in A[p..r] 2 sottoarray A[p..q-1] e A[q+1..r] 
//tali che tutti gli elementi di A[p..q-1] siano ≤ A[q] e tutti gli 
//elementi di A[q+1..r] siano ≥A[q]
//Se ogni volta uno dei 2 sottoarray è vuoto e l'altro contiene n-1 elementi si ha il caso 
//pessimo; la ricorrenza in questo caso è:
// --> T(n) = T(n-1) + Θ(n) = O(n^2)
void QuickSort(int *a, int p, int r){
    int q;
    if (p<r){
        q=Partition(a, p, r);
        QuickSort(a, p, q-1);
        QuickSort(a,q+1, r);
    }
}

//-->T(n) = Θ(n) con n = r-p+1
int Partition(int *a, int p, int r){
    int x, i, j, temp;
    x = a[r];
    i=p-1;
    for(j=p; j<=r-1; j++){
        if(a[j]<=x){
            i++;
            temp = a[i]; 
            a[i] = a[j];
            a[j] = temp;
        }
    }
    temp = a[i+1]; 
    a[i+1] = a[r];
    a[r] = temp;
    return i+1;
}

//A è l'array in input(disordinato), B conterrà gli elementi
//ordinati(output), k è il massimo tra i valori di A
//--> T(n) = Θ(k + n)
void CountingSort(int *a, int *b, int k){
    int i, c[k+1],j;
    for(i=0; i<=k; i++){ //Θ(k)
        c[i] = 0;
    }
    for(j=0; j<=N-1; j++){//Θ(n)
        c[a[j]] ++; 
    }
    for(i=0; i<=k; i++){//Θ(k)
        c[i] = c[i] +c[i-1]; 
        //c[i] ora contiene il numero di elementi minori di i
    }
    for(j=N-1; j>=0; j--){//Θ(n)
        b[c[a[j]]] = a[j];
        c[a[j]] = c[a[j]] -1;
    }
}

//Pila, top ultimo elemento inserito in arr
typedef struct{
    int arr[N];
    int top;
    int length;
}stack_t;

void initStack(stack_t *p){
    p->top = -1; //lista vuota
    p->length = N-1;
}

void PushStack(stack_t *p, int x){
    if(p->top = p->length){
        printf("overflow");
    }else{
        p->top ++;
        p->arr[p->top] = x;
    }
}

int PopStack(stack_t *p){
    if(p->top ==-1){
        printf("Underflow");
    }else{
        p->top --;
        return p->arr[p->top+1];
    }
}

int checkEmptyStack(stack_t *p){
    if(p->top ==-1){
        return 1;
    }else{
        return 0;
    }
}

//arr, head è l'elemento più vecchio e
//tail è l'indice in cui il prossimo elemento sarà inserito.
typedef struct{
    int arr[N];
    int head;
    int tail;
    int length;
}queue_t;

void initQueue(queue_t *q){
    q->head = 0;
    q->tail = 0;
    q->length = N-1;
}

// --> T(n) = O(1)
void enqueue(queue_t *q, int x){
    q->arr[q->tail] = x;
    if(q->tail == q->length){
        q->tail = 0;
    }else{
        q->tail ++;
    }
} 

// --> T(n) = O(1)
int dequeue(queue_t *q){
    int x;
    x = q->arr[q->head];
    if(q->head ==q->length){
        q->head = 0;
    }else{
        q->head ++;
    }
    return x;
}


typedef struct nodo{
    int key;
    struct nodo *next;
    struct nodo *prev;
}nodo_t;

typedef nodo_t *Ptr_nodo;

typedef struct{
    Ptr_nodo head;
}list_t;

void list_free(list_t l){
    Ptr_nodo tmp, a;
    tmp=l.head;
    while(tmp){
        a = tmp->next;
        free(tmp);
        tmp = a;
    }
}

//--> T(n) = O(1)
void listInsert(list_t l, Ptr_nodo x){
    x->next=l.head;
    if(l.head!=NULL){
        l.head->prev = x;
    }
    l.head = x;
    x->prev = NULL;
}

//Nel caso pessimo (k not in list) :
//-->T(n) = Θ(n)
Ptr_nodo listSearch(list_t l, int k){
    Ptr_nodo x;
    x=l.head;
    while(x!=NULL && x->key !=k){
        x=x->next;
    }
    return x;
}

//--> T(n) = O(1)
void listDelete(list_t l, Ptr_nodo x){
    if(x->prev!=NULL){
        x->prev->next = x->next;
    }else{
        l.head = x->next;
    }
    if(x->next!=NULL){
        x->next->prev =x->prev;
    }
}

typedef struct{
    int key;
    int value;
}dictObj_t;

//tutte T(n) = O(1)   
int directAccessSearch(int * t, int k){
    return t[k];
}

void directAddressInsert(int *t, dictObj_t x){
    t[x.key] = x.value;
}

void directAddressDelete(int *t, dictObj_t x){
    t[x.key] = NULL;
}

//Alberi e grafi

typedef struct{
    int key;
    struct treenode_t *father;
    struct treenode_t *left;
    struct treenode_t *right;
}treenode_t;

typedef treenode_t *Ptr_treenode;

typedef struct{
    struct treenode_t *root;
}tree_t;

//Se l'albero è BST, inorderTreeWalk(T.root) stampa tutti gli 
//elementi in ordine crescente.
//--> T(n) = Θ(n) 
void inorderTreeWalk(Ptr_treenode x){
    if(x!=NULL){
        inorderTreeWalk(x->left);
        printf("%d", x->key);
        inorderTreeWalk(x->right);
    }
}

void preorderTreeWalk(Ptr_treenode x){
    if(x!=NULL){
        printf("%d", x->key);
        inorderTreeWalk(x->left);
        inorderTreeWalk(x->right);
    }
}

void postorderTreeWalk(Ptr_treenode x){
    if(x!=NULL){
        inorderTreeWalk(x->left);
        inorderTreeWalk(x->right);
        printf("%d", x->key);    
    }
}

//T(n) = O(h) con h altezza dell'albero
Ptr_treenode treeSearch(int k, Ptr_treenode x){
    if(x == NULL || x->key == k){
        return x;
    }
    if(k<x->key){
        return treeSearch(x->left, k);
    }else{
        return treeSearch(x->right, k);
    }
}

Ptr_treenode treeMinimum(Ptr_treenode x){
    while(x->left !=NULL){
        x = x->left;
    }
    return x;
}


Ptr_treenode treeMaximum(Ptr_treenode x){
    while(x->right !=NULL){
        x = x->right;
    }
    return x;
}

//T(n) = O(h)
Ptr_treenode treeSuccessor(Ptr_treenode x){
    Ptr_treenode y;
    if(x->right!=NULL){
        return treeMinimum(x->right);
    }
    y = x->father;
    while(y!=NULL && x == y->right){
        x = y;
        y = y->father;
    }
    return y;
}

//T(n) = O(h)
Ptr_treenode treePredecessor(Ptr_treenode x){
    Ptr_treenode y;
    if(x->left!=NULL){
        return treeMaximum(x->left);
    }
    y = x->father;
    while(y!=NULL && x == y->left){
        x = y;
        y = y->father;
    }
    return y;
}

//T(n) = O(h)
void treeInsert(tree_t T, Ptr_treenode z){
    Ptr_treenode x,y;
    y = NULL;
    x = T.root;
    while(x!=NULL){
        y=x;
        if(z->key<x->key){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    z->father = y;
    if(y==NULL){
        T.root = z; //l'albero T è vuoto
    }else if(z->key<y->key){
        y->left = z;
    }else{
        y->right = z;
    }
}

//y nodo da eliminare, x è quello con cui lo sostituiamo
//T(n) = O(h)
void treeDelete(Ptr_treenode z, tree_t T){
    Ptr_treenode x,y , f;
    if(z->left==NULL ||z->right == NULL){//primo caso
        y = z;
    }else{
        y = treeSuccessor(z);
    }
    if(y->left !=NULL){
        x = y->left;
    }else{
        x = y->right;
    }
    f=y->father;
    if(x!=NULL){
        x->father = y->father;
    }
    if(y->father == NULL){
        T.root = x;
    }else if (y==f->left){
        f->left = x;
    }else{
        f->right = x;
    }
    if(y!=z){
        z->key = y->key;
    }
    return y;
}


void RBdeleteFixup(treenode_t ** T, treenode_t ** x){
    treenode_t * w;
    if((*x)->color == 'R' || (*x)->father == NULL){
        (*x)->color = 'B';
    }else if ((*x) == (*x)->father->left){
        w = (*x)->father->right;
        if(w && w->color == 'R'){
            w->color = 'B';
            (*x)->father->color = 'R';
            LeftRotate(T, &(*x)->father);
            w = (*x)->father->right;
        }
        if(w && w->left && w->right && w->left->color == 'B' && w->right->color == 'B'){
            w->color = 'R';
            RBdeleteFixup(T, &(*x)->father);
        }else if(w && w->right && w->left){
            if(w->right->color == 'B'){
                w->left->color = 'B';
                w->color = 'R';
                RightRotate(T, &w);
                w= (*x)->father->right;
            }
            w->color = (*x)->father->color;
            (*x)->father->color = 'B';
            w->right->color = 'B';
            LeftRotate(T, &(*x)->father);
        }
    }else{
        w = (*x)->father->left;
        if(w && w->color == 'R'){
            w->color = 'B';
            (*x)->father->color = 'R';
            RightRotate(T, &(*x)->father);
            w = (*x)->father->left;
        }
        if(w && w->left && w->right && w->left->color == 'B' && w->right->color == 'B'){
            w->color = 'R';
            RBdeleteFixup(T, &(*x)->father);
        }else if(w && w->right && w->left){
            if(w->left->color == 'B'){
                w->right->color = 'B';
                w->color = 'R';
                LeftRotate(T, &w);
                w= (*x)->father->left;
            }
            w->color = (*x)->father->color;
            (*x)->father->color = 'B';
            w->left->color = 'B';
            RightRotate(T, &(*x)->father);
        }
    }
}

treenode_t * treeDelete(treenode_t * T, treenode_t * z){
    treenode_t * x, * y;
    if(z->left ==NULL || z->right == NULL){
        y=z;
    }else{
        y= RBtreeSuccessor(z);
    }
    if(y->left !=NULL){
        x = y->left;
    }else{
        x = y->right;
    }
    if(x!=NULL){
        x->father = y->father;
    }
    if(y->father == NULL){
        T = x;
    }else if(y == y->father->left){
        y->father->left = x;
    }else{
        y->father->right = x;
    }
    if ( y!=z){
        strcpy(z->key, y->key);
    }
    if(x && y->color =='B'){
        RBdeleteFixup(&T, &x);
    }
    return y;
}

//alberi RB h= O(log(n))
typedef struct RBtreenode{
    int key;
    char color;
    struct RBtreenode *father;
    struct RBtreenode *left;
    struct RBtreenode *right;
}RBtreenode_t;

typedef RBtreenode_t *Ptr_RBtreenode;

typedef struct{
    struct RBtreenode_t *root;
    struct RBtreenode_t *nil;
}RBtree_t;


//T(n) = O(h) con h altezza dell'albero
Ptr_RBtreenode RBtreeSearch(int k, Ptr_RBtreenode x){
    if(x == NULL || x->key == k){
        return x;
    }
    if(k<x->key){
        return treeSearch(x->left, k);
    }else{
        return treeSearch(x->right, k);
    }
}

Ptr_RBtreenode RBtreeMinimum(Ptr_RBtreenode x){
    while(x->left !=NULL){
        x = x->left;
    }
    return x;
}


Ptr_RBtreenode RBtreeMaximum(Ptr_RBtreenode x){
    while(x->right !=NULL){
        x = x->right;
    }
    return x;
}

//T(n) = O(h)
Ptr_RBtreenode RBtreeSuccessor(Ptr_RBtreenode x){
    Ptr_RBtreenode y;
    if(x->right!=NULL){
        return RBtreeMinimum(x->right);
    }
    while(x->father!=NULL && x == x->father->right){
        x = y;
        y = y->father;
    }
    return y;
}

//T(n) = O(h)
Ptr_RBtreenode RBtreePredecessor(Ptr_RBtreenode x){
    Ptr_RBtreenode y;
    if(x->left!=NULL){
        return RBtreeMaximum(x->left);
    }
    while(x->father!=NULL && x == x->father->left){
        x = y;
        y = y->father;
    }
    return y;
}

void leftRotate(RBtree_t T, Ptr_RBtreenode x){
    Ptr_RBtreenode y;
    y = x->right;
    x->right = y->left;//il sottoalbero sinistro di y diventa quello destro di x
    if(y->left != T.nil){
        y->left->father = x;
    }
    y->father =x->father; 
    if(x->father == T.nil){
        T.root = y;
    }else if (x==x->father->left){
        x->father->left = y;
    }else{
        x->father->right = y;
    }
    y->left = x;
    x->father = y;
}

void rightRotate(RBtree_t T, Ptr_RBtreenode y){
    Ptr_RBtreenode x;
    x = y->right;
    y->left = x->right;//il sottoalbero sinistro di y diventa quello destro di x
    if(x->right != T.nil){
        x->right->father = y;
    }
    x->father =y->father; 
    if(x->father == T.nil){
        T.root = x;
    }else if (y==y->father->left){
        y->father->left = x;
    }else{
        y->father->right = x;
    }
    x->left = y;
    y->father = x;
}   

void RBinsert(RBtree_t T, Ptr_RBtreenode z){
    Ptr_RBtreenode x,y;
    y=T.nil;
    x=T.root;
    while(x!=T.nil){
        y=x;
        if(z->key<x->key){
            x=x->left;
        }else{
            x=x->right;
        }
    }
    z->father = y;
    if(y==T.nil){
        T.root = z;
    }else if( z->key<y->key){
        y->left = z;
    }else{
        y->right = z;
    }
    z->left = T.nil;
    z->right = T.nil;
    z->color = "R";
    RBinsertFixup(T, z);
}

void RBinsertFixup(RBtree_t T, Ptr_RBtreenode z){
    Ptr_RBtreenode x, y;
    x = T.root;
    if(z==T.root){
        x->color = "B";
    }else{
        x = z->father;
        if(x->color == "R"){
            if(x == x->father->left){
                y = x->father->right;
                if(y->color == "R"){  //caso 1
                    x->color = "B";
                    y->color = "B";
                    x->father->color = "R";
                    RBinsertFixup(T,  x->father);
                }else{
                    if(z == x->right){//caso 2
                        z = x;
                        leftRotate(T, z);
                        x = z->father;
                    }
                    x->color = "B";   //caso3
                    x->father->color = "R";
                    rightRotate(T, x->father);         
                }
            }else{
                y =  x->father->left;
                if(y->color == "R"){  //caso 1
                    x->color = "B";
                    y->color = "B";
                    x->father->color = "R";
                    RBinsertFixup(T, x->father);
                }else{
                    if(z == x->left){//caso 2
                        z = x;
                        leftRotate(T, z);
                        x = z->father;
                    }
                    x->color = "B";   //caso3
                    x->father->color = "R";
                    rightRotate(T, x->father);         
                }
            }
        }
    }
}


void RBdeleteFixup(treenode_t ** T, treenode_t ** x){
    treenode_t * w;
    if((*x)->color == 'R' || (*x)->father == NULL){
        (*x)->color = 'B';
    }else if ((*x) == (*x)->father->left){
        w = (*x)->father->right;
        if(w && w->color == 'R'){
            w->color = 'B';
            (*x)->father->color = 'R';
            LeftRotate(T, &(*x)->father);
            w = (*x)->father->right;
        }
        if(w && w->left && w->right && w->left->color == 'B' && w->right->color == 'B'){
            w->color = 'R';
            RBdeleteFixup(T, &(*x)->father);
        }else if(w && w->right && w->left){
            if(w->right->color == 'B'){
                w->left->color = 'B';
                w->color = 'R';
                RightRotate(T, &w);
                w= (*x)->father->right;
            }
            w->color = (*x)->father->color;
            (*x)->father->color = 'B';
            w->right->color = 'B';
            LeftRotate(T, &(*x)->father);
        }
    }else{
        w = (*x)->father->left;
        if(w && w->color == 'R'){
            w->color = 'B';
            (*x)->father->color = 'R';
            RightRotate(T, &(*x)->father);
            w = (*x)->father->left;
        }
        if(w && w->left && w->right && w->left->color == 'B' && w->right->color == 'B'){
            w->color = 'R';
            RBdeleteFixup(T, &(*x)->father);
        }else if(w && w->right && w->left){
            if(w->left->color == 'B'){
                w->right->color = 'B';
                w->color = 'R';
                LeftRotate(T, &w);
                w= (*x)->father->left;
            }
            w->color = (*x)->father->color;
            (*x)->father->color = 'B';
            w->left->color = 'B';
            RightRotate(T, &(*x)->father);
        }
    }
}

Ptr_RBtreenode RBdelete(RBtree_t T, Ptr_RBtreenode z){
    Ptr_RBtreenode x, y;
    if(z->left ==T.nil || z->right == T.nil){
        y=z;
    }else{
        y= RBtreeSuccessor(z);
    }
    if(y->left !=T.nil){
        x = y->left;
    }else{
        x = y->right;
    }
    x->father = y->father;
    if(y->father == T.nil){
        T.root = x;
    }else if(y == y->father->left){
        y->father->left = x;
    }else{
        y->father->right = x;
    }
    if ( y!=z){
        z->key = y->key;
    }
    if(y->color =="B"){
        RBdeleteFixup(T, x);
    }
    return y;
}

typedef struct grafonodo{
    int key;
    int dist; //distanza da un nodo s
    char color;
    struct nodo *next;
}grafonodo_t;

typedef grafonodo_t *Ptr_grafonodo;

typedef struct{
    grafonodo_t arr[N];
    int head;
    int tail;
    int length;
}queueGrafo_t;


//• La complessità totale di BFS è O(|V| + |E|)
void Bfs(queueGrafo_t * G,Ptr_grafonodo s){
    queueGrafo_t * q;
    Ptr_grafonodo u, n;
    int i;
    for(i=0; i<N; i++){
        G->arr[i].color = "W";
        G->arr[i].dist = 1000;
    }
    s->color = "G";
    s->dist = 0;
    q = NULL;
    enqueueGrafo(q, *s);
    while(q!=NULL){
        u = dequeue(q);
        n = u->next;
        while(n!=NULL){
            if(n->color=="W"){
                n->color = "G";
                n->dist = u->dist+1;
                enqueueGrafo(q, *n);
            }  
            n = n->next;
        }
        u->color = "B";
    }
}

void enqueueGrafo(queueGrafo_t *q, grafonodo_t x){
    q->arr[q->tail] = x;
    if(q->tail == q->length){
        q->tail = 0;
    }else{
        q->tail ++;
    }
} 

grafonodo_t dequeueGrafo(queueGrafo_t *q){
    grafonodo_t x;
    x = q->arr[q->head];
    if(q->head ==q->length){
        q->head = 0;
    }else{
        q->head ++;
    }
    return x;
}
