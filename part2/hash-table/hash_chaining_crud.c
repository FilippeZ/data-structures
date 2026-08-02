#include<stdio.h>
#include<stdlib.h>

#define size 11

typedef struct dedomena

{
	char date[11];
	
	float temperature;
} mydata;

struct node
{
    float temperature;
    char date[11];
    struct node *next;
};

struct node *chain[size];

void initialize_table()
{
    int i;
    for(i = 0; i < size; i++)
        chain[i] = NULL;
}

int calculate_hash_value(char str[11]);

void insert(char imerominia[11],float thermokrasia)
{
    struct node *newNode = malloc(sizeof(struct node));
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

void print()
{
    int i;

    for(i = 0; i < size; i++)
    {
        struct node *temp = chain[i];
        printf("table[%d]-->",i);
        while(temp)
        {
            printf("(%s ,%d)  -->",temp->date, temp->temperature);
            temp = temp->next;
        }
        printf("NULL\n");
    }
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

float search(char imerominia[11])
{
    int key = calculate_hash_value(imerominia);
    struct node *temp = chain[key];
    while(temp)
    {
        if(strcmp(temp->date,imerominia)==0)
            return temp->temperature;
        temp = temp->next;
    }
    return 0;
}

void update(char imerominia[11], int nea_thermokrasia)


{
	int key = calculate_hash_value(imerominia);
    struct node *temp = chain[key];
    while(temp)
    {
        if(strcmp(temp->date,imerominia)==0)
            temp->temperature=nea_thermokrasia;
        temp = temp->next;
    }
	
}

int delete_node(char imerominia[11])
{
    int key = calculate_hash_value(imerominia);
    struct node *temp = chain[key], *dealloc;
    if(temp != NULL)
    {
        if(strcmp(temp->date,imerominia)==0)
        {
            dealloc = temp;
            temp = temp->next;
            free(dealloc);
            return 1;
        }
        else
        {
            while(temp->next)
            {
                if(strcmp(temp->next->date,imerominia)==0)
                {
                    dealloc = temp->next;
                    temp->next = temp->next->next;
                    free(dealloc);
                    return 1;
                }
                temp = temp->next;
            }
        }
    }

    return 0;
}


int main()
{
    initialize_table();
    
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
    	
    	  insert(array[i].date, array[i].temperature);

    	
    	
	}
	
	printf("Data loaded to Hash Table successfully\n");


    int selection;
    
    printf("1. Search\n");
    
    printf("2. Update\n");
    
    printf("3. Delete\n");
    
    printf("4. Exit\n");
    
    
    scanf("%d",&selection);
    
    
    while(selection!=4)
    
    {
    	if(selection==1)
    	
    	{
    		printf("Please give date to search:");
    		
    		char given_search[11];
    		
    		scanf("%s",given_search);
    		
    		printf("The temperature for the given Date is:%f\n",search(given_search));
    		
    		
		}
		
		else if(selection==2)
		
		{
			char given_upd[11];
			
			float nea_therm;
			
			printf("Please give date:");
			scanf("%s",given_upd);
			
			printf("Please give New Temperature:");
			
			scanf("%f",&nea_therm);
			
			update(given_upd,nea_therm);
		}
		
		else if(selection==3)
		
		{
			char given_del[11];
			
			printf("Please give date:");
			
			scanf("%s",given_del);
			
			if(delete_node(given_del)==1)
			
			{
				printf("Delete Done successfully\n");
			}
			
			else
			
			{
				printf("Node Not Found\n");
			}
			
			
		}
		
	printf("1. Search\n");
    
    printf("2. Update\n");
    
    printf("3. Delete\n");
    
    printf("4. Exit\n");
    
    
    scanf("%d",&selection);
    	
	}






    return 0;
}


