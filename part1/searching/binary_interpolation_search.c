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
	
	char wanted[11];
	
	 printf("Please Give the date:");
	 
	 scanf("%s",wanted);
	 
	int position;
	
    struct timeb start, end;
    
    int diff;


	ftime(&start);

	position = binarySearch(array,wanted,1406);
	
	printf("Temperature found from Binary Search:%f\n",array[position].temperature);
	
    ftime(&end);
    
    
    diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));

    printf("BinarySeach took %u milliseconds\n", diff);
    

	ftime(&start);

	position = interpolationSearch(array,1406 ,wanted);
	
	printf("Temperature found from Interpolation Search:%f\n",array[position].temperature);
	
	ftime(&end);
    
    diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    
    printf("Interpolation Search took %u milliseconds\n", diff);

    

	

	
	fclose(fp);
}

int binarySearch(mydata a[], char key[11],int n)
    {
    	int low, high, mid;
    	
         low=0;
 		high=n-1;
 		while(low<=high)
 		{
 			mid=(low+high)/2;
 		if (strcmp(key,a[mid].date)==0)
         {
             return mid;
         }
 		else if(strcmp(key,a[mid].date)>0)
 		{
 			high=high;
 			low=mid+1;
 		}
 		else
 		{
 			low=low;
 			high=mid-1;
 		}
 }
 
        return -1;
    }



int interpolationSearch(mydata A[], int n, char x[11])
{
	int low = 0, high = n - 1, mid;

	while (strcmp(A[low].date,A[high].date)!=0 && strcmp(x,A[low].date) >=0 && strcmp(x,A[high].date)>=0)
	{
		mid = low + (strcmp(x,A[low].date) * (high - low) / strcmp(A[high].date,A[low].date));
		mid = roundf(mid);
		

		if (strcmp(x,A[mid].date)==0)
			return mid;

	
		else if (strcmp(x,A[mid].date)<0)
			high = mid - 1;

		
		else
			low = mid + 1;
	}

	if (strcmp(x,A[low].date)==0)
		return low ;

	else
		return -1;
}
