
#include <stdio.h>

#define MAXELEMENTS 50

int a[MAXELEMENTS];

quicksort(m,n)
  int m,n;
{
  int i,j;
  int v,x;

  if (n<=m) return;

  i=m-1; j=n; v=a[n];
  while (1) {
    do i=i+1; while (a[i]<v);
    do j=j-1; while (a[j]>v);
    if (i>=j) break;
    x=a[i]; a[i]=a[j]; a[j]=x;
  }
  x=a[i]; a[i]=a[n]; a[n]=x;
  quicksort(m,j); quicksort(i+1,n);
}

main()
{
  int i;
  char s[10];

  a[0]=8; a[1]=3; a[2]=7; a[3]=9; a[4]=1; a[5]=5; a[6]=2;

  printf("Before sorting:\n");
  for (i=0;i<7;i++)
    printf("a[%d]=%d\n",i,a[i]);
  
  quicksort(0,6);

  printf("\nAfter sorting:\n");
  for (i=0;i<7;i++)
    printf("a[%d]=%d\n",i,a[i]);

  printf("\nPress <Return> to continue...");
  gets(s);
}
