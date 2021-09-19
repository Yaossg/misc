int gcd1(int a, int b) {
	while(a%=b^=a^=b);
	return b;
}

int gcd2(int a, int b) {
	while(b%=a-=b=(a+=b)-b);
	return a;
}