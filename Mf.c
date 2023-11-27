#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<time.h>
#include<pthread.h>



//Assuming Arrival Time is 0
long long int sum1=0,x=2,num1,s6=0,s7=0,s8=0,i1=0,i2=0,i3=0,num2,num3,sum3=0,n1,n2,n3,choice,s1,s2,s3,run1=0,run2=0,run3=0,counter1=0,counter2=0,counter3=0,slen1,slen3,checker1=0,checker2=0,checker3=0;
double t1=0.0,t2=0.0,t3=0.0,ti1,ti2,ti3,time_quantum;
double tatf1,tatf2,tatf3,wf1,wf2,wf3,tatr1,tatr2,tatr3,wr1,wr2,wr3;
double sf1,sf2,sf3,ef1,ef2,ef3,sr1,sr2,sr3,er1,er2,er3;
clock_t begin,begin1,end1,begin2,end2,begin3,end3;
char result1[50],result3[50];
char *shmPtr1, *shmPtr2, *shmPtr3;
long long int shmid1,shmid2,shmid3,shmid;
long long int count1=1,count2=0,count3=0;

//Arrays for read and write end of the pipes
int pfds1[2];
int pfds2[2];
int pfds3[2];

//The arrays containing the messages to be sent using pipes(Results of the child processes)
char buf1[50];
char buf2[50];
char buf3[50];

//Runnnerx is task thread for xth Child Process
//monitorx is the thread of xth Child Process monitoring communications
void *runner1(void *param);
void *runner2(void *param);
void *runner3(void *param);
void *monitor1(void *param);
void *monitor2(void *param);
void *monitor3(void *param);


int main()
{
	//Prompts for user inputs
	printf("Enter Scheduling algorithm: \n0 for FCFS and 1 for Round Robin \n");
	scanf("%lld",&choice);

	printf("Enter value of n1: \n");
	scanf("%lld",&n1);

	printf("Enter value of n2: \n");
	scanf("%lld",&n2);

	printf("Enter value of n3: \n");
    	scanf("%lld",&n3);
    	
    	
    
    if(choice==1)
    {
		printf("Enter time quantum for Round Robin: \n");
		scanf("%lf",&time_quantum);
    }

	//Creation of pipes
	int p1 = pipe(pfds1);
	int p2 = pipe(pfds2);
	int p3 = pipe(pfds3);

    
    if(p1<0)
	{
    	perror("Error in creating pipe 1");
    	exit(1);
    }	
    if(p2<0)
	{
    	perror("Error in creating pipe 2");
    	exit(2);
    }	
    if(p3<0)
	{
    	perror("Error in creating pipe 3");
    	exit(3);
    }

    pid_t pid,pid2,pid3;
    
	//Creating C1
    double timer=0.0;
    begin=clock();
    pid=fork();
    if(pid==0)
	{
        //C1's Code

		//Creating a shared memory segment for C1 and M
        if((shmid1 = shmget(2041, 32, 0)) == -1)
	{
        	perror("Error in shmget() in c1\n");
        	exit(4);
        }
        shmPtr1 = shmat(shmid1, 0, 0);
        if(shmPtr1 == (char *)-1)
	{
        	perror("Error in shmat() in c1\n");
        	exit(5);
        }
        
		//Creating the monitor thread for C1
		pthread_t tid12; /* the thread identifier */
		pthread_attr_t attr12; /* set of thread attributes */
		pthread_attr_init(&attr12); /* set the default attributes */
		pthread_create(&tid12, &attr12, monitor1, NULL); /* create the thread */
        
		//Creating the Task thread for C1
    	pthread_t tid11; /* the thread identifier */
		pthread_attr_t attr11; /* set of thread attributes */
		pthread_attr_init(&attr11); /* set the default attributes */
		pthread_create(&tid11, &attr11, runner1, NULL); /* create the thread */


		pthread_join(tid12, NULL);  /* wait for the thread to exit */
		pthread_join(tid11, NULL);  /* wait for the thread to exit */
		//printf("sum = %lld\n", sum1);
    }
    
    else
	{   
		//Creating C2 
        pid2=fork();
        if(pid2==0)
		{
            //C2

			//Creating the Shared memory segment for C2 and M
            if((shmid2 = shmget(2042, 32, 0)) == -1)
			{
        		perror("Error in shmget() in c2\n");
        		exit(6);
	   		}
	   		shmPtr2 = shmat(shmid2, 0, 0);
	   		if(shmPtr2 == (char*)-1)
			{
				perror("Error in shmat() in c2\n");
				exit(7);
          	}
            
			//Creating the monitor thread for C2 
			pthread_t tid22; /* the thread identifier */
			pthread_attr_t attr22; /* set of thread attributes */
			pthread_attr_init(&attr22); /* set the default attributes */
			pthread_create(&tid22, &attr22, monitor2, NULL); /* create the thread */

			//Creating the Task thread for C2
        	pthread_t tid21; /* the thread identifier */
			pthread_attr_t attr21; /* set of thread attributes */
			pthread_attr_init(&attr21); /* set the default attributes */
			pthread_create(&tid21, &attr21, runner2, NULL); /* create the thread */

			
			
			pthread_join(tid22, NULL);  /* wait for the thread to exit */
			pthread_join(tid21, NULL);  /* wait for the thread to exit */
        }
 
        else
		{
			//Creating C3
            pid3=fork();
            if(pid3==0)
			{
                //C3's Code

				//Creating a shared memory segment for C3 and M
                if((shmid3 = shmget(2043, 32, 0)) == -1)
				{
					perror("Error in shmget() in c3\n");
					exit(8);
		   		}
		   		shmPtr3 = shmat(shmid3, 0, 0);
		   		if(shmPtr3 == (char*)-1)
		   		{
					shmPtr1[1]=slen1;perror("Error in shmat() in c3\n");
					exit(9);
				}

				//Creating monitor thread of C3
				pthread_t tid32; /* the thread identifier */
				pthread_attr_t attr32; /* set of thread attributes */
				pthread_attr_init(&attr32); /* set the default attributes */
				pthread_create(&tid32, &attr32, monitor3, NULL); /* create the thread */
                
				//Creating task thread of C3
				pthread_t tid31; /* the thread identifier */
				pthread_attr_t attr31; /* set of thread attributes */
				pthread_attr_init(&attr31); /* set the default attributes */
				pthread_create(&tid31, &attr31, runner3, NULL); /* create the thread */

				
				pthread_join(tid32, NULL);  /* wait for the thread to exit */
				pthread_join(tid31, NULL);  /* wait for the thread to exit */
				//printf("sum = %lld\n", sum1);
            }
 
            else
			{
 
                //M's Code

				//Creating the shared memory segments with C1,C2 and C3	
		   		if(((shmid1 = shmget(2041, 32, 0644 | IPC_CREAT)))== -1)
				{
					perror("Error in shmget() in M for c1\n");
					exit(10);
		   		}
		   		shmPtr1 = shmat(shmid1, 0, 0);
		   		if(shmPtr1 == (char*)-1)
		   		{
					perror("Error in shmat() in M for c1\n");
					exit(11);
		  		}
		  		
				if(((shmid2 = shmget(2042, 32, 0644 | IPC_CREAT)))== -1)
				{
					perror("Error in shmget() in M for c3\n");
					exit(12);
				}
		   		shmPtr2 = shmat(shmid2, 0, 0);
		   		if(shmPtr2 == (char*)-1)
				{
					perror("Error in shmat() in M for c2\n");
					exit(13);
		  		}
		  		
				if(((shmid3 = shmget(2043, 32, 0644 | IPC_CREAT)))== -1)
				{
					perror("Error in shmget() in M for c3\n");
					exit(14);
		   		}
		   		shmPtr3 = shmat(shmid3, 0, 0);
		   		if(shmPtr3 == (char*)-1)
				{
					perror("Error in shmat() in M for c3\n");
					exit(15);
		  		}
				
				//Shared memory variable to keep track of whether a process completed its task or not
				//0 when not complete and 1 when completed
				shmPtr1[2]=0;
				shmPtr2[2]=0;
				shmPtr3[2]=0;

				//Shared memory variable used for switching between processes
				//if shmPtrX[0]=1, then Xth process is running currently
				shmPtr1[0]=1;
				shmPtr2[0]=0;
				shmPtr3[0]=0;
				
				//sleep(5);

				//To keep track of time
				clock_t end=clock();
				timer=(double)(end-begin) / CLOCKS_PER_SEC;
				printf("C1 starts at %f seconds.\n",timer);
				wf1=timer;
				wr1=timer;
				

				//To switch between processes in FCFS in the order C1->C2->C3
				if(choice==0)
				{
					while(shmPtr1[2]==0 || shmPtr2[2]==0 || shmPtr3[2]==0)
					{
						end=clock();
						timer=(double)(end-begin) / CLOCKS_PER_SEC;
						//Once C1 is completed, C2 starts and time when it starts and ends is recorded
						if(shmPtr1[2]==1 && checker1==0)
						{
							printf("C1 ends at %f seconds.\n",timer);
							ef1=timer;
							sleep(1);
							shmPtr1[0]=0;
							shmPtr2[0]=1;
							printf("C2 starts at %f seconds.\n",timer);
							wf2=timer;
							checker1++;
						}

						//Once C2 is also done,C3 starts
						if(shmPtr2[2]==1 && checker2==0)
						{
							printf("C2 ends at %f seconds.\n",timer);
							ef2=timer;
							sleep(1);
							shmPtr2[0]=0;
							shmPtr3[0]=1;
							printf("C3 starts at %f seconds.\n",timer);
							wf3=timer;
							checker2++;
						}
					}
					end=clock();
					timer=(double)(end-begin) / CLOCKS_PER_SEC;
					printf("C3 ends at %f seconds.\n",timer);
					ef3=timer;
				}

				//To switch between processes in Round Robin
				else
				{
					wr2=wr1+time_quantum;
					wr3=wr2+time_quantum;
					//To check if there is any process that is yet to complete its task
					while(shmPtr1[2]==0 || shmPtr2[2]==0 || shmPtr3[2]==0)
					{
					
						//To get the time and switch between processes at a fixedd interval(time quantum)
						/*end=clock();
						timer=(double)(end-begin) / CLOCKS_PER_SEC;
						double timer2=timer/(time_quantum);*/
						sleep(time_quantum);
						//if(timer2==(int)timer2)
						//{
							//printf("%d %d %d %f\n",shmPtr1[0],shmPtr2[0],shmPtr3[0],timer);

							//If-else ladder to check which process is next and has not yet completed its task
							//and to switch to that process
							if(shmPtr1[2]==0 && shmPtr2[2]==0 && shmPtr3[2]==0)
							{
								end=clock();
								timer=(double)(end-begin) / CLOCKS_PER_SEC;
								if(shmPtr1[0]==1)
								{
									shmPtr1[0]=0;
									shmPtr2[0]=1;
									count2+=1;
									wr1+=2*time_quantum;
									printf("C1 to C2 %f\n",timer);
								}
								else if(shmPtr2[0]==1)
								{
									shmPtr2[0]=0;
									shmPtr3[0]=1;
									wr2+=2*time_quantum;
									count3+=1;
									printf("C2 to C3 %f\n",timer);
								}
								else if(shmPtr3[0]==1)
								{
									shmPtr3[0]=0;
									shmPtr1[0]=1;
									count1+=1;
									wr3+=2*time_quantum;
									printf("C3 to C1 %f\n",timer);
								}
							}
							else if(shmPtr1[2]==1 && shmPtr2[2]==0 && shmPtr3[2]==0)
							{
								end=clock();
								timer=(double)(end-begin) / CLOCKS_PER_SEC;
								if(shmPtr1[0]==1)
								{
									shmPtr1[0]=0;
									shmPtr2[0]=1;
									count2+=1;
									er1=wr1+count1*time_quantum;
									printf("C1 to C2 %f\n",timer);
								}
								else if(shmPtr2[0]==1)
								{
									shmPtr2[0]=0;
									shmPtr3[0]=1;
									wr2+=time_quantum;
									count3+=1;
									printf("C2 to C3 %f\n",timer);
								}
								else
								{
									shmPtr3[0]=0;
									shmPtr2[0]=1;
									count2+=1;
									wr3+=time_quantum;
									printf("C3 to C2 %f\n",timer);
								}
							}

							else if(shmPtr1[2]==0 && shmPtr2[2]==1 && shmPtr3[2]==0)
							{
								end=clock();
								timer=(double)(end-begin) / CLOCKS_PER_SEC;
								if(shmPtr1[0]==1)
								{
									shmPtr1[0]=0;
									shmPtr3[0]=1;
									count3+=1;
									wr1+=time_quantum;
									printf("C1 to C3 %f\n",timer);
								}
								else if(shmPtr2[0]==1)
								{
									shmPtr3[0]=1;
									shmPtr2[0]=0;
									count3+=1;
									er2=wr2+count2*time_quantum;
									printf("C2 to C3 %f\n",timer);
								}
								else
								{
									shmPtr3[0]=0;
									shmPtr1[0]=1;
									count1+=1;
									wr3+=time_quantum;
									printf("C3 to C1 %f\n",timer);
								}
							}	

							else if(shmPtr1[2]==0 && shmPtr2[2]==0 && shmPtr3[2]==1)
							{
								end=clock();
								timer=(double)(end-begin) / CLOCKS_PER_SEC;
								if(shmPtr1[0]==1)
								{
									shmPtr1[0]=0;
									shmPtr2[0]=1;
									count2+=1;
									wr1+=time_quantum;
									printf("C1 to C2 %f\n",timer);
								}
								else if(shmPtr2[0]==1)
								{
									shmPtr2[0]=0;
									shmPtr1[0]=1;
									count1+=1;
									wr2+=time_quantum;
									printf("C2 to C1 %f\n",timer);
								}
								else
								{
									shmPtr1[0]=1;
									shmPtr3[0]=0;
									count1+=1;
									er3=wr3+count3*time_quantum;
									printf("C3 to C1 %f\n",timer);
								}
							}

							else if(shmPtr1[2]==1 && shmPtr2[2]==1 && shmPtr3[2]==0)
							{
								end=clock();
								timer=(double)(end-begin) / CLOCKS_PER_SEC;
								if(shmPtr1[0]==1)
								{

									shmPtr1[0]=0;
									shmPtr3[0]=1;
									count3+=1;
									er1=wr1+count1*time_quantum;
									printf("C1 to C3 %f\n",timer);
								}
								else if(shmPtr2[0]==1)
								{
									shmPtr2[0]=0;
									shmPtr3[0]=1;
									count3+=1;
									er2=wr2+count2*time_quantum;
									printf("C2 to C3 %f\n",timer);
								}
								else
								{
									count3+=1;
								}
							}
							else if(shmPtr1[2]==0 && shmPtr2[2]==1 && shmPtr3[2]==1)
							{
								end=clock();
								timer=(double)(end-begin) / CLOCKS_PER_SEC;
								if(shmPtr2[0]==1)
								{
									shmPtr2[0]=0;
									shmPtr1[0]=1;
									count1+=1;
									er2=wr2+count2*time_quantum;
									printf("C2 to C1 %f\n",timer);
								}
								else if(shmPtr3[0]==1)
								{
									shmPtr1[0]=1;
									shmPtr3[0]=0;
									count1+=1;
									er3=wr3+count3*time_quantum;
									printf("C3 to C1 %f\n",timer);
								}
								else
								{
									count1+=1;
								}
							}

							else if(shmPtr1[2]==1 && shmPtr2[2]==0 && shmPtr3[2]==1)
							{
								end=clock();
								timer=(double)(end-begin) / CLOCKS_PER_SEC;
								if(shmPtr1[0]==1)
								{
									shmPtr1[0]=0;
									shmPtr2[0]=1;
									count2+=1;
									er1=wr1+count1*time_quantum;
									printf("C1 to C2 %f\n",timer);
								}
								else if(shmPtr3[0]==1)
								{
									shmPtr2[0]=1;
									shmPtr3[0]=0;
									count2+=1;
									er3=wr3+count3*time_quantum;
									printf("C3 to C2 %f\n",timer);
								}
								else
								{
									count2+=1;
								}
							}
							//sleep(0.1);
						//}
					}
					er1=wr1+count1*time_quantum;
					er2=wr2+count2*time_quantum;
					er3=wr3+count3*time_quantum;
				}
                		printf("Parent is reading...\n");
				
				//Closing the Write end of the pipes
				close(pfds1[1]);
                		close(pfds2[1]);
                		close(pfds3[1]);
                
				//Reading the results(messages) sent by C1,C2 and C3
				read(pfds1[0], buf1, shmPtr1[1]);
				read(pfds2[0], buf2, 14);
				read(pfds3[0], buf3, shmPtr3[1]);
				
				
				printf("%s\n",buf1);
				printf("%s\n",buf2);
				printf("%s\n",buf3);
				if(choice==0)
				{
					printf("Waiting time for C1: %f\n",wf1);
					printf("Turnaround time for C1: %f\n",ef1);
					printf("Waiting time for C2: %f\n",wf2);
					printf("Turnaround time for C2: %f\n",ef2);
					printf("Waiting time for C3: %f\n",wf3);
					printf("Turnaround time for C3: %f\n",ef3);
				}
				else
				{
					printf("Waiting time for C1: %f\n",wr1);
					printf("Turnaround time for C1: %f\n",er1);
					printf("Waiting time for C2: %f\n",wr2);
					printf("Turnaround time for C2: %f\n",er2);
					printf("Waiting time for C3: %f\n",wr3);
					printf("Turnaround time for C3: %f\n",er3);
				}
            }
        }
    }
}

//Task Thread of C1
void *runner1(void *param)
{
	
	FILE *file;
	file = fopen("Test5.txt", "r");
	//printf("begin 1 %f",(double)begin/CLOCKS_PER_SEC);
	
	//For FCFS
	if(choice==0)
	{
		while(s1==0);
		//Reading n1 numbers
		while(i1<n1)
		{
			fscanf(file,"%lld",&num1);
			sum1+=num1;
			i1++;
		}
		
		//Converting the result to a string and finding its length
		sprintf(result1, "%lld", sum1);
		slen1=strlen(result1);
		shmPtr1[1]=slen1+1;
		
		//Writing the converted result into the pipe for M to read
		close(pfds1[0]);
		write(pfds1[1],result1,slen1+1);
		close(pfds1[1]);
		
		//Updating the variable to let M know C1 has completed its task and then exiting the thread
		run1=1;
		shmPtr1[2]=1;
		pthread_exit(0);
	}

	//Round Robin
	else
	{
		while(i1<n1)
		{
			//When s1=1, it executes. Else, it awaits its turn which is after 2*time_quantum
			if(s1==0)
			{
				/*sleep(x*time_quantum);
				end1=clock();
				t1=(double)(end1-begin) / CLOCKS_PER_SEC;
				printf("C1 starts at %f seconds.\n",t1);*/
			}
			else
			{
				fscanf(file,"%lld",&num1);
				sum1+=num1;
				i1++;
			}
		}
		/*if(s1==0)
		{
			sleep(2*time_quantum);
			end1=clock();
			t1=(double)(end1-begin) / CLOCKS_PER_SEC;
			printf("C1 starts at %f seconds.\n",t1);
		}
		else*/
		//{
			//Converting result to string
			sprintf(result1, "%lld", sum1);
			slen1=strlen(result1);
			shmPtr1[1]=slen1+1;
			
			//Writing the result into the pipe
			close(pfds1[0]);
			write(pfds1[1],result1,slen1+1);
			close(pfds1[1]);
			
			//Updating required variables to let M know C1 has completed
			run1=1;
			shmPtr1[2]=1;
			x--;
			pthread_exit(0);
		//}
	}
}

//Task thread of C2
void *runner2(void *param)
{
	FILE *file;
	file = fopen("Test5.txt", "r");
	//printf("begin 2 %f",(double)begin/CLOCKS_PER_SEC);
	

	//FCFS
	if(choice==0)
	{
		while(s2==0);
		//Scanning each number and printing it
		
		while (i2<n2)
		{
			fscanf(file,"%lld",&num2);
			printf("C2 %lld\n",num2);
			i2++;
		}
		
		//Sending "Done Printing" to M through the pipe
		close(pfds2[0]);
		write(pfds2[1],"Done Printing",14);
		close(pfds2[1]);
		
		//Updating variables to let M know C2 has completed its task and exiting the thread
		run2=1;
		shmPtr2[2]=1;
		pthread_exit(0);
	}

	//Round Robin
	else
	{
		while (i2<n2)
		{
			if(s2==0)
			{
				/*if(counter2==0)
				{
					sleep(time_quantum);
					end2=clock();
					t2=(double)(end2-begin) / CLOCKS_PER_SEC;
					printf("C2 starts at %f seconds.\n",t2);
					counter2++;
				}
				else
				{
					sleep(x*time_quantum);
					end2=clock();
					t2=(double)(end2-begin) / CLOCKS_PER_SEC;
					printf("C2 starts at %f seconds.\n",t2);
				}*/
			}
			else
			{
				//printf("%f\n",(double)clock()/CLOCKS_PER_SEC);
				fscanf(file,"%lld",&num2);
				printf("C2 %lld\n",num2);
				i2++;
				//printf("%f\n",(double)clock()/CLOCKS_PER_SEC);
			}
		}
		/*if(s2==0)
		{
			sleep(2*time_quantum);
			end2=clock();
			t2=(double)(end2-begin) / CLOCKS_PER_SEC;
			printf("C2 starts at %f seconds.\n",t2);
		}
		else*/
		//{
			close(pfds2[0]);
			write(pfds2[1],"Done Printing",14);
			close(pfds2[1]);
			
			run2=1;
			shmPtr2[2]=1;
			x--;
			pthread_exit(0);
		//}
	}
}

void *runner3(void *param)
{
	FILE *file;
	file = fopen("Test5.txt", "r");
	//printf("begin 3 %f",(double)begin/CLOCKS_PER_SEC);

	if(choice==0)
	{
		while(s3==0);
		while(i3<n3)
		{
			fscanf(file,"%lld",&num3);
			printf("C3 %lld\n",num3);
			sum3+=num3;
			i3++;
		}
		sprintf(result3, "%lld", sum3);
		slen3=strlen(result3);
		shmPtr3[1]=slen3+1;
		
		close(pfds3[0]);
		write(pfds3[1],result3,slen3+1);
		close(pfds3[1]);
		
		run3=1;
		shmPtr3[2]=1;
		pthread_exit(0);
	}
	else
	{
		while(i3<n3)
		{
			if(s3==0)
			{
				/*
			sleep(2*time_quantum);
			end3=clock();
			t3=(double)(end3-begin) / CLOCKS_PER_SEC;
			printf("C3 starts at %f seconds.\n",t3);*/
			}
			else
			{
				fscanf(file,"%lld",&num3);
				printf("C3 %lld\n",num3);
				sum3+=num3;
				i3++;
			}
		}
		/*if(s3==0)
		{
			sleep(2*time_quantum);
			end3=clock();
			t3=(double)(end3-begin) / CLOCKS_PER_SEC;
			printf("C3 starts at %f seconds.\n",t3);
		}
		else*/
		//{
			sprintf(result3, "%lld", sum3);
			slen3=strlen(result3);
			shmPtr3[1]=slen3+1;
			
			close(pfds3[0]);
			write(pfds3[1],result3,slen3+1);
			close(pfds3[1]);
			
			run3=1;
			shmPtr3[2]=1;
			x--;
			pthread_exit(0);
		//}
	}
}

//Monitor thread of C1
void *monitor1(void *param)
{
	//begin1=clock();
	//run1=0 when C1 hasn't finished its task and run1=1 when it does
	while(run1==0)
	{
		//Monitoring the communications every 0.1 s(ms?) and updating s1 from the shared memory segment
		end1=clock();
		t1=(double)(end1-begin) / CLOCKS_PER_SEC;
		ti1=t1/0.000001;
		if(ti1==(int)ti1)
		{
			s1=shmPtr1[0];
		}
	}
	pthread_exit(0);
}

//Monitor thread of C2
void *monitor2(void *param)
{
	//begin2=clock();

	//run2=0 when C1 hasn't finished its task and run2=1 when it does
	while(run2==0)
	{
		//Monitoring the communications every 0.1 s(ms?) and updating s2 from the shared memory segment
		end2=clock();
		t2=(double)(end2-begin) / CLOCKS_PER_SEC;
		ti2=t2/0.000001;
		if(ti2==(int)ti2)
		{
			s2=shmPtr2[0];
		}
	}
	pthread_exit(0);
}

//Task thread of C3
void *monitor3(void *param)
{
	//begin3=clock();
	//run3=0 when C1 hasn't finished its task and run3=1 when it does
	while(run3==0)
	{
		//Monitoring the communications every 0.1 s(ms?) and updating s3 from the shared memory segment
		end3=clock();
		t3=(double)(end3-begin) / CLOCKS_PER_SEC;
		ti3=t3/0.000001;
		if(ti3==(int)ti3)
		{
			s3=shmPtr3[0];
		}
	}
	pthread_exit(0);
}
