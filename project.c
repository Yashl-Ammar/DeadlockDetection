#include <stdlib.h>  
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

struct processRunTime
{
	int pid;
	int rfiles [2];
	int afiles [2];
};


void readFile(char * fileName,int arr [30][3])
{
	FILE * file;
	
	file = fopen(fileName,"r");
	if(file == NULL)
	{
		printf("Could not read the file ! \n");
		return;
	}
	
	printf("File reading has started\n");
	
	int i = 0;
	while (!feof(file))
	{
		fscanf(file, "%d %d %d" , &arr[i][0], &arr[i][1],&arr[i][2]);
		i++;
	}
	
	printf("File reading has ended\n");
	
	
}

void printData(int arr[30][3])
{
	printf("Printing data read : \n");
	
	for(int k = 0 ; k < 30 ; k++)
	{
		int b = 0;
		for(int j = 0 ; j < 3 ; j++)
		{
			if(arr[k][j] != -1){
				printf("%d ", arr[k][j]);
				b = 1;
			}
		}
		if(b == 1)
		{
			printf("\n");
		}
	}
}

void printStats(int arr[30][3])
{
	//printf("Processes : \n");
	int count = 0;
	for(int k = 0 ; k < 30 ; k++)
	{
		int b = 0;
			if(arr[k][0] != -1){
				//printf("%d ", arr[k][0]);
				count++;
				b = 1;
			}
		if(b == 1)
		{
			//printf("\n");
		}
	}
	
	printf("Total Processes : %d\n" , count);
	
	//printf("Files : \n");
	count = 0;
	// array used to remove duplicates
	int temparr[60];
	// to manage garbage values
	for(int i = 0 ; i < 60 ; i++)
	{
		temparr[i] = -1;
	}
	
	
	for(int k = 0 ; k < 30 ; k++)
	{
		int b = 0;
		for(int j = 1 ; j < 3 ; j++)
		{
			if(arr[k][j] != -1){
				//printf("%d ", arr[k][j]);
				b = 1;
				int flag = 0;
				for(int i = 0 ; i < 60 ; i++)
				{
					if(temparr[i] != -1){
						if(temparr[i] == arr[k][j])
						{
							flag = 1;
						}
					}
				}
				
				if(flag == 0)
				{
					for(int i = 0 ; i < 60 ; i++)
					{
						if(temparr[i] == -1){
							temparr[i] = arr[k][j];
							break;
						}
					}
				}
				
			}
		}
		if(b == 1)
		{
			//printf("\n");
		}
	}
	
	for(int i = 0 ; i < 60 ; i++)
	{
		if(temparr[i] != -1)
		{
			count++;
		}
	}
	
	printf("Total Number of Files : %d\n" , count);
	
	
}


int bringNextProcess(struct processRunTime p, struct processRunTime arr[30] )
{
	for(int i = 0 ; i < 30 ; i++)
	{
		if(arr[i].pid != p.pid && arr[i].pid != -1)
		{
			for(int j = 0 ; j < 2 ; j++){
				if(p.rfiles[0] == arr[i].afiles[j])
				{
					return arr[i].pid;
				}
			}
		}
	}
	
	return -1;
	
}
void simulation(int arr[30][3])
{
	
	int b = 0;
	int files[60];
	
	int counter = 0;
	
	printf("Simualating : \n");
	do
	{
		b = 1;
		for(int i = 0  ; i < 60 ; i++)
		{
			files[i] = -1;
		}
		
		
		for(int i = 0 ; i < 30 ; i++)
		{
			int flag = 0;
			if(arr[i][0] != -1)
			{
				for(int k = 0 ; k < 60 ; k++)
				{
					if(files[k] != -1)
					{
						if(arr[i][1] == files[k] || arr[i][2] == files[k])
						{
							flag = 1;
							break;
						}
					}
				}
				if(flag == 0)
				{
					printf("%d %d %d,%d\n" , counter , arr[i][0],arr[i][1],arr[i][2]);
				
					for(int j = 0 ; j < 60 ; j++)
					{
						if(files[j] == -1)
						{
						
							files[j] = arr[i][1];
							files[j+1] = arr[i][2];
							break;
						}
					}
					arr[i][0] = -1;
					arr[i][1] = -1;
					arr[i][2] = -1;
				}
			}
		}
		
		
		for(int i = 0 ; i < 30 ; i++)
		{
			if(arr[i][0] != -1)
			{
				b = 0;
				break;
			}
			
		}
		
		counter++;
	}while(b == 0);
	
	printf("Simulation Time : %d\n", counter);
}

void executingProcess(int arr[30][3], int z)
{
	int tempBool = 0 ;
	int terminatePids[30];
	for(int i = 0 ; i < 30 ; i++)
		{
			terminatePids[i] = -1;
		}

	do {
		struct processRunTime p[30];
		// to manage garbage values
		for(int i = 0 ; i < 30 ; i++)
		{
				p[i].pid = -1;
				for(int k = 0; k < 2 ; k++)
				{
					p[i].rfiles[k] = -1;
					p[i].afiles[k] = -1;
				}
		}
		
		
		for(int i = 0 ; i < 30 ; i++)
		{
			int y = 0;
			for(int j = 0 ;  j < 30 ; j++)
			{
				if(terminatePids[j] != -1){
					if(arr[i][0] == terminatePids[j]){
						y = 1;
						break;
					}
				}
			}
			
			if(y == 0)
			{
				p[i].pid = arr[i][0];
				p[i].rfiles[0] = arr[i][1];
				p[i].rfiles[1] = arr[i][2];	
			}		
		}
		
		/*printf("Process about to be executed : \n");
		for(int i = 0 ; i < 30 ; i++)
		{
			if(p[i].pid != -1){
				printf("%d %d %d \n" , p[i].pid ,p[i].rfiles[0], p[i].rfiles[1]);
			}
		}*/
		
		//printf("Starting the execution : \n");
		
		int counter = 0;
		
		// used to detect deadlocks
		int pidarr[30];
		// to manage garbage values 
		for(int i = 0 ; i < 30 ; i++)
		{
			pidarr[i] = -1;
		}
		
		
		
		
		
		int x = 0;
		while(1)
		{
			
			int flag = 0;
			
			for (int i = 0 ; i < 30 ; i++)
			{
				
				
				
			 
				int b = 0;
				int flag = 0;
				if(p[i].pid != -1 )
				{
					
					
					for(int j = 0 ; j < 30 ; j++)
					{
						if(p[i].pid != p[j].pid)
						{
							for(int k = 0 ; k < 2 ; k++)
							{
								if(p[i].rfiles[0] == p[j].afiles[k])
								{
									b = 1;
								}
							}
						}
					}
					
					if(b == 0)
					{
						for(int j = 0 ; j < 2 ; j++)
						{
							if(p[i].afiles[j] == -1)
							{
								p[i].afiles[j] = p[i].rfiles [0];
								p[i].rfiles[0] = p[i].rfiles[1];
								p[i].rfiles[1] = -1;
								break;
							}
						}
					}
					
					
					
					
				}
				
					
				
					
				
			}
			for (int i = 0 ; i < 30 ; i++){
				if(p[i].rfiles[0] == -1 && p[i].rfiles[1] == -1 )
					{
						//releasing
						p[i].afiles[0] = -1;
						p[i].afiles[1] = -1;
						
						
					}
			}
			
					
			counter++;
			
			for(int j =  0 ; j < 30 ; j++)
			{
				if(p[j].rfiles[0] != -1 || p[j].rfiles[1] != -1 )
				{
					flag = 1;
					break;
				}
			}
			
			if(flag == 0)
			{
				if(tempBool == 0){
					printf("All executed\nExecution Time : %d\n" , counter);
				}
				else
				{	
					printf("Terminate Id : ");
					for(int l = 0 ; l < 30 ; l++)
					{
						if(terminatePids[l]  != -1)
						{
							printf("%d " , terminatePids[l]);
						}
					}
					printf("\n");
					if(z == 1){
						simulation(arr);
					}
				}
				x = 1;
				tempBool = 0;
				
				break;
			}

			int temparray[30];
			
			for(int i = 0 ; i < 30 ; i++)
			{
				temparray[i] = -1;
			}
			
			int deadlockFlag = 0;
			int term = -1;
			for(int j =  0 ; j < 30 ; j++)
			{
				if(p[j].rfiles[0] != -1){
					int temp = 0;
					struct processRunTime tempProcess = p[j];
					do{
						temp = bringNextProcess(tempProcess,p);
						
						for(int i = 0 ; i < 30 ; i++)
						{
							if(temparray[i] == -1)
							{
								temparray[i] = temp;
							}
						}
						
						//printf("%d\n" , temp);
						if(temp != -1)
						{
							for(int i = 0 ; i < 30 ; i++)
							{
								if(p[i].pid == temp)
								{
									tempProcess = p[i];
								}
							}
						}		
						 
					}while(temp != -1 && temp != p[j].pid);
					
					if(temp == p[j].pid)
					{
						
						deadlockFlag = 1;
						if(tempBool == 0)
						{
							printf("Dead Lock has occured !\n");
						}
						tempBool = 1;
						int min = temp;
						for(int i = 0 ; i < 30; i++ )
						{
							if(temparray[i] != -1)
							{
								if( temparray[i] < min)
								{
									min = temparray[i];
								}
							}
						}
						
						
						term = min;
						break;
					}
					
				}
			}
			if(deadlockFlag == 1)
			{
				for( int i = 0 ; i < 30 ; i++)
				{
					if(terminatePids[i] == -1)
					{
						terminatePids[i] = term;
						break;
					}
				}
				
				break;
			}
			
		}
		if(x == 1)
		{
			break;
		}
		
	}while(tempBool == 1);
	

	//printf("Execution Ended \n");
	
}






int main(int argc , char * argv[])
{

	char * filePath = "";
	int b = 0;
	for(int i = 1 ; i < argc ; i++)
	{
		if(strcmp("-f",argv[i]) == 0)
		{
			filePath = argv[i+1];
		}
		else if(strcmp("-c",argv[i]) == 0)
		{
			b = 1;
		}
	}
	

	int arr[30][3];
	
	// this is done to manage the garbage values
	for(int i = 0 ; i < 30 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			arr[i][j] = -1;
		}
	}
	
	
	readFile(filePath,arr);
	printf("\n");
	printData(arr);
	printf("\n");
	printStats(arr);
	printf("\n");
	executingProcess(arr,b);
	printf("\n");
	
	
}



