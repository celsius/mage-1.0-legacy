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


#include "pHTS.h"
#include <unistd.h>

/** 
 *	\file pHTS.c 
 *	\brief Contains the implementation of all the streaming functions of the streaming pHTS environment.
 */

/*  ---------------------------------------------- static methods ---------------------------------------------- */

/* Usage: output usage */
static void Usage(void)
{
	HTS_show_copyright(stderr);
	fprintf(stderr, "\n");
	fprintf(stderr, "hts_engine - An HMM-based speech synthesis engine\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       hts_engine [ options ] [ infile ] \n");
	fprintf(stderr,
			"  options:                                                                   [  def][ min--max]\n");
	fprintf(stderr,
			"    -td tree       : decision trees file for state duration                  [  N/A]\n");
	fprintf(stderr,
			"    -tf tree       : decision trees file for Log F0                          [  N/A]\n");
	fprintf(stderr,
			"    -tm tree       : decision trees file for spectrum                        [  N/A]\n");
	fprintf(stderr,
			"    -md pdf        : model file for state duration                           [  N/A]\n");
	fprintf(stderr,
			"    -mf pdf        : model file for Log F0                                   [  N/A]\n");
	fprintf(stderr,
			"    -mm pdf        : model file for spectrum                                 [  N/A]\n");
	fprintf(stderr,
			"    -df win        : window file for calculation delta of Log F0             [  N/A]\n");
	fprintf(stderr,
			"    -dm win        : filename of delta coeffs for spectrum                   [  N/A]\n");
	
	
	
	fprintf(stderr,
			"    -dl delay      : number of phonemes of delay                             [  1.0][ 0--N]\n");
	
	
	
	fprintf(stderr,
			"    -od s          : filename of output label with duration                  [  N/A]\n");
	fprintf(stderr,
			"    -of s          : filename of output Log F0                               [  N/A]\n");
	fprintf(stderr,
			"    -om s          : filename of output spectrum                             [  N/A]\n");
	fprintf(stderr,
			"    -or s          : filename of output raw audio (generated speech)         [  N/A]\n");
	fprintf(stderr,
			"    -ow s          : filename of output wav audio (generated speech)         [  N/A]\n");
	fprintf(stderr,
			"    -ot s          : filename of output trace information                    [  N/A]\n");
	fprintf(stderr,
			"    -vp            : use phoneme alignment for duration                      [  N/A]\n");
	fprintf(stderr,
			"    -i  i f1 .. fi : enable interpolation & specify number(i),coefficient(f) [    1][   1-- ]\n");
	fprintf(stderr,
			"    -s  i          : sampling frequency                                      [16000][   1--48000]\n");
	fprintf(stderr,
			"    -p  i          : frame period (point)                                    [   80][   1--]\n");
	fprintf(stderr,
			"    -a  f          : all-pass constant                                       [ 0.42][ 0.0--1.0]\n");
	fprintf(stderr,
			"    -g  i          : gamma = -1 / i (if i=0 then gamma=0)                    [    0][   0-- ]\n");
	fprintf(stderr,
			"    -b  f          : postfiltering coefficient                               [  0.0][-0.8--0.8]\n");
	fprintf(stderr,
			"    -l             : regard input as log gain and output linear one (LSP)    [  N/A]\n");
	fprintf(stderr,
			"    -r  f          : speech speed rate                                       [  1.0][ 0.0--10.0]\n");
	fprintf(stderr,
			"    -fs f          : multilply F0                                            [  1.0][ 0.1--1.9]\n");
	fprintf(stderr,
			"    -fm f          : add F0             f                                    [  0.0][-200.0--200.0]\n");
	fprintf(stderr,
			"    -u  f          : voiced/unvoiced threshold                               [  0.5][ 0.0--1.0]\n");
	fprintf(stderr,
			"    -cf pdf        : filename of GV for Log F0                               [  N/A]\n");
	fprintf(stderr,
			"    -cm pdf        : filename of GV for spectrum                             [  N/A]\n");
	fprintf(stderr,
			"    -jf f          : weight of GV for Log F0                                 [  0.7][ 0.0--2.0]\n");
	fprintf(stderr,
			"    -jm f          : weight of GV for spectrum                               [  1.0][ 0.0--2.0]\n");
	
	
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "    label file\n");
	fprintf(stderr, "  note:\n");
	fprintf(stderr,
			"    option '-d' may be repeated to use multiple delta parameters.\n");
	fprintf(stderr,
			"    generated spectrum and log F0 sequences are saved in natural\n");
	fprintf(stderr, "    endian, binary (float) format.\n");
	fprintf(stderr, "\n");
	
	exit( 0 );
}

/* Error: output error message */
static void Error(const int error, char *message, ...)
{
	va_list arg;
	
	fflush(stdout);
	fflush(stderr);
	
	if (error > 0)
		fprintf(stderr, "\nError: ");
	else
		fprintf(stderr, "\nWarning: ");
	
	va_start(arg, message);
	vfprintf(stderr, message, arg);
	va_end(arg);
	
	fflush(stderr);
	
	if (error > 0)
		exit(error);
}

/* Getfp: wrapper for fopen */
static FILE *Getfp(const char *name, const char *opt)
{
	FILE *fp = fopen(name, opt);
	
	if (fp == NULL)
		Error(2, "Getfp: Cannot open %s.\n", name);
	
	return ( fp );
}

/* GetNumInterp: get number of speakers for interpolation from argv */
static int GetNumInterp(int argc, char **argv_search)
{
	int num_interp = 1;
	while (--argc) {
		if (**++argv_search == '-') {
			if (*(*argv_search + 1) == 'i') {
				num_interp = atoi(*++argv_search);
				if (num_interp < 1) {
					num_interp = 1;
				}
				--argc;
			}
		}
	}
	return ( num_interp );
}

/* ---------------------------------------------- environment methods ---------------------------------------------- */

/* --------------------------------------------- initialization method --------------------------------------------- */

/* pHTS_initialize: initialize all variables */
void pHTS_initialize (int num_arg,  char **arguments, HTS_Engine *engine, pHTS *p)
{
	char fname[500];
	int i;
	double f;	
	
#ifndef HTS_EMBEDDED
	p->p_lf0fp = NULL, p->p_mcpfp = NULL;
#endif                          
	/* !HTS_EMBEDDED */
	
	p->p_durfp = NULL, p->p_tracefp = NULL, p->p_rawfp = NULL, p->p_wavfp = NULL;
	p->p_labels = NULL;
	
	/* number of speakers for interpolation */
	p->num_interp = 0;
	p->rate_interp = NULL;
	
	/* file names of models */
	p->fn_ms_lf0;
	p->fn_ms_mcp;
	p->fn_ms_dur;
	
	/* number of each models for interpolation */
	p->num_ms_lf0 = 0, p->num_ms_mcp = 0, p->num_ms_dur = 0;
	
	/* file names of trees */
	p->fn_ts_lf0;
	p->fn_ts_mcp;
	p->fn_ts_dur;
	
	/* number of each trees for interpolation */
	p->num_ts_lf0 = 0, p->num_ts_mcp = 0, p->num_ts_dur = 0;
	
	/* file names of windows */
	p->fn_ws_lf0;
	p->fn_ws_mcp;
	p->num_ws_lf0 = 0, p->num_ws_mcp = 0;
	
	/* file names of global variance */
	p->fn_ms_gvl = NULL;
	p->fn_ms_gvm = NULL;
	p->num_ms_gvl = 0, p->num_ms_gvm = 0;
	
	/* global parameter */
	p->sampling_rate = 16000;
	p->fperiod = 80;
	p->alpha = 0.42;
	p->stage = 0.0;          /* gamma = -1.0/stage */
	p->beta = 0.0;
	p->uv_threshold = 0.5;
	p->gv_weight_lf0 = 0.7;
	p->gv_weight_mcp = 1.0;
	
	p->f0_std = 1.0;
	p->f0_mean = 0.0;
	p->phoneme_alignment = FALSE;
	p->speech_speed = 1.0;
	p->use_log_gain = FALSE;

//--------------------------> pHTS	
	
	/* initialize the phoneme delay */
	p->delay = 0;
	/* initialize the reset factor */
	p->reset = FALSE;
		
	/* initialize the pitch control */
	p->pitch = 0;
	/* initialize the pitch control mode */
	p->pitch_mode = -1;
	
	/* initialize the duration control */
	p->duration = -1;
	/* initialize the duration control mode */
	p->duration_mode = -1;
	
	/* initialize the volume control */
	p->volume = 1;
	
	/* initialize the label ringbuffer */
	p->labelRingBuffer = initRingBuffer(LABEL_BUFFER_SIZE*HTS_MAXBUFLEN, sizeof(char));
	
	/* initialize the audio ringbuffer */
	p->audioRingBuffer = initRingBuffer(PHTS_AUDIO_RINGBUFFER_SIZE, sizeof(float)); 
	
//--------------------------> pHTS

	p->labfn = NULL;	
	
	/* parse command line */
	if (num_arg == 1)
		Usage();
	
	/* initialize (stream[0] = spectrum , stream[1] = lf0) */
	HTS_Engine_initialize(engine, 2);
	
	/* delta window handler for log f0 */
	p->fn_ws_lf0 = (char **) calloc(num_arg, sizeof(char *));
	/* delta window handler for mel-cepstrum */
	p->fn_ws_mcp = (char **) calloc(num_arg, sizeof(char *));
	
	/* prepare for interpolation */
	p->num_interp = GetNumInterp(num_arg, arguments);
	p->rate_interp = (double *) calloc(p->num_interp, sizeof(double));
	for (i = 0; i < p->num_interp; i++)
		p->rate_interp[i] = 1.0;
	
	p->fn_ms_lf0 = (char **) calloc(p->num_interp, sizeof(char *));
	p->fn_ms_mcp = (char **) calloc(p->num_interp, sizeof(char *));
	p->fn_ms_dur = (char **) calloc(p->num_interp, sizeof(char *));
	p->fn_ts_lf0 = (char **) calloc(p->num_interp, sizeof(char *));
	p->fn_ts_mcp = (char **) calloc(p->num_interp, sizeof(char *));
	p->fn_ts_dur = (char **) calloc(p->num_interp, sizeof(char *));
	p->fn_ms_gvl = (char **) calloc(p->num_interp, sizeof(char *));
	p->fn_ms_gvm = (char **) calloc(p->num_interp, sizeof(char *));
	
	/* read command */
	while (--num_arg)
	{
		if (**++arguments == '-')
		{
			switch (*(*arguments + 1))
			{
				case 'v':
					switch (*(*arguments + 2))
				{
					case 'p':
						p->phoneme_alignment = TRUE;
						break;
					default:
						Error(1, "hts_engine: Invalid option '-v%c'.\n", *(*arguments + 2));
				}
					break;
				case 't':
					switch (*(*arguments + 2))
				{
					case 'f':
					case 'p':
						p->fn_ts_lf0[p->num_ts_lf0++] = *(arguments + 1);
						break;
					case 'm':
						p->fn_ts_mcp[p->num_ts_mcp++] = *(arguments + 1);
						break;
					case 'd':
						p->fn_ts_dur[p->num_ts_dur++] = *(arguments + 1);
						break;
					default:
						Error(1, "hts_engine: Invalid option '-t%c'.\n", *(*arguments + 2));
				}
					++arguments;
					--num_arg;
					break;
				case 'm':
					switch (*(*arguments + 2))
				{
					case 'f':
					case 'p':
						p->fn_ms_lf0[p->num_ms_lf0++] = *(arguments + 1);
						break;
					case 'm':
						p->fn_ms_mcp[p->num_ms_mcp++] = *(arguments + 1);
						break;
					case 'd':
						p->fn_ms_dur[p->num_ms_dur++] = *(arguments + 1);
						break;
					default:
						Error(1, "hts_engine: Invalid option '-m%c'.\n", *(*arguments + 2));
				}
					++arguments;
					--num_arg;
					break;
				case 'd':
					switch (*(*arguments + 2))
				{
					case 'f':
					case 'p':
						p->fn_ws_lf0[p->num_ws_lf0] = *(arguments + 1);
						p->num_ws_lf0++;
						break;
					case 'm':
						p->fn_ws_mcp[p->num_ws_mcp] = *(arguments + 1);
						p->num_ws_mcp++;
						break;
					case 'l':
						p->delay = atoi(*(arguments + 1));
						break;
					default:
						Error(1, "hts_engine: Invalid option '-d%c'.\n", *(*arguments + 2));
				}
					++arguments;
					--num_arg;
					break;
				case 'o':
					switch (*(*arguments + 2))
				{
					case 'w':  
						fname[0] = '\0';
						sprintf(fname, "%s_delay%d.wav",*(arguments + 1), p->delay );
						p->p_wavfp = Getfp(fname, "wb");
						p->p_wavfp = Getfp(fname, "ab");
						break;
					case 'r':
						fname[0] = '\0';
						sprintf(fname, "%s_delay%d.raw",*(arguments + 1), p->delay );
						p->p_rawfp = Getfp(fname, "wb");
						p->p_rawfp = Getfp(fname, "ab");
						break;
					case 'f':
#ifndef HTS_EMBEDDED
					case 'p':
						fname[0] = '\0';
						sprintf(fname, "%s_delay%d.lf0",*(arguments + 1), p->delay );
						p->p_lf0fp = Getfp(fname, "wb");
						p->p_lf0fp = Getfp(fname, "ab");
						break;
					case 'm':
						fname[0] = '\0';
						sprintf(fname, "%s_delay%d.mgc",*(arguments + 1), p->delay );
						p->p_mcpfp = Getfp(fname, "wb");
						p->p_mcpfp = Getfp(fname, "ab");
						break;
#endif                          /* !HTS_EMBEDDED */
					case 'd':
						fname[0] = '\0';
						sprintf(fname, "%s_delay%d.lab",*(arguments + 1), p->delay );
						p->p_durfp = Getfp(fname, "wb");
						p->p_durfp = Getfp(fname, "ab");
						break;
					case 't':
						fname[0] = '\0';
						sprintf(fname, "%s_delay%d.trace",*(arguments + 1), p->delay );
						p->p_tracefp = Getfp(fname, "wb");
						p->p_tracefp = Getfp(fname, "ab");
						break;
					default:
						Error(1, "hts_engine: Invalid option '-o%c'.\n", *(*arguments + 2));
				}
					++arguments;
					--num_arg;
					break;
				case 'h':
					Usage();
					break;
				case 's':
					p->sampling_rate = atoi(*++arguments);
					--num_arg;
					break;
				case 'p':
					p->fperiod = atoi(*++arguments);
					--num_arg;
					break;
				case 'a':
					p->alpha = atof(*++arguments);
					--num_arg;
					break;
				case 'g':
					p->stage = atoi(*++arguments);
					--num_arg;
					break;
				case 'l':
					p->use_log_gain = TRUE;
					break;
				case 'b':
					p->beta = atof(*++arguments);
					--num_arg;
					break;
				case 'r':
					p->speech_speed = atof(*++arguments);
					--num_arg;
					break;
				case 'f':
					switch (*(*arguments + 2))
				{
					case 's':
						f = atof(*++arguments);
						if (f < 0.1)
							f = 0.1;
						if (f > 1.9)
							f = 1.9;
						p->f0_std = f;
						break;
					case 'm':
						f = atof(*++arguments);
						if (f < -200.0)
							f = -200.0;
						if (f > 200.0)
							f = 200.0;
						p->f0_mean = f;
						break;
					default:
						Error(1, "hts_engine: Invalid option '-f%c'.\n", *(*arguments + 2));
				}
					--num_arg;
					break;
				case 'u':
					p->uv_threshold = atof(*++arguments);
					--num_arg;
					break;
				case 'i':
					++arguments;
					num_arg--;
					for (i = 0; i < p->num_interp; i++)
					{
						p->rate_interp[i] = atof(*++arguments);
						num_arg--;
					}
					break;
				case 'c':
					switch (*(*arguments + 2))
				{
					case 'f':
					case 'p':
						p->fn_ms_gvl[p->num_ms_gvl++] = *(arguments + 1);
						break;
					case 'm':
						p->fn_ms_gvm[p->num_ms_gvm++] = *(arguments + 1);
						break;
					default:
						Error(1, "hts_engine: Invalid option '-c%c'.\n", *(*arguments + 2));
				}
					++arguments;
					--num_arg;
					break;
				case 'j':
					switch (*(*arguments + 2))
				{
					case 'f':
					case 'p':
						p->gv_weight_lf0 = atof(*(arguments + 1));
						break;
					case 'm':
						p->gv_weight_mcp = atof(*(arguments + 1));
						break;
					default:
						Error(1, "hts_engine: Invalid option '-j%c'.\n", *(*arguments + 2));
				}
					++arguments;
					--num_arg;
					break;
				default:
					Error(1, "hts_engine: Invalid option '-%c'.\n", *(*arguments + 1));            
			}
		} 
		else
		{
			p->labfn = *arguments;
		}
	}
	
	/* number of models,trees check */
	if (p->num_interp != p->num_ts_lf0 || p->num_interp != p->num_ts_mcp ||
		p->num_interp != p->num_ts_dur || p->num_interp != p->num_ms_lf0 ||
		p->num_interp != p->num_ms_mcp || p->num_interp != p->num_ms_dur) 
	{
		Error(1,"hts_engine: specify %d models(trees) for each parameter.\n", p->num_interp);
	}
	
	/* load duration model */
	HTS_Engine_load_duration_from_fn(engine, p->fn_ms_dur, p->fn_ts_dur, p->num_interp);
	
	/* load stream[0] (spectrum model) */
	HTS_Engine_load_parameter_from_fn(engine, p->fn_ms_mcp, p->fn_ts_mcp, p->fn_ws_mcp, 0, FALSE, p->num_ws_mcp, p->num_interp);
	
	/* load stream[1] (lf0 model) */
	HTS_Engine_load_parameter_from_fn(engine, p->fn_ms_lf0, p->fn_ts_lf0, p->fn_ws_lf0, 1, TRUE, p->num_ws_lf0, p->num_interp);
	
	/* load gv[0] (GV for spectrum) */
	if (p->num_interp == p->num_ms_gvm)
		HTS_Engine_load_gv_from_fn(engine, p->fn_ms_gvm, 0, p->num_interp);
	
	/* load gv[1] (GV for lf0) */
	if (p->num_interp == p->num_ms_gvl)
		HTS_Engine_load_gv_from_fn(engine, p->fn_ms_gvl, 1, p->num_interp);
	
	/* set parameter */
	HTS_Engine_set_sampling_rate(engine, p->sampling_rate);
	
	HTS_Engine_set_fperiod(engine, p->fperiod);
	
	HTS_Engine_set_alpha(engine, p->alpha);
	
	HTS_Engine_set_gamma(engine, p->stage);
	
	HTS_Engine_set_log_gain(engine, p->use_log_gain);
	
	HTS_Engine_set_beta(engine, p->beta);
	
	HTS_Engine_set_msd_threshold(engine, 1, p->uv_threshold);  /* set voiced/unvoiced threshold for stream[1] */
	
	HTS_Engine_set_gv_weight(engine, 0, p->gv_weight_mcp);
	
	HTS_Engine_set_gv_weight(engine, 1, p->gv_weight_lf0);
	
	for (i = 0; i < p->num_interp; i++)
	{
		HTS_Engine_set_duration_interpolation_weight(engine, i, p->rate_interp[i]);
		
		HTS_Engine_set_parameter_interpolation_weight(engine, 0, i, p->rate_interp[i]);
		
		HTS_Engine_set_parameter_interpolation_weight(engine, 1, i, p->rate_interp[i]);
	}
	
	if (p->num_interp == p->num_ms_gvm)
		for (i = 0; i < p->num_interp; i++)
			HTS_Engine_set_gv_interpolation_weight(engine, 0, i, p->rate_interp[i]);
	
	if (p->num_interp == p->num_ms_gvl)
		for (i = 0; i < p->num_interp; i++)
			HTS_Engine_set_gv_interpolation_weight(engine, 1, i, p->rate_interp[i]);
}


/* ----------------------------------- updating the internal state of the engine ----------------------------------- */

/* pHTS_pushLabel: set a new label into the labels set in order to be prosessed */
void pHTS_pushLabel(HTS_Engine *engine, pHTS *p, char *label)
{
	writeRingBuffer(p->labelRingBuffer, label, HTS_MAXBUFLEN );
}

/* pHTS_parseLabel: parse the last inserted label */
int pHTS_parseLabel(HTS_Engine *engine, pHTS *p)
{
	int j, tmp_total_state = 0;    
	double f;
	
	char label[HTS_MAXBUFLEN];
	
	int fullBuffer = readRingBuffer( p->labelRingBuffer, label, HTS_MAXBUFLEN);	

	if(!fullBuffer)
		return(-1);
	
	p_HTS_Engine_load_label_from_str(engine, label);  /* load on label */
	
	if (!p->phoneme_alignment)      /* modify label */
		p_HTS_Label_set_frame(&engine->label, -1);  
		
	if (p->speech_speed != 1.0 && p->speech_speed > 0)     /* modify label */
	{
		p_HTS_Label_set_frame(&engine->label, -1);   
		HTS_Label_set_speech_speed(&engine->label, p->speech_speed);
	}
	
	p_HTS_Engine_create_sstream(engine, &p->total_frame_array, p->duration, p->duration_mode);  /* parse label and determine state duration */
	
	if (p->f0_std != 1.0 || p->f0_mean != 0.0)
	{     /* modify f0 */
		for (j = tmp_total_state; j < HTS_SStreamSet_get_total_state(&engine->sss); j++)
		{
			f = exp(HTS_SStreamSet_get_mean(&engine->sss, 1, j, 0));
			f = p->f0_std * f + p->f0_mean;
			
			if (f < 10.0)
				f = 10.0;
			
			HTS_SStreamSet_set_mean(&engine->sss, 1, j, 0, log(f));
		}           
		tmp_total_state = HTS_SStreamSet_get_total_state(&engine->sss);
	}   
	return(0);
}

/* pHTS_updateEngine: generate the speech samples from PDFs */
void pHTS_updateEngine(HTS_Engine *engine, pHTS *p)
{
	pHTS_updatePDFs(engine, p);
	pHTS_updateFilter(engine, p);
	pHTS_updateSamples(engine, p);
}

/* pHTS_updatePDFs: generate PDFs */
void pHTS_updatePDFs(HTS_Engine *engine, pHTS *p)
{	
	/* p_HTS_Engine_create_pstream: generate speech parameter vector sequence */
	p_HTS_Engine_create_pstream(engine, p->total_frame_array, &p->length_array, 1);  /* generate speech parameter vector sequence */
}

/* pHTS_updateFilter: generate the speech parameters from the PDFs */
void pHTS_updateFilter(HTS_Engine *engine, pHTS *p)
{
	/* p_HTS_Engine_create_gstream_param: generate the speech parameters */
	p_HTS_Engine_create_gstream_param(engine, p->delay, p->total_frame_array, p->length_array, p->reset);
}

/* pHTS_updateSamples: generate speech samples from the speech parameters */
void pHTS_updateSamples(HTS_Engine *engine, pHTS *p)
{
	/* p_HTS_Engine_set_gstream_pitch: set pitch in the generated speech parameters */
	p_HTS_Engine_set_gstream_pitch(engine, p->delay, p->total_frame_array, p->pitch, p->pitch_mode);
	
	/* p_HTS_Engine_create_gstream_speech_samples: generate speech samples */
	p_HTS_Engine_create_gstream_speech_samples(engine, p->delay, &p->vocoder, p->total_frame_array);
	
	/* updateSamples() call pushing in the samples in the audio ringbuffer */
	
	int k;
	short *buffer;
	
	float *fbuffer;

	int nOfSamples;
	long nOfWrittenSamples;
	
	nOfSamples = pHTS_getNumberOfSamples(engine, p); /* get number of processed samples */
	
	buffer = (short *)calloc(nOfSamples, sizeof(short)); /* create temp buffer to extract samples */
	
	pHTS_getSamples(engine, p, buffer, nOfSamples); /* extract samples from pHTS engine */
	
	fbuffer = (float *)calloc(nOfSamples, sizeof(float)); /* create temp buffer to extract samples */
	
	char *ptr = (char *)fbuffer;
	
	if(p->volume < 0)
		p->volume = 1;
		
	for( k=0; k<nOfSamples; k++ ) 
		fbuffer[k] = (float)(buffer[k]*p->volume) / 32768.0;
	
	while( nOfSamples > 0 )
	{
		nOfWrittenSamples = writeRingBuffer(p->audioRingBuffer, ptr, nOfSamples);	
		nOfSamples -= nOfWrittenSamples; 
		ptr += (nOfWrittenSamples*sizeof(float));
		if( nOfSamples > 0 ) 
			usleep(10000);
	}
	
	free(fbuffer);
	free(buffer);
}


/* ----------------------------------- gettting the internal state of the engine ----------------------------------- */

/* pHTS_getNumberOfLabels: get the # of labels queued in the engine */
int pHTS_getNumberOfLabels(HTS_Engine *engine, pHTS *p)
{
	int numberOfLabels = HTS_Label_get_size(&engine->label);
	return ( numberOfLabels );
}

/* pHTS_getNumberOfSamples: get the number of the generated samples */
int pHTS_getNumberOfSamples(HTS_Engine *engine, pHTS *p)
{
	int samples;
	int samplesEnd;
	int index = HTS_Label_get_size(&engine->label) - p->delay;
	int indexEnd = HTS_Label_get_size(&engine->label) - p->delay - 1;
	
	if (index < 0 || p->delay < 0)
		index = 0;
	
	if (indexEnd < 0 || p->delay < 0)
		indexEnd = 0;
	
	samples = engine->gss.total_frame*engine->global.fperiod; 
	samplesEnd = p->total_frame_array[indexEnd]*engine->global.fperiod;
	
	return ( samples - samplesEnd );
}

/* pHTS_getSamples: get the speech samples */
void pHTS_getSamples(HTS_Engine *engine, pHTS *p, short *buffer, int nOfSamples)
{
	int samples, i, j;
	int index = HTS_Label_get_size(&engine->label) - p->delay - 1;

	if (index < 0 || p->delay < 0)
		index = 0;
	
	samples = p->total_frame_array[index]*engine->global.fperiod;	

	for(i=0, j = samples; j<nOfSamples+samples; i++, j++)
		buffer[i] = engine->gss.gspeech[j]; 
}

/* pHTS_popSamples : access and pop speech samples from ring buffer ~ to be called from audio callback */
void pHTS_popSamples(HTS_Engine *engine, pHTS *p, float *buffer, int nOfSamples) {
	
	int k;
	int nOfReadSamples = readRingBuffer( p->audioRingBuffer, buffer, nOfSamples );
		
	if( nOfReadSamples < nOfSamples && nOfReadSamples>0) 
		printf("glitch !\n");
	
	for( k=nOfReadSamples; k<nOfSamples; k++ )
		buffer[k] = 0.0;
	
}

/* pHTS_getLabels: get last Label */
HTS_LabelString * pHTS_getLabel(HTS_Engine *engine, pHTS *p)
{
	HTS_Label *LabelSet = &engine->label;
	HTS_LabelString *label = LabelSet->tail;
	return ( label );
}

/* pHTS_getPDFs: get PDFs */
HTS_PStreamSet * pHTS_getPDFs(HTS_Engine *engine, pHTS *p)
{
	HTS_PStreamSet *pss = &engine->pss;
	return ( pss );
}

/* pHTS_getFilter: get filter coefficients */
HTS_GStreamSet * pHTS_getFilter(HTS_Engine *engine, pHTS *p)
{
	HTS_GStreamSet * gss = &engine->gss; 
	return ( gss );
}

/* pHTS_getSpeed: get speech speed rate from the pHTS environment */
double pHTS_getSpeed(HTS_Engine *engine,  pHTS *p)
{
	return( p->speech_speed );
}

/* pHTS_getPitch: get pitch value from the pHTS environment */
double pHTS_getPitch(HTS_Engine *engine, pHTS *p)
{
	return ( p->pitch );
}

/* pHTS_getVolume: get volume value from the pHTS environment */
double pHTS_getVolume(HTS_Engine *engine, pHTS *p)
{
	return ( p->volume );
}

/* pHTS_getAlpha: get alpha, all-pass constant value from the pHTS environment */
double pHTS_getAlpha(HTS_Engine *engine, pHTS *p)
{
	return ( p->alpha );
}

/* pHTS_getDuration: get durations value from the pHTS environment */
double pHTS_getDuration(HTS_Engine *engine, pHTS *p)
{
	return( p->duration );
}


/* ----------------------------------- settting the internal state of the engine ----------------------------------- */

/* pHTS_setLabels: set Labels */
void pHTS_setLabels(HTS_Engine *engine, pHTS *p, HTS_Label *label)
{
	/* p_HTS_Engine_set_labels: set different set of labels */
	p_HTS_Engine_set_labels(engine, label);
}

/* pHTS_setPDFs: set PDFs */
void pHTS_setPDFs(HTS_Engine *engine, pHTS *p, HTS_PStreamSet *pss)
{
	/* p_HTS_PStreamSet_set: set different set of PDF streams */
	p_HTS_Engine_set_pstream(engine, pss);
}

/* pHTS_setFilter: set filter coefficients */
void pHTS_setFilter(HTS_Engine *engine, pHTS *p, HTS_GStreamSet *gss)
{
	/* p_HTS_Engine_set_gstream: set different speech parameters */
	p_HTS_Engine_set_gstream(engine,  gss);
}

/* pHTS_setSpeed: set speech speed rate value in the pHTS environment */
void pHTS_setSpeed(HTS_Engine *engine,  pHTS *p, double speechSpeed)
{
	if(speechSpeed > 0)
		p->speech_speed = speechSpeed;
}

/* pHTS_setPitch: set pitch value in the pHTS environment */
void pHTS_setPitch(HTS_Engine *engine, pHTS *p, double pitch, int mode)
{
	p->pitch_mode = mode;
	p->pitch = pitch;
}

/* pHTS_setVolume: set volume value in the pHTS environment */
void pHTS_setVolume(HTS_Engine *engine, pHTS *p, double volume)
{
	if(volume > 0)
		p->volume = volume;
}

/* pHTS_setAlpha: set alpha, all-pass constant value in the pHTS environment */
void pHTS_setAlpha(HTS_Engine *engine, pHTS *p, double alpha)
{
	if(alpha > 0 && alpha < 1)
	{
		p->alpha = alpha;
		engine->global.alpha = alpha;
	}
}

/* pHTS_setDuration: set duration value in the pHTS environment  */
void pHTS_setDuration(HTS_Engine *engine, pHTS *p, double duration, int mode)
{
	if (duration > 0) 
	{
		p->duration_mode = mode;
		p->duration = duration;
	}
}


/* ---------------------------------------------- load / save / free ----------------------------------------------- */

/* pHTS_getLabelFromFile: get one label string from a label file */
char * pHTS_getLabelFromFile(HTS_Engine *engine, pHTS *p)
{
	if(p->p_labels == NULL)
		p->p_labels = fopen(p->labfn, "r");
	
	char *label = (char *)malloc(HTS_MAXBUFLEN * sizeof(char));
	
	/* parse label file until a specific line */
	return (fgets (label , HTS_MAXBUFLEN , p->p_labels));
}

/* pHTS_finalize: flushes the delayed labels */
void pHTS_finalize(HTS_Engine *engine, pHTS *p)
{
	if(p->delay == 0)
		return;
	
	p->reset = TRUE;
		
	pHTS_updateFilter(engine, p);

	p->reset = FALSE;
}

/* pHTS_refresh: free model per one time synthesis */
void pHTS_refresh(HTS_Engine *engine, pHTS *p)
{
	/* HTS_Engine_refresh: free model per one time synthesis */
	HTS_Engine_refresh(engine);
}


/* pHTS_free: free alocated memory */
void pHTS_free(HTS_Engine *engine, pHTS *p)
{
	/* free */
	/* HTS_Engine_refresh: free model per one time synthesis */
	HTS_Engine_refresh(engine);
	
	/* free memory */
	free(p->rate_interp);
	free(p->fn_ws_mcp);
	free(p->fn_ws_lf0);
	free(p->fn_ms_mcp);
	free(p->fn_ms_lf0);
	free(p->fn_ms_dur);
	free(p->fn_ts_mcp);
	free(p->fn_ts_lf0);
	free(p->fn_ts_dur);
	free(p->fn_ms_gvm);
	free(p->fn_ms_gvl);
	
	free(p->total_frame_array);
	free(p->length_array);
	
	/* HTS_Engine_clear: free engine */
	HTS_Engine_clear(engine);	
}
