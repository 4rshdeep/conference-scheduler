#include <cstdlib>
#include <cmath>
#include <random>

#include "SessionOrganizer.h"

using namespace std;

int main ( int argc, char** argv )
{
    auto start = chrono::high_resolution_clock::now();

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
    
    double score = organizer->organisePapersAlternative(start);

    // Score the organization against the gold standard.
    cout << "Final score : " << organizer->scoreOrganization( organizer->conference ) << endl;
    organizer->printSessionOrganiser ( argv[2]);

    return 0;
}

