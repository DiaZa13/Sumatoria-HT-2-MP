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
#include <cmath>//
#include <thread>
#include <sys/wait.h>	
using namespace std;

float sumatoria (int numero1, int numero2){
	float resultado = 0;
	for (;numero1<=numero2;numero1++)
	{
		resultado += 1/pow(numero1,2);
	} 
	return resultado;
}

int main(int argc,char *argv[]) {
	int i = 1;
	pid_t pid;
	float respuestaHijo = 0;
	float respuestaPadre = 0;
	cout<<"Ingrese el valor hasta donde quiere la sumatoria (solo permitido para valores menores a 100): ";
	cin >> i;
	printf("%s %d","Numero ingresado: ", i);
	cout<<endl;
	pid = fork();
	if (i < 100)
	{
		if (i <= 0)
		{
			printf("%s","Ha ocurrido un error con el numero ingresado");
			cout<<endl;
		}
		else if (pid !=0)//Padre
		 {
			 //Aqui recupero la respuesta que dio el hijo (ver mas abajo en la parte del hijo) 
			 int childval=1;
             wait(&childval);
             //---------------------------------------
			 respuestaPadre = sumatoria(1,(i/2));
			 printf("%s %f","Resultado del proceso Padre: ", respuestaPadre);
			 cout<<endl;
			 //Transformo el dato enviado por el hijo a su valor original
			 respuestaHijo = childval/255;
			 respuestaHijo = respuestaHijo/1000;
			 //-----------------------------------------
			 printf("%s %f","La respuesta de la suma es: ", (respuestaPadre + (respuestaHijo)));
			 
		 } else if (pid ==0) { //Hijo
			 //Programacion para evitar errores cuando se reparten a la mitad las sumatorias que deben de hacer
			 if (i%2 == 0)
			 {
				 respuestaHijo = sumatoria((i/2) + 1,i);
			 printf("%s %f","Resultado del proceso Hijo: ", respuestaHijo);
			 }else 
			 {
				 int h = i;
				 respuestaHijo = sumatoria((h/2) + 1,i);
				 printf("%s %f","Resultado del proceso Hijo: ", respuestaHijo);
			 }
			 cout<<endl;
			 //Tomo 4 decimales de la respuesta del hijo para enviarlos hacia el padre Exit() es como un return por asi decirlo
			 //La funcion exit() aproxima el resultado, en este caso si no 
			 //lo multiplico por 1000 envia un 0. Y si intento multiplicarlo 
			 //por mas de 1000, el padre recibe un valor muy distinto
			 int resultado = respuestaHijo*1000 ;
			 exit(resultado);
		 }
		 
	}else 
	{
		std::cout <<"El numero ";
		std::cout <<" es demasiado grande";
		cout<<endl;		 
	}
	
	return 0; 
}
