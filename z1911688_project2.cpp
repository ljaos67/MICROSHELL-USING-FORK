/****************************************************************
   PROGRAM:   Assignment 2
   AUTHOR:    Leonart Jaos
   LOGON ID:  Z1911688
   DUE DATE:  02/18/2021

   FUNCTION: This project consists of designing a C/C++ program to serve 
   as a shell interface that accepts user
   commands and then executes each command in a separate process.

   INPUT: Could possibly input/output for directing i/o    

   OUTPUT: Could possibly input/output for directing i/o   

   NOTES:     
****************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <iterator>
#include <bits/stdc++.h>
#include <fcntl.h>
#define MAX_LINE 1024

using namespace std;

// Demonstrates conversion
// from character array to string
 
#include <bits/stdc++.h>
using namespace std;

//for i/o redirection 
bool inpFlag = false;
bool outpFlag = false;

// INPUT: converts character array
// OUTPUT: string and returns it
// FUNCTION: parse buffer to string
string convertToString(char buf[1024])
{
	// parse buffer to string
    int i;
	int arrSize = *(&buf + 1) - buf;
    string s = "";
    for (i = 0; i < arrSize; i++) {
        // null terminating
		if(buf[i] == '\0')
		{
			break;
			
		}
		//input will be used
		else if(buf[i] == '<')
		{
		
			if(outpFlag)
			{
				cout << "helllloooo" << endl;
				// do nothing
			}
			else
			{
				s = s + buf[i];
				inpFlag = true;
			}
		}
		// output will be used
		else if(buf[i] == '>')
		{
			if(inpFlag)
			{
				// do nothing
			}
			else
			{
				s = s + buf[i];
				outpFlag = true;
			}
		}
		else
		{
			s = s + buf[i];
		}
	}
	// returns buffer as string
    return s;
}

// INPUT: string buffer
// OUTPUT: vector of strings
// FUNCTION: separates args into vector of strings
vector<string> argSep(string a)
{
	bool pass = false;
	// checks i/o redirect
	bool chk2 = false;
	string tmp = "";
	vector<string> v1;
	// parse on space, or i/o
	for(unsigned long int i = 0; i < a.length(); i++)
	{
		if(a[i] == ' ')
		{
			
			v1.push_back(tmp);
			tmp = "";
		}
		// cases for i/o
		else if(a[i] == '>' || a[i] == '<')
		{
			
			if(tmp.length() > 0)
			{
				
				v1.push_back(tmp);
				tmp = "";
				tmp += a[i];
				v1.push_back(tmp);
				tmp = "";
			}
			else
			{
				tmp = "";
				tmp += a[i];
			}
			chk2 = true;
		}
		// null termination, the rest are random vals
		else if(a[i] == '\0')
		{
			
			v1.push_back(tmp);
			tmp = "";
			pass = true;
		}
		// if null termination has passed
		else if(pass == false)
		{
			if(chk2)
			{
				tmp += a[i];
				chk2 = false;
			}
			else
			{	
				tmp += a[i];
			}
		}
		else
		{
			
		}
	}
	if(tmp.length() > 0)
	{
		v1.push_back(tmp);
	}
	// return vector of string args
	return v1;
}

int main(void)
{
	vector<string> args;
	char buf[MAX_LINE];
	string sBuf; 			// char arrays are confusing
	//int sBuf_size;
	pid_t pid;
	int number;
	int status;
	// args in vector of strings
	vector<string> v1;
	// buffer 
	vector<string> v2;
	char * command[MAX_LINE] = {NULL, NULL, NULL};
	printf("mysh>");
	while (fgets(buf, MAX_LINE, stdin) != NULL)
	{
		buf[strlen(buf) -1] = 0; //chop the newline at the end of buf
		sBuf = convertToString(buf);
		// exit shell
		if(sBuf == "quit" || sBuf == "q")
		{
			exit(0);
		}
		// repeat last viable command
		else if(sBuf == "!!")
		{
			// error
			if(v2.empty())
			{
				fprintf(stderr, "No commands in history for %s\n", buf);
				exit(0);
			}
			// if last viable command was history
			else if(v2.back() == "history" || v2.back() == "History" )
			{
				v2.push_back("history");
				cout << v2.back() << endl;
				for(unsigned long int i = 0; i < v2.size(); i++)
				{
					cout << " " << i + 1 << " " << v2[i] << endl;
				}
				buf[0] = '\0';			
			}
			// regular case
			else
			{
				v2.push_back(v2[v2.size() -1]);
				cout << v2.back() << endl;
				//cout << v2.size() << endl;
				char cstr[v2.back().size() + 1];
				strcpy(cstr, v2[v2.size() - 1].c_str());
				//rebuild buf
				for(unsigned long int i = 0; i < v2.back().size(); i++)
				{
					buf[i] = v2.back()[i];
				}
				buf[v2.back().size()] = '\0';
			}
		}
		// command history
		else if(sBuf == "history" || sBuf == "History")
		{
			v2.push_back("history");
			for(unsigned long int i = 0; i < v2.size(); i++)
			{
				cout << " " << i + 1 << " " << v2[i] << endl;
			}
			// set buffer termination character
			buf[0] = '\0';
		}
		else
		{
			v2.push_back(sBuf);
		}
		// splilt processes for commands
		if ((pid = fork()) <0)
			fprintf(stderr,"Fork error\n");
		else if (pid==0)
			{ /* child */
				// separate buffer into strings into vector
				v1 = argSep(sBuf);
				// input in command
				if(inpFlag)
				{
					int ind = 0;

					for(unsigned long int i = 0; i < v1.size(); i++)
					{
						if(v1[i] == "<")
						{
							ind = i;
						
						}
						
					}
					// open file, set permissions
					int fd1 = open(v1[ind + 1].c_str(), O_CREAT | O_RDONLY, 0666);
					//error
					if(fd1 < 0)
						printf("Error opening the file\n");
					
					dup2(fd1, STDIN_FILENO);
					close(fd1);
					string s = "";
					// rebuild buffer
					for(unsigned long int i = 0; i < (long unsigned int)ind; i++)
					{
						s += v1[i] + " ";
					}
					for(unsigned long int j = 0; j <=s.length(); j++)
					{
						if(j == s.length())
						{
							buf[j] = '\0';
						}
						else
						{
							buf[j] = s[j];
						}
					}
					// parse for execv
					char * ptr;
					number = 0;
					command[0]=strtok(buf," ");
					while((ptr=strtok(NULL, " ")) != NULL)
					{
						number++;
						command[number]=ptr;
					}
					command[number +1] = NULL;
					execvp(command[0],command); //handle argument array
					//error
					fprintf(stderr, "Err 3 here Couldn’t execute the command %s\n", buf);
					exit(127);

				}
				// for output redirection
				else if(outpFlag)
				{
					int ind = 0;
					// find location of >
					for(unsigned long int i = 0; i < v1.size(); i++)
					{
						if(v1[i] == ">")
						{
							ind = i;
						
						}
						
					}
					// open file for write
					int fd1 = open(v1[ind + 1].c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if(fd1 < 0)
						printf("Error opening the file\n");
					
					dup2(fd1, STDOUT_FILENO);
					close(fd1);
					string s = "";
					// rebuild buffer
					for(unsigned long int i = 0; i < (long unsigned int)ind; i++)
					{
						s += v1[i] + " ";
					}
					for(unsigned long int j = 0; j <=s.length(); j++)
					{
						if(j == s.length())
						{
							buf[j] = '\0';
						}
						else
						{
							buf[j] = s[j];
						}
					}
					// parse for execvp command
					char * ptr;
					number = 0;
					command[0]=strtok(buf," ");
					while((ptr=strtok(NULL, " ")) != NULL)
					{
						number++;
						command[number]=ptr;
					}
					command[number +1] = NULL;
					execvp(command[0],command); //handle argument array
					//error
					fprintf(stderr, "Err 2 here Couldn’t execute the command %s\n", buf);
					exit(127);
				}
				char * ptr;
				number =0;
				command[0]=strtok(buf," ");
				while((ptr=strtok(NULL, " ")) != NULL)
				{
					number++;
					command[number]=ptr;
				}
				command[number +1] = NULL;
				execvp(command[0],command); //handle argument array
				//error
				fprintf(stderr, "Err 2 here Couldn’t execute the command %s\n", buf);
				exit(127);
			}
		/* parent */
		if ( (pid = waitpid(pid, &status, 0)) <0)
		{
			fprintf(stderr,"waitpid error\n");
		}
		//reset flags that indicate i/o
		outpFlag = false;
		inpFlag = false;
		printf("mysh>");
	}
	exit(0);
}