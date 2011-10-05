
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



#include "ofxMageThread.h"

void ofxMageThread::linkApp( testApp *app ) 
{
	App = app;
}

void ofxMageThread::threadedFunction( void )
{
	while( threadRunning ) 
	{
		if ( App->labelIndex < NUMBER_OF_LABELS )
		{
			App->label = (char*)MAGE_getLabelFromFile(App->mage);
			
			MAGE_pushLabel(App->mage, App->label);
			
			MAGE_parseLabel(App->mage);
			
			MAGE_updatePDFs(App->mage);
			
			MAGE_updateFilter(App->mage);
			
			
			if(App->label != NULL && strstr(App->label,"+pau")!=NULL) 
				MAGE_finalize(App->mage);
			
			MAGE_updateSamples(App->mage);
			
			if(App->label != NULL && strstr(App->label,"+pau")!=NULL) 
				MAGE_refresh(App->mage);
			
			App->labelIndex++;
		}
		
		if (App->labelIndex == 206) 
		{
			
			if(App->label != NULL && strstr(App->label,"=pau")!=NULL) 
				MAGE_finalize(App->mage);
			
			MAGE_updateSamples(App->mage);
			
			if(App->label != NULL && strstr(App->label,"=pau")!=NULL) 
				MAGE_refresh(App->mage);
			
			App->labelIndex = 0;
			pHTS *p = (pHTS *)App->mage->data;
			fclose(p->p_labels);
			fopen(p->labfn, "r");
			
		}
	}
}
