//
//	Straight C ports of the 80286 assembler routines
//



extern void block_and(int length,unsigned char *source,unsigned char *dest) {
	for(int ctr=0;ctr<length;ctr++) {
		*dest = (*dest) & (*source);
		dest++;
		source++;
	}
}


extern int collide(int length,unsigned char *source) {
	for(int ctr=0;ctr<length;ctr++) {
		if(*source) {
//			printf("collision at %d\n", ctr);
			return *source;
		}
		source++;
	}

	return 0;

}


extern void scroll(unsigned char *screen,unsigned char *fontaddr,int fontoffs) {

	// TODO

}

extern void zmov(unsigned char *in,unsigned char *out) {
	// TODO
}
