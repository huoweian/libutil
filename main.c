#include "all.h"
#include "types.h"
#include "byteo.h"

s8 tch[124] = "   1 2 3 4 5 6 7 8 8   ";
u8 bch[124];

int main(int argc, char **argv)
{
	s16 len = 0;
	//s16 i,j;
	u8 *p;
	
	memset(tch, 0x00, sizeof(tch));
	memset(bch, 0x00, sizeof(bch));
	
	printf("Please input asc:\n");
	scanf("%s", tch);
	//printf("%s\n", tch);
	//printf("%s\n", atrim(tch));
	//printf("%s\n", ltrim(tch));
	
	printf("%s\n", hexdump(tch, strlen(tch)));
	
/*	
	len = asc2bcdx(tch, strlen(tch), bch, sizeof(bch), ABC_FCH, '9');
	//len = asc2bcd(tch, strlen(tch), bch, sizeof(bch));
	p = bch;
	
	while(*p){
		printf("%3x", *p);
		p++;
	}
	printf("\nlen = %d\n", len);
	
	memset(tch, 0x00, sizeof(tch));
	//len = 0;
	
	len = bcd2ascx(bch, len, tch, sizeof(tch), ABC_FCH, '9');
	//len = bcd2asc(bch, len, tch, sizeof(tch));
	printf("\nasc is: %s\n", tch);
*/	

	return 0;
}