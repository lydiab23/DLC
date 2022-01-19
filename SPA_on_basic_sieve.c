#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gmp.h>

// a function to calculate the size of the file
int file_size(FILE*fp)
{
  int size;

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  return size;
}
 
//a function to determine the primes in S_x    
int S_x(int bla[],int primes[], char vect[], int size)
{
  int cntr=0, pr_indx=1,i=0;
  while(cntr<size)
  {
    if(vect[cntr]=='A')
    {
      pr_indx++;
    }

    if(vect[cntr]=='B')
    {
      bla[i]=primes[pr_indx];
      i++;
      pr_indx=1;
    }

    cntr++;
}
return i;
}

//a cleaning function that takes vect1 with extra elements and turn it into vector 2
void clean(int vect1[], int vect2[], int i)
{

  //////////////////////////**************//////////////////////////
  // change this which used to be a loop, you can use memory copy 'memcpy' to directly copy the content O(n) 
  memcpy(vect2, vect1, sizeof(int) * i);

}
//extended euclide algorithm
int extended_gcd(int a, int b, int *x, int *y)
{
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }
 
    int _x, _y;
    int gcd = extended_gcd(b % a, a, &_x, &_y);
 
    *x = _y - (b/a) * _x;
    *y = _x;
 
    return *x;
}

//CRT
int CRT ( int S_x[], int eq_x[], int size )
{
//here size is the cardinal of S_x
int N=1,x,y,a_x=0;

//N=product of all elements in S_x
for(int i=0;i<size;i++)
{
 N=N*S_x[i];
}
//calculating a_x
for(int i=0;i<size;i++)
{
a_x=a_x+(eq_x[i]*(N/S_x[i])*extended_gcd(N/S_x[i], S_x[i], &x, &y));
}

//the main to work this programm
int main()
{
  //opening the files

  int k=54,u;//k can be changed
  int primes[k];

  FILE* f1,*f2;
  f1=fopen("traces_de_p.txt","r");

  if(!f1)
    printf("Erreur d'ouverture du fichier p\n");


  f2=fopen("traces_de_q.txt","r");

  if(!f2)
    printf("Erreur d'ouverture du fichier q\n");

      
  //setting up the vectors containing the traces,  
  int size_p=file_size(f1), size_q=file_size(f2);
  char vect_p[size_p+1],vect_q[size_q+1];

  fscanf(f1,"%s",vect_p);
  fscanf(f2,"%s",vect_q);

  fclose(f1);
  fclose(f2);


  mpz_t p,q;
  mpz_inits(p,q,NULL);

  //the vector containing the first k primes that we are looking for
  for(int i=0;i<k;i++){
          mpz_nextprime(p,q);
          u=mpz_get_ui(p);
          primes[i]=u;
          mpz_set(q,p);
  }

  mpz_clears(p,q,NULL);

  //filling up S_p and S_q
  int S_p[k],S_q[k];

  int nz_p=S_x(S_p,primes,vect_p,size_p+1);
  int nz_q=S_x(S_q,primes,vect_q,size_q+1);

  //cleaning up S_p and S_q
  int S_p2[nz_p], S_q2[nz_q];

  clean(S_p, S_p2, nz_p);
  clean(S_q, S_q2,nz_q);

  //collecting the equations of p and q
  int eq_p[nz_p], eq_q[nz_q];

  for(int j=0;j<nz_p;j++)
   {
    //////////////////////////**************//////////////////////////
    //change this, used to be S_p which i guess is a typo, same with S_q
    eq_p[j]=2*(nz_p -j)%S_p2[j];
   }
  for(int j=0;j<nz_p;j++)
   {
    eq_q[j]=2*(nz_q -j)%S_q2[j];
   }
  int a_p, a_q;
  a_p=CRT ( S_p2, eq_p, nz_p);
  a_q=CRT(S_q2,eq_q,nz_q);
}
