#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

// a function to calculate the size of the file
//this function retun the number of characters +1 inside the file
int file_size(FILE*fp)
{
  int size;

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  return size;
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
//Chinese remainder theorem
//cette fonction marchem et il faut que n=taille(vect1), ie le nombre des elements dans vect1
//vect1 represent tout les premiers de vecteurs premiers, vect2 represent the different values of x mod pi
int CRT(int vect1[], int vect2[], int n)
 {

int N=1,x,y,a_x=0;

for(int i=0;i<n;i++)
{
 N=N*vect1[i];
}
for(int i=0;i<n;i++)
{

a_x=a_x+(vect2[i]*(N/vect1[i])*extended_gcd(N/vect1[i], vect1[i], &x, &y));

}
return a_x;
}
//calculating the coefficient of correlation
//this works , N=size of arr ie number of elements in arr
double SUM( double arr[], int N)
 {
  double sum=0;
  for(int i=0;i<N;i++)
   {
    sum=sum+arr[i];
   }
  return sum;
 }
//enlever le ;, et cette fonction n'est pas necessaire, on peut l'omiter
//cette fonction est correcte, et toujours N est le nombre des elements dans arr
double AVG( double arr[], int N)
 {
  return SUM(arr,N)/(double)N;//
 }
//this function works perfectly
void OP(double arr[], double x, int N)
 {
  for(int i=0;i<N;i++)
   {
    printf("arr[%d]=%f\n",i,arr[i]);
    arr[i]=arr[i]-x;
   printf("arr[%d]=%f\n",i,arr[i]);
   }
  }
//this function works
void MUL( double arr1[], double arr2[], double arr3[],int N)
 {
  for(int i=0;i<N;i++)
   {
    arr3[i]=arr1[i]*arr2[i];
    //printf("%f*%f=arr3[%d](%f)\n",arr1[i],arr2[i],i,arr3[i]);
   }
  }
// i am too lazy to test this one, so i will leave it to you girls
double Coeffofcor( double arr1[], double arr2[], int N)
{
 double avg_a, avg_b, cov, var1,var2, arr3[N],rho;
 avg_a=AVG(arr1,N),avg_b=AVG(arr2,N);
 OP(arr1,avg_a,N), OP(arr2,avg_b,N);
 MUL(arr1,arr2,arr3,N);
 cov=AVG(arr3,N);
 MUL(arr1,arr1,arr3,N);
 //printf("SUM(arr3,N)=%f\n",SUM(arr3,N));
 var1=(1/(double)N)*(SUM(arr3,N));
 //printf("var1=%f\n",var1);
 MUL(arr2,arr2,arr3,N);
 var2=(1/(double)N)*(SUM(arr3,N));
// printf("var2=%f\n",var2);
 rho=(cov/sqrt(var1*var2));
 return rho;
 }


void ChooseVect(double vect[], int n, int index_j, double temp[], int k)
 {
  int jump=index_j;
  for(int i=0; i<n;i++)
   {
    temp[i]=vect[jump];
    jump+=k;
  }
 } 
//this function works perfectly, it gives back the index of the element with the highest value 
int ChooseMax(double vect[],int threshhold)
 {
  double max=-1000;
  int index=0;
  for(int i=0;i<threshhold;i++)
   {
    if(max<vect[i])
     {
      max=vect[i];
      index=i;
     }
   }
  return index;
 }
int attack(int primes[], int k, double vect[],int n)
 {
  double m[n];
  int candidate[k];
  unsigned long int  box;
  double  temp[n+1];
  double rho[primes[k-1]-1];
  mpz_t bla,z_box;
 mpz_inits(bla,z_box,NULL);
  for(int j=1;j<k;j++)
   {
    
   for(int h=1;h<primes[j];h++)
     {
     
      for(int i=0;i<n;i++)
       {
        box=(h-(n-i-1)*2 );
        mpz_set_ui(z_box,box);
        m[i]=(mpz_hamdist (z_box, bla) )%primes[j];
        
       
        }
        ChooseVect(vect,n, j, temp, k);
        printf("problem in coefffor?\n");
        rho[h]=Coeffofcor( m, temp,n);
        //printf("rho[%d]=%f\n",h,rho[h]);
     }
      //printf("problem in choose max\n");
      //candidate[j]=ChooseMax(rho,primes[j]);
      
      }

   mpz_clears(bla,z_box,NULL);
   //return  CRT(primes, candidate,k);
 }
int main()
 {
 FILE *f1, *f2;
 int k=54,u;//k can be changed
  int primes[k];

  printf("test1\n");
  f1=fopen("traces_p","r");

  if(!f1)
    printf("Erreur d'ouverture du fichier p\n");


  f2=fopen("traces_q","r");

  if(!f2)
    printf("Erreur d'ouverture du fichier q\n");
  printf("test2\n");
  int size_p=file_size(f1), size_q=file_size(f2);
  int n_p=((size_p/2)-1)/(k-1),n_q=((size_q/2)-1)/(k-1);
  printf("size_p=%d, k=%d, n_p=%d\n",size_p,k,n_p);
  double vect_p[size_p/2],vect_q[size_q/2];

  for(int i=0;i<size_p/2;i++)
   {
    fscanf(f1,"%le",&vect_p[i]);
    fscanf(f2,"%le",&vect_q[i]);
    //printf("vect_p[%d]=%d\n",i,vect_p[i]);//this is just for testing 
   }


  printf("test3\n");
  fclose(f1);
  fclose(f2);
  printf("test4\n");
  mpz_t p,q;
  mpz_inits(p,q,NULL);
 
//generating the primes
for(int i=0;i<k;i++){
		mpz_nextprime(p,q);
		u=mpz_get_ui(p);
		primes[i]=u;
		mpz_set(q,p);
	}

  mpz_clears(p,q,NULL);
printf("test5\n");
int p2=attack(primes, k, vect_q,n_p);
printf("test6\n");
int q2=attack(primes, k, vect_p,n_q);
}
  
