
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

/*
 *  In this example we shift the duration given by the label file.
 *  Attention : use the -vp option in the command line arguments.
 */

#include "MAGE.h"
#define NUMBEER_OF_LABELS 206

int main(int argc, char **argv)
{
	int i, j;
	char *label;
	
	double duration;
	
	MAGE_Environment *mage;
	
	/* MAGE_init: initialize all variables and return the environment */
	mage = MAGE_init(argc, argv);
		
	int currentNumOfSamples;
	float *fbuffer;

	/* ------------------------------------- MAGE synthesis ----------------------------------------------- */		 
		
	duration = 20;
	
	for( i = 0 ; i < NUMBEER_OF_LABELS; i++) 
	{	
		
		/* ------------------------------------------- Duration ------------------------------------------- */	

		/* MAGE_SetDuration : set a different duration value in the generated speech parameters */
		MAGE_setDuration(mage, duration, MAGE_SHIFT);
		
	
		/* --------------------------------------------- Label -------------------------------------------- */	
		/* MAGE_getLabelFromFile: get one label string from a label file */
		label = MAGE_getLabelFromFile(mage);
		
		/* MAGE_pushLabel: sets a new label into the labels set in order to be prosessed */
		MAGE_pushLabel(mage, label);
		
		/* MAGE_parseLabel: parse the last inserted label */
		MAGE_parseLabel(mage);
		
		
		/* ---------------------------------------------- PDFs -------------------------------------------- */			
		/* MAGE_updatePDFs : get the pdfs */
		MAGE_updatePDFs(mage);
		

		/* ---------------------------------------------- MLSAs ------------------------------------------- */	
		/* MAGE_updateFilter : generate the speech parameters */
		MAGE_updateFilter(mage);
		
			
		/* ---------------------------------------- Speech Samples ---------------------------------------- */		
				
		/* MAGE_updateSamples: generate speech samples from the speech parameters */
		MAGE_updateSamples(mage);
				
		/* MAGE_getNumberOfSamples: get the current number of generated samples queued in the engine */
		currentNumOfSamples = MAGE_getNumberOfSamples( mage );
		
		fbuffer = (float *)calloc(currentNumOfSamples, sizeof(float));

		MAGE_popSamples(mage, fbuffer, currentNumOfSamples);	
		
		for (j = 0; j < currentNumOfSamples; j++) 
		{
			pHTS *p = mage->data;
			float temp = fbuffer[j];
			fwrite(&temp, sizeof(float), 1, p->p_rawfp);
		}
		
		free(fbuffer);
	}

	/* MAGE_finalize: flushes the internal memory */
	MAGE_finalize( mage );
	
	/* MAGE_free : free alocated memory */
	MAGE_free(mage);
		
	return ( 0 );
}
