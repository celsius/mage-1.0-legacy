
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

void testApp::setup( void )
{
	ofBackground( 100, 100, 100 );
	
	Mage = MAGE_init( _argc, _argv ); // init MAGE
	
	OscParser = new ofxOscParser( this ); // create the OSC parser
	OscParser->startThread( false, false ); // and start it
	
	MageSynth = new ofxMageSynth( this ); // create the MAGE synth
	MageSynth->startThread( false, false ); // and start it
	
	ofSoundStreamSetup( 2, 0, this, 44100, TICKBUFFERSIZE, 4 );
}


void testApp::update( void )
{
}


void testApp::draw( void )
{
}

void testApp::exit( void )
{
	OscParser->waitForThread( true ); // stop OSC thread
	delete OscParser; // and delete it
	
	MageSynth->waitForThread( true ); // stop OSC thread
	delete MageSynth; // and delete it
	
	MAGE_free( Mage ); // free MAGE
}


void testApp::keyPressed( int key )
{
}


void testApp::keyReleased( int key )
{
}


void testApp::mouseMoved( int x, int y )
{
}


void testApp::mouseDragged( int x, int y, int button )
{
}


void testApp::mousePressed( int x, int y, int button )
{
}


void testApp::mouseReleased( int x, int y, int button )
{
}


void testApp::windowResized( int w, int h )
{
}


void testApp::audioRequested( float *output, int bufferSize, int nChannels )
{
	MAGE_popSamples( Mage, buffer, bufferSize );
	
	for( int k=0; k<bufferSize; k++ ) 
	{
		output[2*k] = buffer[k];
		output[2*k+1] = buffer[k];
	}
}
