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
	int a,i;
	mpz_t r0,r1,z_n,rop1,x;
	mpz_inits(r0,r1,z_n,rop1,x,NULL);
	int isPrime,test;

	do{
		nombre_k_bits(n,b,mon_generateur);
		mpz_cdiv_r_ui(r1,n,2);
	}while(mpz_cmp_ui(r1,0)==0);


	do{
		isPrime=0;
		
		do{
			test=0;
			for(i=1;i<k;i++){
				mpz_cdiv_r_ui(r0,n,primes[i]);
				if(mpz_cmp_ui(r0,0)==0){
					mpz_add_ui(n,n,2);
				}
				else{
					test=1;
				}
			}
		}while(test!=1);

		if(mpz_probab_prime_p(n,t)==0){
			mpz_add_ui(n,n,2);
		}
		else{
			isPrime=1;
		}
	}while(isPrime!=1);

	return isPrime;
}

int main(int argc, char * argv[])
{
	int r,b,k,t,u;
	int seed,i,j,rep;
	mpz_t n,p,q;
	mpz_inits(n,p,q,NULL);
	seed = time(NULL);
	b=atoi(argv[1]);
	k=atoi(argv[2]);
	t=atoi(argv[3]);
	r=atoi(argv[4]);

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

	long int starttime = cputime();
	for(j=0;j<r;j++){
		rep=crible_simple(b,k,t,n,mon_generateur,primes);
		if(rep==1){
			gmp_printf("n%d = %Zd \n",j,n);
		}
	}
	long int endtime = cputime();
	printf("temps d'execution : %ld milli-secondes \n", endtime - starttime);

	gmp_randclear(mon_generateur);
    mpz_clears(n,p,q, NULL);

}
