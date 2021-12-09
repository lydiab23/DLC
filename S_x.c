#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

int file_size(FILE*fp)
 {
  int size;
  fseek(fp, 0, SEEK_END); // seek to end of file
  size = ftell(fp);
  rewind(fp);
  return size;
 }
 
void S_x(int primes[], char vect[], int size, int S_x[])
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
    S_x[i]=primes[pr_indx];
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
printf("the size of f1 is %d\n",size_p);
char vect_p[size_p+1],vect_q[size_q+1];
fscanf(f1,"%s",vect_p);
fscanf(f2,"%s",vect_q);
fclose(f1);
fclose(f2);
int k=60,u;
int primes[k];
mpz_t p,q;
mpz_inits(p,q,NULL);
for(int i=0;i<k;i++){
		mpz_nextprime(p,q);
		u=mpz_get_ui(p);
		primes[i]=u;
		mpz_set(q,p);
}
printf("test\n");
mpz_clears(p,q,NULL);
int S_p[k],S_q[k];
printf("test\n");
for(int i=0;i<k;i++)
 {
  S_p[i]=0;
  S_q[i]=0;
 }
 printf("test\n");
S_x(primes,vect_p,size_p, S_p);
S_x(primes,vect_q,size_q, S_q);
printf("S_p={\n");
for(int j=0;j<k;j++)
 {
  printf("%d\n",S_p[j]);
 }
}
