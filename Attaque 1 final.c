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
int S_x(int primes_x[],int primes[], char vect[], int size)
{
  int cntr=0, pr_indx=1,i=0;
  while(cntr<size-1)
  {
  //printf("vect[%d]=%c\n",cntr,vect[cntr]);
  if(vect[cntr]=='A')
  {
    pr_indx++;
  }

    if(vect[cntr]=='B')
    {
      primes_x[i]=primes[pr_indx];
      printf("S_x[%d]=%d\n",i,primes_x[i]);
      i++;
      pr_indx=1;
    }
    if(vect[cntr]=='C')
    {
      if(cntr==size-3)
      {
        break;
      } 
      else
      {
      pr_indx=1;
      }
    }
    cntr++;
  }
  return i;
}

//a cleaning function that takes vect1 with extra elements and turn it into vector 2
int clean(int vect1[], int vect2[], int i)
{
  i=i-1;
  //////////////////////////**************//////////////////////////
  // change this which used to be a loop, you can use memory copy 'memcpy' to directly copy the content O(n) 
  memcpy(vect2, vect1, sizeof(int) * i);
  i=i+1;
  vect2[i-1]=2;
  return i;

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

int create_vector_for_crt(int vect[],int size)
{
  //on supprime les élements qui se répetent dans la liste
  int i,j,k;
    for(i=0;i<size;i++){
        for(j=i+1;j<size;j++){
            if(vect[i]==vect[j]){
                for ( k = j; k < size - 1; k++)  
                {  
                    vect[k] = vect[k + 1];  
                }  
                size--;  
            // if the position of the elements is changed, don't increase the index j  
                j--;      
            }
        }
    }

  return size;
}

//CRT
int CRT ( int S_xx[], int eq_x[], int size )
{
  //here size is the cardinal of S_x
  int N=1,x,y,a_x=0;

  //N=product of all elements in S_x
  for(int i=0;i<size-1;i++)
  {
    printf("calculating(%d)- N=%d\n",i,N);
    N=N*S_xx[i];
  }
  //calculating a_x
  for(int i=0;i<size-1;i++)
  {
    printf("eq_x[%d]=%d\n",i,eq_x[i]);
    int l;
    l=N/S_xx[i];
    printf("l=%d \n",l);
    //printf("N=%d divisé par S_xx[%d]=%d donne l=%d \n ",N,i,S_xx[i],l);
    //printf("extended_gc(%d,%d,x,y)=%d\n",(N/S_xx[i]),S_xx[i],extended_gcd(N/S_xx[i], S_xx[i], &x, &y));
    a_x=a_x+(eq_x[i]*(N/S_xx[i])*extended_gcd(N/S_xx[i], S_xx[i], &x, &y));
   // printf("testing the a_x\n");
   // printf("a_x=%d\n",a_x);
  }
  if(a_x>N){
    a_x=a_x-N;
  }
  if(a_x<N){
    a_x=a_x+N;
  }
  if(a_x>N){
    a_x=a_x-N;
  }
  else if(a_x<N){
    a_x=a_x+N;
  }
  return a_x;
  
}

int LCM(int n1, int n2){
  int max;
  // maximum number between n1 and n2 is stored in max
  max = (n1 > n2) ? n1 : n2;

  while (1) {
      if (max % n1 == 0 && max % n2 == 0) {
          //printf("The LCM of %d and %d is %d.", n1, n2, max);
          break;
      }
      ++max;
  }
  return max;
}

//the main to work this programm
int main()
{
  //opening the files

  int k=54,u;//k can be changed
  int primes[k];

  FILE* f1,*f2,*f3;
  f1=fopen("traces_de_p.txt","r");

  if(!f1){
    printf("Erreur d'ouverture du fichier p\n");
  }


  f2=fopen("traces_de_q.txt","r");

  if(!f2){
    printf("Erreur d'ouverture du fichier q\n");
  }

  f3=fopen("valeur_de_N=p.q.txt","r");

  if(!f3){
    printf("Erreur d'ouverture du fichier q\n");
  }

      
  //setting up the vectors containing the traces,  
  int size_p=file_size(f1), size_q=file_size(f2);
  char vect_p[size_p+1],vect_q[size_q+1];
  mpz_t N;
  mpz_init(N);

  fscanf(f1,"%s",vect_p);
  fscanf(f2,"%s",vect_q);

  mpz_inp_str(N,f3,10);


  fclose(f1);
  fclose(f2);
  fclose(f3);

  mpz_t p,q;
  mpz_inits(p,q,NULL);
 
  //generating the primes
  for(int i=0;i<k;i++)
  {
  	mpz_nextprime(p,q);
  	u=mpz_get_ui(p);
  	primes[i]=u;
  	mpz_set(q,p);
  }

  mpz_clears(p,q,NULL);

  //filling up S_p and S_q
  int S_p[k],S_q[k];
  printf("testing the S_x function\n");
  int nz_p=S_x(S_p,primes,vect_p,size_p+1)+1;
  int nz_q=S_x(S_q,primes,vect_q,size_q+1)+1;
  printf("we passed the S_x function\n");
  //cleaning up S_p and S_q
  int S_p2[nz_p], S_q2[nz_q];
  printf("testing the cleaning function\n");
  int i;
  nz_p=clean(S_p, S_p2, nz_p);
  nz_q=clean(S_q, S_q2,nz_q);

  printf("the cleaning function passed the test\n");

  int NS_p=1,NS_q=1;
  nz_p=create_vector_for_crt(S_p2,nz_p);
  for(i=0;i<nz_p;i++){
    printf("s_p2[%d] = %d \n",i,S_p2[i]);
    NS_p=NS_p*S_p2[i];
  }

  nz_q=create_vector_for_crt(S_q2,nz_q);
  for(i=0;i<nz_q;i++){
    printf("s_q2[%d] = %d \n",i,S_q2[i]);
    NS_q=NS_q*S_q2[i];
  }

  //collecting the equations of p and q
  int eq_p[nz_p], eq_q[nz_q];
  printf("testing the equations\n");
  for(int j=0;j<nz_p-1;j++)
  {
    eq_p[j]=2*((nz_p) -j)%S_p2[j];
    printf("eq_p[%d]=%d\n",j,eq_p[j]);
  }
  for(int j=0;j<nz_q-1;j++)
  {
    eq_q[j]=2*((nz_q)-j)%S_q2[j];
    printf("eq_q[%d]=%d\n",j,eq_q[j]);
  }
  printf("we passed the equations\n");
  int a_p,b_q;
  printf("testing CRT\n");
  a_p=CRT(S_p2, eq_p, nz_p);
  b_q=CRT(S_q2, eq_q, nz_q);
  printf("a_p=%d\nb_q=%d\n",a_p,b_q);

  //calcul de a_q et b_p
  mpz_t A_q,B_p,rop1,rop2;
  mpz_inits(A_q,B_p,rop1,rop2);
  //a_q=(a_p)^(-1).N mod NS_P
  mpz_set_ui(A_q, a_p);
  mpz_set_ui(rop1, NS_p);
  mpz_invert(A_q, A_q, rop1);
  mpz_mul(A_q,A_q,N);
  mpz_mod(A_q,A_q,rop1);
  gmp_printf("A_q=%Zd \n",A_q);
  printf("\n");
  //b_p=(b_q)^(-1).N mod NS_q
  mpz_set_ui(B_p, b_q);
  mpz_set_ui(rop2, NS_q);
  mpz_invert(B_p, B_p, rop2);
  mpz_mul(B_p,B_p,N);
  mpz_mod(B_p,B_p,rop2);
  gmp_printf("B_p=%Zd \n",B_p);
  printf("\n");

  int a_q,b_p,c_p,c_q;
  a_q=mpz_get_ui(A_q);
  b_p=mpz_get_ui(B_p);
  //maintenant on calcule c_p = p (mod s), c_q = q (mod s) with s := lcm(sp,sq)
  // Dans notre cas on a renommé s_p=NS_p et s_q=NS_q
  int mod[1];
  int val_p[2], val_q[2];

  mod[1]=LCM(NS_p,NS_q);

  val_p[1]=a_p;
  val_p[2]=b_p;

  val_q[1]=a_q;
  val_q[2]=b_q; 

  c_p=CRT(val_p,mod,2);
  c_q=CRT(val_q,mod,2);
  printf("c_p=%d \n",c_p);
  printf("c_q=%d \n",c_q);

  //On peut maintenant passer à l'algorithme LLL avec c_p et c_q
}
