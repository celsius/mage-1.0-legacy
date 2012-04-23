
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


#include "ofxOscParser.h"

ofxOscParser::ofxOscParser( testApp *app ) 
{	
	Receiver.setup( PORT );
	App = app;
}

void ofxOscParser::threadedFunction( void ) 
{
	float pitchInHz;
	float pitchInSemitone;
	float speedFromOSC;
	float volumeFromOSC;
	float alphaFromOSC;
	float durationFromOSC;
	int modeFromOSC;
	
	while( threadRunning ) 
	{
		if( Receiver.hasWaitingMessages() ) 
		{
			// --- get new OSC message ---
			m.clear();
			Receiver.getNextMessage( &m );
			
			// --- THE LIST OF MESSAGES ---
			
			if( m.getAddress() == "/pitch" )
			{
				// --- change pitch ---
				pitchInHz = m.getArgAsFloat( 0 );
				pitchInSemitone = m.getArgAsFloat( 1 ); 
				modeFromOSC = m.getArgAsFloat( 2 );
				
				if (modeFromOSC == 0)
				{
					App->pitch = ofMap(pitchInSemitone, 0, 34, -2, 2, true);
					MAGE_setPitch( App->Mage, App->pitch, MAGE_SHIFT );
				}
				
				if (modeFromOSC == 1)
				{
					App->pitch = log(pitchInHz);
					MAGE_setPitch( App->Mage, App->pitch, MAGE_OVERWRITE );
				}				
			}
			
			if( m.getAddress() == "/duration" )
			{
				// --- change duration ---
				durationFromOSC = m.getArgAsFloat( 0 ); 
				modeFromOSC = m.getArgAsFloat( 1 );
				
				if (modeFromOSC == 0)
				{
					App->duration = ofMap(durationFromOSC, 0, 1000, 0.1, 50, true);
					MAGE_setDuration( App->Mage, App->duration, MAGE_OVERWRITE );
				}
				
				if (modeFromOSC == 1)
				{
					App->duration = ofMap(durationFromOSC, 0, 1000, 0.1, 50, true);
					MAGE_setDuration( App->Mage, App->duration, MAGE_SHIFT );
				}
				
				if (modeFromOSC == 2)
				{
					App->duration = ofMap(durationFromOSC, 0, 1000, 0.1, 5, true);
					MAGE_setDuration( App->Mage, App->duration, MAGE_SCALE );
				}
				
			}
			
			if( m.getAddress() == "/speed" ) 
			{
				// --- change speed  ---
				
				speedFromOSC = m.getArgAsFloat( 0 );
				
				App->speed = ofMap(speedFromOSC, 0, 1, 0.1, 2, true);
				MAGE_setSpeed(App->Mage, App->speed);
			}
			
			if( m.getAddress() == "/volume" ) 
			{
				// --- change volume ---
				
				volumeFromOSC = m.getArgAsFloat( 1 );
								
				App->volume = ofMap(volumeFromOSC, 0, 1, 0, 1, true);
				MAGE_setVolume(App->Mage, App->volume);
			}
			
			if( m.getAddress() == "/alpha" ) 
			{				
				// --- change alpha ---
				
				alphaFromOSC = m.getArgAsFloat( 0 );
		
				App->alpha = ofMap(alphaFromOSC, 0.1, 0.9, 0.1, 0.9, true);
				MAGE_setAlpha(App->Mage, App->alpha);
			}
		}
		
		usleep( 10 );
	}
}