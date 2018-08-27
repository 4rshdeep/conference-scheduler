/* 
 * File:   main.cpp
 * Author: Kapil Thakkar
 *
 */

#include <cstdlib>

#include "SessionOrganizer.h"

using namespace std;

/*
 * 
 */
int main ( int argc, char** argv )
{
    // Parse the input.
    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    
    // Initialize the conference organizer.
    SessionOrganizer *organizer  = new SessionOrganizer( inputfilename );

    // Organize the papers into tracks based on similarity.

    organizer->printSessionOrganiser ( argv[2]);

    // Score the organization against the gold standard.
    double score = organizer->scoreOrganization ( );
    cout<< "score:"<<score<<endl;
    srand(time(0));

    int n = organizer->totalPapers;

    int slot1 = rand() % n;
    int slot2 = rand() % n;
    organizer->swapPapersBaseline ( slot1, slot2 );
    int score2 = organizer->scoreOrganization ( );


    int iter = 0;
    while(score2 < score){
        cout << "while score" << score2 << endl;
        if (score2 < score)
        {
            slot1 = rand() % n;
            slot2 = rand() % n;
            organizer->swapPapersBaseline ( slot1, slot2 );
            score2 = organizer->scoreOrganization ( );
        }
        iter++;
    }
    // Score the organization against the gold standard.
    cout<< "After score:"<<score2<<endl;
    
    return 0;
}

