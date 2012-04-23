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



/* hts_engine libraries */
#include "p_HTS_engine.h"

/** 
 *	\file p_HTS_engine.c 
 *	\brief Contains the implementation of all the streaming functions of the HTS_Engine.
 *
 *	The functions implemented here, are the streaming version of the original HTS_Engine functions.
 */

/* p_HTS_Engine_load_label_from_str: load label from string */
void p_HTS_Engine_load_label_from_str(HTS_Engine *engine,  char *label_str)
{
	/* p_HTS_Label_load_from_str: load label from char pointer */
	p_HTS_Label_load_from_str(&engine->label, engine->global.sampling_rate, engine->global.fperiod, label_str);
}

/* p_HTS_Engine_load_label_from_fp: load label from file pointer */
void p_HTS_Engine_load_label_from_fp(HTS_Engine *engine,  FILE *fp)
{
	/* p_HTS_Label_load_from_fp: load label from file pointer */
	p_HTS_Label_load_from_fp(&engine->label, engine->global.sampling_rate,
							 engine->global.fperiod, fp);
}

/* p_HTS_Engine_create_sstream: parse label and determine state duration */
void p_HTS_Engine_create_sstream(HTS_Engine *engine,  int **total_frame_array, double duration, int mode)
{
	/* p_HTS_SStreamSet_create: parse label and determine state duration */
	p_HTS_SStreamSet_create(&engine->sss, &engine->ms, &engine->label,
							engine->global.duration_iw,
							engine->global.parameter_iw, engine->global.gv_iw, total_frame_array, duration, mode);
}

/* p_HTS_Engine_create_pstream: generate speech parameter vector sequence */
void p_HTS_Engine_create_pstream(HTS_Engine *engine, int *total_frame_array, int **length_array, int past)
{
	/* p_HTS_PStreamSet_create: parameter generation using GV weight */
	p_HTS_PStreamSet_create(&engine->pss, &engine->sss,
							engine->global.msd_threshold,
							engine->global.gv_weight, &engine->label, &engine->ms, total_frame_array, length_array,past);
}

/* p_HTS_Engine_create_gstream_param: generate the speech parameters */
void p_HTS_Engine_create_gstream_param(HTS_Engine *engine, int delay, int *total_frame_array, int *length_array, HTS_Boolean reset)
{
	/* p_HTS_GStreamSet_create_param: generate speech parameters */
	p_HTS_GStreamSet_create_param(&engine->gss, &engine->pss, &engine->sss, &engine->label,
							engine->global.stage,
							engine->global.use_log_gain,
							engine->global.sampling_rate, engine->global.fperiod,
							engine->global.alpha, engine->global.beta,
							engine->global.audio_buff_size, 
							delay, total_frame_array, length_array, reset);
}

/* p_HTS_Engine_create_gstream_speech_samples: generate speech samples */
void p_HTS_Engine_create_gstream_speech_samples(HTS_Engine *engine, int delay, HTS_Vocoder *v, int *total_frame_array)
{
	/* p_HTS_GStreamSet_create_speech_samples: generate speech samples */
	p_HTS_GStreamSet_create_speech_samples(&engine->gss, &engine->pss, &engine->sss, &engine->label,
										   engine->global.stage,
										   engine->global.use_log_gain,
										   engine->global.sampling_rate, engine->global.fperiod,
										   engine->global.alpha, engine->global.beta,
										   engine->global.audio_buff_size, 
										   delay, v, total_frame_array);
}

/* p_HTS_Engine_set_labels: set different set of labels */
void p_HTS_Engine_set_labels(HTS_Engine *engine, HTS_Label *label)
{
	/* p_HTS_Label_set: set different set of labels */
	p_HTS_Label_set(&engine->label,  label->head, label->tail, label->size, label->speech_speed);	
}

/* p_HTS_Engine_set_pstream: set speech parameter vector sequence */
void p_HTS_Engine_set_pstream(HTS_Engine *engine, HTS_PStreamSet *pss)
{
	/* p_HTS_PStreamSet_set: set different set of PDF streams */
	p_HTS_PStreamSet_set(&engine->pss,  pss->pstream, pss->nstream, pss->total_frame);
}

/* p_HTS_Engine_set_gstream: set different speech parameters */
void p_HTS_Engine_set_gstream(HTS_Engine *engine,  HTS_GStreamSet *gss) 
{
	/* p_HTS_GStreamSet_set_gstream: set different set of generated parameter stream */
	p_HTS_GStreamSet_set_gstream(&engine->gss,  gss->total_nsample, gss->total_frame, gss->nstream, gss->gstream, gss->gspeech);
}

/* p_HTS_Engine_set_gstream_pitch: set pitch in the generated speech parameters */
void p_HTS_Engine_set_gstream_pitch(HTS_Engine *engine, int delay, int *total_frame_array, double pitch, int mode)
{
	/* p_HTS_GStreamSet_set_gstream_pitch: set a different pitch value in the generated speech parameters */
	p_HTS_GStreamSet_set_gstream_pitch(&engine->gss, &engine->sss, &engine->label, delay, total_frame_array, pitch, mode);
}

/* !HTS_EMBEDDED */
