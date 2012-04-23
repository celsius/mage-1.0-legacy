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


#include <stdlib.h> /* use of realloc() */

/* hts_engine libraries */
#include "HTS_hidden.h"

/** 
 *	\file p_HTS_gstream.c 
 *	\brief Contains the implementation of all the streaming functions of the HTS_Engine for the set of the generated parameter stream.
 *
 *	The functions implemented here, are the streaming version of the original gsteam functions of the HTS_Engine.
 */

/* p_HTS_GStreamSet_initialize: initialize generated parameter stream set */
void p_HTS_GStreamSet_initialize(HTS_GStreamSet *gss)
{
	gss->nstream = 0;
	gss->total_frame = 0;
	gss->total_nsample = 0;
#ifndef HTS_EMBEDDED
	gss->gstream = NULL;
#endif                          /* !HTS_EMBEDDED */
	gss->gspeech = NULL;
}

/* p_HTS_GStreamSet_create_param: generate speech parameters */
/* (stream[0] == spectrum && stream[1] == lf0) */
void p_HTS_GStreamSet_create_param(HTS_GStreamSet *gss, HTS_PStreamSet *pss, HTS_SStreamSet *sss, HTS_Label *label,
								   int stage, HTS_Boolean use_log_gain, int sampling_rate, int fperiod, double alpha,
								   double beta, int audio_buff_size, int delay, int *total_frame_array, int *length_array, HTS_Boolean reset)
{
	int i, j, k, index = 0, indexEnd = 0;
	int frame_init, msd_frame_init;
	
#ifdef HTS_EMBEDDED
	double lf0;
#endif
	/* HTS_EMBEDDED */
	
	int msd_frame;
	
	/* initialize */
	gss->nstream = HTS_PStreamSet_get_nstream(pss);
	
	index = HTS_Label_get_size(label) - delay - 1;
	indexEnd = HTS_Label_get_size(label) - delay;
	
	if(reset == TRUE)
	{
		index = HTS_Label_get_size(label)  - delay;
		indexEnd = HTS_Label_get_size(label) ;
	}
	
	if (index < 0 || delay < 0)
		index = 0;
	
	if (indexEnd < 0 || delay < 0)
		indexEnd = 0;
	
	frame_init = total_frame_array[index];
	
	if( reset == TRUE || delay == 0 )
		gss->total_frame = HTS_PStreamSet_get_total_frame(pss); 
	else
		gss->total_frame = total_frame_array[indexEnd];
	
	gss->total_nsample = fperiod * gss->total_frame;
	
#ifndef HTS_EMBEDDED
	if(HTS_Label_get_size(label) == 1)
	{
		gss->gstream = (HTS_GStream *) calloc(gss->nstream, sizeof(HTS_GStream));
		for (i = 0; i < gss->nstream; i++)
			gss->gstream[i].par = NULL ;
	}
	
	for (i = 0; i < gss->nstream; i++)
	{
		gss->gstream[i].static_length = HTS_PStreamSet_get_static_length(pss, i);
		gss->gstream[i].par = (double **) realloc(gss->gstream[i].par, gss->total_frame*sizeof(double *));
		
		for (j = frame_init; j < gss->total_frame; j++)
			gss->gstream[i].par[j] = (double *) HTS_calloc(gss->gstream[i].static_length, sizeof(double));
	}
	
#endif   /* !HTS_EMBEDDED */
	if (gss->total_nsample > 0)
		gss->gspeech = (short *) realloc(gss->gspeech, gss->total_nsample*sizeof(short));
#ifndef HTS_EMBEDDED
	
	/* copy generated parameter */
	for (i = 0; i < gss->nstream; i++)
	{
		msd_frame_init = length_array[index];
		
		if (HTS_PStreamSet_is_msd(pss, i))
		{      /* for MSD */
			for (j = frame_init, msd_frame = msd_frame_init; j < gss->total_frame; j++)
			{
				if (HTS_PStreamSet_get_msd_flag(pss, i, j))
				{
					for (k = 0; k < gss->gstream[i].static_length; k++)
						gss->gstream[i].par[j][k] = HTS_PStreamSet_get_parameter(pss, i, msd_frame, k);
					msd_frame++;
				}
				else
				{
					for (k = 0; k < gss->gstream[i].static_length; k++)
						gss->gstream[i].par[j][k] = LZERO;
				}
			}
		}
		else
		{  /* for non MSD */
			for (j = frame_init; j < gss->total_frame; j++)
			{
				for (k = 0; k < gss->gstream[i].static_length; k++)
					gss->gstream[i].par[j][k] = HTS_PStreamSet_get_parameter(pss, i, j, k);
			}
		}
	}
	
#endif  /* !HTS_EMBEDDED */
	
	/* check */
	if (gss->nstream != 2)
		HTS_error(1,"HTS_GStreamSet_create: The number of streams should be 2.\n");
	
	if (HTS_PStreamSet_get_static_length(pss, 1) != 1)
		HTS_error(1, "HTS_GStreamSet_create: The size of lf0 static vector should be 1.\n");
}

/* p_HTS_GStreamSet_create_speech_samples: generate speech samples */
/* (stream[0] == spectrum && stream[1] == lf0) */
void p_HTS_GStreamSet_create_speech_samples(HTS_GStreamSet *gss, HTS_PStreamSet *pss, HTS_SStreamSet *sss, HTS_Label *label,
									int stage, HTS_Boolean use_log_gain, int sampling_rate, int fperiod, double alpha,
									double beta, int audio_buff_size,  int delay, HTS_Vocoder *v, int *total_frame_array)
{
	int i, index, frame_init;
	
	index = HTS_Label_get_size(label) - delay - 1;
	
	if (index < 0 || delay < 0)
		index = 0;
	
	frame_init = total_frame_array[index];
	
	/* synthesize speech waveform */
#ifdef HTS_EMBEDDED
	
	if(HTS_Label_get_size(label) == 1)
		HTS_Vocoder_initialize(v, HTS_PStreamSet_get_static_length(pss, 0) - 1,
							   stage, use_log_gain, sampling_rate, fperiod,
							   audio_buff_size);
	
	for (i = frame_init, msd_frame = msd_frame_init; i < gss->total_frame; i++)
	{
		lf0 = LZERO;
		if (HTS_PStreamSet_get_msd_flag(pss, 1, i))
			lf0 = HTS_PStreamSet_get_parameter(pss, 1, msd_frame++, 0);
		
		HTS_Vocoder_synthesize(v, HTS_PStreamSet_get_static_length(pss, 0) - 1,
							   lf0,
							   HTS_PStreamSet_get_parameter_vector(pss, 0, i),
							   alpha, beta, &gss->gspeech[i * fperiod]);
	}
	
#else
	
	if(HTS_Label_get_size(label) == 1)
		HTS_Vocoder_initialize(v, gss->gstream[0].static_length - 1, stage,
							   use_log_gain, sampling_rate, fperiod,
							   audio_buff_size);
	
	for (i = frame_init; i < gss->total_frame; i++)
	{
		HTS_Vocoder_synthesize(v, gss->gstream[0].static_length - 1,
							   gss->gstream[1].par[i][0],
							   &gss->gstream[0].par[i][0], alpha, beta,
							   &gss->gspeech[i * fperiod]);
	}
	

#endif                          /* HTS_EMBEDDED */
}

/* p_HTS_GStreamSet_create: generate speech parameters & speech samples */
/* (stream[0] == spectrum && stream[1] == lf0) */
void p_HTS_GStreamSet_create(HTS_GStreamSet *gss, HTS_PStreamSet *pss, HTS_SStreamSet *sss, HTS_Label *label,
							 int stage, HTS_Boolean use_log_gain, int sampling_rate, int fperiod, double alpha,
							 double beta, int audio_buff_size, int delay, HTS_Vocoder *v, int *total_frame_array, int *length_array, HTS_Boolean reset)
{
	int i, j, k, index = 0, indexEnd = 0;
	int frame_init, msd_frame_init;
	
#ifdef HTS_EMBEDDED
	double lf0;
#endif
	/* HTS_EMBEDDED */
	
	int msd_frame;
	
	/* initialize */
	gss->nstream = HTS_PStreamSet_get_nstream(pss);
	
	index = HTS_Label_get_size(label) - delay - 1;
	indexEnd = HTS_Label_get_size(label) - delay;
	
	if( reset == TRUE )
	{
		index = HTS_Label_get_size(label)  - delay; 
		indexEnd = HTS_Label_get_size(label) ;
	}

	if (index < 0 || delay < 0)
		index = 0;

	if (indexEnd < 0 || delay < 0)
		indexEnd = 0;
	
	frame_init = total_frame_array[index];

	if( reset == TRUE || delay == 0 )
		gss->total_frame = HTS_PStreamSet_get_total_frame(pss); 
	else
		gss->total_frame = total_frame_array[indexEnd];

	gss->total_nsample = fperiod * gss->total_frame;
	
#ifndef HTS_EMBEDDED
	if(HTS_Label_get_size(label) == 1)
	{
		gss->gstream = (HTS_GStream *) calloc(gss->nstream, sizeof(HTS_GStream));
		for (i = 0; i < gss->nstream; i++)
			gss->gstream[i].par = NULL ;
	}
	
	for (i = 0; i < gss->nstream; i++)
	{
		gss->gstream[i].static_length = HTS_PStreamSet_get_static_length(pss, i);
		gss->gstream[i].par = (double **) realloc(gss->gstream[i].par, gss->total_frame*sizeof(double *));
		
		for (j = frame_init; j < gss->total_frame; j++)
			gss->gstream[i].par[j] = (double *) HTS_calloc(gss->gstream[i].static_length, sizeof(double));
	}
	
#endif   /* !HTS_EMBEDDED */
	if (gss->total_nsample > 0)
		gss->gspeech = (short *) realloc(gss->gspeech, gss->total_nsample*sizeof(short));
#ifndef HTS_EMBEDDED
	
	/* copy generated parameter */
	for (i = 0; i < gss->nstream; i++)
	{
		msd_frame_init = length_array[index];
		
		if (HTS_PStreamSet_is_msd(pss, i))
		{      /* for MSD */
			for (j = frame_init, msd_frame = msd_frame_init; j < gss->total_frame; j++)
			{
				if (HTS_PStreamSet_get_msd_flag(pss, i, j))
				{
					for (k = 0; k < gss->gstream[i].static_length; k++)
						gss->gstream[i].par[j][k] = HTS_PStreamSet_get_parameter(pss, i, msd_frame, k);
					msd_frame++;
				}
				else
				{
					for (k = 0; k < gss->gstream[i].static_length; k++)
						gss->gstream[i].par[j][k] = LZERO;
				}
			}
		}
		else
		{  /* for non MSD */
			for (j = frame_init; j < gss->total_frame; j++)
			{
				for (k = 0; k < gss->gstream[i].static_length; k++)
					gss->gstream[i].par[j][k] = HTS_PStreamSet_get_parameter(pss, i, j, k);
			}
		}
	}
	
#endif  /* !HTS_EMBEDDED */
	
	/* check */
	if (gss->nstream != 2)
		HTS_error(1,"HTS_GStreamSet_create: The number of streams should be 2.\n");
	
	if (HTS_PStreamSet_get_static_length(pss, 1) != 1)
		HTS_error(1, "HTS_GStreamSet_create: The size of lf0 static vector should be 1.\n");
	
	/* synthesize speech waveform */
#ifdef HTS_EMBEDDED
	
	if(HTS_Label_get_size(label) == 1)
		HTS_Vocoder_initialize(v, HTS_PStreamSet_get_static_length(pss, 0) - 1,
							   stage, use_log_gain, sampling_rate, fperiod,
							   audio_buff_size);
	
	for (i = frame_init, msd_frame = msd_frame_init; i < gss->total_frame; i++)
	{
		lf0 = LZERO;
		if (HTS_PStreamSet_get_msd_flag(pss, 1, i))
			lf0 = HTS_PStreamSet_get_parameter(pss, 1, msd_frame++, 0);
		
		HTS_Vocoder_synthesize(v, HTS_PStreamSet_get_static_length(pss, 0) - 1,
							   lf0,
							   HTS_PStreamSet_get_parameter_vector(pss, 0, i),
							   alpha, beta, &gss->gspeech[i * fperiod]);
	}
	
#else
	
	if(HTS_Label_get_size(label) == 1)
		HTS_Vocoder_initialize(v, gss->gstream[0].static_length - 1, stage,
							   use_log_gain, sampling_rate, fperiod,
							   audio_buff_size);
	
	for (i = frame_init; i < gss->total_frame; i++)
	{
		HTS_Vocoder_synthesize(v, gss->gstream[0].static_length - 1,
							   gss->gstream[1].par[i][0],
							   &gss->gstream[0].par[i][0], alpha, beta,
							   &gss->gspeech[i * fperiod]);
	}
#endif                          /* HTS_EMBEDDED */	
}

/* p_HTS_GStreamSet_set_gstream: set different set of generated parameter stream */
void p_HTS_GStreamSet_set_gstream(HTS_GStreamSet *gss,  int total_nsample, int total_frame, int nstream, HTS_GStream *gstream, short *gspeech)
{
	gss->total_nsample = total_nsample;
	gss->total_frame = total_frame;
	gss->nstream = nstream;
	gss->gstream = gstream;
	gss->gspeech = gspeech;
}

/* p_HTS_GStreamSet_set_gstream_pitch: set a different pitch value in the generated speech parameters */
void p_HTS_GStreamSet_set_gstream_pitch(HTS_GStreamSet *gss, HTS_SStreamSet *sss, HTS_Label *label, int delay,  int *total_frame_array, double lf0, int mode)
{
	int i, index, frame_init;
	
	index = HTS_Label_get_size(label) - delay - 1;
	
	if (index < 0 || delay < 0)
		index = 0;
		
	frame_init = total_frame_array[index];
	
	/* change pitch value */
#ifdef HTS_EMBEDDED
	
	/*
	 for (i = frame_init, msd_frame = msd_frame_init; i < gss->total_frame; i++)
	 {
		lf0 = F0;
	 }	
	 */
#else
	
	for (i = frame_init; i < gss->total_frame; i++)
	{
		if( mode == 0 ) // OVERWRITE 0
		{
			if (lf0 <= 0)
				return;
			
			if( lf0 != LZERO && gss->gstream[1].par[i][0] > 0)
				gss->gstream[1].par[i][0] = lf0;   
		}
		else if ( mode == 1 ) // SHIFT 1
		{
			if( lf0 != LZERO && gss->gstream[1].par[i][0] > 0)
				gss->gstream[1].par[i][0] += lf0; 
		}
	}
#endif                          
/* HTS_EMBEDDED */
}
