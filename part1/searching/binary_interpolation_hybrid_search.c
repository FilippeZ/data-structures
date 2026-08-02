#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <time.h>

#include <math.h>


typedef struct dedomena

{
	
	
	char date[11];
	
	float temperature;
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
	    
        array[count].temperature = atof(token);			    
	    
	    count++;

	}
	
	char wanted[11];
	
	 printf("Please Give the date:");
	 
	 scanf("%s",wanted);
	 
	int position;
	
	clock_t start;
	
	clock_t end;

	start = clock();
	
	position = binary_interpolation(array,0, 1405, wanted);
    	
    
    printf("Temperature found from Binary Interpolation Search:%f\n",array[position].temperature);
 	
    	
	end = clock();
	
	double time_spent;
	
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
	
    printf("Time needed for Binary Interpolation Search: %d\n", time_spent);


	
	fclose(fp);
}


int linearSearch (mydata *list, int l, int r, char key[11]) {
	int found = 0, i = l;
	while (!found && i < r) {
		if (strcmp(list[i].date,key)==0) return i;
		++i;
	}
	return -1;
}


int binary_interpolation (mydata *arr, int l, int r, char key[11])  {
	int left 	= l;
	int right 	= r;

	int size = right - left;
	int next = (int) (size * ((strcmp(key,arr[left].date)))/(strcmp(arr[right].date,arr[left].date))) + 1;

	while (strcmp(key,arr[next].date)!=0) {
		int i = 0;
		size = right - left;
		if (size <= 3)
			return linearSearch (arr, l, r, key);
		if (strcmp(key,arr[next].date)>=0) {
			while (strcmp(key,arr[next + i*((int) sqrt(size)) - 1].date)>0) {
				++i;
			}
			right = next + (int) (i*sqrt(size));
			left  = next + (int) ((i-1)*sqrt(size));
		}
		else if (strcmp(key,arr[next].date)<0) {
			while (strcmp(key,arr[next -i* ((int) sqrt(size)) + 1].date)<0) {
				++i;
			}
			right = next - (int) ((i-1)*sqrt(size));
			left  = next - (int) (i*sqrt(size));
		}
		next = (int) (left + ((right - left +1)*(strcmp(key,arr[left].date))/(strcmp(arr[right].date,arr[left].date)))) - 1;
		
	}
	if (strcmp(key,arr[next].date)==0) 
			return next;
		else return -1;
}
