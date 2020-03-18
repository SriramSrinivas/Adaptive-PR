//
// Created by sriramsrinivas@unomaha.edu on 2/18/20.
//

#ifndef BHOWMICKPR_ADAPTIVE_H
#define BHOWMICKPR_ADAPTIVE_H
#include <bits/stdc++.h>
#include<string>
#include <sstream>
#include <iostream>
#include <fstream>

/* Any other way */
using namespace std;

void prepareinputforGalois(PR_Network *X)
{
    ofstream myfile;
    myfile.open ("galoisinput");

    for (int i=0;i<X->size();i++)
    {

        for (int j=0;j<X->at(i).Out_Neigh.size();j++)
        {
            if(X->at(i).Out_Neigh[j].second!=-1)
                myfile<<i <<" "<<X->at(i).Out_Neigh[j].first<<"\n";
        }
    }
    myfile.close();

}



void adaptiveModel(int *p, char *file)
{
    cout <<"I am here running adaptive";

    string graphConvert="./../Galois-2.2.1/build/tools/graph-convert/graph-convert";
    graphConvert=graphConvert+=" -intedgelist2gr ";
    graphConvert=graphConvert+"galoisinput";
    graphConvert=graphConvert+=" graphconvertoutput";
    const char *command=graphConvert.c_str();
    cout << "Running file using " << command << endl;
    system(command);

    string str = "./../Galois-2.2.1/build/apps/pagerank/pagerank";
   // str = str + " ../pagerankData/rmat21/rmat21er10mgaloisinputbinary";
   str=str+" graphconvertoutput";
   //str=str+" -graphTranspose=../pagerankData/rmat21/rmat21er10mgaloisinputbinary";
   str+=" -graphTranspose=graphconvertoutput ";
   str =str+" -t ";
    std::ostringstream ss;
    ss << *p;
    str= str+ss.str();

    str= str+" >galoisoutput";

   command = str.c_str();
    cout << "Running file using " << command << endl;
    system(command);

}



std::string getLastLine(std::ifstream& in)
{
    std::string line;
    while (in >> std::ws && std::getline(in, line)) // skip empty lines
        ;

    return line;
}


void readGaloisOutput()
{
    std::ifstream file("galoisoutput");

    if (file)
    {
        std::string line = getLastLine(file);
        cout <<line<<"\n";

    }
    else
        std::cout << "Unable to open file.\n";


}

#endif //BHOWMICKPR_ADAPTIVE_H
