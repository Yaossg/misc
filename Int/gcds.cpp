// fancy versions of gcd

int gcd_xor(int a, int b) {
	while(a%=b^=a^=b);
	return b;
}

int gcd_sub(int a, int b) {
	while(b%=a-=b=(a+=b)-b);
	return a;
}