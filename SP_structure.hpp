#ifndef SP_STRUCTURE_HPP
#define SP_STRUCTURE_HPP

#include<iostream>
#include<vector>
#include<algorithm>
#include <iterator>
#include<utility>
#include <fstream>
#include<cmath>
#include <ctime>
#include <queue>
#include <stack>
#include <map>
#include<list>
#include <omp.h>

//#include <concurrent_vector.h>
using namespace std;


typedef pair<int, int> int_int;  /** /typedef pair of integers */
typedef pair<int,double>int_double;

//Properties of a vertex requored for SCC
struct PR_Vertex
{
    
    vector <int_int> In_Neigh; //indegree neighbors
    vector<int_int> Out_Neigh; //outsideConnnection neighbors
    double page_rank; //Page rank values
    bool updateFlag; //whether the vertex's page rank must be updated
    int level; //number of dependencies before processing
    double dValue; //value of d
    int Out_size;
    int reach;
    int msg;
    bool visited;

    //Constructor
   PR_Vertex()
    {In_Neigh.clear();
        Out_Neigh.clear();
	page_rank=0;
	updateFlag=false;
	level=-1;
	dValue=1;
	Out_size=0;
	visited=false;
	reach=0;
	msg=0;
    }
    
    //Destructor
    void clear()
    {
        while(!In_Neigh.empty()) {In_Neigh.pop_back();}
        while(!Out_Neigh.empty()) {Out_Neigh.pop_back();}
    }
};


typedef  vector<PR_Vertex> PR_Network;


    

struct PageRank_MetaInformation{
    int id;
    double pageRank;
    double dValue;
    double intermediateValue;
    bool vertexLock;
    bool updateFlag;
    double previousIterationPageRankValue;
    vector<int_double>inConnnection;
    vector<int_double >outsideConnnection;
    vector<double> value;
    int degree;
    int inConnectionSize;
    int outsideConnectionSize;
    int afterProcessigCEInConnectionSize;
    int afterProcessingCEOutsideConnectionSize;
    int level;
    bool visited;
    bool potential_visit;
    int visit_count=0;
    PageRank_MetaInformation()
    {
        inConnnection.clear();
        outsideConnnection.clear();
        vertexLock=false;
        updateFlag=false;
        previousIterationPageRankValue=false;
        inConnectionSize=0;
        outsideConnectionSize=0;
        level=-1;
        visited= false;
        potential_visit=false;
        visit_count=0;
    }



    void clear()
    {

    }

};

typedef  vector<PageRank_MetaInformation> PageRank_Network;






//Structure of Edge with level
/*struct lEdge {
    Edge theEdge;
    int Level;
};

//Data Structure for Remainder Edges
//Bucket of edge list and its Level in buckets ranging from 0-10; 10-20 20-30  ....90-100
struct RE_Bucket
{
    int st;
    int end;
    vector<Edge> REdges; //set of edges in this bucket
    vector<int_double> val_ind; //index from which a particular value starts
    
    //Constructor
    RE_Bucket()
    {
        st=-1;
        end=-1;
        REdges.clear();
        val_ind.clear();
    }
    
    //Destructor
    void clear()
    {}
    
};

//Define compare for edge
struct EdgeCompare
{
    bool operator() (const Edge& edge1, const Edge& edge2) const
    {
        return edge1.edge_wt< edge2.edge_wt;
    }
};
//Map of Edge to Position Index
typedef map<Edge,int, EdgeCompare> map_Edge_int;
*/





#endif
