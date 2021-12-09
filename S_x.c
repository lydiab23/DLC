#include<stdio.h>
#include<stdlib.h>
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
//a function to count the non zero elements in a vector
int count_non_zero(int vect[], int nz, int vect_sz)
 {
  nz=0;
  for(int i=0; i<vect_sz;i++)
   {
    if(vect[i]!=0)
     {
      nz++;
     }
   }
  return nz;
 }
// a function to clean a vector from the zeros by creating another vector containing all its non zero elements
void cleaning(int vect1[], int vect2[], int nz, int vect_sz)
 {
  int j=0;
  for(int i=0;i<vect_sz;i++)
   {
    if(vect1[i]!=0)
     {
      vect2[j]=vect1[i];
      j++;
     }
   }
  }
    
//a function to determine the primes in S_x    
void S_x(int primes[], char vect[], int size, int bla[])
{
 int cntr=0, pr_indx=0,i=0;
 while(cntr<size)
 {
  if(vect[cntr]=='A')
   {
    pr_indx++;
   }
  if(vect[cntr]=='B')
   {
    bla[i]=primes[pr_indx+1];
    i++;
    pr_indx=0;
   }
   cntr++;
}
}
int main()
{
FILE* f1,*f2;
f1=fopen("traces_de_p.txt","r");
if(f1==NULL){
        printf("Erreur d'ouverture du fichier \n");
    }
f2=fopen("traces_de_q.txt","r");
 if(f2==NULL){
        printf("Erreur d'ouverture du fichier \n");
    }
    
  
int size_p=file_size(f1), size_q=file_size(f2);
char vect_p[size_p+1],vect_q[size_q+1];
fscanf(f1,"%s",vect_p);
fscanf(f2,"%s",vect_q);
fclose(f1);
fclose(f2);


int k=60,u;//k can be changed
int primes[k];
mpz_t p,q;
mpz_inits(p,q,NULL);
for(int i=0;i<k;i++){
		mpz_nextprime(p,q);
		u=mpz_get_ui(p);
		primes[i]=u;
		mpz_set(q,p);
}
mpz_clears(p,q,NULL);


int S_p[k],S_q[k];
//Initializing S_p and S_q to 0
for(int i=0;i<k;i++)
 {
  S_p[i]=0;
  S_q[i]=0;
 }
//getting the values of S_p and S_q
S_x(primes,vect_p,size_p, S_p);
S_x(primes,vect_q,size_q, S_q);
printf("S_p={\n");
for(int j=0;j<k;j++)
 {
  printf("%d\n",S_p[j]);
 }
int nz_p, nz_q;
nz_p=count_non_zero(S_p,nz_p,size_p+1);
nz_q=count_non_zero(S_q,nz_q,size_q +1);
int S_p1[nz_p], S_q1[nz_q];
cleaning(S_p,S_p1,nz_p,size_p+1);
cleaning(S_q,S_q1,nz_q,size_q+1);
}
