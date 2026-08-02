#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define size 11


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

struct node
{
    float temperature;
    char date[11];
    struct node *next;
};
 
struct node *chain[size]; 

int max(int a, int b);
 

int height(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 

int max(int a, int b)
{
    return (a>b)? a : b;
}
 

struct Node* newNode(char imerominia[11], float thermokrasia)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
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
 

struct Node* insert_to_avl_by_date(struct Node* node, char imerominia[11], float thermokrasia)
{
    if (node == NULL)
        return(newNode(imerominia,thermokrasia));
 
    if (strcmp(imerominia,node->date)<0)
        node->left  = insert_to_avl_by_date(node->left, imerominia,thermokrasia);
    else if (strcmp(imerominia,node->date)>0)
        node->right = insert_to_avl_by_date(node->right, imerominia,thermokrasia);
    else 
        return node;
 
    node->height = 1 + max(height(node->left),
                           height(node->right));
 

    int balance = getBalance(node);
 
    if (balance > 1 && strcmp(imerominia, node->left->date)<0)
        return rightRotate(node);
 
    if (balance < -1 && strcmp(imerominia, node->right->date)>0)
        return leftRotate(node);
 
    if (balance > 1 && strcmp(imerominia, node->left->date)>0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    if (balance < -1 && strcmp(imerominia, node->right->date)<0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
    return node;
}


struct Node* insert_to_avl_by_temperature(struct Node* node, char imer[11], float key)
{
    if (node == NULL)
        return(newNode(imer, key));
 
    if (key < node->temperature)
        node->left  = insert_to_avl_by_temperature(node->left,imer, key);
    else if (key > node->temperature)
        node->right = insert_to_avl_by_temperature(node->right,imer, key);
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

void initialize_table()
{
    int i;
    for(i = 0; i < size; i++)
        chain[i] = NULL;
}

int calculate_hash_value(char str[11])

{
	int sum=0;
    
    int i;
    
    
    for(i=0;i<10;i++)
    
    {
    	sum = sum + str[i];
	}
	
	int key = sum %size;
	
	return key;
}

void insert_to_chain(char imerominia[11],float thermokrasia)
{
    struct node *newNode = malloc(sizeof(struct Node));
    strcpy(newNode->date, imerominia);
    newNode->temperature= thermokrasia;
    
    newNode->next = NULL;

   
	
	int key = calculate_hash_value(imerominia);

    if(chain[key] == NULL)
        chain[key] = newNode;
    
    else
    {
        struct node *temp = chain[key];
        while(temp->next)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }
}

int main()

{
	struct Node *root = NULL;
	
	struct node *root_chain = NULL;
  
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
	
	
	
	printf("Welcome!Please select one of the following choices:\n");
	
	printf("1. AVL\n");
	
	printf("2.Hasing with chains\n");
	
	int selection;
	
	scanf("%d",&selection);
	
	if(selection==1)
	
	{
		printf("1.Create AVL by Date\n");
		
		printf("2.Create AVL by Temperature\n");
		
		int choice;
		
		scanf("%d",&choice);
		
		if(choice==1)
		
		{
			for(i=0;i<count;i++)
    
    
    		{	
    	
    	  root = insert_to_avl_by_date(root,array[i].date, array[i].temperature);

    	
    	
			}
			
			printf("Data Loaded!!!!");
		}
		
		
		else if (choice==2)
		
		{
		   for(i=0;i<count;i++)
    
    
    		{	
    	
    	  root = insert_to_avl_by_temperature(root, array[i].date, array[i].temperature);

    	
    	
			}	
			
					printf("Data Loaded!!!!");

			
		}
		
		
		
	}
	else if(selection==2)
	
	{
		    initialize_table();

		
		for(i=0;i<count;i++)
    
    
    		{	
    	
    	  insert_to_chain(array[i].date, array[i].temperature);

    	
    	
			}	
		printf("Data Loaded!!!!");
		
	}
	
	
	
	
}




