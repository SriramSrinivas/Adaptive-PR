#ifndef READIN_DATA_HPP
#define READIN_DATA_HPP
#include <algorithm>
#include "SP_structure.hpp"
#include <iomanip>
using namespace std;


//Read the edgelist of the network
void readin_networkE(char *myfile, vector<int_int> *allEdges, int *nodes)
{
    //File reading parameters
    FILE *graph_file;
    char line[128];
    int_int edge;
    
    allEdges->clear();
    *nodes=0;
    
    graph_file=fopen(myfile, "r");
    int l=0;
    while(fgets(line,128,graph_file) != NULL)
    {
        if(l==0) {l++; continue;}
        sscanf(line,"%d  %d",&edge.first, &edge.second);
        allEdges->push_back(edge);
        
        if(*nodes<edge.first) {*nodes=edge.first;}
        if(*nodes<edge.second) {*nodes=edge.second;}
        
        l++;
        
    }//end of while
    
    fclose(graph_file);
    
    
}
/*** End of Function ***/

//Read Network From File
void readin_network(PR_Network *X, char *file)
{
    //File reading parameters
    FILE *graph_file;
    char line[128];
    
    graph_file=fopen(file, "r");
    int l=0;
    int nodes=0;
    int_int dummy;
    dummy.first=1;
    dummy.second=0;
    while(fgets(line,128,graph_file) != NULL)
    {
        int n1,n2;
        //Read line
        sscanf(line,"%d %d",&n1,&n2);
        //cout <<n1<<"\n";
        //Number of nodes given in the first line
//        if(l==0)
//        {l++; continue;}

        dummy.first=n1;
        X->at(n2).In_Neigh.push_back(dummy);
        dummy.first=n2;
        X->at(n1).Out_Neigh.push_back(dummy);
       X->at(n1).Out_size++;

        l++;
    }//end of while
    fclose(graph_file);
    
    return;
}
/********* End of Function **********/



// information about Page Rank initial Values take them for updating at later stage

void readin_PageRank(char *file, PR_Network *X)
{

    FILE *graph_file;
    char line[128];
    graph_file=fopen(file, "r");
    int ID;
    float pageRank;

    while(fgets(line,128,graph_file) != NULL)
    {
        sscanf(line,"%d %f",&ID, &pageRank);

        //Add vertex to the component list
            X->at(ID).page_rank=pageRank;

    }
    fclose(graph_file);
}

/*** End of Function ***/


//Information about set of changed edges
void readin_changes(char *myfile, PR_Network *X)
{
    //File reading parameters
    FILE *graph_file;
    char line[128];
    int_int edge;
    int type;
    int count=0;

    
    graph_file=fopen(myfile, "r");
    while(fgets(line,128,graph_file) != NULL)
    {
        sscanf(line,"%d  %d %d",&edge.first, &edge.second, &type);
        
        int n1=edge.first;
        int n2=edge.second;
        int_int dummy;
        dummy.first=1;
        dummy.second=0;
        
        if(type==0)
        {

            for (int i=0;i<X->at(n1).Out_Neigh.size();i++)
            {
                if(X->at(n1).Out_Neigh[i].first==n2)
                {
	            X->at(n1).Out_Neigh[i].second=-1;
		    X->at(n1).Out_size--;
                    break;
                }
            }
            
	    for (int i=0;i<X->at(n2).In_Neigh.size();i++)
            {
                if(X->at(n2).In_Neigh[i].first==n2)
                {
	            X->at(n2).In_Neigh[i].second=-1;
                    break;
                }
            }
           
	    
	    X->at(n2).updateFlag=true;
            X->at(n2).level=0;


        }//end of delete
        

        if(type==1)
        {
         //Add edges in X
            dummy.first=n1;
            X->at(n2).In_Neigh.push_back(dummy);
            dummy.first=n2;
            X->at(n1).Out_Neigh.push_back(dummy);
            X->at(n1).Out_size++;
	    X->at(n2).updateFlag=true;
            X->at(n2).level=0;

        }//end of add
        
        
    }//end of while
    
    fclose(graph_file);
    
}
/*** End of Function ***/


#endif
