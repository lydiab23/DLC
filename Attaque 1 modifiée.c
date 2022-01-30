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
//it also adds "2" at the end of the vector
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

//a functing that deletes elements that are doubles in vect[]
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

//Chinese residual theorem
//The first CRT takes vector of int, the second CRT takes vectors of mpz_t
void CRT (mpz_t a_x, int S_xx[], int eq_x[], int size )
{
  //here size is the cardinal of S_x
  mpz_t N,L,x,y,g,rop1,rop2;
  mpz_inits(N,L,x,y,g,rop1,rop2,NULL);
  mpz_set_ui(N,1);

  //N=product of all elements in S_x
  for(int i=0;i<size-1;i++)
  {
    gmp_printf("calculating(%d)- N=%Zd\n",i,N);
    mpz_mul_ui(N,N,S_xx[i]);
  }
  //calculating a_x
  //la boucle va jusqu'a size-1 car on a ajouté "2" à S_p et à S_q mais on n'a aucune congruence pour 2
  for(int i=0;i<size-1;i++)
  {
    printf("eq_x[%d]=%d\n",i,eq_x[i]);
    mpz_cdiv_q_ui(L,N,S_xx[i]);
    gmp_printf("N=%Zd divisé par S_xx[%d]=%d donne L=%Zd \n ",N,i,S_xx[i],L);
    mpz_set_ui(rop1,S_xx[i]);
    mpz_gcdext(g,x,y,L,rop1);
    //on s'intéresse au x tel que L.x+y.S_xx[i]=g
    gmp_printf("extended_gcd(%Zd,%d,x,y)=%Zd\n",L,S_xx[i],x);
    mpz_mul(rop2,L,x);
    mpz_mul_ui(rop2,rop2,eq_x[i]);
    mpz_add(a_x,a_x,rop2);
    mpz_mod(a_x,a_x,N);
    //a_x=a_x+(eq_x[i]*(N/S_xx[i])*extended_gcd(N/S_xx[i], S_xx[i], &x, &y));
    printf("testing the a_x\n");
    gmp_printf("a_x=%Zd\n",a_x);
  }
}

void CRT2(mpz_t a_x, mpz_t S_xx[], mpz_t eq_x[], int size )
{
  //here size is the cardinal of S_x
  mpz_t N,L,x,y,g,rop1,rop2;
  mpz_inits(N,L,x,y,g,rop1,rop2,NULL);
  mpz_set_ui(N,1);

  //N=product of all elements in S_x
  for(int i=0;i<size;i++)
  {
    gmp_printf("calculating(%d)- N=%Zd\n",i,N);
    mpz_mul(N,N,S_xx[i]);
  }
  //calculating a_x
  for(int i=0;i<size;i++)
  {
    //printf("eq_x[%d]=%Zd \n",i,eq_x[i]);
    mpz_cdiv_q(L,N,S_xx[i]);
    gmp_printf("N=%Zd divisé par S_xx[%d]=%Zd donne L=%Zd \n ",N,i,S_xx[i],L);
    mpz_gcdext(g,x,y,L,S_xx[i]);
    gmp_printf("extended_gcd(%Zd,%Zd,x,y)=%Zd\n",L,S_xx[i],x);
    mpz_mul(rop2,L,x);
    mpz_mul(rop2,rop2,eq_x[i]);
    mpz_add(a_x,a_x,rop2);
    mpz_mod(a_x,a_x,N);
    //a_x=a_x+(eq_x[i]*(N/S_xx[i])*extended_gcd(N/S_xx[i], S_xx[i], &x, &y));
    printf("testing the a_x\n");
    gmp_printf("a_x=%Zd\n",a_x);
  }
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
  mpz_inits(N,NULL);

  //Lecture des courbes de consommations
  fscanf(f1,"%s",vect_p);
  fscanf(f2,"%s",vect_q);

  //lecture de N a partir du fichier, pour rappel N=p.q et est une donnée publique à laquelle l'attaquant a accès
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

  mpz_t NS_p,NS_q;
  mpz_inits(NS_p,NS_q,NULL);
  mpz_set_ui(NS_p,1);
  mpz_set_ui(NS_q,1);

  nz_p=create_vector_for_crt(S_p2,nz_p);
  for(i=0;i<nz_p;i++){
    printf("s_p2[%d] = %d \n",i,S_p2[i]);
    mpz_mul_ui(NS_p,NS_p,S_p2[i]);
  }

  nz_q=create_vector_for_crt(S_q2,nz_q);
  for(i=0;i<nz_q;i++){
    printf("s_q2[%d] = %d \n",i,S_q2[i]);
    //NS_q=NS_q*S_q2[i];
    mpz_mul_ui(NS_q,NS_q,S_q2[i]);
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
  mpz_t a_p,b_q,a_q,b_p,c_p,c_q,lcm;
  mpz_inits(a_p,b_q,a_q,b_p,c_p,c_q,lcm,NULL);
  printf("testing CRT\n");
  CRT(a_p,S_p2, eq_p, nz_p);
  CRT(b_q,S_q2, eq_q, nz_q);
  gmp_printf("a_p=%Zd\nb_q=%Zd\n",a_p,b_q);
  printf("\n");


  //calcul de a_q et b_p
  printf("Calcul de a_q et b_p\n");
  //a_q=N/a_p mod NS_P
  mpz_cdiv_q(a_q,N,a_p);
  mpz_mod(a_q,a_q,NS_p);
  gmp_printf("a_q=%Zd \n",a_q);
  printf("\n");
  //b_p=N/b_q mod NS_q
  mpz_cdiv_q(b_p,N,b_q);
  mpz_mod(b_p,b_p,NS_q);
  gmp_printf("b_p=%Zd \n",b_p);
  printf("\n");

  //maintenant on calcule c_p = p (mod s), c_q = q (mod s) with s := lcm(sp,sq)
  // Dans notre cas on a renommé s_p=NS_p et s_q=NS_q
  mpz_t mod[1];
  mpz_t val_p[2], val_q[2];
  mpz_inits(mod[0],NULL);
  for(i=0;i<2;i++){
    mpz_inits(val_p[i],val_q[i],NULL);
  }

  mpz_lcm(mod[0],NS_p,NS_q);
  gmp_printf("lcm de NS_p=%Zd et NS_q=%Zd est lcm=%Zd \n",NS_p,NS_q, mod[0]);

  mpz_set(val_p[0],a_p);
  mpz_set(val_p[1],b_p);
  
  mpz_set(val_q[0],a_q);
  mpz_set(val_q[1],b_q); 

  for(i=0;i<2;i++){
    gmp_printf("val_p[%d]=%Zd \n",i,val_p[i]);
  }

  for(i=0;i<2;i++){
    gmp_printf("val_q[%d]=%Zd \n",i,val_q[i]);
  }

  CRT2(c_p,val_p,mod,2);
  CRT2(c_q,val_q,mod,2);

  gmp_printf("c_p=%Zd \n",c_p);
  gmp_printf("c_q=%Zd \n",c_q);

  //On peut maintenant passer à l'algorithme LLL avec c_p et c_q
}
