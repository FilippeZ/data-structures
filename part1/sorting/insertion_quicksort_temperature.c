#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <time.h>

#include <sys\timeb.h> 



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
	
	
    struct timeb start, end;
    
    int diff;

	ftime(&start);

	insertionSort(array,1406);
	
    ftime(&end);
        
    diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));

    printf("Insertion Sort took %u milliseconds\n", diff);
    

	ftime(&start);

	quicksort(array,1406);
	
	ftime(&end);
    
    diff =  (int)(1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    
    printf("Quick Sort took %u milliseconds\n", diff);
	
	fclose(fp);
}


void insertionSort(mydata *a, int length) {
int j;
int i;
mydata temp;
for (i = 1; i < length; i++) {
    j = i;
    while (j > 0 && a[j].temperature < a[j - 1].temperature || (a[j].temperature == a[j - 1].temperature && strcmp(a[j].date,a[j-1].date) < 0)) {
        temp = a[j];
        
        a[j] = a[j-1];
        
        a[j-1] = temp;
        j--;
    }
}

}


void swap_data(mydata *left, mydata *right)
{
    mydata tmp = *right;
    *right = *left;
    *left = tmp;
}

int compare_data(const mydata* left,
                     const mydata* right)
{
    float therm = left->temperature - right->temperature;
    return (therm ? therm : (strcmp(left->date,right->date)));
}

static void quicksort_(mydata *arr, int left, int right)
{
    mydata p = arr[(left+right)/2];    
    int l = left, r = right;   

    while (l <= r)
    {
        while (compare_data(arr+l, &p) < 0)
            ++l;
        while (compare_data(arr+r, &p) > 0)
            --r;
        if (l <= r)
        {
            swap_data(arr+l, arr+r);
            ++l; --r;
        }
    }

    if (left < r)
        quicksort_(arr, left, r);
    if (l < right)
        quicksort_(arr, l, right);
}

void quicksort(mydata *arr, int count)
{
    if (arr && (count>0))
        quicksort_(arr, 0, count-1);
}

