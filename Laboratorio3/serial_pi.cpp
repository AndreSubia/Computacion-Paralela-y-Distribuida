#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <ctime> 

using namespace std;

double sum;

double Serial_pi(long long n) {
   double sum = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }
   return 4.0*sum;

}  

int main(int argc, char* argv[]){

    double start, finish, elapsed;
    long long n = 10000000;
    sum = 0.0;
    start = clock();
    sum = Serial_pi(n);
    finish = clock();
    elapsed = finish - start;
    printf("With n = %lld terms,\n", n);
    printf("Estimate value of pi = %.15f\n", sum);
    printf("The elapsed time is %e seconds\n", elapsed);

}
