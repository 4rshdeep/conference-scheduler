#ifndef SESSIONORGANIZER_H
#define	SESSIONORGANIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>


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

    Conference *conference;

    double tradeoffCoefficient ; // the tradeoff coefficient


public:
    int totalPapers;
    double processingTimeInMinutes;
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

    double organisePapersBaseline(time_t t);

    void swapPapersBaseline(int s1, int s2);
    
    
    
    /**
     * Get the distance matrix.
     * @return the distance matrix.
     */
    double** getDistanceMatrix();
    
    
    /**
     * Score the organization.
     * @return the score.
     */
    double scoreOrganization();
    
    
    void printSessionOrganiser(char *);
};

#endif	/* SESSIONORGANIZER_H */

