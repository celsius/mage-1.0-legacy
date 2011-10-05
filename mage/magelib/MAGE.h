
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


#ifndef _MAGE
#define _MAGE 1

/**
 *	\def MAGE_OVERWRITE 
 *	\brief A macro that defines overwriting as the opperation to be applied. 
 */
#define MAGE_OVERWRITE 0

/**
 *	\def MAGE_SHIFT 
 *	\brief A macro that defines shifting as the opperation to be applied. 
 */
#define MAGE_SHIFT 1

/**
 *	\def MAGE_SCALE 
 *	\brief A macro that defines scaling as the opperation to be applied. 
 */
#define MAGE_SCALE 2

#include "pHTS.h"

/** 
 *	\file MAGE.h 
 *	\brief Contains the decleration of all the functions of the threading MAGE_Environment.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------- MAGE ARGUMENTS ---------------------------------------- */



/* ------------------------------------------ MAGE TYPES ------------------------------------------ */

/**
 *	\var typedef void MAGE_Engine
 *	\brief A generic type for identifiying the engine used in the MAGE_Environment.
 */
typedef void MAGE_Engine;  

/**
 *	\var typedef void MAGE_Label
 *	\brief A type definition for the Labels used in MAGE_Environment.  
 */
typedef void  MAGE_Label;

/**
 *	\var typedef void MAGE_PDFs
 *	\brief A type definition for the PDFs used in MAGE_Environment.
 */
typedef void MAGE_PDFs;

/**
 *	\var typedef void MAGE_Filter
 *	\brief A type definition for the generated speech parameters used in MAGE_Environment.
 */
typedef void MAGE_Filter;

/**
 *	\struct MAGE_Environment 
 *	\var typedef struct _MAGE_Environment MAGE_Environment
 *
 *	\brief A type definition for the MAGE_Environment, that is the main data type that is  
 *		   shared through all MAGE functions.
 *
 *	The MAGE_Environment struct contains all the passed arguments for the 
 *	MAGE_Engine and the variables needed for the threading environment. 
 */
typedef struct _MAGE_Environment
{
	/** 
	 *  An MAGE_Environment variable. 
	 *	
	 *	MAGE_Engine pointer to the engine used.
	 */
 	MAGE_Engine *engine;
	
	/** 
	 *  An MAGE_Environment variable. 
	 *	
	 *	Void pointer to to any non-engine specific data used.
	 */
 	void * data;
	
}MAGE_Environment;  


/* ------------------------------------------------------------------ MAGE FUNCTIONS ------------------------------------------------------------------ */

/* ---------------------------------------- Initialization ---------------------------------------- */

/** 
 * A MAGE_Environment method to initialize the MAGE_Environment and returns its pointer. 
 *
 * @param argc an integer for the number of arguments. 
 * @param argv a character pointer to all arguments. 
 * @return the initialized MAGE_Environment pointer. 
 */
/* MAGE_init: initialize all variables and return the environment */  
MAGE_Environment * MAGE_init(int argc, char **argv); 



/* --------------------------- Input interface to the engine : phonetics -------------------------- */

/** 
 * A MAGE_Environment method to push a new string label recevied from the program 
 * ( from a file or on the fly ) onto the label queue of the MAGE_Environment.
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @param labelAsStr character pointer to a phonetic label. 
 */
/* MAGE_pushLabel: set a new label into the labels set in order to be prosessed */
void MAGE_pushLabel(MAGE_Environment *environment, char *labelAsStr);


/** 
 * A MAGE_Environment method to parse the the last pushed label in the list of labels of  
 * the MAGE_Environment and determine state duration. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 */
/* MAGE_parseLabel: parse the last inserted label */  
int MAGE_parseLabel(MAGE_Environment *environment);



/* --------------------------- Input interface to the engine : prosody ---------------------------- */

/** 
 * A MAGE_Environment method to set the speech speed rate in the MAGE_Environment to a new value. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @param speechSpeed a double for the new speech speed rate.
 */
/* MAGE_setSpeed: set speech speed rate */ 
void MAGE_setSpeed( MAGE_Environment *environment, double speechSpeed );


/** 
 * A MAGE_Environment method to get the current speech speed from the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @return a double with the current speech speed from the MAGE_Environment.
 */
/* MAGE_getSpeed: get speech speed rate */ 
double MAGE_getSpeed( MAGE_Environment *environment );


/** 
 * A MAGE_Environment method to set the pitch in the MAGE_Environment to a new value. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @param pitch a double for the new pitch value.
 * @param mode an integer to determine the action applied on the pitch trajectory, if mode=0, the existing trajectory 
 *             is overwritten with the value of pitch, if mode=1, the existing trajectory is shifted up if pitch>0 or shifted down if pitch<0.
 */
/* MAGE_setPitch: set a different pitch value in the generated speech parameters */    
void MAGE_setPitch( MAGE_Environment *environment, double pitch, int mode );

/** 
 * A MAGE_Environment method to get the current pitch from the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @return a double with the current pitch from the MAGE_Environment.
 */
/* MAGE_getPitch: get current pitch */ 
double MAGE_getPitch( MAGE_Environment *environment );
	
/** 
 * A MAGE_Environment method to set the current alpha, all-pass constant value in the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @param alpha a double for the new alpha value.
 */	
/* MAGE_setAlpha: set alpha, all-pass constant value in the MAGE_Environment */	
void MAGE_setAlpha( MAGE_Environment *environment, double alpha );

/** 
 * A MAGE_Environment method to get the current alpha, all-pass constant value from the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @return a double with the current alpha, all-pass constant value from the MAGE_Environment.
 */
/* MAGE_getAlpha: get current alpha, all-pass constant value */ 
double MAGE_getAlpha( MAGE_Environment *environment );
	
/** 
 * A MAGE_Environment method to set the volume value in the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @param volume a double for the new volume value.
 */	
/* MAGE_setVolume: set volume value in the MAGE_Environment */		
void MAGE_setVolume( MAGE_Environment *environment, double volume );

/** 
 * A MAGE_Environment method to get the volume value from the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @return a double with the current volume value from the MAGE_Environment.
 */
/* MAGE_getVolume: get current volume value */ 
double MAGE_getVolume( MAGE_Environment *environment );


/** 
 * A MAGE_Environment method to set the duration in the MAGE_Environment to a new value. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @param duration a double for the new duration value.
 * @param mode an integer to determine the action applied on the duration trajectory, if mode=0, the existing trajectory 
 *             is overwritten with the value of duration, if mode=1, the existing trajectory is shifted up if duration>0 
 *			   or shifted down if duration<0, if mode=2, the existing trajectory is scaled.
 
 */
/* MAGE_setDuration: set a different duration value */  
void MAGE_setDuration(MAGE_Environment *environment, double duration, int mode);


/** 
 * A MAGE_Environment method to get the current duration from the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 * @return a double with the current duration from the MAGE_Environment.
 */
/* MAGE_getDuration: get current duration */ 
double MAGE_getDuration( MAGE_Environment *environment );



/* ---------------------------- Updating internal state of the engine ----------------------------- */

/** 
 * A MAGE_Environment method to generate the speech samples. This function is a high-level 
 * call for converting first queued label and current prosody into sound.
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 */
/* MAGE_update: generate the speech samples */ 
void MAGE_update(MAGE_Environment *environment);


/** 
 * A MAGE_Environment method to convert first queued label into PDFs. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 */
/* MAGE_updatePDFs: generate PDFs */   
void MAGE_updatePDFs(MAGE_Environment *environment);  


/** 
 * A MAGE_Environment method to convert current PDFs into speech parameters. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 */
/* MAGE_updateFilter: generate the speech parameters */ 
void MAGE_updateFilter(MAGE_Environment *environment);   


/** 
 * A MAGE_Environment method to generate speech samples from the speech parameters. 
 *
 * @param environment a MAGE_Environment pointer to be updated. 
 */
/* MAGE_updateSamples: generate speech samples from the speech parameters */ 
void MAGE_updateSamples(MAGE_Environment *environment);




/* ----------------------------- Accessing engine internal data types ----------------------------- */

/** 
 * A MAGE_Environment method to get the current number of labels queued in the engine of 
 * the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be accessed.
 * @return an integer with the current number of labels queued in the engine of the 
 *		   MAGE_Environment.
 */
/* MAGE_getNumberOfLabels: get the # of labels queued in the engine of the MAGE_Environment */ 
int MAGE_getNumberOfLabels(MAGE_Environment *environment);


/** 
 * A MAGE_Environment method to get the current number of generated samples queued in the engine 
 * of the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be accessed.
 * @return an integer with the current number of generated samples queued in the engine of 
 *		   the MAGE_Environment.
 */
/* MAGE_getNumberOfSamples: get the current number of generated samples queued in the engine */
int MAGE_getNumberOfSamples(MAGE_Environment *environment);


/** 
 * A MAGE_Environment method to get the current label processed by the engine of 
 * the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be accessed.
 * @return a void pointer to the current label processed by the engine of the MAGE_Environment.
 */
/* MAGE_getLabel: get the current label processed by the engine of the MAGE_Environment */                              
MAGE_Label * MAGE_getLabel(MAGE_Environment *environment);


/** 
 * A MAGE_Environment method to get the current PDFs set processed by the engine of the 
 * MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be accessed.
 * @return a void pointer to the current PDFs set processed by the engine of the MAGE_Environment.
 */
/* MAGE_getPDFs: get the current PDFs stream processed by the engine of the MAGE_Environment */                              
MAGE_PDFs * MAGE_getPDFs(MAGE_Environment *environment); 


/** 
 * A MAGE_Environment method to override the current PDFs set processed by the engine of the 
 * MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be accessed.
 * @param pdfs a MAGE_PDFs pointer to override the current PDFs set.
 */
/* MAGE_setPDFs: set a different PDFs stream in the engine of the MAGE_Environment */                              
void MAGE_setPDFs( MAGE_Environment *environment, MAGE_PDFs *pdfs );


/** 
 * A MAGE_Environment method to get the current generated speech parameters set of 
 * the engine of the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be accessed.
 * @return a void pointer to the current generated speech parameters set of the engine 
 *		   of the MAGE_Environment.
 */
/* MAGE_getFilter: get the current stream with the generated speech parameter processed by the engine of the MAGE_Environment */                              
MAGE_Filter *MAGE_getFilter( MAGE_Environment *environment );


/** 
 * A MAGE_Environment method to override the current set of generated speech parameters 
 * processed by the engine of the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be accessed.
 * @param filter a MAGE_Filter pointer to override the current set of generated speech parameters.
 */
/* MAGE_setFilter: set a different stream with the generated speech parameters in the engine of the MAGE environment */  
void MAGE_setFilter( MAGE_Environment *environment, MAGE_Filter *filter );



/* ---------------------------------------- Audio routine ----------------------------------------- */

/** 
 * A MAGE_Environment method to acces the generated speech samples. 
 *
 * @param environment a MAGE_Environment pointer to be accessed. 
 * @param buffer a short array to store the generated speech samples.
 * @param nOfSamples an integer for the number of dumples to be accessed.
 */
/* MAGE_getSamples: access the speech samples */
void MAGE_getSamples(MAGE_Environment *environment, short *buffer, int nOfSamples);

/** 
 * A MAGE_Environment method to access and pop the generated speech samples from MAGE ring buffer. 
 *
 * @param environment a MAGE_Environment pointer to be accessed. 
 * @param buffer float pointer to the buffer to be filled with the generated samples. 
 * @param nOfSamples an integer for the number of samples to be poped out of the ring buffer.
 *
 * @attention Must be called from audio callback
 */
/* MAGE_popSamples : access and pop speech samples from ring buffer ~ to be called from audio callback */
void MAGE_popSamples(MAGE_Environment *environment, float *buffer, int nOfSamples);



/* -------------------------------------------- Exiting ------------------------------------------- */

/** 
 * A MAGE_Environment method to to flush the delayed labels.
 *
 * @param environment a MAGE_Environment pointer to be flushed. 
 */
/* MAGE_finalize: flushes the delayed labels */	
void MAGE_finalize( MAGE_Environment *environment );
	
/** 
 * A MAGE_Environment method to flush the internal memory of the engine occupied by the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be flushed. 
 *
 */
/* MAGE_refresh: flushes the internal memory */
void MAGE_refresh( MAGE_Environment *environment );


/** 
 * A MAGE_Environment method to free all the alocated memory occupied by the MAGE_Environment. 
 *
 * @param environment a MAGE_Environment pointer to be freed. 
 */
/* MAGE_free: free alocated memory */
void MAGE_free(MAGE_Environment *environment);
	
	
/* ---------------------------------------- specific usage ---------------------------------------- */
	
/** 
 * A MAGE_Environment method to get one label string from a label file. 
 *
 * @param environment a MAGE_Environment pointer to be freed. 
 * @return a pointer to the new label string retrieved from the label file.
 *
 */
/* MAGE_getLabelFromFile: get one label string from a label file */
void * MAGE_getLabelFromFile(MAGE_Environment *environment);
	
#ifdef __cplusplus
}
#endif

#endif //_MAGE