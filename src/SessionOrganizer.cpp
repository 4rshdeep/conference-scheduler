#include "SessionOrganizer.h"
#include "Util.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <random>

SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
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

void SessionOrganizer::randomRestart( ) {
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(papers), std::end(papers), rng);
    
    cout << scoreOrganization(conference) << "\n\n";
    int paperCounter = 0;
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                conference->setPaper ( j, i, k, papers[paperCounter] );
                paperCounter++;
            }
        }
    }
    
    // Should I do this?
    if (scoreOrganization(conference) < 0.75*max_score)
    {
        randomRestart();
    }

}

SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );
    
    processingTimeInMinutes = processingTimeInMinutes*60;

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

// Change this
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


double SessionOrganizer::organisePapersBaseline ( time_t t1 ) {

    time_t t2;
    srand(time(0));
    int n = totalPapers;
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> rndm(0,n-1);

    std::uniform_real_distribution<double> prob(0.0,1.0);

    int slot1 = slot1 = rand() % n;
    int slot2 = rand() % n;
        
    double score, score2, delta, p;
    score  = scoreOrganization ( conference);
    
    int iter = 0; // keep track of local maxima

    string rep_state;
    int count_temp = 0;
    for (int i = 0; i < 1000000; ++i)
    {   
        if (iter > 2000)
        {
            cout << "\n\nRandom Restart\n\n" <<endl ;
            score = scoreOrganization(conference);
            if (score > max_score)
            {
                max_score = score;
            }
            randomRestart();
            score = scoreOrganization(conference);
            iter = 0;
        }
        
        if (score > max_score)
        {
            max_score = score;
        }
        time(&t2);
        double dif = difftime (t2, t1);

        if ( processingTimeInMinutes - dif < 0.3)
        {
            break;
        }
        slot1 = rand() % n;
        slot2 = rand() % n;
        
        // swapPapersBaseline ( conference, slot1, slot2 );
        // score2 = scoreOrganization ( );
        // cout << "Time : " << dif << " | Iteration : " << i << " | Score :" << score << endl; 
        // delta = score2 - score;
        // if(delta > 0){
        //     score = score2;
        //     score2 = -1;
        //     iter = 0;
        // }
        // else {
        //     p = prob(generator);
        //     // p = ((double) rand() / (RAND_MAX));
        //     // if (p > exp(delta*(pow((i+1), 0.5))))
        //     if (p > exp(delta*(i+1)))
        //     {
        //         // undo swap if probability is greater
        //         swapPapersBaseline ( slot1, slot2 );
        //         iter++;
        //     }
        //     else{
        //         score = scoreOrganization();
        //         cout << "Taking road less taken " << endl;
        //         iter = 0;
        //     }
        swapPapersBaseline (conference, slot1, slot2 );
        rep_state = conf2str(conference);      
        cout << "Time : " << dif << " | Iteration : " << i << " | Score :" << score << endl; 

        if(visited.find(rep_state)==visited.end()){
            visited.insert(make_pair(rep_state, true));
            score2 = scoreOrganization ( conference);
            // cout << "Iteration : " << i << " Score :" << score << endl; 
            delta = score2 - score;
            if(delta > 0){
                score = score2;
                score2 = -1;
                iter = 0;
            }
            else {
                p = prob(generator);
                // p = ((double) rand() / (RAND_MAX));
                // undo swap if probability is greater
                if (p > exp(delta*((i+1))))
                {
                    swapPapersBaseline ( conference, slot1, slot2 );
                    iter++;
                }
                else{
                    score = score2;
                    cout << "Taking road less taken " << endl;
                    iter = 0;
                }

            }
        }
        else{
            count_temp++;
        }
    }
    
    cout << "Score :" << max_score << " c=" <<count_temp <<endl; 
    return score;
}

double SessionOrganizer::organisePapersSystematicSearch (time_t t) {
    priority_queue<pair<double,Conference *> > states;
    int n = totalPapers;
    double score, score2, delta, p;
    score  = scoreOrganization ( conference);
    states.push(make_pair(score,conference));
    string rep_state = conf2str(conference);
    visited.insert(make_pair(rep_state,true));
    double max_score = score;
    Conference* max_conference = conference;
    Conference * temp_conf;
    int count = 0;
    int i,j,k;
    for (i = 0; i < 100; ++i){
        pair<double, Conference*> to_visit = states.top();
        states.pop(); // see
        if(to_visit.first>max_score){
            max_conference = to_visit.second;
            max_score = to_visit.first;
        }
        cout<<" Exploring node with score "<<to_visit.first<<" max so far is "<<max_score<<endl;
        // actions are replacing paper at pos 0 ,0, 0, with random paper
       // slot1  = 1+ (rand() % (n-1)); 1 to n-1 random number
        for(j=1;j<n;j++){
            temp_conf = to_visit.second->create_copy();
            swapPapersBaseline(temp_conf, 0, j);
            // swapPapersBaseline(temp_conf, rand() % n, rand() % n);            
            rep_state = conf2str(temp_conf);      
            if(visited.find(rep_state)==visited.end()){
                count++;
                visited.insert(make_pair(rep_state,true));
                score  = scoreOrganization (temp_conf);
                states.push(make_pair(score,temp_conf));
            }
            else{
                temp_conf->freeTracks();
                delete(temp_conf);
            }
        }
    }
    cout<<"Scored "<<count<< " nodes"<<endl <<"Max score found is "<<max_score<<endl;
   
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

    processingTimeInMinutes = atof ( lines[0].c_str () );
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
