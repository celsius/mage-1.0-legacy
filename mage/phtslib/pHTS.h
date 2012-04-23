/* ----------------------------------------------------------------------------------------------- */
/*                                                                                                 */
/*  This file is part of MAGE / pHTS (the performative HMM-based speech synthesis system)          */
/*                                                                                                 */
/*  MAGE / pHTS is free software: you can redistribute it and/or modify it under the terms         */
/*  of the GNU General Public License as published by the Free Software Foundation, either         */
/*  version 3 of the license, or any later version.                                                */
/*                                                                                                 */
/*  MAGE / pHTS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;       */   
/*  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.      */
/*  See the GNU General Public License for more details.                                           */
/*                                                                                                 */   
/*  You should have received a copy of the GNU General Public License along with MAGE / pHTS.      */
/*  If not, see http://www.gnu.org/licenses/                                                       */
/*                                                                                                 */
/*                                                                                                 */   
/*  Copyright 2011 University of Mons :                                                            */
/*                                                                                                 */   
/*         Numediart Institute for New Media Art Technology (www.numediart.org)	        		   */
/*         Acapela Group (www.acapela-group.com)                                                   */
/*                                                                                                 */
/*                                                                                                 */
/*   Developed by :                                                                                */
/*                                                                                                 */
/*        Maria Astrinaki, Geoffrey Wilfart, Alexis Moinet, Nicolas d'Alessandro, Thierry Dutoit   */
/*                                                                                                 */
/*   based on  The HMM-Based Speech Synthesis System (HTS)                                         */
/*           		  developed by HTS Working Group                                               */
/*                                                                                                 */
/*   Copyright 2001-2011 Nagoya Institute of Technology                                            */
/*                    Department of Computer Science                                               */
/*                                                                                                 */
/*   Copyright 2001-2008 Tokyo Institute of Technology                                             */
/*                    Interdisciplinary Graduate School of Science and Engineering                 */
/*                                                                                                 */
/*                                                                                                 */
/* Redistribution and use in source and binary forms, with or                                      */
/* without modification, are permitted provided that the following                                 */
/* conditions are met:                                                                             */
/*                                                                                                 */
/* - Redistributions of source code must retain the above copyright                                */
/*   notice, this list of conditions and the following disclaimer.                                 */
/* - Redistributions in binary form must reproduce the above                                       */
/*   copyright notice, this list of conditions and the following                                   */
/*   disclaimer in the documentation and/or other materials provided                               */
/*   with the distribution.                                                                        */
/* - Neither the name of the HTS working group nor the names of its                                */
/*   contributors may be used to endorse or promote products derived                               */
/*   from this software without specific prior written permission.                                 */
/*                                                                                                 */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND                                          */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,                                     */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF                                        */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                                        */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS                               */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,                                        */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED                                 */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,                                   */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON                               */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,                                 */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY                                  */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE                                         */
/* POSSIBILITY OF SUCH DAMAGE.                                                                     */
/* ----------------------------------------------------------------------------------------------- */


#ifndef __pHTS
#define __pHTS 1

#include <math.h>				/* for exp(),log() */
#include <stdlib.h>             /* for exit(),calloc(),free() */
#include <stdarg.h>				/* for va_start(), va_end() */

#include "HTS_engine.h"
#include "p_HTS_engine.h"
#include "pa_ringbuffer.h"			/* for the label and audio RingBuffers */

/**
 *	\def LABEL_BUFFER_SIZE 
 *	\brief A macro that defines the number of labels kept in the label ring buffer.
 */
#define LABEL_BUFFER_SIZE 32

/**
 *	\def PHTS_AUDIO_RINGBUFFER_SIZE 
 *	\brief A macro that defines the number of samples kept in the audio ring buffer.
 */

#ifdef MAGE_REACTIVE
#define PHTS_AUDIO_RINGBUFFER_SIZE  4096
#else
#define PHTS_AUDIO_RINGBUFFER_SIZE  65536
#endif


/** 
 *	\file pHTS.h 
 *	\brief Contains the decleration of all the streaming functions of the streaming pHTS environment.
 *
 *	The functions here, are the streaming version of the original HTS_Engine functions combined with the streaming pHTS environment.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------- environment ---------------------------------------------- */

/**
 *	\struct pHTS 
 *	\var typedef struct _pHTS pHTS
 *	\brief A type definition for the pHTS environment.
 *
 *	The pHTS struct contains all the passed arguments for the engine, as well as the vocoder, and the variables needed for the streaming processes.
 */
/* pHTS: structure for setting all the default values of the engine */
typedef struct _pHTS 
{
#ifndef HTS_EMBEDDED
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	File pointer to the file where the produced log f0 will be saved.
	 */
	FILE *p_lf0fp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	File pointer to the file where the produced spectrum will be saved.
	 */
	FILE *p_mcpfp;
#endif                          /* !HTS_EMBEDDED */
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	File pointer to the file where the produced labels with time will be saved.
	 */
	FILE *p_durfp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	File pointer to the file where the general engine information will be saved.
	 */
	FILE *p_tracefp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	File pointer to the file where the produced speech samples will be saved.
	 */
	FILE *p_rawfp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	File pointer to the file where the produced wave file from the speech samples will be saved.
	 */
	FILE *p_wavfp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	File pointer to the file where the precomputed labels are stored, in order to be loaded.
	 */
	FILE *p_labels;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the speakers for interpolation.
	 *  Not mandatory as a command line argument.
	 */
	/* number of speakers for interpolation */
	int num_interp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer array for the speakers interpolation.
	 *  Not mandatory as a command line argument.
	 */
	double *rate_interp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the log f0 model is stored.
	 *	@attention Mandatory as a command line argument.
	 */
	/* file names of models */
	char **fn_ms_lf0;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the spectrum model is stored.
	 *	@attention Mandatory as a command line argument.
	 */
	char **fn_ms_mcp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the duration model is stored.
	 *	@attention Mandatory as a command line argument.
	 */
	char **fn_ms_dur;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the log f0 models for interpolation.
	 */
	/* number of each models for interpolation */
	int num_ms_lf0;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the spectrum models for interpolation.
	 */
	int num_ms_mcp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the duration models for interpolation.
	 */
	int num_ms_dur;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the log f0 tree is stored.
	 *	@attention Mandatory as a command line argument.
	 */
	/* file names of trees */
	char **fn_ts_lf0;

	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the spectrum tree is stored.
	 *	@attention Mandatory as a command line argument.
	 */
	char **fn_ts_mcp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the duration tree is stored.
	 *	@attention Mandatory as a command line argument.
	 */
	char **fn_ts_dur;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the log f0 trees for interpolation.
	 */
	/* number of each trees for interpolation */
	int num_ts_lf0;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the spectrum trees for interpolation.
	 */
	int num_ts_mcp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the duration trees for interpolation.
	 */
	int num_ts_dur;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the log f0 windows are stored.
	 *	@attention Mandatory as a command line argument.
	 */
	/* file names of windows */
	char **fn_ws_lf0;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the spectrum windows are stored.
	 *	@attention Mandatory as a command line argument.
	 */
	char **fn_ws_mcp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the log f0 windows.
	 */
	/* number of windows */
	int num_ws_lf0;

	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the spectrum windows.
	 */
	int num_ws_mcp;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the global variance for log f0 is stored.
	 *	@attention Mandatory as a command line argument.
	 */
	/* file names of global variance */
	char **fn_ms_gvl;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the global variance for spectrum is stored.
	 *	@attention Mandatory as a command line argument.
	 */
	char **fn_ms_gvm;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the global variance for log f0.
	 */
	/* number of global variances */
	int num_ms_gvl;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the global variance for spectrum.
	 */
	int num_ms_gvm;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the sampling rate.
	 *
	 *  @attention Default value is 16000.
	 */
	/* global parameter */
	int sampling_rate;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the frame period.
	 *
	 *  @attention Default value is 80.
	 */
	int fperiod;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the all-pass constant.
	 *
	 *  @attention Default value is 0.42.
	 */
	double alpha;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the stage.
	 *
	 *  @attention Default value is 0.
	 */
	double stage;          /* gamma = -1.0/stage */
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the postfiltering coefficient.
	 *
	 *  @attention Default value is 0.0. If you set beta large value, 
	 *			   formant structure will be emphasized strongly.
	 */
	double beta;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the voiced/unvoiced threshold.
	 */
	double uv_threshold;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the global variance weight for the log f0.
	 */
	double gv_weight_lf0;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the global variance weight for the spectrum.
	 */
	double gv_weight_mcp;
	
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the standard deviation for the log f0.
	 */
	double f0_std;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the standard deviation for the spectrum.
	 */
	double f0_mean;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the alignment of the phonemes (0 for false or 1 for true).
	 */
	HTS_Boolean phoneme_alignment;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the speech of the produced speech.
	 */
	double speech_speed;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for use the log gain (0 for false or 1 for true).
	 */
	HTS_Boolean use_log_gain;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Char pointer to the file name where the precomputed labels are stored, in order to be loaded.
	 *
	 *  @attention Mandatory as a command line argument.
	 */
	char *labfn;	
	
//--------------------------> pHTS
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of future phonetic labels that will be used for the co-articulation. 
	 *	If delay < 0 the pHTS behaves in the same way as the original HTS, waiting for all the phonemes to 
	 *	arrive before synthesising the speech samples.
	 */
	/* delay of labels */
	int delay;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the new pitch value to be used in the pitch trajectory of the engine.
	 */
	/* pitch */
	double pitch;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer to determine the action applied on the pitch trajectory, if pitch_mode=0, the existing trajectory 
	 *  is overwritten with the value of pitch, if pitch_mode=1, the existing trajectory is shifted up if pitch>0 
	 *	or shifted down if pitch<0.
	 */	
	int pitch_mode;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the duration value to be used in the duration trajectory of the engine.
	 */
	/* duration */
	double duration;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer to determine the action applied on the duration trajectory, if duration_mode=0, the existing trajectory 
	 *	is overwritten with the value of duration, if duration_mode=1, the existing trajectory is shifted up if duration>0 
	 *	or shifted down if duration<0, if duration_mode=2, the existing trajectory is scaled.
	 */	
	int duration_mode;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Double for the volume value to be used in the generation of the speech samples.
	 */	
	/* volume */
	double volume;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer for the number of the phonetic labels that will be kept im memory before refreshing the engine.
	 *	if reset == FALSE then the delayed labels will not be estimated/synthesised, 
	 *	if reset == TRUE, then for each # labels + the delayed labels will be estimated/synthesised.
	 */
	HTS_Boolean reset;	
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	HTS_Vocoder for the synthesis of the speech samples.
	 */
	/* the vocoder */
	HTS_Vocoder vocoder;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer array for the number of frames for all the labels.
	 */
	/* total frame_array */ 
	int *total_frame_array;        
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	Integer array for the length of all the streams.
	 */
	/* previous stream length array */  
	int *length_array;    
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	RingBuffer for storing the incomming labels to be processed.
	 */
	RingBuffer *labelRingBuffer;
	
	/** 
	 *  An pHTS environment variable. 
	 *	
	 *	RingBuffer for storing the speech samples before the audio callback().
	 */
	RingBuffer *audioRingBuffer; 
		
//--------------------------> pHTS

} pHTS;


/* --------------------------------------------------------- environment methods --------------------------------------------------------- */
	

/* --------------------------------------------- initialization method --------------------------------------------- */

/** 
 * A streaming environment method to initialize all environment variables. 
 *
 * @param num_arg an integer for the number of arguments (argc). 
 * @param arguments a character pointer to all arguments (argv). 
 * @param engine a HTS_Engine pointer to be initialized. 
 * @param p a pHTS pointer to be initialized. 
 *
 * @attention To start the pHTS environment, first you must call this function.
 */
/* pHTS_initialize: initialize all variables */
void pHTS_initialize (int num_arg,  char **arguments, HTS_Engine *engine, pHTS *p);

/* ----------------------------------- updating the internal state of the engine ----------------------------------- */

/** 
 * A streaming environment method to set a new label into the labels set in order to be prosessed. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated. 
 * @param label character pointer to a phonetic label.  
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_pushLabel: set a new label into the labels set in order to be prosessed */
void pHTS_pushLabel(HTS_Engine *engine, pHTS *p, char *label);

/** 
 * A streaming environment method to parse the last inserted label. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated. 
 * @return an integer, 0 if the label ring buffer is full, and -1 is there is still space.
 *
 * @attention You must push a label in the pHTS environment using pHTS_pushLabel before calling this function.
 */
/* pHTS_parseLabel: parse the last inserted label */
int pHTS_parseLabel(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to generate the speech samples from PDFs. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated. 
 *
 * @attention You must parse the last pushed label in the pHTS environment using pHTS_parseLabel before calling this function.
 */
/* pHTS_updateEngine: generate the speech samples from PDFs */
void pHTS_updateEngine(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to generate the PDFs. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated. 
 *
 * @attention You must parse the last pushed label in the pHTS environment using pHTS_parseLabel before calling this function.
 */
/* pHTS_updatePDFs: generate PDFs */
void pHTS_updatePDFs(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to generate the speech parameters from the PDFs. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated. 
 *
 * @attention You must generate the PDFs of the last parsed label in the pHTS environment using pHTS_updatePDFs before calling this function.
 */
/* pHTS_updateFilter: generate the speech parameters from the PDFs */
void pHTS_updateFilter(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to generate the speech samples from the speech parameters. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated. 
 *
 * @attention You must generate the speech parameters from the PDFs of the last parsed label in the pHTS environment using pHTS_updateFilter before calling this function.
 */
/* pHTS_updateSamples: generate speech samples from the speech parameters */
void pHTS_updateSamples(HTS_Engine *engine, pHTS *p);


/* ----------------------------------- gettting the internal state of the engine ----------------------------------- */

/** 
 * A streaming environment method to get the number of labels from a label file. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return an integer for the number of labels from a label file.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_getNumberOfLabels: get the # of labels from label file */
int pHTS_getNumberOfLabels(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to get the number of the generated samples. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return an integer for the number of the generated samples.
 *
 * @attention You must generate the speech samples of the last parsed label in the pHTS environment using pHTS_updateSamples before calling this function.
 */
/* pHTS_getNumberOfSamples: get the number of the generated samples */
int pHTS_getNumberOfSamples(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to get the generated speech samples. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @param buffer short pointer to the buffer to be filled with the generated samples. 
 * @param nOfSamples an integer for the number of samples to be copied in in the buffer.
 *
 * @attention You must generate the speech samples of the last parsed label in the pHTS environment using pHTS_updateSamples before calling this function.
 */
/* pHTS_getSamples: get the generated speech samples */
void pHTS_getSamples(HTS_Engine *engine, pHTS *p, short *buffer, int nOfSamples);
	
/** 
 * A streaming environment method to access and pop the generated speech samples from ring buffer. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @param buffer float pointer to the buffer to be filled with the generated samples. 
 * @param nOfSamples an integer for the number of samples to be poped out of the ring buffer.
 *
 * @attention Must be called from audio callback
 */
/* pHTS_popSamples : access and pop speech samples from ring buffer ~ to be called from audio callback */
void pHTS_popSamples(HTS_Engine *engine, pHTS *p, float *buffer, int nOfSamples);

/** 
 * A streaming environment method to get the last label from the label set. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a HTS_Label pointer to the last label from the label set.
 *
 * @attention You must push a label in the pHTS environment using pHTS_pushLabel before calling this function.
 */
/* pHTS_getLabels: get last Label */
HTS_LabelString * pHTS_getLabel(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to get the set of PDF streams. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a HTS_Label pointer to the PDF stream set.
 *
 * @attention You must generate the PDFs of the last parsed label in the pHTS environment using pHTS_updatePDFs before calling this function.
 */
/* pHTS_getPDFs: get PDFs */
HTS_PStreamSet * pHTS_getPDFs(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to get the set of generated parameter stream. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a HTS_GStreamSet pointer to the set of generated parameter stream.
 *
 * @attention You must generate the speech parameters from the PDFs of the last parsed label in the pHTS environment using pHTS_updateFilter before calling this function.
 */
/* pHTS_getFilter: get filter coefficients */
HTS_GStreamSet * pHTS_getFilter(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to get the speech speed rate. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a double for the speech speed rate.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_getSpeed: get speech speed rate */
double pHTS_getSpeed(HTS_Engine *engine,  pHTS *p);

/** 
 * A streaming environment method to get the pitch value. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a double for the pitch value.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_getPitch: get pitch value */
double pHTS_getPitch(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to get the volume value. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a double for the volume value.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_getVolume: get volume value */
double pHTS_getVolume(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to get the alpha value. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a double for the alpha, all-pass constant value.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */	
/* pHTS_getAlpha: get alpha, all-pass constant value */
double pHTS_getAlpha(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to get the duration value. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a double for the duration value.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_getDuration: get the duration value */
double pHTS_getDuration(HTS_Engine *engine, pHTS *p);

/* ----------------------------------- settting the internal state of the engine ----------------------------------- */

/** 
 * A streaming environment method to set a new set of labels. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated.
 * @param label a HTS_Label pointer to the new label set.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize and also create a valid set of labels before calling this function.
 */
/* pHTS_setLabels: set Labels */
void pHTS_setLabels(HTS_Engine *engine, pHTS *p, HTS_Label *label);

/** 
 * A streaming environment method to set a new set of PDF streams. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated.
 * @param pss a HTS_PStreamSet pointer to the new PDF streams set.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize and also create a valid set of PDFs before calling this function.
 */
/* pHTS_setPDFs: set PDFs */
void pHTS_setPDFs(HTS_Engine *engine, pHTS *p, HTS_PStreamSet *pss);

/** 
 * A streaming environment method to set a new set of generated parameter stream. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated.
 * @param gss a HTS_GStreamSet pointer to the new set of generated parameter stream.
 *
 * @attention You must initialize the pHTS environment using pHTS_initialize and also create a valid set of generated parameters before calling this function.
 */
/* pHTS_setFilter: set filter coefficients */
void pHTS_setFilter(HTS_Engine *engine, pHTS *p, HTS_GStreamSet *gss);

/** 
 * A streaming environment method to set different speech speed rate. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated.
 * @param speechSpeed a double for the new speech speed rate.
 *
 * @attention Values less than zero are ignored. Default value is 1.
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_setSpeed: set speech speed rate */
void pHTS_setSpeed(HTS_Engine *engine,  pHTS *p, double speechSpeed);

/** 
 * A streaming environment method to set different pitch value in the generated speech parameters. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated.
 * @param pitch a double for the new pitch value.
 * @param mode an integer to determine the action applied on the pitch trajectory, if mode=0, the existing trajectory 
 *             is overwritten with the value of pitch, if mode=1, the existing trajectory is shifted up if pitch>0 or shifted down if pitch<0.
 *
 * @attention the engine is using log f0.
 * @attention values less than zero are ignored if mode=0.
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_setPitch: set a different pitch value in the generated speech parameters */
void pHTS_setPitch(HTS_Engine *engine, pHTS *p, double pitch, int mode);

/** 
 * A streaming environment method to set different volume. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated.
 * @param volume a double for the new volume.
 *
 * @attention Values less than zero are ignored. Default value is 1.
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_setVolume: set volume */
void pHTS_setVolume(HTS_Engine *engine, pHTS *p, double volume);
	
/** 
 * A streaming environment method to set different alpha value, the all-pass constant. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated.
 * @param alpha a double for the new alpha, all-pass constant value.
 *
 * @attention Values less than zero or greater then one are ignored. Default value is 0.42.
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_setAlpha: set alpha, the all-pass constant */	
void pHTS_setAlpha(HTS_Engine *engine, pHTS *p, double alpha);

/** 
 * A streaming environment method to set different duration value in the generated speech parameters. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param p a pHTS pointer to be updated.
 * @param duration a double for the new duration value.
 * @param mode an integer to determine the action applied on the duration trajectory, if mode=0, the existing trajectory 
 *             is overwritten with the value of duration, if mode=1, the existing trajectory is shifted up if duration>0 
 *			   or shifted down if duration<0, if mode=2, the existing trajectory is scaled.
 *
 * @attention Values less than zero are ignored if mode=0 or mode=2.
 * @attention You must initialize the pHTS environment using pHTS_initialize before calling this function.
 */
/* pHTS_setDuration: set a different duration value in the generated speech parameters */
void pHTS_setDuration(HTS_Engine *engine, pHTS *p, double duration, int mode);


/* ---------------------------------------------- load / save / free ----------------------------------------------- */

/** 
 * A streaming environment method to get one label string from a label file. 
 *
 * @param engine a HTS_Engine pointer to be accessed. 
 * @param p a pHTS pointer to be accessed.
 * @return a pointer to the new label string retrieved from the label file.
 *
 * @attention You must initialize the pHTS environment and the HTS engine using pHTS_initialize before calling this function.
 */
/* pHTS_getLabelFromFile: get one label string from a label file */
char * pHTS_getLabelFromFile(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to flush the delayed labels. 
 *
 * @param engine a HTS_Engine pointer to be accessed and refreshed. 
 * @param p a pHTS pointer to be accessed. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine using pHTS_initialize before calling this function.
 */
/* pHTS_finalize: flushes the delayed labels */
void pHTS_finalize(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to flush the internal memory of the engine occupied by the pHTS environment. 
 *
 * @param engine a HTS_Engine pointer to be accessed and refreshed. 
 * @param p a pHTS pointer to be accessed. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine using pHTS_initialize before calling this function.
 */
/* pHTS_refresh: flushes the internal memory */
void pHTS_refresh(HTS_Engine *engine, pHTS *p);

/** 
 * A streaming environment method to free all the alocated memory. 
 *
 * @param engine a HTS_Engine pointer to be freed. 
 * @param p a pHTS pointer to be accessed.
 *
 * @attention You must initialize the pHTS environment and the HTS engine using pHTS_initialize before calling this function.
 */
/* pHTS_free: free alocated memory */
void pHTS_free(HTS_Engine *engine, pHTS *p);
	
#ifdef __cplusplus
}
#endif

#endif//__pHTS
