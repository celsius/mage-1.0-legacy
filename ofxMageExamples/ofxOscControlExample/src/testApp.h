
/* ------------------------------------------------------------------------------------------- */
/*																							   */
/*  This file is part of MAGE / pHTS (the performative HMM-based speech synthesis system)      */
/*																							   */
/*  MAGE / pHTS is free software: you can redistribute it and/or modify it under the terms     */
/*  of the GNU General Public License as published by the Free Software Foundation, either     */
/*  version 3 of the license, or any later version.											   */
/*																							   */
/*  MAGE / pHTS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;   */	
/*  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  */
/*  See the GNU General Public License for more details.									   */
/*																							   */	
/*  You should have received a copy of the GNU General Public License along with MAGE / pHTS.  */ 
/*  If not, see http://www.gnu.org/licenses/												   */
/*																							   */
/*																							   */	
/*  Copyright 2011 University of Mons :													       */
/*																							   */	
/*			Numediart Institute for New Media Art (www.numediart.org)						   */
/*         Acapela Group (www.acapela-group.com)											   */
/*																							   */
/*																							   */
/*   Developed by :																			   */
/*																							   */
/*		Maria Astrinaki, Geoffrey Wilfart, Alexis Moinet, Nicolas d'Alessandro, Thierry Dutoit */
/*																							   */
/* ------------------------------------------------------------------------------------------- */


#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOscParser.h"
#include "ofxMageSynth.h"
#include "MAGE.h"


#define MAXBUFLEN 1024
#define TICKBUFFERSIZE 512
#define NUMBER_OF_LABELS 206
class ofxOscParser;
class ofxMageSynth;

class testApp : public ofBaseApp
{
  public:
	
	void setup( void );
	void update( void );
	void draw( void );
	void exit( void );
	
	void keyPressed( int key );
	void keyReleased( int key );
	
	void mouseMoved( int x, int y );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseDragged( int x, int y, int button );
	
	void windowResized( int w, int h );
	
	void audioRequested( float *output,
	int bufferSize, int nChannels );
	
	int _argc;
	char **_argv;
	
	char *Label;
	int labelIndex;
	
	MAGE_Environment *Mage;
	
	ofxOscParser *OscParser;
	ofxMageSynth *MageSynth;
	
	float buffer[TICKBUFFERSIZE];
	
	double pitch;
	double speed;
	double volume;
	double alpha;
	double duration;
};

#endif
