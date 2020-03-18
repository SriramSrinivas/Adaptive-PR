//
// Created by sriramsrinivas@unomaha.edu on 1/9/20.
//

#ifndef PR_PR_H
#define PR_PR_H
#include <limits>
#include <cmath>
typedef std::numeric_limits< double > dbl;
#include "SP_structure.hpp"
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <queue>


/*
 *
 * d= (Pr-V)/(1/n-V)
 * where V= {PR[v]/p(V)
 */


void compute_dValue(PR_Network *X, int *p)
{
    cout <<"I am here";

 #pragma omp parallel for schedule(dynamic) num_threads(*p)
    for(int i=0;i<X->size();i++) {


	if(X->at(i).Out_Neigh.size()==0) {X->at(i).dValue=X->at(i).page_rank*X->size();}
	else
	{
	    //no checking for -1 since this is dvalue before update
     double sum=0.00;
	for(int j=0;j<X->at(i).Out_Neigh.size();j++)
          {		  
	    int neigh=X->at(i).Out_Neigh[j].first;
	    //printf("%d--%d\n",i,neigh);
	    sum=sum+(X->at(neigh).page_rank/X->at(neigh).Out_size);
	  }

	double numerator=sum-X->at(i).page_rank;
	double denominator=sum-1.00/X->size();

	X->at(i).dValue=numerator/denominator;
	} //end of else

	//check for negative
	if(X->at(i).dValue<0) {X->at(i).dValue=-X->at(i).dValue;}
    
	}//end of for
}
/***End of Function ****/



void  printPageRank(PR_Network *X)
{
    for (int i=0;i< X->size();i++) {
        cout << i << ":" << X->at(i).page_rank <<"\n";
    }
}
/***End of Function ****/


/*
     * PR(v)= (d(given in preprocessing file )/n(number of nodes in the network) + (1-d)*[PR(v1)/O(V1)+PR(v2)/)(V2)])

     *   PR(v1)/O(v1)+PR(v2)/ O(v2) == Value which is stored in the meta information structure
     *    V1 and v2 have edges which reach V
     *
     *    not now
     *
     */

void updatePageRank(PR_Network *X, int *p, int *maxL)
{
    //cout<<*maxL;
  int counter=0;
  bool change=true;  
  while(change || counter<*maxL) {
 
	  change=false;
       #pragma omp parallel for schedule(dynamic) num_threads(*p)
        for (int i = 0; i < X->size(); i++) {

              // add condition for level
            if (X->at(i).msg>=X->at(i).reach && X->at(i).updateFlag==true && X->at(i).Out_Neigh.size()>0)
            //if (X->at(i).level==counter && X->at(i).updateFlag==true && X->at(i).Out_Neigh.size()>0)
	    {

	       //Compute new page rank
		 double sum=0.00;
		 for(int j=0;j<X->at(i).Out_Neigh.size();j++)
                  {
	           if(X->at(i).Out_Neigh[j].second!=-1)		  
	            {int neigh=X->at(i).Out_Neigh[j].first;
	             sum=sum+(X->at(neigh).page_rank/X->at(neigh).Out_size);}
		  }//end of for
             
		 double new_pr=X->at(i).dValue/(X->size())+(1-X->at(i).dValue)*sum;

		 for(int j=0;j<X->at(i).In_Neigh.size();j++)
		 {
		    if(X->at(i).In_Neigh[j].second!=-1)
		    {
		      int neigh=X->at(i).In_Neigh[j].first;
                      #pragma omp atomic update 
		      X->at(neigh).msg++;
		      
	    if(abs(new_pr-X->at(i).page_rank) >0.0000000001)
		     {X->at(neigh).updateFlag=true;
		     change=true;
		     X->at(i).page_rank=new_pr;
		     }
		    
		    
		    }
		 
            }//end of for		     

            }//end of if
        }//end of for

        counter++;
    }//end of while
    cout<<"counterStatus"<<counter<<"\n";
}
/***End of Function ***/



//Finding the level of each node
void updateLevel(PR_Network *X, int *p, int *maxL)
{
#pragma omp parallel for schedule(dynamic) num_threads(*p)
   for(int i=0;i<X->size();i++)
   {  
	   
	   if(X->at(i).level==0)
	   {
	    queue<int> myqueue;
            myqueue.push(i);

	    vector<bool> visited;
	    visited.resize(X->size(),false);
    while(!myqueue.empty())
    {

         int node=myqueue.front();
	 myqueue.pop();
	 visited[node]=true;

	 for(int j=0;j<X->at(node).In_Neigh.size();j++)
        {

		int m=X->at(node).In_Neigh[j].first;
                int value=X->at(node).In_Neigh[j].second;
            
		if(value!=-1) {
                    int parentLevel=X->at(node).level+1;
                    int currentNodeLevel=X->at(m).level;
                   
		    //update node level if needed
		    if(currentNodeLevel<parentLevel) {
                        currentNodeLevel=parentLevel;
                         
			#pragma omp atomic write
                        X->at(m).level=currentNodeLevel;
			if(visited[m]==false)
			{

			myqueue.push(m);}
		    }

		    if(currentNodeLevel >*maxL)
		    {*maxL=currentNodeLevel;}

            }//end of if

        }//end of for

    }//end of while


   }//end of if
   }//end of for

}
/**** End of Function ***/


void updateReach(PR_Network *X, int *p, int *maxL)
{
#pragma omp parallel for schedule(dynamic) num_threads(*p)
   for(int i=0;i<X->size();i++)
   {  
	   
	   if(X->at(i).updateFlag==true)
	   {
	    queue<int> myqueue;
            myqueue.push(i);

    while(!myqueue.empty())
    {

         int node=myqueue.front();
	 myqueue.pop();
	 X->at(node).visited=true;

	 for(int j=0;j<X->at(node).In_Neigh.size();j++)
        {

		int m=X->at(node).In_Neigh[j].first;
                int value=X->at(node).In_Neigh[j].second;
            
		if(value!=-1) {

                       #pragma omp atomic update
                        X->at(m).reach++; 

			if(X->at(m).visited==false && X->at(m).updateFlag==false)
			myqueue.push(m);

            }//end of if

        }//end of for

    }//end of while


   }//end of if
   }//end of for

}
/**** End of Function ***/
#endif //PR_PR_H
