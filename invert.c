
invert1(s)
  char *s;
{
  int i,l,t;
  
  l=strlen(s);
  for (i=0;i<l/2;i++)
  {
    t=s[i];
    s[i]=s[l-i-1]; 
    s[l-i-1]=t;
  }
}

invert2(s)
  char *s;
{
  int t;
  char *ps1,*ps2;

  ps1=ps2=s;
  while (*ps2++); 

  for (ps2--;ps1<ps2;t=*ps1,*ps1=*ps2,*ps2--=*ps1++);
}

char s1[50],s2[50];
main()
{
  printf("Give a sentence    : ");
  gets(s1); 
  strcpy(s2,s1);
  invert1(s1);
  printf("Inverted sentence 1: %s\n",s1);
  invert2(s2);
  printf("Inverted sentence 2: %s\n",s1);
}

