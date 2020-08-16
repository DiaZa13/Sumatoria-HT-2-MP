/**----------------------------------------* HT2.2.cpp* ---------------------------------------* 
UNIVERSIDAD DEL VALLE DE GUATEMALA* 
*CC3056 - Programacion de Microprocesadores* 
Autor: Orlanco Cabrera 19943 y Diana Corado 191025* 
*Fecha: 2020/08/15*
---------------------------------------* 
*Realiza una sumatoria repartiendo los procesos en el hijo y en el padre*
----------------------------------------* */
#include <iostream> 
#include <sys/types.h>
#include <unistd.h>
#include <cmath>
#include <thread>
#include <sys/wait.h>
using namespace std;

//function that does the sumatory 
double sum (int start, int limit){
	double result = 0;
	int i = start;
	for(; i <= limit; i++){
	   result += 1/pow(i,2);
	}
	return result;
}


int main(int argc,char *argv[]) {
    
    //Colors
    const string cyan("\033[1;36m");
    const string blue("\033[1;34m");
    const string bWhite("\033[1;37m");
    const string reset("\033[0m");
    
    //variables
    double total_result;
    double child_result = 0;
    double parent_result = 0;
    
    int fd[2];//pipe options
    pipe(fd);//pipe creation
    int limit = 0;
    
    cout<<"****SUMATORIA EN PROCESO PADRE E HIJO****"<<endl;
    
    //User input
    cout<<"Ingrese un número como valor limite de la sumatoria: ";
    //Defensive programming
    while(!(cin >> limit)){
	    cin.clear();
        cin.ignore(10000,'\n');
        cout<<"Ingrese un número como valor limite de la sumatoria: ";
    }
    
	
	//verify if the limit is even
    int even = limit%2;
    //process creation
	pid_t pid = fork();
	
    switch(pid){
        case -1: //fork creation fails
            cout<<"Error en la crecion del proceso"<<endl;
            break;
            
        case 0: //child process
            close(fd[0]);//close pipe-read
            
            if(even == 0)
                child_result = sum(1,(limit/2));
            else{
                limit = limit + 1;
                child_result = sum(1,(limit/2));
            }
            
            cout<<cyan<<"Resultado del proceso hijo:    "<<child_result<<reset<<endl;
            //Save the result of the sum did by the child
            write(fd[1],&child_result,sizeof(child_result));
            close(fd[1]);//close the pipe-write
            exit(0);//end the child process
            break;
            
        default: //parent process
            wait(0);
            close(fd[1]);//close the parent pipe-write
            
            if(even == 0)
                parent_result = sum((limit/2)+1,limit);
            else{
                limit = limit - 1;
                parent_result = sum((limit/2)+2,limit+1);
            }
            
            cout<<blue<<"Resultado del proceso padre:   "<<parent_result<<reset<<endl;
            //Read the result of the sum the child made
            read(fd[0],&child_result,sizeof(child_result));
            close(fd[0]);//close the parent pipe-read
            total_result = parent_result + child_result;
            cout<<"                            + ---------- "<<endl;
            cout<<bWhite<<"Resultado de la sumatoria:     "<<total_result<<reset<<endl;
            break;
    }
	
	return 0; 
}
