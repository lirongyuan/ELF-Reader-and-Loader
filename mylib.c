
// Library that all the executable files loaded by our loader
// will link in.

typedef int (*FUNCPTR)();

// Pointer to the table of functions used by our executables.
// It will be passed as an argument in mystart
FUNCPTR * FUNC_TABLE;

// This is the entry point called by the loader "load_and_run"
// In the Makefile, in the line
//     "ld -dn -e mystart -M mapfile hello.o mylib.o -o hello"
// we set the entry point to by mystart
//
void mystart(FUNCPTR * func_table, int argc, char ** argv) {
  FUNC_TABLE = func_table;
  main(argc, argv);
}

int printf(char * s, int a1, int a2, int a3, int a4, int a5) {
  return (FUNC_TABLE[0])(s, a1, a2, a3, a4, a5);
}

// Add here other functions you need besides printf
char *gets(char *s){
	return (char *)(FUNC_TABLE[1])(s);
}

char *strcpy(char *s1,char *s2){
	return (char *)(FUNC_TABLE[2])(s1,s2);
}

int strlen(const char *s){
	return (FUNC_TABLE[3])(s);
}

int scanf(char * s, int a1, int a2, int a3, int a4, int a5){
	return (FUNC_TABLE[4])(s,a1,a2,a3,a4,a5);
}
