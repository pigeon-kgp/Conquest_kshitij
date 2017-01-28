#include<bits/stdc++.h>
using namespace std;
char listName[20]="list.txt";
int print(int a[])
{
  for (int i=0;i<6;i++)
  {
  cout<<a[i]<<" ";
  }
  cout<<endl;
}
int list2path()
{
    fstream files;
    int a[6];
    float d;
    files.open(listName,ios::in);
     if(files.is_open())
     {
      while(!files.eof())
      {
        for(int i=0;i<6;i++)
        {
          files>>d;
          a[i]=d;
        }
        print(a);
        //botMove(a,a+3);
    }
    files.close();
  }
return 0;
}

int main()
{
  list2path();
  return 0;
}
