
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


#include "MAGE.h"

/** 
 *	\file MAGE.c 
 *	\brief Contains the implementation of all the functions of the MAGE environment. It is the threading interface of pHTS.
 * 
 */

/* ---------------------------------------- Initialization ---------------------------------------- */

/* MAGE_init: initialize all variables and return the MAGE environment */  
MAGE_Environment * MAGE_init(int argc, char **argv)
{
	MAGE_Environment * env;
	
	env	 = (MAGE_Environment*) calloc(1, sizeof(MAGE_Environment));
	
	env->engine = (HTS_Engine*) calloc(1, sizeof(HTS_Engine));
	env->data = (pHTS*) calloc(1, sizeof(pHTS));
	
	pHTS_initialize (argc, argv, env->engine, env->data);
	
	return ( env );
}


/* --------------------------- Input interface to the engine : phonetics -------------------------- */

/* MAGE_pushLabel: set a new label into the labels ring buffer set in order to be prosessed */
void MAGE_pushLabel(MAGE_Environment *environment, char *labelAsStr)
{
	/* pHTS_pushLabel: set a new label into the labels set in order to be prosessed */
	pHTS_pushLabel(environment->engine, environment->data, labelAsStr);
}

/* MAGE_parseLabel: parse the last inserted label */
int MAGE_parseLabel(MAGE_Environment *environment)
{
	/* pHTS_parseLabel: parse the last inserted label */
	return( pHTS_parseLabel( environment->engine, environment->data));
}


/* --------------------------- Input interface to the engine : prosody ---------------------------- */

/* MAGE_setSpeed: set speech speed rate in the MAGE environment */
void MAGE_setSpeed(MAGE_Environment *environment, double speechSpeed)
{
	/* pHTS_setSpeed: set speech speed rate in the pHTS environment */
	pHTS_setSpeed(environment->engine, environment->data, speechSpeed); 
}

/* MAGE_getSpeed: get speech speed rate from the MAGE environment */ 
double MAGE_getSpeed( MAGE_Environment *environment )
{
	/* pHTS_getSpeed: get speech speed rate from the pHTS environment */
	double speechSpeed = pHTS_getSpeed( environment->engine,  environment->data);
	return( speechSpeed );
}

/* MAGE_setPitch: set a different pitch value in the generated speech parameters in the MAGE environment */
void MAGE_setPitch(MAGE_Environment *environment, double pitch, int mode)
{
	/* pHTS_setPitch: set a different pitch value in the generated speech parameters */
	pHTS_setPitch(environment->engine, environment->data, pitch, mode);
}

/* MAGE_getPitch: get current pitch value from the MAGE environment */ 
double MAGE_getPitch( MAGE_Environment *environment )
{
	/* pHTS_getPitch: get pitch value from the pHTS environment */
	double pitch = pHTS_getPitch(environment->engine, environment->data);
	return( pitch );
}

/* MAGE_setVolume: set a different volume value in the generated speech samples in the MAGE environment */
void MAGE_setVolume( MAGE_Environment *environment, double volume )
{
	/* pHTS_setVolume: set volume value in the pHTS environment */
	pHTS_setVolume(environment->engine, environment->data, volume);
}

/* MAGE_getVolume: get current volume value from the MAGE environment */
double MAGE_getVolume( MAGE_Environment *environment )
{
	/* pHTS_getVolume: get volume value from the pHTS environment */
	double volume = pHTS_getVolume(environment->engine, environment->data);
	return( volume );
}

/* MAGE_setAlpha: set alpha, all-pass constant value in the MAGE environment */
void MAGE_setAlpha(MAGE_Environment *environment, double alpha)
{
	/* pHTS_setAlpha: set alpha value in the pHTS environment */
	pHTS_setAlpha(environment->engine, environment->data, alpha);
}

/* MAGE_getAlpha: get alpha, all-pass constant value from the MAGE environment */
double MAGE_getAlpha( MAGE_Environment *environment )
{
	/* pHTS_getAlpha: get alpha, all-pass constant value from the pHTS environment */
	double alpha = pHTS_getAlpha(environment->engine, environment->data);
	return( alpha );
}

/* MAGE_setDuration: set a different duration value in the MAGE environment */  
void MAGE_setDuration(MAGE_Environment *environment, double duration, int mode)
{
	/* pHTS_setDuration: set a different duration value in the generated speech parameters in the pHTS environment */
	pHTS_setDuration(environment->engine, environment->data, duration, mode);
}

/* MAGE_getDuration: get current duration from the MAGE environment */ 
double MAGE_getDuration( MAGE_Environment *environment )
{
	/* pHTS_getDuration: get durations value from the pHTS environment */
	double duration = pHTS_getDuration( environment->engine,  environment->data);
	return( duration );
}



/* ---------------------------- Updating internal state of the engine ----------------------------- */

/* MAGE_update: generate the speech samples */ 
void MAGE_update(MAGE_Environment *environment)
{
	/* pHTS_updateEngine: generate the speech samples from PDFs */
	pHTS_updateEngine(environment->engine, environment->data);
}
 
/* MAGE_updatePDFs: generate PDFs */
void MAGE_updatePDFs(MAGE_Environment *environment)
{
	/* pHTS_updatePDFs: generate PDFs */
	pHTS_updatePDFs(environment->engine, environment->data);
}

/* MAGE_updateFilter: generate the speech parameters */ 
void MAGE_updateFilter(MAGE_Environment *environment)
{
	/* pHTS_updateFilter: generate the speech parameters from the PDFs */
	pHTS_updateFilter(environment->engine, environment->data);
}

/* MAGE_updateSamples: generate speech samples from the speech parameters */
void MAGE_updateSamples(MAGE_Environment *environment)
{
	/* pHTS_updateSamples: generate speech samples from the speech parameters */
	pHTS_updateSamples(environment->engine, environment->data);
}



/* ----------------------------- Accessing engine internal data types ----------------------------- */

/* MAGE_getNumberOfLabels: get the # of labels queued in the engine of the MAGE environment */ 
int MAGE_getNumberOfLabels(MAGE_Environment *environment)
{
	/* pHTS_getNumberOfLabels: get the # of labels queued in the engine */
	int numberOfLabels = pHTS_getNumberOfLabels(environment->engine, environment->data);
	return ( numberOfLabels );
}

/* MAGE_getNumberOfSamples: get the current number of generated samples queued in the engine */
int MAGE_getNumberOfSamples(MAGE_Environment *environment)
{
	/* pHTS_getNumberOfSamples: get the number of the generated samples */
	int samples = pHTS_getNumberOfSamples(environment->engine, environment->data);
	return ( samples );
}

/* MAGE_getLabel: get the current label processed by the engine of the MAGE environment */                              
MAGE_Label * MAGE_getLabel(MAGE_Environment *environment)
{
	/* pHTS_getLabels: get Label */
	HTS_LabelString *label = pHTS_getLabel(environment->engine, environment->data);
	return ( label );
}
 
/* MAGE_getPDFs: get the current PDFs stream processed by the engine of the MAGE environment */                              
MAGE_PDFs * MAGE_getPDFs(MAGE_Environment *environment)
{
	/* pHTS_getPDFs: get PDFs */
	HTS_PStreamSet *pss = pHTS_getPDFs(environment->engine, environment->data);
	return ( pss );
}

/* MAGE_setPDFs: set a different PDFs stream in the engine of the MAGE environment */                              
void MAGE_setPDFs(MAGE_Environment *environment, MAGE_PDFs *pdfs)
{
	/* pHTS_setPDFs: set PDFs */
	pHTS_setPDFs(environment->engine, environment->data, pdfs);
}

/* MAGE_getFilter: get the current stream with the generated speech parameter processed by the engine of the MAGE environment */                              
MAGE_Filter * MAGE_getFilter(MAGE_Environment *environment)
{
	/* pHTS_getFilter: get filter coefficients */
	HTS_GStreamSet * gss = pHTS_getFilter(environment->engine, environment->data);
	return ( gss );
}

/* MAGE_setFilter: set a different stream with the generated speech parameters in the engine of the MAGE environment */  
void MAGE_setFilter( MAGE_Environment *environment, MAGE_Filter *filter )
{
	/* pHTS_setFilter: set filter coefficients */
	pHTS_setFilter(environment->engine, environment->data, filter);
}


/* ---------------------------------------- Audio routine ----------------------------------------- */

/* MAGE_getSamples: get the speech samples */
void MAGE_getSamples(MAGE_Environment *environment, short *buffer, int nOfSamples)
{
	/* pHTS_getSamples: get the speech samples */
	pHTS_getSamples(environment->engine, environment->data, buffer,  nOfSamples);
}

/* MAGE_popSamples : access and pop speech samples from ring buffer ~ to be called from audio callback */
void MAGE_popSamples(MAGE_Environment *environment, float *buffer, int nOfSamples) 
{
	/* pHTS_popSamples : access and pop speech samples from ring buffer ~ to be called from audio callback */
	pHTS_popSamples(environment->engine, environment->data, buffer,  nOfSamples);
}


/* -------------------------------------------- Exiting ------------------------------------------- */

/* MAGE_finalize: flushes the delayed labels */
void MAGE_finalize( MAGE_Environment *environment )
{
	/* pHTS_finalize: flushes the delayed labels */
	pHTS_finalize(environment->engine, environment->data);
}

/* MAGE_refresh : free model per one time synthesis */
void MAGE_refresh(MAGE_Environment *environment)
{
	/* pHTS_refresh: free model per one time synthesis */
	pHTS_refresh(environment->engine, environment->data);
}

/* MAGE_free: free alocated memory */
void MAGE_free(MAGE_Environment *environment)
{
	/* pHTS_free: free alocated memory */
	pHTS_free(environment->engine, environment->data);
}


/* ---------------------------------------- specific usage ---------------------------------------- */
/* MAGE_getLabelFromFile: get one label string from a label file */
void * MAGE_getLabelFromFile(MAGE_Environment *environment)
{
	/* pHTS_getLabelFromFile: get one label string from a label file */
	char * label = pHTS_getLabelFromFile(environment->engine, environment->data);
	return(label);
}

