#include <cstdlib>
#include <cmath>

#include "SessionOrganizer.h"

using namespace std;

int main ( int argc, char** argv )
{
    time_t start;
    time(&start);

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
    
    double score2 = organizer->organisePapersBaseline(start);

    // double score2 = organizer->organisePapersSystematicSearch(start);
    // Score the organization against the gold standard.
    cout<< "After score:"<<score2<<endl;
    
    organizer->printSessionOrganiser ( argv[2]);
    return 0;
}

