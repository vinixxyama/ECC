/*
	vinicius yamamoto
	Francisco Pegorel G dias
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Coord{
	long long int x;
	long long int y;
} coord;

const coord O = { .x = 0, .y = 0 };

long long int extendedEuclidean(long long int a, long long int b, long long int *x, long long int *y) {
	long long int tempx = 0;
	long long int tempy = 0;
	
	// Base Case
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }
 
    long long int gcd = extendedEuclidean(b%a, a, &tempx, &tempy);
 

    *x = tempy - (b / a) * tempx;
    *y = tempx;
 
    return gcd;
}

//(Yg - Yq)/((Xg - Xq) mod p)
long long int lambda1(coord G, coord Q, long long int p) {
	long long int dividendo, divisor;
	long long int inverso_divisor;
	long long int temp;
	long long int ret;
	
	dividendo = (G.y - Q.y) % p;
	if(dividendo < 0)
		dividendo += p;
		

	divisor = (G.x - Q.x) % p;
	if(divisor < 0)
		divisor += p;

	extendedEuclidean(divisor, p, &inverso_divisor, &temp);

	if(inverso_divisor < 0)
		inverso_divisor += p;
		
	ret = (dividendo * inverso_divisor) % p;
	if(ret < 0)
		ret += p;

	return ret;
}

//(3Xq^2 + a) / (2Yq) mod p
long long int lambda2(coord G, coord Q, long long int a, long long int p) {
	long long int dividendo, divisor;
	long long int inverso_divisor;
	long long int temp;
	long long int ret;
	
	dividendo = (3 * (int)pow(G.x, 2) + a) % p;
	if(dividendo < 0)
		dividendo += p;
		
	divisor = (2 * G.y) % p;
	if(divisor < 0)
		divisor += p;
	
	extendedEuclidean(divisor, p, &inverso_divisor, &temp);

	if(inverso_divisor < 0)
		inverso_divisor += p;
		
	ret = (dividendo * inverso_divisor) % p;
	if(ret < 0)
		ret += p;
	
	return ret;
}

//This function implementes multiplication of a point with escalar
void ecc(long long int n, long long int a, long long int p, coord G) {
	coord R;
	coord Q;
	long long int lambda;
	
	int cont;
	
	Q.x = G.x;
	Q.y = G.y;

	for (cont = 1; cont < n; cont++) {
		//If point Q or G situated in infinite, in this case
		//doesn't matter if Q or G are situated in infinte
		//because they are equals
		if(G.x == 0 && G.y == 0) {
			R.x = Q.x;
			R.y = Q.y;
		}
		else if (Q.x == 0 && Q.y == 0) {
			R.x = G.x;
			R.y = G.y;
		}
		//Q = -G or G =-Q
		else {
			int inverseG = (p - G.y) % p >= 0 ? (p - G.y) % p : ((p - G.y) % p) + p;
			int inverseQ = (p - Q.y) % p >= 0 ? (p - Q.y) % p : ((p - Q.y) % p) + p;
			
			if ((Q.x == G.x && Q.y == inverseG) || (G.x == Q.x && G.y == inverseQ)) { 
				R = O;
			}
			else {
				//Q != +-G and Q,G != O
				if (((Q.x != G.x && Q.y != G.y) || (Q.x != G.x && Q.x != inverseG)) && (Q.x != 0 && Q.y != 0) && (G.x != 0 && G.y != 0)) {
					lambda = lambda1(G, Q, p);
				}
				//Q = G and Q,G != O and Yq != 0
				else if ((Q.x == G.x && Q.y == G.y) && (Q.x != 0 && Q.y != 0) && (G.x != 0 && G.y != 0) && Q.y != 0) {
					lambda = lambda2(G, Q, a, p);
				}
				
				
				R.x = (int)( pow(lambda,2) - Q.x - G.x ) % p;
				R.y = (int)( lambda * (Q.x - R.x) - Q.y ) % p;
				
				if(R.x < 0)
					R.x += p;
				if(R.y < 0)
					R.y += p;
				
			}
		}
		Q.x = R.x;
		Q.y = R.y;
	}

	printf("%lld %lld\n", Q.x, Q.y);
}


// TODO
int main(){
	long long int n; 		// Multiplicative scalar
	long long int a; 		// Curve parameter
	long long int p; 		// Prime number
	coord G; 				// Coordinates of G

	scanf("%lld", &n);

	while(n!=0) {
		scanf("%lld %lld %lld %lld", &a, &p, &G.x, &G.y);

		ecc(n, a, p, G);

		scanf("%lld", &n);
	}

	return 0;
}
