
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


/*
 *  In this example there is one thread that handles mage and the synthesis of the speech, 
 *	and another thread that handles the osc messages and the incoming control of speed, pitch, 
 *	volume, duration, and alpha, all-pass constant value.
 */


#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "testApp.h"

//========================================================================
int main( int argc, char **argv )
{
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 200, 200, OF_WINDOW);

	testApp *_app = new testApp();
	
	_app->_argc = argc;
	_app->_argv = argv;
	
	ofRunApp( _app );
}
