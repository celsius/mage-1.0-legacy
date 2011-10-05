
/* ------------------------------------------------------------------------------------------- */
/*																							   */
/*  This file is part of MAGE / pHTS (the performative HMM-based speech synthesis system)      */
/*																							   */
/*  MAGE / pHTS is free software: you can redistribute it and/or modify it under the terms     */
/*  of the GNU General Public License as published by the Free Software Foundation, either     */
/*  version 3 of the license, or any later version.										       */
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
/*   Developed by :																		       */
/*																							   */
/*		Maria Astrinaki, Geoffrey Wilfart, Alexis Moinet, Nicolas d'Alessandro, Thierry Dutoit */
/*																							   */
/*																							   */
/*																							   */
/*   Copyright 2001-2011 Nagoya Institute of Technology									       */
/*                    Department of Computer Science										   */
/*																							   */
/*   Copyright 2001-2008 Tokyo Institute of Technology										   */
/*                    Interdisciplinary Graduate School of Science and Engineering			   */
/*																							   */
/* ------------------------------------------------------------------------------------------- */


#include <stdlib.h>	 /* for realloc */

/* hts_engine libraries */
#include "p_HTS_engine.h"

/** 
 *	\file p_HTS_sstream.c 
 *	\brief Contains the implementation of all the streaming functions of the HTS_Engine for the set of state stream.
 *
 *	The functions implemented here, are the streaming version of the original ssteam functions of the HTS_Engine.
 */

/* p_HTS_SStreamSet_initialize: initialize state stream set */
void p_HTS_SStreamSet_initialize(HTS_SStreamSet *sss)
{
	sss->nstream = 0;
	sss->nstate = 0;
	sss->sstream = NULL;
	sss->duration = NULL;
	sss->total_state = 0;
	sss->total_frame = 0;
}

/* p_HTS_SStreamSet_create: parse label and determine state duration */
void p_HTS_SStreamSet_create(HTS_SStreamSet *sss, HTS_ModelSet *ms, HTS_Label *label, 
							 double *duration_iw, double **parameter_iw, double **gv_iw, int **total_frame_array, float duration, int mode )
{
	int i, j, k, state;
	double temp1, temp2;
	HTS_SStream *sst;
	double *duration_mean, *duration_vari;
	double duration_remain;
	double rho = 0.0;
	
	/* initialize state sequence -- OK */   
	sss->nstate = HTS_ModelSet_get_nstate(ms);
	sss->total_state = HTS_Label_get_size(label) * sss->nstate;
	sss->duration = (int *) realloc(sss->duration, sss->total_state*sizeof(int));  
	state = sss->nstate*(HTS_Label_get_size(label)-1);
	
	if(HTS_Label_get_size(label) == 1)
	{
		sss->nstream = HTS_ModelSet_get_nstream(ms);
		sss->sstream = (HTS_SStream *) realloc(sss->sstream, sss->nstream*sizeof(HTS_SStream));
		sss->total_frame = 0;
		*total_frame_array = NULL;
		
		for (i = 0; i < sss->nstream; i++)
		{
			sst = &sss->sstream[i];
			sst->mean = NULL;
			sst->vari =  NULL;
			sst->msd = NULL;
		}
	}
	
	for (i = 0; i < sss->nstream; i++)
	{
		sst = &sss->sstream[i];
		sst->vector_length = HTS_ModelSet_get_vector_length(ms, i);       
		sst->mean = (double **) realloc(sst->mean , sss->total_state*sizeof(double *));
		sst->vari = (double **) realloc(sst->vari, sss->total_state*sizeof(double *));
		
		if (HTS_ModelSet_is_msd(ms, i))
			sst->msd = (double *) realloc(sst->msd, sss->total_state*sizeof(double));
		
		else
			sst->msd = NULL;
		
		for (j = ((HTS_Label_get_size(label)-1) * sss->nstate); j < sss->total_state; j++)
		{
			sst->mean[j] = (double *) HTS_calloc(sst->vector_length, sizeof(double));
			sst->vari[j] = (double *) HTS_calloc(sst->vector_length, sizeof(double));
		}
	}
	
	/* check interpolation weights */
	if(HTS_Label_get_size(label) == 1)
	{
		for (i = 0, temp1 = 0.0;  i < HTS_ModelSet_get_duration_interpolation_size(ms); i++)
			temp1 += duration_iw[i];
		
		for (i = 0; i < HTS_ModelSet_get_duration_interpolation_size(ms); i++)
			duration_iw[i] /= temp1;
		
		for (i = 0; i < sss->nstream; i++)
		{
			for (j = 0, temp1 = 0.0; j < HTS_ModelSet_get_parameter_interpolation_size(ms, i); j++)
				temp1 += parameter_iw[i][j];
			
			for (j = 0; j < HTS_ModelSet_get_parameter_interpolation_size(ms, i); j++)
				parameter_iw[i][j] /= temp1;
			
			if (HTS_ModelSet_use_gv(ms, i))
			{
				for (j = 0, temp1 = 0.0; j < HTS_ModelSet_get_gv_interpolation_size(ms, i); j++)
					temp1 += gv_iw[i][j];
				
				for (j = 0; j < HTS_ModelSet_get_gv_interpolation_size(ms, i); j++)
					gv_iw[i][j] /= temp1;
			}
		}
	}
	/* memory allocate for duration_mean & duration_vari */
	duration_mean = (double *) HTS_calloc(sss->nstate, sizeof(double));   
	duration_mean -= 2;
	duration_vari = (double *) HTS_calloc(sss->nstate, sizeof(double));
	duration_vari -= 2;
	duration_remain = 0.0;
		
	/* determine rho -- OK */
	if (HTS_Label_get_speech_speed(label) != 1.0)
	{
		temp1 = 0.0;
		temp2 = 0.0;
		
		p_HTS_ModelSet_get_duration(ms, (char*)p_HTS_Label_get_string(label), duration_mean, duration_vari, duration_iw);
		
		for (j = 2; j <= sss->nstate + 1; j++)
		{
			temp1 += duration_mean[j];
			temp2 += duration_vari[j];
		}
		rho = (temp1 / HTS_Label_get_speech_speed(label) - temp1) / temp2;
	}
	
	/* parse label file */
	/* determine state duration */
	p_HTS_ModelSet_get_duration(ms, p_HTS_Label_get_string(label),duration_mean, duration_vari, duration_iw);
	
	if (p_HTS_Label_get_frame_specified_flag(label))
	{     /* use duration set by user */
		temp1 = 0.0;
		for (j = 2; j <= sss->nstate + 1; j++)
		{
			duration_mean[j] += rho * duration_vari[j];
			temp1 += duration_mean[j];
		}
				
		if (mode == 0) // OVERWRITE 0 
		{
			if ( duration <= 0 ) 
				duration = p_HTS_Label_get_frame(label);
			temp1 = (double) (duration)/ temp1 ;
		}
		else if (mode == 1) // SHIFT 1
		{
			temp1 = (double) (p_HTS_Label_get_frame(label) + duration)/ temp1 ;
		}
		else if (mode == 2) // SCALE 2
		{
			if ( duration <= 0 ) 
				duration = 1;
			temp1 = (double) (p_HTS_Label_get_frame(label) * duration)/ temp1 ;
		}
		
		for (j = 2; j <= sss->nstate + 1; j++)
		{
			temp2 = temp1 * duration_mean[j];
			duration_mean[j] = (double) ((int) (temp2 + duration_remain + 0.5));
			
			if (duration_mean[j] < 1.0)
				duration_mean[j] = 1.0;
			
			duration_remain += temp2 - duration_mean[j];
		}
	}
	else
	{     /* estimate using gauss */
		for (j = 2; j <= sss->nstate + 1; j++)
		{
			temp1 = duration_mean[j] + rho * duration_vari[j];
			duration_mean[j] = (double) ((int) (temp1 + duration_remain + 0.5));
			
			if (duration_mean[j] < 1.0)
				duration_mean[j] = 1.0;
			
			duration_remain += temp1 - duration_mean[j];
		}
	}
	
	/* determine parameter */
	*total_frame_array = (int *) realloc(*total_frame_array, HTS_Label_get_size(label)*sizeof(int));
	*(*total_frame_array+HTS_Label_get_size(label)-1) = sss->total_frame;

	for (j = 2; j <= sss->nstate + 1; j++)
	{
		sss->duration[state] = (int) duration_mean[j];
		
		for (k = 0; k < sss->nstream; k++)
		{
			sst = &sss->sstream[k];
			
			if (sst->msd)
				HTS_ModelSet_get_parameter(ms, p_HTS_Label_get_string(label),
										   sst->mean[state], sst->vari[state],
										   &sst->msd[state], k, j,
										   parameter_iw[k]);
			else
				HTS_ModelSet_get_parameter(ms, p_HTS_Label_get_string(label),
										   sst->mean[state], sst->vari[state],
										   NULL, k, j, parameter_iw[k]);
		}
		sss->total_frame += sss->duration[state];
		state++;
	}
	
	HTS_free(duration_mean + 2);
	HTS_free(duration_vari + 2);
	
	/* copy dynamic window */
	if(HTS_Label_get_size(label) == 1)
	{
		for (i = 0; i < sss->nstream; i++)
		{
			sst = &sss->sstream[i];
			sst->win_size = HTS_ModelSet_get_window_size(ms, i);
			sst->win_max_width = HTS_ModelSet_get_window_max_width(ms, i);
			sst->win_l_width = (int *) HTS_calloc(sst->win_size, sizeof(int));
			sst->win_r_width = (int *) HTS_calloc(sst->win_size, sizeof(int));
			sst->win_coefficient = (double **) HTS_calloc(sst->win_size, sizeof(double));
			
			for (j = 0; j < sst->win_size; j++)
			{
				sst->win_l_width[j] = HTS_ModelSet_get_window_left_width(ms, i, j);
				sst->win_r_width[j] = HTS_ModelSet_get_window_right_width(ms, i, j);
				
				if (sst->win_l_width[j] + sst->win_r_width[j] == 0)
					sst->win_coefficient[j] = (double *) HTS_calloc(-2 * sst->win_l_width[j] + 1, sizeof(double));
				else
					sst->win_coefficient[j] = (double *) HTS_calloc(-2 * sst->win_l_width[j], sizeof(double));
				
				sst->win_coefficient[j] -= sst->win_l_width[j];
				
				for (k = sst->win_l_width[j]; k <= sst->win_r_width[j]; k++)
					sst->win_coefficient[j][k] = HTS_ModelSet_get_window_coefficient(ms, i, j, k);
			}
		}
	}
	
	/* determine GV */
	if(HTS_Label_get_size(label) == 1)
	{
		for (i = 0; i < sss->nstream; i++)
		{
			sst = &sss->sstream[i];
			
			if (HTS_ModelSet_use_gv(ms, i))
			{
				sst->gv_mean = (double *) HTS_calloc(sst->vector_length / sst->win_size, sizeof(double));
				sst->gv_vari = (double *) HTS_calloc(sst->vector_length / sst->win_size, sizeof(double));
				HTS_ModelSet_get_gv(ms, sst->gv_mean, sst->gv_vari, i, gv_iw[i]);
			}
			else
			{
				sst->gv_mean = NULL;
				sst->gv_vari = NULL;
			}
		}
	}
}
