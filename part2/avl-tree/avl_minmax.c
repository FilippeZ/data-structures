#include<stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct dedomena

{
	
	
	char date[11];
	
	float temperature;
} mydata;
 
struct Node
{
    char date[11];
    
    float temperature;
    
    struct Node *left;
    struct Node *right;
    int height;
};

int max(int a, int b);
 
int height(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b)
{
    return (a > b)? a : b;
}

struct Node* newNode(char imerominia[11], float thermokrasia)
{
    struct Node* node = (struct Node*)
                        malloc(sizeof(struct Node));
    strcpy(node->date,imerominia);
    node->temperature = thermokrasia;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  
    return(node);
}

struct Node *rightRotate(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *T2 = x->right;
 
    x->right = y;
    y->left = T2;
 
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
 
    return x;
}

struct Node *leftRotate(struct Node *x)
{
    struct Node *y = x->right;
    struct Node *T2 = y->left;
 
    y->left = x;
    x->right = T2;
 
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
 
    return y;
}

int getBalance(struct Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct Node* insert(struct Node* node, char imer[11], float key)
{
    if (node == NULL)
        return(newNode(imer, key));
 
    if (key < node->temperature)
        node->left  = insert(node->left,imer, key);
    else if (key > node->temperature)
        node->right = insert(node->right,imer, key);
    else 
        return node;
 
    node->height = 1 + max(height(node->left),
                           height(node->right));
 
   
    int balance = getBalance(node);
 
   
 
    if (balance > 1 && key < node->left->temperature)
        return rightRotate(node);
 
    if (balance < -1 && key > node->right->temperature)
        return leftRotate(node);
 
    if (balance > 1 && key > node->left->temperature)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    if (balance < -1 && key < node->right->temperature)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
    return node;
}

char* minValue(struct Node* node) {
  struct Node* current = node;
  

  while (current->left != NULL) {
    current = current->left;
  }
  return(current->date);
}

char* maxValue(struct Node* node) {
  struct Node* current = node;
  

  while (current->right != NULL) {
    current = current->right;
  }
  return(current->date);
}

int main()

{
	struct Node *root = NULL;
  
  	mydata array[1406];
	
	
	FILE *fp;
	
	fp = fopen("ocean.csv", "r");
	
	
	char line[200];
	
	char *token;
    
    int count=0;
	
	fscanf(fp,"%s\n",line);
	
	while(fscanf(fp,"%s\n",line)!=EOF)
	
	
	{		
		token = strtok(line, ",");
				
	    strcpy(array[count].date,token);
	    
	    token = strtok(NULL, ",");
	    
        array[count].temperature = atof(token);			    
	    
	    count++;

	}
 
    int i;
    
    
    for(i=0;i<count;i++)
    
    
    {
    	
    	  root = insert(root, array[i].date, array[i].temperature);

    	
    	
	}
	
	printf("Data loaded to tree successfully\n");
	
	int selection;
	
	printf("1. Find Date with minimum temperature\n");
	
	printf("2. Find Date with maximum temperature\n");
	
	printf("3. Exit\n");
	
	scanf("%d",&selection);
	
	
	while(selection!=3)
	
	{
		if(selection==1)
		
		{
			printf("The date with minimum temperature :%s\n",minValue(root));
			
		}
		
		else if(selection==2)
		
		{
			printf("The date with maximum temperature:%s\n",maxValue(root));
			
		}
		
	printf("1. Find Date with minimum temperature\n");
	
	printf("2. Find Date with maximum temperature\n");
	
	printf("3. Exit\n");
	
	scanf("%d",&selection);
		
		
	}
	
	
}


