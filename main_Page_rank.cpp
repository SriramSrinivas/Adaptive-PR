//
// Created by sriramsrinivas@unomaha.edu on 1/9/20.
//


#include "SP_structure.hpp"
#include "readin_data.hpp"
//#include "printout_others.hpp"
#include "pr.h"
#include "adaptive.h"
#include <string>
int main(int argc, char *argv[])
{
    clock_t q, q1, q2,t;
    clock_t updateStartTime;
    clock_t updateEndTime;
    if ( argc <7) { cout << "INPUT ERROR:: 7 inputs required. First: filename \n Second: file with PR\n  Third: Set of changed edges \n Fourth: Number of nodes \n Fifth: Number of threads \n Sixth: Number of iterations\n Seventh: Output File name ";}
    //Check to see if file opening succeeded
    ifstream the_file1 ( argv[1] ); if (!the_file1.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
    ifstream the_file2 ( argv[2] ); if (!the_file2.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
    ifstream the_file3 ( argv[3] ); if (!the_file3.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
    int p = atoi(argv[5]);  //total number of threads per core
    int nodes=atoi(argv[4]);//total number of nodes



    /**=============================================**/

    /**=============================================**/
    /*** Reading in Network at Time Step 0 ***/
    /***============================================**/
    //Obtain the list of edges.
    q=clock();
    PR_Network X;
    PR_Vertex PR_v;
    X.resize(nodes,PR_v);
    readin_network(&X,argv[1]);
    cout<< "done"<< "\n";
    readin_PageRank(argv[2],&X);
    q=clock()-q;
    cout << "Total Time for Reading Input"<< ((float)q)/CLOCKS_PER_SEC <<"\n";
    
    
    q=clock();
    readin_changes(argv[3], &X);
    q=clock()-q;
    cout << "Total Time for Reading Changes"<< ((float)q)/CLOCKS_PER_SEC <<"\n";
    /**=============================================**/


int count=0;
for(int i=0;i<X.size();i++)
{ if(X[i].level==0)
	{count++;}
}
//    cout <<"in";

    prepareinputforGalois(&X);
    adaptiveModel(&p, argv[1]);
    // read Galois Output

    readGaloisOutput();
//cout<<count<<":::\n";

// Update all the meta information
int maxL=atoi(argv[6]);
 //Obtain value of probability of reaching page--guve n by dValue
   updateStartTime=clock();

    compute_dValue(&X,&p);

  
    //updateLevel(&X, &p, &maxL);
    updateReach(&X, &p, &maxL);


    //cout <<"max: "<<maxL<<"\n";
    updatePageRank(&X,&p,&maxL);
    updateEndTime=clock()-updateStartTime;
    float updateTime=float(((float)updateEndTime)/CLOCKS_PER_SEC);
    cout <<fixed<< "Total Time for updating Network "<< updateTime <<"\n";






}


