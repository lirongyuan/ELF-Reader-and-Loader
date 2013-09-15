
int factorial(n)
  int n;
{
  int i,fact;

  fact=1;
  for (i=1;i<=n;i++)
    fact= fact * i;

  return fact;
}


main()
{
  int n;

  printf("Factorial of n:\n");
  for (;;)
  {
    printf("Value of n? (0=exit)");
    scanf("%d",&n);
    if (n==0) goto ex;
    printf("Factorial of %d is %d\n",n,factorial(n));
  }
  ex: ;
}

