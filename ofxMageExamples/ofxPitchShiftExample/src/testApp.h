
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
/*		   Numediart Institute for New Media Art Technology (www.numediart.org)		     	   */
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

// ofxMage
#include "pHTS.h"
#include "MAGE.h"
// ofxMage


//ofxThread
#include "ofxMageThread.h"
//ofxThread

//--------------------------------------------------------
class ofxMageThread;

class testApp : public ofBaseApp
{
	public:

		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void audioRequested(float * input, int bufferSize, int nChannels);

	// ofxMage
		int _argc;
		char **_argv;
		MAGE_Environment *mage;
	
		double pitch;
	
		int labelIndex;
		char *label;
	
		float buffer[1024]; 
	// ofxMage
	
	//ofxThread
		ofxMageThread *MageThread;
	//ofxThread
};

#endif
