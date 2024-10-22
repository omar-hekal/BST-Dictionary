#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct 
{
    char * word;
    struct Node* left;
    struct Node* right;
} Node;

Node* newNode(char* word){
    Node* n = (Node*)malloc(sizeof(Node));
    n->word = malloc(strlen(word)+1);
    strcpy(n->word, word);
    n->left = n->right = NULL;
    return n; 
}

Node* insert(Node* root, char* new_word){
    if(root == NULL) {return newNode(new_word);}
    if(strcasecmp(root->word, new_word) > 0) root->left = insert(root->left, new_word);
    else root->right = insert(root->right, new_word);
    return root;
}


Node* populate_tree(int *count_ptr){
    FILE * file = fopen("Dictionary.txt", "r");
    char word[100];
    int i=0;
    Node * root = NULL;
    while(!feof(file)){
        (*count_ptr)++;
        fgets(word, 100, file);
        int len = strlen(word);
        if(word[len-1] == '\n')
            word[strlen(word)-1] = '\0';
        root = insert(root, word);
    }

    return root;
}

void inordertraversal(Node* n){
    if(n){
        inordertraversal(n->left);
        printf("%s\n", n->word);
        inordertraversal(n->right);
    }
}

int get_height(Node* node, int height){
    if (!node) return height;
    int l_height = get_height(node->left, height+1);
    int r_height = get_height(node->right, height+1);
    return l_height > r_height ? l_height : r_height;
}


int get_height2(Node* node){
    if (!node) return 0;
    int l_height = get_height2(node->left);
    int r_height = get_height2(node->right);
    return 1 + (l_height > r_height ? l_height : r_height);
}

Node* search(Node* root, char* word, int * is_exsist){
    if (!root) {*is_exsist = 0; return root;}
    if(strcasecmp(root->word,word)==0){
        *is_exsist = 1;
        return root;
    }
    if(strcasecmp(root->word, word) > 0){ 
        if(!root->left){
            *is_exsist = 0;
            return root;
        }
        return search(root->left, word, is_exsist);
    }
    
    else {
        if(!root->right){
            *is_exsist = 0;
            return root;
        }
        return search(root->right, word, is_exsist);
    }
}

Node* get_parrent(Node* root, Node* target){
    if(!root || root->left == target || root->right == target) return root;
    if(strcasecmp(root->word, target->word) > 0) return get_parrent(root->left, target);  
    else return get_parrent(root->right, target);  
}

Node* get_inorder_predecessor(Node* root, Node* target){
    if(target->left){
        // find rightmost(maximum) node in the left subtree
        Node * temp = target->left;
        while (temp->right)
            temp = temp->right;
        return temp;
    }
    else{
        // look for the parrent
        // if (root == target) return NULL; 
        // Node * parrent = get_parrent(root, target);
        // if(parrent-> right == target) return parrent;
        // else return NULL;
        Node* successor = NULL;
        Node* ancestor = root;
        while (ancestor != target){
            if(strcasecmp(ancestor->word, target->word) < 0){
                successor = ancestor;
                ancestor = ancestor->right;
            }
            else{
                ancestor = ancestor->left;
            }
        }
        return successor;
    }

}

Node* get_inorder_successor(Node* root, Node* target){
    if(target->right){
        Node * temp = target->right;
        while (temp->left)
            temp = temp->left;
        return temp;
    }
    else{
        Node* successor = NULL;
        Node* ancestor = root;
        while (ancestor != target){
            if(strcasecmp(ancestor->word, target->word) > 0){
                successor = ancestor;
                ancestor = ancestor->left;
            }
            else{
                ancestor = ancestor->right;
            }
        }
        return successor;
        
    }
}

void check_word(Node* root, char * word){
    printf("%s - ", word);
    int is_exsist = 0;
    Node * n = search(root, word, &is_exsist);
    if (is_exsist){
        printf("CORRECT\n");
    }
    else{
        printf("Incorrect, ");
        if (!n)
            printf("No Suggestion\n");
        else{
            printf("Suggestions : ");
            printf("%s ", n->word);
            Node * predecessor = get_inorder_predecessor(root, n);
            Node * successor = get_inorder_successor(root, n);
            if (successor) printf("%s ", successor->word);
            if (predecessor){ 
                printf("%s ", predecessor->word);}
            printf("\n");
        }
        
    }
}

void validate_text(Node* root, char * text){
    char * token = strtok(text, " ");
    while (token)
    {
        check_word(root, token);
        token = strtok(NULL, " ");
    }
    
}


void main(){
    int counter = 0;
    Node * node = populate_tree(&counter);
    printf("Dictionary Loaded Successfully..!\n");
    printf(".................................\n");
    printf("Size = %d\n", counter);
    printf(".................................\n");
    printf("Height = %d\n", get_height2(node));
    printf(".................................\n");
    printf("Enter a sentence\n");


    char buf[100];
    fgets(buf, 100, stdin);
    buf[strlen(buf)-1] = '\0'; // remove the newline character
    validate_text(node, buf);

}