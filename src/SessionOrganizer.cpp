#include "SessionOrganizer.h"
#include "Util.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <random>

int besti1, besti2, besti3, besti4, besti5, besti6, besti7, besti8, besti9;

SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInSeconds = 0;
    tradeoffCoefficient = 1.0;
    totalPapers = 0;
    max_score = 0;

}

void print_vec( vector<int> vec ) {
    for(int i=0; i < vec.size(); i++) {
        cout << vec[i] << ", ";
    }
    cout << "---" << endl;
}


SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );
    
    processingTimeInSeconds = processingTimeInSeconds*60;

    totalPapers = parallelTracks*papersInSession*sessionsInTrack;

    // Initialize the papers //
    int paperCounter = 0;
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                papers.push_back(paperCounter);
                conference->setPaper ( j, i, k, paperCounter );
                paperCounter++;
            }
        }
    }

    // Initialisation done //

}

void SessionOrganizer::organizePapers ( )
{
    int paperCounter = 0;
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                conference->setPaper ( j, i, k, paperCounter );
                paperCounter++;
            }
        }
    }
}

void SessionOrganizer::next_best_neighbour( Conference *conf ) {

    int i1, i2, i3, i4, i5, i6, i7, i8, i9;//, besti1, besti2, besti3;
    double score, max_score = -1;

    for (int i = 0; i < 50; ++i) // check for 10 return the best one
    {
        i1 = rand() % totalPapers;
        i2 = rand() % totalPapers;
        i3 = rand() % totalPapers;
        i4 = rand() % totalPapers;
        i5 = rand() % totalPapers;
        i6 = rand() % totalPapers;
        i7 = rand() % totalPapers;
        i8 = rand() % totalPapers;
        i9 = rand() % totalPapers;
        
        swapPapersBaseline(conf, i1, i2);
        swapPapersBaseline(conf, i2, i3);
        swapPapersBaseline(conf, i3, i4);
        swapPapersBaseline(conf, i4, i5);
        swapPapersBaseline(conf, i5, i6);
        swapPapersBaseline(conf, i6, i7);
        swapPapersBaseline(conf, i7, i8);
        swapPapersBaseline(conf, i8, i9);
        score = scoreOrganization(conf);
        if (score > max_score)
        {
            besti1 = i1;
            besti2 = i2;
            besti3 = i3;
        }
        swapPapersBaseline(conf, i8, i9);
        swapPapersBaseline(conf, i7, i8);
        swapPapersBaseline(conf, i6, i7);
        swapPapersBaseline(conf, i5, i6);
        swapPapersBaseline(conf, i4, i5);
        swapPapersBaseline(conf, i3, i4);
        swapPapersBaseline(conf, i2, i3);
        swapPapersBaseline(conf, i1, i2);

    }

    swapPapersBaseline(conf, besti1, besti2);
    swapPapersBaseline(conf, besti2, besti3);
    swapPapersBaseline(conf, besti3, besti4);
    swapPapersBaseline(conf, besti4, besti5);
    swapPapersBaseline(conf, besti5, besti6);
    swapPapersBaseline(conf, besti6, besti7);
    swapPapersBaseline(conf, besti7, besti8);
    swapPapersBaseline(conf, besti8, besti9);

}


double SessionOrganizer::swapAndReturnScore( Conference *conf, double score, int slot1, int slot2 ) {
    
    double tempScore1, tempScore2;
    int paper_per_sess = conference -> getPapersInSession();
    int parallel_sess  = conference -> getParallelTracks();
    int time_slots     = conference -> getSessionsInTrack();
 

    int k1 = slot1 % paper_per_sess ;
    int temp = (slot1 - k1) / paper_per_sess;
    int j1 = temp % parallel_sess;
    int i1 = (temp - j1) / parallel_sess;

    int k2 = slot2 % paper_per_sess ;
    temp = (slot2 - k2) / paper_per_sess;
    int j2 = temp % parallel_sess;
    int i2 = (temp - j2) / parallel_sess;

    if( (i1 == i2) && (j1 == j2) ) {
        return score;
    }
    
    if (i1 == i2) {
        tempScore1 = getRowScore(conf, i1);
        swapPapersBaseline(conf, slot1, slot2);
        score = score - tempScore1 + getRowScore(conf, i2);
    }
    else {
        // compute for i1, i2 before swap and after swap
        tempScore1 = getRowScore(conf, i1);
        tempScore2 = getRowScore(conf, i2);
        swapPapersBaseline(conf, slot1, slot2);
        score = score - ( tempScore1 + tempScore2 ) + ( getRowScore(conf, i1) + getRowScore(conf, i2) );
    }

    return score;
}

double SessionOrganizer::organisePapersAlternative ( chrono::high_resolution_clock::time_point start ) {
    
    srand(time(0));
    int n = totalPapers;
    
    std::default_random_engine generator;
    std::uniform_real_distribution<double> prob(0.0,1.0);

    int slot1 = rand() % n;
    int slot2 = rand() % n;
        
    double score, score2, temp_score, delta, delta_prob, p, temperature = 0.005;
    score  = scoreOrganization ( conference);
    

    int iter = 0, annealing=0; // keep track of local maxima

    int i = 0;
    while(true)
    {   
        if (i%2000)
        {   
            temperature += 0.002;
        }
        
        if (score > max_score)
        {
            max_score = score;
            bestConference = conference->create_copy();
        }
        
        auto curr_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(curr_time - start);
        if ( 1000*processingTimeInSeconds - duration.count() < 500)
        {
            break;
        }

        i++;
        
        if (iter > 5000)
        {
            cout << "\n\nRandom Restart\n\n" <<endl ;
            // score = scoreOrganization(conference);
            next_best_neighbour(conference);
            score = scoreOrganization(conference);
            iter = 0;
        }
        
        

        // make changes to a state
        slot1 = rand() % n;
        slot2 = rand() % n;

        score2 = swapAndReturnScore (conference, score, slot1, slot2 );
        
        // cout << "Tracker : " << iter << " | Time : " << duration.count() << " | Iteration : " << i << " | Score :" << score << endl; 

        delta = score2 - score;
        if(delta > 0){
            score = score2;
            iter = 0;
        }
        else {
            p = prob(generator);
            delta_prob = exp(delta*temperature);

            // undo swap if probability is greater
            if (p > delta_prob) // change 10 to temp when needed
            {
                swapPapersBaseline ( conference, slot1, slot2 );
                iter++;
            }
            else{
                score = score2;
                annealing++;
                // cout << "Taking road less taken with delta " << delta << " with probability: " << p << " should be less than : " << delta_prob << endl;
                iter = 0;
            }
        }
    }
    
    // cout << "Times annealling step was taken : " << annealing << " Final Temperature : " << temperature << endl;
    // cout << "Max Score : " << max_score << " Iterations : " << i << endl;
    conference = bestConference;
    return scoreOrganization(conference);

}



void SessionOrganizer::swapPapersBaseline (Conference *conf, int slot1, int slot2 )
{   
    int paper_per_sess = conference -> getPapersInSession();
    int parallel_sess  = conference -> getParallelTracks();
    int time_slots     = conference -> getSessionsInTrack();
 
    int n = paper_per_sess * parallel_sess * time_slots;

    while(slot1 == slot2)
    {
        slot2 = rand() % n;
    }   

    int k1 = slot1 % paper_per_sess ;
    int temp = (slot1 - k1) / paper_per_sess;
    int j1 = temp % parallel_sess;
    int i1 = (temp - j1) / parallel_sess;

    int k2 = slot2 % paper_per_sess ;
    temp = (slot2 - k2) / paper_per_sess;
    int j2 = temp % parallel_sess;
    int i2 = (temp - j2) / parallel_sess;

    int paper_id1 = conference->getPaper ( j1, i1, k1 ); 
    int paper_id2 = conference->getPaper ( j2, i2, k2 ); 

    conference->setPaper ( j1, i1, k1, paper_id2 );
    conference->setPaper ( j2, i2, k2, paper_id1 );

}

void SessionOrganizer::readInInputFile ( string filename )
{
    vector<string> lines;
    string line;
    ifstream myfile ( filename.c_str () );
    if ( myfile.is_open ( ) )
    {
        while ( getline ( myfile, line ) )
        {
            lines.push_back ( line );
        }
        myfile.close ( );
    }
    else
    {
        cout << "Unable to open input file";
        exit ( 0 );
    }

    if ( 6 > lines.size ( ) )
    {
        cout << "Not enough information given, check format of input file";
        exit ( 0 );
    }

    processingTimeInSeconds = atof ( lines[0].c_str () );
    papersInSession = atoi ( lines[1].c_str () );
    parallelTracks = atoi ( lines[2].c_str () );
    sessionsInTrack = atoi ( lines[3].c_str () );
    tradeoffCoefficient = atof ( lines[4].c_str () );

    int n = lines.size ( ) - 5;
    double ** tempDistanceMatrix = new double*[n];
    for ( int i = 0; i < n; ++i )
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for ( int i = 0; i < n; i++ )
    {
        string tempLine = lines[ i + 5 ];
        string elements[n];
        splitString ( tempLine, " ", elements, n );

        for ( int j = 0; j < n; j++ )
        {
            tempDistanceMatrix[i][j] = atof ( elements[j].c_str () );
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if ( slots != numberOfPapers )
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit ( 0 );
    }
}

double** SessionOrganizer::getDistanceMatrix ( )
{
    return distanceMatrix;
}

void SessionOrganizer::printSessionOrganiser ( char * filename)
{
    conference->printConference ( filename);
}

string SessionOrganizer::conf2str(Conference *conf){
    int pap;
    vector<vector<vector<int> > > paps(0);// = new vector<vector<vector<int> > > ();
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        vector<vector<int> > temp3(0);// = new  vector<vector<int> >();
        paps.push_back(temp3);
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            vector<int> temp4(0);// = new vector<int>();
            paps[i].push_back(temp4);            
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                pap = conference->getPaper ( j, i, k );
                paps[i][j].push_back(pap);
            }
        }
    }
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            sort(paps[i][j].begin(),paps[i][j].end());
        }
    }
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        sort(paps[i].begin(),paps[i].end());
    }
    sort(paps.begin(),paps.end());    
    ostringstream oss("");
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                oss << paps[i][j][k];
                oss << "-"; // just for safety
            }
        }
    }
    return oss.str();
}

double SessionOrganizer::getRowScore ( Conference *conf, int row ) {
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
            Session tmpSession = tmpTrack.getSession ( row );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
            Session tmpSession1 = tmpTrack1.getSession ( row );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( row );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
    }

    double score = score1 + tradeoffCoefficient*score2;
    return score;
}

double SessionOrganizer::scoreOrganization ( Conference *conf )
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession = tmpTrack.getSession ( j );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}
