#include <stdio.h>
//#include <cstring>
#include <string.h>
//#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
//#include <stdio.h>
//#include "minishell.h"
#include <stdbool.h>
#include "nehal.h"
#ifdef __cplusplus__
#include <cstdlib>
#else
#endif

//using namespace std;

char *lookupPath(char**, char**);
int parseCommand(char*, struct command_t *);
int parsePath(char**);
void printPrompt();
void readCommand(char*);

int main(int argc, char *argv[])
{
 struct command_t command;
 char commandLine[1024];
 char**pathv= malloc(MAX_ARGS*sizeof(char *));//new char *[MAX_ARGS];
 for(int i = 0; i < MAX_ARGS; i++) 
  pathv[i] = malloc(100*sizeof(char)); //new char[100];
	
  parsePath(pathv);
 //if (system("CLS")) system("clear");//to clear the screen
//cout<<"\t\t\033[1;31mWELCOME TO THE BEST MINISHELL IN TOWN\033[0m\n"<<endl;
printf("\t\t\033[1;31mWELCOME TO THE BEST MINISHELL IN TOWN\033[0m\n\n");  
while(true)
	{
	  printPrompt();
	  readCommand(commandLine);
	  parseCommand(commandLine, &command);

	 command.name= lookupPath(command.argv,pathv);
	  if(command.name==NULL)
		{
		  printf("ERROR!\n");
		  //cout<<"ERROR!"<<endl;
		}


	  int pid=fork();

		if(pid>0){
			wait(NULL);
			 }

		else if(pid==0)
			{
				//CHILD PROCESS
			    if(command.name!=command.argv[0])
				execvp(command.name, command.argv);

			}

	}
	printf("TERMINATING\n");
	//cout<<"TERMINATING"<<endl;
return 0;
}
	
void printPrompt()
{
  char cwd[1024];
  if(getcwd(cwd, sizeof(cwd)) != NULL) {
  //cout<<"\033[7;34m"<<cwd<<"-------->\033[0m";
  printf("\033[7;34m");
  printf(cwd);
  printf("-------->\033[0m");
  }
  else {	
  	//cout<<"No sorry"<<endl;
  	printf("No sorry\n");
  }
}
void readCommand(char* buffer)
{
 //cin.getline( buffer, 1024, '\n' );
	scanf("%s", buffer);
}
//////////////////////////////////////////////////
int parsePath(char *dirs[])
{
  char *pathEnvVar;
  char *thePath;
  
  for(int i=0;i<MAX_ARGS;i++)
   dirs[i]=NULL;
  
  pathEnvVar=(char*) getenv("PATH");
 
  thePath= (char*) malloc(strlen(pathEnvVar)+1);
 
  strcpy(thePath, pathEnvVar);
  


  int i=0;
  //char *temp=new char[100];
  char * temp = malloc(100 * sizeof(char));
  temp=strtok(thePath,":\n");
  while(temp!=NULL)
	{
	
	  dirs[i]=temp;
	  temp=strtok(NULL,":\n");
	  i++;
	}

 return 0;
}

char *lookupPath(char**argv, char **dir)
{ 
  //char**arr=new char*[11];
	char ** arr = malloc(11 * sizeof(char *));
  for(int i=0;i<11;i++)
	{
	//arr[i]=new char[10];
		arr[i] = malloc(10 * sizeof(char));
	}
  for(int i=0;i<11;i++)
	{
	 arr[i][0]='/';
	}
 for(int i=0;i<11;i++)
	{
	for(int j=1;j<strlen(argv[0])+1;j++)
	 arr[i][j]=argv[0][j-1];
	}
 
  char *result;    
  char pName[MAX_PATH_LEN];
  if(*argv[0]=='/')
    {
	//result=new char[strlen(argv[0])];
	result = malloc(sizeof(char) * strlen(argv[0]));
	result=argv[0];
	return result;
    }
 //char **newarr=new char *[11];
char ** newarr = malloc(11 * sizeof(char *));
   for(int i=0;i<11;i++)
	{
	  //newarr[i]=new char[100];
		newarr[i] = malloc(100 * sizeof(char));
	}
   for(int i=0;i<11;i++)
	{
	for(int j=0;j<strlen(dir[i]);j++)
	  newarr[i][j]=dir[i][j];
	int x=strlen(dir[i]);
        for(int t=0;t<strlen(arr[i]);t++)
	  newarr[i][t+x]=arr[i][t];
        }

  for(int i=0;i<11;i++)
    {
	
	if( access( newarr[i], F_OK ) != -1 ) 
	 { 
		//result=new char[strlen(dir[i])];
		result = malloc(sizeof(char) * strlen(dir[i]));
		result=newarr[i];	
		return result;
	 }

    }
  if(argv[0][0]=='c'&&argv[0][1]=='d')
	{
	  chdir(argv[1]);
	  return argv[0];
	  
	}

   //cout<<"COMMAND NOT FOUND SORRY!"<<endl;
	printf("COMMAND NOT FOUND SORRY!\n");
   return NULL;
}

int parseCommand(char*cmdline, struct command_t *command)
{
  char *temparr[MAX_ARGS];
  for (int i=0;i<MAX_ARGS;i++){
   		//temparr[i]=new char[100];
		temparr[i] = malloc(sizeof(char) * 100);		
	}
	
  int i=0;
  //char *temp=new char[100];
	char * temp = malloc(sizeof(char) * 100);
  temp=strtok(cmdline," \n");
  while(temp!=NULL)
	{
	  temparr[i]=temp;
	  temp=strtok(NULL," \n");
	  i++;
	}
  command->argc=i;
  for(int j=0;j<command->argc;j++)
	{
	 // command->argv[j]=new char[100];
		command->argv[j] = malloc(sizeof(char) * 100);
	  command->argv[j]=temparr[j];
	}
  //command->argv[command->argc]=new char[100];
	command->argv[command->argc] = malloc(sizeof(char) * 100);
   command->argv[command->argc]=NULL;

  return 0;
}

