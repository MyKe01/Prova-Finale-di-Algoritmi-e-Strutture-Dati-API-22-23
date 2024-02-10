#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 70

typedef struct{
    int freq;
    char key;
    int maxtemp;
}freq_t;

typedef struct{
    char arr[N]; //elementi che non ci sono
    char **not_here; //array con in posizione i-esima un carattere che non è nella posizione i-esima di rif ma che è in rif
    freq_t *frequencies; //tabella hash su k chiavi che incrementa il valore della casella quando trova un | (da inizializzare a 0)
    freq_t *maxfrequencies;
    char sim[]; //array con in posizione i-esima un carattere che è nell'i-esima posizione di rif
}constraints_t;

typedef struct treenodo{
    char color;
    struct treenodo *father;
    struct treenodo *left;
    struct treenodo *right;
    char key[];
}treenode_t;

typedef struct{
    struct treenodo * root;
}tree_t;

int fastStrcmp(char * str, char * str1, int k){
    int i;
    for(i=0; i<k; i++){
        if(*(str+i)<*(str1+i)){
            return -1;
        }
        if(*(str+i)>*(str1+i)){
            return 1;
        }
    }
    return 0;
}

void inorderTreeWalk(treenode_t * x){
    if(x!=NULL){
        if(x->left){
            inorderTreeWalk(x->left);
        }
        fprintf(stdout, "%s", x->key);
        if(x->right){
            inorderTreeWalk(x->right);
        }
    }
}

void LeftRotate(treenode_t  ** T,treenode_t ** x){
    treenode_t *y = (*x)->right;
    (*x)->right = y->left;
    if(y->left!=NULL){
        y->left->father = *x;
    }
    y->father = (*x)->father;
    if((*x)->father == NULL) {
        *T = y;
    }else if(*x == (*x)->father->left) {
        (*x)->father->left = y;
    }else {
        (*x)->father->right = y;
    }
    y->left = *x;
    (*x)->father = y;
}

void RightRotate(treenode_t ** T,treenode_t ** x) {
    treenode_t * y = (*x)->left;
    (*x)->left = y->right;
    if (y->right != NULL) {
        y->right->father = *x;
    }
    y->father = (*x)->father;

    if ((*x)->father == NULL) {
        *T = y;
    } else if ((*x) == (*x)->father->left){
        (*x)->father->left = y;
    }else {
        (*x)->father->right = y;
    }
    y->right = *x;
    (*x)->father = y;
}

void RBinsertfixup(treenode_t ** T, treenode_t ** z){
    treenode_t *father, *grandparent, *y =NULL;
    if((*z)==(*T)){
        (*T)->color = 'B';
    }else{
        father = (*z)->father;
        if(father->color == 'R'){
            grandparent = father->father;
            if(grandparent!=NULL){
                if(father == grandparent->left){
                    y = grandparent->right;
                    if(y!=NULL && y->color == 'R'){
                        father->color = 'B';
                        y->color = 'B';
                        grandparent->color = 'R';
                        RBinsertfixup(T, &grandparent);
                    }else{
                        if((*z) == father->right ){
                            (*z) = father;
                            LeftRotate(T, z);
                            father = (*z)->father;
                        }
                        father ->color = 'B';
                        grandparent->color = 'R';
                        RightRotate(T, &grandparent);
                    }
                }else{
                    grandparent = father->father;
                    if(grandparent!=NULL){
                        if(father == grandparent->right){
                            y = grandparent->left;
                            if(y!=NULL && y->color == 'R'){
                                father->color = 'B';
                                y->color = 'B';
                                grandparent->color = 'R';
                                RBinsertfixup(T, &grandparent);
                            }else{
                                if((*z) == father->left ){
                                    (*z) = father;
                                    RightRotate(T, z);
                                    father = (*z)->father;
                                }
                                father->color = 'B';
                                grandparent->color = 'R';
                                LeftRotate(T, &(grandparent));
                            }
                        }
                    }
                }
            }
        }
    }
}

int treeSearch(char * str, treenode_t * x, int k){
    int res;
    if(x == NULL){
        return 0;
    }else{
        res = fastStrcmp(x->key, str, k);
        if(!res) {
            return 1;
        }else if(res > 0){
            return treeSearch(str, x->left, k);
        }else{
            return treeSearch(str,x->right, k);
        }
    }
}

void treeInsert(tree_t * T, treenode_t * z, int k){
    treenode_t *x,*y;
    y = NULL;
    x = T->root;
    while(x!=NULL){
        y=x;
        if(fastStrcmp(z->key, x->key, k)<0){
            x = x->left;
        }else{
            x = x->right;
        }
    }
    z->father = y;
    z->color = 'R';
    if(y==NULL){
        T->root = z; //l'albero T è vuoto
        z->color = 'B';
    }else if(fastStrcmp(z->key, y->key, k)<0){
        y->left = z;
    }else{
        y->right = z;
    }
    z->left =NULL;
    z->right = NULL;
    RBinsertfixup(&T->root, &z);
}

int hashFunction(char a, int k){
    return a%k;
}

int inspectionFunction(char a, int k, freq_t *arr){
    int i, res;
    res = hashFunction(a, k);
    i = -res;
    while(i + res <k){
        while(arr[res+i].freq!=0 ){
            if(arr[res+i].key==a){
                //printf("\nSONO NELLA FUNZIONE HASH %c = %c", arr[hashFunction(a,k)+i].key, a);
                return res+i;
            }
            i++;
        }
        if(arr[res+i].key == '\0'){
            arr[res+i].key = a;
            arr[res+i].maxtemp = 0;
            return res+i;
        }else if(arr[res+i].key ==a){
            return res+i;
        }else{
            i++;
        }
    }
    return -1;
}

treenode_t * treeMinimum(treenode_t * x){
    if(x == NULL){
        return NULL;
    }
    while(x->left !=NULL){
        x = x->left;
    }
    return x;
}

treenode_t * treeSuccessor(treenode_t * x){
    treenode_t * y;
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


treenode_t * treeMaximum(treenode_t * x){
    while(x->right !=NULL){
        x = x->right;
    }
    return x;
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

treenode_t * treeDelete(treenode_t ** z, treenode_t ** T, int len){
    treenode_t * x = NULL, *y, *f;
    char str[len];
    if((*z)->left==NULL ||(*z)->right == NULL){//primo caso
        y = *z;
    }else{
        y = treeSuccessor(*z);
    }
    if(y->left !=NULL){
        x = y->left;
    }else if(y->right!=NULL){
        x = y->right;
    }
    f=y->father;
    if(x!=NULL){
        x->father = y->father;
    }
    if(y->father == NULL){
        *T = x;
    }else if (y==f->left){
        f->left = x;
    }else{
        f->right = x;
    }
    if(y!=(*z)){
        strcpy(str, (*z)->key);
        strcpy((*z)->key, y->key);
        strcpy(y->key, str);
    }
    if(x!=NULL && y->color =='B'){
        RBdeleteFixup(T, &x);
    }
    return y;
}


int filter(char * xkey, constraints_t * vincoli, int len){
    int freq,i , j;
    char key;
    for (i = 0; i < len; i++) {
        freq = 0;
        for (j = 0; vincoli->arr[j] != '\0'; j++) {
            if (xkey[i] == vincoli->arr[j]) {
                return 0;
                //printf("\n %s %c 3 ", curr->key, vincoli->arr[j]);
            }
        }
        key = vincoli->maxfrequencies[i].key;
        if (key != '\0') {
            for (j = 0; j < len; j++) {
                if (xkey[j] == key) {
                    freq++;
                }
            }
            if (freq != vincoli->maxfrequencies[i].freq) {
                //printf("\n 5 %s ", curr->key);
                return 0;
            }
        }
        key = vincoli->frequencies[i].key;
        freq = 0;
        if (key != '\0') {
            for (j = 0; j < len; j++) {
                if (xkey[j] == key) {
                    freq++;
                }
            }
            if (freq < vincoli->frequencies[i].freq) {
                return 0;
                //printf("\n 4 - %s - %c - %d ", curr->key,vincoli->frequencies[i].key, vincoli->frequencies[i].freq);
            }
        }
        if (vincoli->sim[i] != '$' && xkey[i] != vincoli->sim[i]) {
            //printf("\n %s 1 ", curr->key);
           return 0;
        }
        for (j = 0; vincoli->not_here[i][j] != '\0'; j++) {
            if (xkey[i] == vincoli->not_here[i][j]) {
                return 0;
                //printf("\n %s 2 ", curr->key);
            }
        }

    }
    return 1;
}

void mergeTree(tree_t * dest, treenode_t * src, int len){
    if (src){
        if(src->left){
            mergeTree(dest, src->left, len);
        }
        if(src->right){
            mergeTree(dest, src->right, len);
        }
        treeInsert(dest, src, len);
    }
}

treenode_t * treePredecessor(treenode_t * x){
    treenode_t * y;
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

int checkVincoli(tree_t * tree, constraints_t *vincoli, int len, tree_t * filtered){
    int counter;
    counter = 0;
    treenode_t * x = treeMinimum(tree->root);
    treenode_t * pred = NULL;
    while(x!=NULL) {
        pred = treePredecessor(x);
        if(filter(x->key, vincoli,len)){
            counter++;
            treeInsert(filtered, treeDelete(&x, &tree->root, len), len);
            if(pred){
                x = pred;
            }else{
                x = treeMinimum(tree->root);
            }
        }else{
            x = treeSuccessor(x);
        }
    }
    return counter;
}


int main(){
    int k, flag, n, i, j, found, correct, uncorrect, end = 0, newmatch;
    char *str, *rif;
    int modified;
    char command[20];
    int counterVincoli = 0;
    tree_t * tree;
    constraints_t *vincoli;
    tree_t * filtered, *support,  *temp, *deleted;
    treenode_t  * curr =NULL;

    filtered = malloc(sizeof (tree_t));
    support = malloc(sizeof (tree_t));
    tree = malloc(sizeof(tree_t));
    temp = malloc(sizeof (tree_t));
    deleted = malloc(sizeof (tree_t));

    temp->root = NULL;
    tree->root = NULL;
    support->root = NULL;
    filtered->root = NULL;
    deleted->root = NULL;


    flag = fscanf (stdin, "%d\n", &k);
    vincoli = malloc(sizeof(constraints_t) +sizeof (char)*k);
    rif = malloc(sizeof(char)*(k+2));

    if(flag){
        vincoli->frequencies = malloc(sizeof(freq_t)*k);
        vincoli->not_here = malloc(sizeof(char*)*(k));
        vincoli->maxfrequencies = malloc(sizeof(freq_t)*k);
        for(i=0; i<k; i++){
            vincoli->not_here[i] = malloc(sizeof(char)*k);
            vincoli->frequencies[i].freq = 0;
            vincoli->not_here[i][0] = '\0';
            vincoli->sim[i] = '$';
            vincoli->maxfrequencies[i].freq = 0;
            vincoli->frequencies[i].key = '\0';
            vincoli->maxfrequencies[i].key = '\0';
            vincoli->frequencies[i].maxtemp = 0;
        }
        vincoli->arr[0] = '\0';
    }
    //inizializzo parole ammissibili
    str = malloc(sizeof(char)*(k+2));
    while(!feof(stdin)){
        if(fgets(str, k+2, stdin)!=NULL){
            if(str[0]!='+'){
                curr = malloc(sizeof (treenode_t) + (k+2)* sizeof(char));
                strcpy(curr->key, str);
                treeInsert(tree, curr, k);
            }else{
                //+nuova_partita
                strcpy(command, str);
                if(fgets(str, 20, stdin)!=NULL){
                    strcat(command, str);
                    if(!fastStrcmp("+nuova_partita\n", command, k)){
                        if(fgets(rif, k+2, stdin)!=NULL){
                            flag = fscanf(stdin, "%d\n", &n);
                        }
                    } else if(!fastStrcmp("+inserisci_inizio\n", command,k)) {
                        while (!feof(stdin)) {
                            if (fgets(str, k + 2, stdin) != NULL) {
                                if (str[0] != '+') {
                                    curr = malloc(sizeof (treenode_t) + (k+2)* sizeof(char));
                                    strcpy(curr->key, str);
                                    treeInsert(tree, curr, k);
                                }else{
                                    if (fgets(str, 20, stdin) != NULL) { //str == inserisci fine
                                        break;
                                    };
                                    //printf("\nComando : %s", command);
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            }
        };
    }
    //partita ciclo
    newmatch = 0;
    while(!feof(stdin)){
        if(fgets(str, k+2, stdin)!=NULL){
            if(str[0] == '+'){
                strcpy(command, str);
                if(fgets(str, 20, stdin)!=NULL){
                    strcat(command, str);
                    if(!fastStrcmp("+stampa_filtrate\n", command, k)){
                        if(newmatch){
                            mergeTree(tree, deleted->root, k);
                            deleted->root = NULL;
                        }
                        inorderTreeWalk(tree->root);
                    }else if(!fastStrcmp("+nuova_partita\n", command, k)){
                        newmatch = 1;
                        if(fgets(rif, k+2, stdin)!=NULL){
                            flag = fscanf(stdin, "%d\n", &n);
                            for(i=0; i<k; i++){
                                vincoli->frequencies[i].freq = 0;
                                vincoli->frequencies[i].maxtemp = 0;
                                vincoli->not_here[i][0] = '\0';
                                vincoli->sim[i] = '$';
                                vincoli->maxfrequencies[i].freq = 0;
                                vincoli->frequencies[i].key = '\0';
                                vincoli->maxfrequencies[i].key = '\0';
                            }
                            vincoli->arr[0] = '\0';
                            counterVincoli = 0;
                            end = 0;
                        }
                    }else if(!fastStrcmp("+inserisci_inizio\n", command, k)){
                        while(!feof(stdin)) {
                            if (fgets(str, k + 2, stdin) != NULL) {
                                if (str[0] != '+'){
                                    curr = malloc(sizeof (treenode_t) + (k+2)* sizeof(char));
                                    strcpy(curr->key, str);
                                    treeInsert(tree, curr, k);
                                }else{
                                    if (fgets(str, 20, stdin) != NULL) { //str == inserisci fine
                                        break;
                                    }
                                    //printf("\nComando : %s", command);
                                    break;
                                }
                            }
                        }
                        if(newmatch){
                            newmatch = 0;
                            flag = checkVincoli(deleted,vincoli, k, filtered);
                            flag+= checkVincoli(tree, vincoli, k, filtered);
                            if(deleted->root == NULL){
                                deleted->root = tree->root;
                            }else{
                                mergeTree(deleted, tree->root, k);
                            }
                            tree->root = filtered->root;
                            filtered->root = NULL;
                        }else{
                            flag = checkVincoli(tree,vincoli, k, filtered);
                            if(deleted->root == NULL){
                                deleted->root = tree->root;
                                tree->root = NULL;
                            }else{
                                mergeTree(deleted, tree->root, k);
                                tree->root = NULL;
                            }
                            tree->root = filtered->root;
                            filtered->root = NULL;
                        }
                    }
                }
            }else if(!end){
                for(i=0; i<k; i++){
                    vincoli->frequencies[i].maxtemp = 0;
                }
                modified = 0;
                found = treeSearch(str, tree->root, k);
                found += treeSearch(str, deleted->root, k);
                //printf("\n'%s' ->", str);
                if(!found){
                    fprintf(stdout, "not_exists\n");
                }else if(!fastStrcmp(rif, str, k)){
                    fprintf(stdout,"ok\n");
                    end = 1;
                }else {
                    n--;
                    for(i=0; i<k; i++){
                        correct = 0;
                        uncorrect = 0;
                        if(str[i] == rif[i]){//posizione corretta
                            fprintf(stdout,"+");
                            modified = 1;
                            vincoli->sim[i] = str[i];
                            vincoli->frequencies[inspectionFunction(str[i], k, vincoli->frequencies)].maxtemp ++;
                        }else{
                            found = 0;
                            for(j=0 ; j<k; j++){
                                if(rif[j] == str[i]){
                                    found ++;
                                    if(rif[j] == str[j]){
                                        correct++;
                                    }
                                }
                                if(str[j] == str[i] && j<i && str[j]!=rif[j]){
                                    uncorrect ++;
                                }
                            }
                            if(!found || uncorrect >= (found - correct)){
                                fprintf(stdout,"/");
                                if(!found){
                                    for(j=0; vincoli->arr[j]!='\0'; j++){
                                        if(vincoli->arr[j] == str[i]){
                                            found = 1;
                                            break;
                                        }
                                    }
                                    if(!found){
                                        vincoli->arr[counterVincoli] = str[i];
                                        vincoli->arr[counterVincoli+1] = '\0';
                                        counterVincoli ++;
                                        modified = 1;
                                    }
                                }else{
                                    for(j=0; vincoli->not_here[i][j]!='\0'; j++);
                                    vincoli->not_here[i][j] = str[i];
                                    vincoli->not_here[i][j+1] = '\0';
                                    modified = 1;
                                    vincoli->maxfrequencies[inspectionFunction(str[i], k, vincoli->maxfrequencies)].freq = found;
                                    vincoli->frequencies[inspectionFunction(str[i], k, vincoli->frequencies)].freq = found;
                                }
                            }else{
                                fprintf(stdout,"|");
                                vincoli->frequencies[inspectionFunction(str[i], k, vincoli->frequencies)].maxtemp++;
                                for(j=0; vincoli->not_here[i][j]!='\0'; j++);
                                vincoli->not_here[i][j] = str[i];
                                vincoli->not_here[i][j+1] = '\0';
                                modified = 1;
                            }
                        }
                    }
                    for(i=0 ; i<k; i++){
                        if(vincoli->frequencies[i].maxtemp > vincoli->frequencies[i].freq){
                            vincoli->frequencies[i].freq = vincoli->frequencies[i].maxtemp;
                            modified = 1;
                        }
                    }
                    if(modified){
                        if(newmatch){
                            newmatch = 0;
                            flag = checkVincoli(deleted,vincoli, k, filtered);
                            flag+= checkVincoli(tree, vincoli, k, filtered);
                            if(deleted->root == NULL){
                                deleted->root = tree->root;
                            }else{
                                mergeTree(deleted, tree->root, k);
                            }
                            tree->root = filtered->root;
                            filtered->root = NULL;
                        }else{
                            flag = checkVincoli(tree,vincoli, k, filtered);
                            if(deleted->root == NULL){
                                deleted->root = tree->root;
                                tree->root = NULL;
                            }else{
                                mergeTree(deleted, tree->root, k);
                                tree->root = NULL;
                            }
                            tree->root = filtered->root;
                            filtered->root = NULL;
                        }
                    }
                    fprintf(stdout,"\n%d\n", flag);
                    //counter ammissibili con vincoli
                    if(n<=0){
                        fprintf(stdout,"ko\n");
                        end = 1;
                    }
                }
            }else{
                end = 1;
            }
        }
    }
    return 0;
}