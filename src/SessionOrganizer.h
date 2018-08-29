#ifndef SESSIONORGANIZER_H
#define	SESSIONORGANIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include<string>
#include<unordered_map>
#include<algorithm>
#include<sstream>
#include<queue>
#include <chrono>



#include "Conference.h"
#include "Track.h"
#include "Session.h"

using namespace std;



/**
 * SessionOrganizer reads in a similarity matrix of papers, and organizes them
 * into sessions and tracks.
 * 
 *
 */
class SessionOrganizer {
private:
    double ** distanceMatrix;

    int parallelTracks ;
    int papersInSession ;
    int sessionsInTrack ;
    unordered_map<string, bool> visited;

    Conference *conference;

    double tradeoffCoefficient ; // the tradeoff coefficient


public:
    int totalPapers;
    double processingTimeInSeconds;
    vector<int> papers;
    double max_score;

    SessionOrganizer();
    SessionOrganizer(string filename);
    
    
    /**
     * Read in the number of parallel tracks, papers in session, sessions
     * in a track, and the similarity matrix from the specified filename.
     * @param filename is the name of the file containing the matrix.
     * @return the similarity matrix.
     */
    void readInInputFile(string filename);
    
    void randomRestart();
    
    /**
     * Organize the papers according to some algorithm.
     */
    void organizePapers();

    double organisePapersBaseline( chrono::high_resolution_clock::time_point);
    double organisePapersSystematicSearch( chrono::high_resolution_clock::time_point);
    double organisePapersAlternative ( chrono::high_resolution_clock::time_point start );


    void swapPapersBaseline(Conference *,int s1, int s2);
    
    double swapAndReturnScore( Conference*, double, int, int );

    double getRowScore ( Conference *conf, int row );


    void next_best_neighbour( Conference * );
    
    double greedySearch(chrono::high_resolution_clock::time_point );
    /**
     * Get the distance matrix.
     * @return the distance matrix.
     */
    double** getDistanceMatrix();
    
    
    /**
     * Score the organization.
     * @return the score.
     */
    double scoreOrganization(Conference *);


    string conf2str(Conference *);
    
    void printSessionOrganiser(char *);
};

#endif	/* SESSIONORGANIZER_H */

