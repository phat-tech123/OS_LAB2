#include <stdio.h>
#include <stdlib.h>


unsigned long long sum(int n){
	unsigned long long s = 0;
	for(int i=1; i<=n; i++){
		s += i;
	}
	return s;
}

int main(int argc, char* argv[]){
	int n = atoi(argv[1]);
	if(n < 0){
		return 1;
	}

	printf("%llu\n", sum(n));
	return 0;
}
