
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
 

struct Node* insert(struct Node* node, char imerominia[11], float thermokrasia)
{
    if (node == NULL)
        return(newNode(imerominia,thermokrasia));
 
    if (strcmp(imerominia,node->date)<0)
        node->left  = insert(node->left, imerominia,thermokrasia);
    else if (strcmp(imerominia,node->date)>0)
        node->right = insert(node->right, imerominia,thermokrasia);
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
 

struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;
  
    while (current->left != NULL)
        current = current->left;
  
    return current;
} 
 
 
struct Node* deleteNode(struct Node* root, char imerominia[11])
{
  
    if (root == NULL)
        return root;
  
  
    if (strcmp(imerominia,root->date)<0)
        root->left = deleteNode(root->left, imerominia);
  
  
    else if( strcmp(imerominia,root->date)>0 )
        root->right = deleteNode(root->right, imerominia);
  

    else
    {
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                                             root->right;
  
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else 
             *root = *temp; 
                            
            free(temp);
        }
        else
        {
           
            struct Node* temp = minValueNode(root->right);
  
            strcpy(root->date,temp->date);
            
            root->temperature = temp->temperature;
  
            root->right = deleteNode(root->right, temp->date);
        }
    }
  
    if (root == NULL)
      return root;
  
    root->height = 1 + max(height(root->left),
                           height(root->right));
  
   
    int balance = getBalance(root);
  
   
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
  
    
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
  
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
  
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
  
    return root;
} 
 
 
void printInorder(struct Node* node)
{
    if (node == NULL)
        return;
 
    printInorder(node->left);
 
    printf("---Date: %s  Temperature: %f  ---\n", node->date, node->temperature);
    
 
    printInorder(node->right);
} 

struct Node* search(struct Node* root, char imerominia[11])
{
    if (root == NULL || strcmp(root->date,imerominia)==0)
       return root;
    
    if (strcmp(root->date,imerominia)<0)
       return search(root->right, imerominia);
 
    return search(root->left, imerominia);
}

void update(struct Node* root, char imerominia[11], float nea_thermokrasia)

{
	
	struct Node *komvos = search(root,imerominia);
	
	if(komvos!=NULL)
	
	{
	  komvos->temperature= nea_thermokrasia;
	  
	  printf("Update Done\n");

	}
	
	else
	
	{
		printf("Node Not Found\n");
	}
	
	
	
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
	
	
		printf("1. Traverse Tree with InOrder\n");
		
		printf("2. Search\n");
		
		printf("3. Update\n");
		
		printf("4. Delete\n");
		
		printf("5. Exit\n");
		
		scanf("%d",&selection);
	
	while(selection!=5)
	
	{
	  if(selection==1)
	  
	  {
	  	
	  	printf("Results Of Traverse with InOrder\n");
	  	
	    printInorder(root);

	  }
	  
	  else if(selection==2)
	  
	  {
	  	char given[11];
	  	
	  	printf("Please Give a Date:");
	  	
	  	scanf("%s",given);
	  	
	  	struct Node* result = search(root, given);
	  	
	  	if(result!=NULL)
	  	{
		  
	  	printf("The temperature of the given date is: %f\n",result->temperature);
	  	
	    }
	    
	    else
	    
	    {
	    	printf("Node Not Found\n");
		}
	  	
	  }
	  
	  else if(selection==3)
	  
	  {
	  	char given_upd[11];
	  	
	  	float nea_therm;
	  	
	  	printf("Please give a date:");
	  	
	  	
	  	scanf("%s",given_upd);
	  	
	  	printf("Please give new temperature for this date:");
	  	
	  	scanf("%f",&nea_therm);
	  	
	   update(root,given_upd,nea_therm);	
	  	
	  }
	  
	  else if(selection==4)
	  
	  {
	  	char given_del[11];
	  	
	  	printf("Please give a date:");
	  	
	  	scanf("%s", given_del);

        root = deleteNode(root, given_del);	
        
        printf("Delete Done\n");
	  	
	  }
		
		
	printf("1. Traverse Tree with InOrder\n");
		
		printf("2. Search\n");
		
		printf("3. Update\n");
		
		printf("4. Delete\n");
		
		printf("5. Exit\n");
		
		scanf("%d",&selection);
	
		
		
		
	}
	
	
	
 
  return 0;
}
