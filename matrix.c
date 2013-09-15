
int a[3][3]={1,2,3,4,5,6,7,8,9},
    b[3][3]={{1,2,3},4,5,6,{7,8,9}},
    c[3][3];  

mult(a,b,c)
  int (*a)[3],(*b)[3],(*c)[3];
{
  int i,j,k;
  for (i=0;i<3;i++)
    for (j=0;j<3;j++)
    {
      c[i][j]=0;
      for(k=0;k<3;k++)
        c[i][j]+=a[i][k]*b[k][j];
    }
}

displaymatrix(a)
  int (*a)[3];
{
  int i,j,k;
  for (i=0;i<3;i++)
  {
    for (j=0;j<3;j++)
      printf("%d, ",a[i][j]);
    printf("\n");
  }
}

main()
{
  printf("A=:\n"); displaymatrix(a);
  printf("\nB=:\n"); displaymatrix(b);
  mult(a,b,c);
  printf("\nA X B=:\n"); displaymatrix(c);
}
