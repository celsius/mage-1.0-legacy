
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


#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground( 100, 100, 100);

// ofxMage	
	mage = MAGE_init( _argc, _argv );
	
	labelIndex = 0;
	label = NULL;
// ofxMage
	
//ofxThread
	MageThread = new ofxMageThread();
	MageThread->linkApp(this);
	MageThread->startThread(false, false);	
//ofxThread

// ofxMage	
	ofSoundStreamSetup( 2, 0, this, 44100 , 1024, 4 );
// ofxMage
}

//--------------------------------------------------------------
void testApp::update()
{
}


//--------------------------------------------------------------
void testApp::draw()
{
}

void testApp::exit()
{
//ofxThread
	MageThread->waitForThread(true);
	delete MageThread;
//ofxThread
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
// ofxMage
	pitch = ofMap(x, 0, 1000, 0.1, 7, true);
	MAGE_setPitch(mage, pitch, MAGE_OVERWRITE);
// ofxMage
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::audioRequested(float * output, int bufferSize, int nChannels)
{	
// ofxMage
	MAGE_popSamples(mage, buffer, bufferSize);
	for( int k=0; k<bufferSize; k++ ) 
	{
		output[2*k] = buffer[k];
		output[2*k+1] = buffer[k];
	}
// ofxMage
}