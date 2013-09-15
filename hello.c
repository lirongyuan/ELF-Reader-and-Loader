int a = 1234;
int b;

int ch[ 3000 ];

extern int etext, edata, end;

main(argc, argv)
     int argc;
     char **argv;
{
	int i;
	printf("\n-------------------------------------\n");
	printf("Welcome to CS354 Operating Systems\n\n");

	printf("argc=%d\n", argc);

	for (i=0; i<argc; i++)
	    printf("argc[%d] = %s\n", i, argv[i]);

	printf("\n");

	printf("etext=0x%x\n", &etext);
	printf("edata=0x%x\n", &edata);
	printf("end=0x%x\n", &end);

	printf("a = %d\n", a);
	printf("b = %d\n", b);
	printf("&b = 0x%x\n", &b);
	printf("a+b =%d\n", a+b);
}
