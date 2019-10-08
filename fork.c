/* 
   Calculate Functions using the concept of threads
   By: Paulo Trindade

   F(x) = G(x) + H(x)
   G(x) = x!
   H(x) = I(x) + L(x)
   I(x) = (x * x) / sqrt(x)
   L(x) = not defined (default = 4)
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <math.h>


double i(double x) {
   return (x * x) / (sqrt(x));
}

double l(double x) {
   return 4;
}

double g(double x) {
   double result = 1;
   int i;
   for (i = x; i > 0; i--) {
      result = result * i;
   }
   return result;
}

double h(double x) {
   double funcaoI;
   double funcaoL;
   double result;
   int tubo_pHxpIx[2];

   pid_t processoLx = fork();
   
   if(processoLx == 0) {
      funcaoL = l(x);
      close(tubo_pHxpIx[0]);
      write(tubo_pHxpIx[1], &funcaoL, sizeof(funcaoL));
      printf("Função L: %.2lf\n", funcaoL);
      exit(0);
   } else {
      funcaoI = i(x);
      printf("Função I: %.2lf\n", funcaoI);
      wait(0);
      close(tubo_pHxpIx[1]);
      read(tubo_pHxpIx[0], &funcaoL, sizeof(funcaoL));
   }
   result = funcaoI + funcaoL;
   return result;
}

int main() {
   
   double x;
   double funcaoG;
   double funcaoH;
   double result;
   int tubo_p1pGx[2];
	
   pipe(tubo_p1pGx);
   printf("Digite o valor de x\n");
   scanf("%lf", &x);
  
   pid_t processoGx = fork();

   if(processoGx == 0) {
      //Calculate g(x)
      funcaoG = g(x);
      close(tubo_p1pGx[0]);
      write(tubo_p1pGx[1], &funcaoG, sizeof(funcaoG));
      printf("Função G: %.2lf\n", funcaoG);
      exit(0);
   } else {
      int tubo_p1pHx[2];
      pipe(tubo_p1pHx);
      pid_t processoHx = fork();
      if(processoHx == 0) {
         //Calculate h(x)
         funcaoH = h(x);
         close(tubo_p1pHx[0]);
      	write(tubo_p1pHx[1], &funcaoH, sizeof(funcaoH));
         printf("Função H: %.2lf\n", funcaoH);
         exit(0);
      } else {
	      wait(0);
	      //Read h(x)
         close(tubo_p1pHx[1]);
         read(tubo_p1pHx[0], &funcaoH, sizeof(funcaoH));
      }
      wait(0);
      //Read g(x)
      close(tubo_p1pGx[1]);
      read(tubo_p1pGx[0], &funcaoG, sizeof(funcaoG));

      result = funcaoG + funcaoH;
      printf("Resultado de G(x) + H(x) = %.2lf\n", result);  
   }
   return 0;
}
