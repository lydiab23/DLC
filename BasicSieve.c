#include<stdio.h>
#include"gmp.h"
#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/resource.h>

unsigned long int cputime()
{
	struct rusage rus;
	getrusage (0, &rus);
	return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}

//fonction qui genere un nombre de taille exactement k bits
int nombre_k_bits(mpz_t n,int b, gmp_randstate_t mon_generateur){
	mpz_t z_n;
	mpz_inits(z_n,NULL);
	mpz_ui_pow_ui(z_n,2,b-1);
	mpz_urandomb(n, mon_generateur, b-1);
	mpz_add(n,n,z_n);
	return 1;

}


int crible_simple(int b,int k, int t, mpz_t n, gmp_randstate_t mon_generateur, int primes[])
{
	//on genere un nombre de k bits et vérifie qu'il est impair
    do
    {
        nombre_k_bits(n, b, mon_generateur);
    } while (mpz_divisible_ui_p(n, 2) != 0);
	
    int isPrime = 1;
    do
    {
        isPrime = 1;
        int i;
        for (i = 1; i < k; i++)
        {
        	//tests de divisions de n par les k premiers nombres premiers
			gmp_printf("n=%Zd \n",n);
			printf("prime[%d]=%d \n",i,primes[i]);
			printf("D_%d \n",i);
            if (mpz_divisible_ui_p(n, primes[i]) != 0)
            {
                isPrime = 0;
                mpz_add_ui(n, n, 2);
                printf("G \n"); //G = q=q+2 et retour a la ligne 2
                break;
            }
            printf("I \n");//incrémentation
        }

        if (isPrime)
        {
        	gmp_printf("T[%Zd} \n",n);
			if(mpz_probab_prime_p(n,t)==0){
				mpz_add_ui(n,n,2);
			}
			else{
				isPrime=1;
			}
        }
    } while (!isPrime);

    return isPrime;
}

int main(int argc, char * argv[])
{
	int r,b,k,t,u;
	int seed,i,j,rep;
	mpz_t n,p,q;
	mpz_t N;
	mpz_inits(n,N,p,q,NULL);
	seed = time(NULL);
	//ce programme genere 2 nombres : p et q 
	r=2;
	//de taille exactement 512 bits chacun
	b=512;
	//non divisible par les k premiers nombres premiers
	//on aura trois cas à étudier (T1, k = 54), (T2, k = 60), (T3, k = 70)
	k=atoi(argv[1]);
	//avant d’etre testé par la méthode de Miller-Rabin avec t bases
	t=atoi(argv[2]);
	

	int primes[k];

	for(i=0;i<k;i++){
		mpz_nextprime(p,q);
		u=mpz_get_ui(p);
		primes[i]=u;
		mpz_set(q,p);
	}

	gmp_randstate_t mon_generateur; //Declaration du générateur
	gmp_randinit_default(mon_generateur); //Initialisation du génératuer
	gmp_randseed_ui(mon_generateur, seed);

	
	mpz_set_ui(N,1);
	long int starttime = cputime();
	for(j=0;j<r;j++){
		rep=crible_simple(b,k,t,n,mon_generateur,primes);
		if(rep==1){
			gmp_printf("n%d = %Zd \n",j,n);
			printf("\n");
			mpz_mul(N,N,n);
		}
	}
	gmp_printf("N = %Zd \n",N);
	long int endtime = cputime();
	printf("temps d'execution : %ld milli-secondes \n", endtime - starttime);

	gmp_randclear(mon_generateur);
    mpz_clears(n,N,p,q, NULL);

}
