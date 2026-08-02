#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <time.h>

#include <sys\timeb.h> 



typedef struct dedomena

{
	
	
	char date[11];
	
	float phosphate;
} mydata;


int main()



{
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
	
	    token = strtok(NULL, ",");

	    
        array[count].phosphate = atof(token);			    
	    
	    count++;

	}
	
	
    struct timeb start, end;
    
    int diff;

	ftime(&start);

    heap_sort(array,1406);
	
    ftime(&end);
        
    diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));

    printf("Heap Sort took %u milliseconds\n", diff);
    

	ftime(&start);

	countSort(array, 1406) ;
	
	ftime(&end);
    
    diff =  (int)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    
    printf("Counting Sort took %u milliseconds\n", diff);
	
	fclose(fp);
}


void swap(mydata* a,mydata* b){
     mydata temp_data = *a;
     *a = *b;
     *b = temp_data;

     return ;
 }
 
 
 void max_heapify(mydata arr[],int len_arr){
    int i;
 
    for(i=len_arr-1;i>0;i--){
    	
        mydata current_node_data = arr[i];
        
        int father_node_idx = (int)((i-1)/2);
        
        mydata father_node_data = arr[father_node_idx];
        
        if(current_node_data.phosphate>father_node_data.phosphate || (current_node_data.phosphate == father_node_data.phosphate && strcmp(current_node_data.date,father_node_data.date) > 0)){
        	
            swap(&(arr[i]),&(arr[father_node_idx]));
        }
    }
    
    swap(&(arr[0]),&(arr[len_arr-1]));
  
}

void heap_sort(mydata arr[],int len_arr){
    int i;
    for(i=len_arr;i>1;i--){
        max_heapify(arr,i);
    }
}


float getMax(mydata a[], int n) {  
   float max = a[0].phosphate; 
   
   int i; 
   for(i = 1; i<n; i++) {  
      if(a[i].phosphate > max)  
         max = a[i].phosphate;  
   }  
   return max; 
}  
  
void countSort(mydata a[], int n) 
{  
   mydata output[n+1];  
   int max = (int)getMax(a, n);  
   int count[max+1]; 
  
  int i;
  
  for (i = 0; i <= max; ++i)   
  {  
    count[i] = 0; 
  }  
    
  for (i = 0; i < n; i++) 
  {  
    count[(int)a[i].phosphate]++;  
  }  
  
   for(i = 1; i<=max; i++)   
      count[i] += count[i-1]; 
  
    
  for (i = n - 1; i >= 0; i--) {  
    output[(int)count[(int)a[i].phosphate] - 1] = a[i];  
    count[(int)a[i].phosphate]--; 
}  
  
   for(i = 0; i<n; i++) {  
      a[i] = output[i]; 
   }  
}  

