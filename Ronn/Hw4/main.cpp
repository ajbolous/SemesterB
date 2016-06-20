/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: bolous
 *
 * Created on June 17, 2016, 9:39 PM
 */

#include <stdlib.h>
#include <stdio.h>

class DiGraph {
    double infinity = 9999999;
    double** graph;
    int N;

public:

    DiGraph() {
        N=0;
    }

    DiGraph(int n, double infinity) {
        graph = (double**)malloc(n * sizeof (double*));
        N=n;
        this->infinity = infinity;
        for (int i = 0; i < n; i++){
            graph[i] = (double*)malloc(n * sizeof (double));
            for(int j =0;j<n;i++)
                graph[i][j] = infinity;
        }
    }
    
    int getN() {
        return N;
    }

    int getInfinity() {
        return infinity;
    }

    void addArc(int i,int j, double w){
        graph[i][j] = w;
    }
    
    double getArcDistance(int i,int j){
        return graph[i][j];
    }

};

class ADiGraph:DiGraph{
    
};

print(DiGraph ad)
{
  int i, j, n;
  double infinity;
  char str[80];

  n = ad.getN();
  infinity = ad.getInfinity();
  cout << "n = " << n << endl;
  for(i=0; i < n; i++)
  {
    for(j=0; j < n; j++)
      if ( ad.getArcDistance(i,j) == infinity)
        cout << "  ------  ";
      else
      {
        sprintf(str,"%8.2lf", ad.getArcDistance(i,j));        
        cout <<  str;
      } // else
      cout << endl;        
  } // for  

} // print 

int main()
{

 DiGraph d1(8, 10000.0);
 int i;


 d1.addArc(0,1,11.1);
 d1.addArc(0,2,22.2);
 d1.addArc(0,3,0.33);
 d1.addArc(0,5,5.5);
 d1.addArc(1,2,1.2);
 d1.addArc(1,3,11.3);
 d1.addArc(1,0,0.1);
 d1.addArc(2,4,24.4);
 d1.addArc(2,5,2.5);
 d1.addArc(2,6,26.6);
 d1.addArc(2,7,2.7);
 d1.addArc(2,1,2.1);
 d1.addArc(3,4,3.4);
 d1.addArc(3,0,3.0);
 d1.addArc(3,1,3.1);
 d1.addArc(4,6,4.5);
 d1.addArc(4,7,47.7);
 d1.addArc(4,2,4.2);
 d1.addArc(5,6,5.6);
 d1.addArc(5,7,57.7);
 d1.addArc(5,3,5.3);
 d1.addArc(5,1,5.1);
 d1.addArc(6,7,7.6);
 d1.addArc(6,3,6.3);
 d1.addArc(7,4,7.4);


 DiGraph d2, d3;

 d2 = d3 = d1;

 d1.addArc(7,2,7.2);



 cout << " d1 \n"; 
 print(d1);

 cout << " d2 \n"; 
 print(d2);


 ADiGraph ad1(8, 10000.0);
 double distances[8];


 ad1.addArc(0,1,11.1);
 ad1.addArc(0,2,22.2);
 ad1.addArc(0,3,0.33);
 ad1.addArc(0,5,5.5);
 ad1.addArc(1,2,1.2);
 ad1.addArc(1,3,11.3);
 ad1.addArc(2,4,24.4);
 ad1.addArc(2,5,2.5);
 ad1.addArc(2,6,26.6);
 ad1.addArc(2,7,2.7);
 ad1.addArc(3,4,3.4);
 ad1.addArc(3,6,36.6);
 ad1.addArc(4,6,4.5);
 ad1.addArc(4,7,47.7);
 ad1.addArc(5,6,5.6);
 ad1.addArc(5,7,57.7);
 ad1.addArc(6,7,7.6);


 ADiGraph ad2, ad3;

 ad2 = ad3 = ad1;

 cout << " ad1 \n"; 
 print(ad2);
 ad3.shortest_distances(distances);

 cout << "Distances\n";
 for(i=0; i < 8; i++)
   cout << "  " <<distances[i];

 cout << endl;
 
} // main