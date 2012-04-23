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


#include <math.h>               /* for sqrt() */
#include <stdlib.h>              /* for realloc */

/* hts_engine libraries */
#include "HTS_hidden.h"

/** 
 *	\file p_HTS_pstream.c 
 *	\brief Contains the implementation of all the streaming functions of the HTS_Engine for the set of the PDF streams.
 *
 *	The functions implemented here, are the streaming version of the original pstream functions of the HTS_Engine.
 */

/* HTS_finv: calculate 1.0/variance function */
static double HTS_finv(const double x)
{
	if (x >= INFTY2)
		return 0.0;
	if (x <= -INFTY2)
		return 0.0;
	if (x <= INVINF2 && x >= 0)
		return INFTY;
	if (x >= -INVINF2 && x < 0)
		return -INFTY;
	
	return (1.0 / x);
}

/* HTS_PStream_ldl_factorization: Factorize W'*U^{-1}*W to L*D*L' (L: lower triangular, D: diagonal) */
static void HTS_PStream_ldl_factorization(HTS_PStream *pst)
{
	int t, i, j;
	
	for (t = 0; t < pst->length; t++) {
		for (i = 1; (i < pst->width) && (t >= i); i++)
			pst->sm.wuw[t][0] -= pst->sm.wuw[t - i][i] *
			pst->sm.wuw[t - i][i] * pst->sm.wuw[t - i][0];
		
		for (i = 1; i < pst->width; i++) {
			for (j = 1; (i + j < pst->width) && (t >= j); j++)
				pst->sm.wuw[t][i] -= pst->sm.wuw[t - j][j] *
                pst->sm.wuw[t - j][i + j] * pst->sm.wuw[t - j][0];
			pst->sm.wuw[t][i] /= pst->sm.wuw[t][0];
		}
	}
}

/* HTS_PStream_forward_substitution: forward subtitution for mlpg */
static void HTS_PStream_forward_substitution(HTS_PStream *pst)
{
	int t, i;
	
	for (t = 0; t < pst->length; t++) {
		pst->sm.g[t] = pst->sm.wum[t];
		for (i = 1; (i < pst->width) && (t >= i); i++)
			pst->sm.g[t] -= pst->sm.wuw[t - i][i] * pst->sm.g[t - i];
	}
}

/* HTS_PStream_backward_substitution: backward subtitution for mlpg */
static void HTS_PStream_backward_substitution(HTS_PStream *pst, const int m)
{
	int t, i;
	
	for (t = pst->length - 1; t >= 0; t--) {
		pst->par[t][m] = pst->sm.g[t] / pst->sm.wuw[t][0];
		for (i = 1; (i < pst->width) && (t + i < pst->length); i++)
			pst->par[t][m] -= pst->sm.wuw[t][i] * pst->par[t + i][m];
	}
}

/* HTS_PStream_calc_wuw_and_wum: calcurate W'U^{-1}W and W'U^{-1}M */
static void HTS_PStream_calc_wuw_and_wum(HTS_PStream *pst, const int m)
{
	int t, i, j, k;
	double wu;
	
	for (t = 0; t < pst->length; t++) {
		/* initialize */
		pst->sm.wum[t] = 0.0;
		for (i = 0; i < pst->width; i++)
			pst->sm.wuw[t][i] = 0.0;
		
		/* calc WUW & WUM */
		for (i = 0; i < pst->win_size; i++)
			for (j = pst->win_l_width[i]; j <= pst->win_r_width[i]; j++)
				if ((t + j >= 0) && (t + j < pst->length)
					&& (pst->win_coefficient[i][-j] != 0.0)) {
					wu = pst->win_coefficient[i][-j] *
					pst->sm.ivar[t + j][i * pst->static_length + m];
					pst->sm.wum[t] +=
					wu * pst->sm.mean[t + j][i * pst->static_length + m];
					for (k = 0; (k < pst->width) && (t + k < pst->length); k++)
						if ((k - j <= pst->win_r_width[i])
							&& (pst->win_coefficient[i][k - j] != 0.0))
							pst->sm.wuw[t][k] += wu * pst->win_coefficient[i][k - j];
				}
	}
}

/* HTS_PStream_calc_gv: subfunction for mlpg using GV */
static void HTS_PStream_calc_gv(HTS_PStream *pst, double *mean, double *vari)
{
	int t;
	
	*mean = 0.0;
	for (t = 0; t < pst->length; t++)
		*mean += pst->gv_buff[t];
	*mean /= pst->length;
	*vari = 0.0;
	for (t = 0; t < pst->length; t++)
		*vari += (pst->gv_buff[t] - *mean) * (pst->gv_buff[t] - *mean);
	*vari /= pst->length;
}

/* HTS_PStream_conv_gv: subfunction for mlpg using GV */
static void HTS_PStream_conv_gv(HTS_PStream *pst, const int m)
{
	int t;
	double ratio;
	double mean;
	double vari;
	
	HTS_PStream_calc_gv(pst, &mean, &vari);
	ratio = sqrt(pst->gv_mean[m] / vari);
	for (t = 0; t < pst->length; t++)
		pst->gv_buff[t] = ratio * (pst->gv_buff[t] - mean) + mean;
}

/* HTS_PStream_calc_derivative: subfunction for mlpg using GV */
static double HTS_PStream_calc_derivative(HTS_PStream *pst, const int m)
{
	int t, i;
	double mean;
	double vari;
	double dv;
	double h;
	double gvobj;
	double hmmobj;
	const double w = 1.0 / (pst->win_size * pst->length);
	
	HTS_PStream_calc_gv(pst, &mean, &vari);
	gvobj = -0.5 * W2 * vari * pst->gv_vari[m] * (vari - 2.0 * pst->gv_mean[m]);
	dv = -2.0 * pst->gv_vari[m] * (vari - pst->gv_mean[m]) / pst->length;
	
	for (t = 0; t < pst->length; t++) {
		pst->sm.g[t] = pst->sm.wuw[t][0] * pst->gv_buff[t];
		for (i = 1; i < pst->width; i++) {
			if (t + i < pst->length)
				pst->sm.g[t] += pst->sm.wuw[t][i] * pst->gv_buff[t + i];
			if (t + 1 > i)
				pst->sm.g[t] += pst->sm.wuw[t - i][i] * pst->gv_buff[t - i];
		}
	}
	
	for (t = 0, hmmobj = 0.0; t < pst->length; t++) {
		hmmobj +=
		W1 * w * pst->gv_buff[t] * (pst->sm.wum[t] - 0.5 * pst->sm.g[t]);
		h = -W1 * w * pst->sm.wuw[t][1 - 1]
		- W2 * 2.0 / (pst->length * pst->length) *
		((pst->length - 1) * pst->gv_vari[m] * (vari - pst->gv_mean[m])
		 + 2.0 * pst->gv_vari[m] * (pst->gv_buff[t] -
									mean) * (pst->gv_buff[t] - mean));
		pst->sm.g[t] =
		1.0 / h * (W1 * w * (-pst->sm.g[t] + pst->sm.wum[t]) +
				   W2 * dv * (pst->gv_buff[t] - mean));
	}
	
	return (-(hmmobj + gvobj));
}

/* HTS_PStream_gv_parmgen: function for mlpg using GV */
static void HTS_PStream_gv_parmgen(HTS_PStream *pst, const int m)
{
	int t, i;
	double step = STEPINIT;
	double prev = -LZERO;
	double obj;
	
	for (t = 0; t < pst->length; t++)
		pst->gv_buff[t] = pst->par[t][m];
	HTS_PStream_conv_gv(pst, m);
	if (GV_MAX_ITERATION > 0) {
		HTS_PStream_calc_wuw_and_wum(pst, m);
		for (i = 1; i <= GV_MAX_ITERATION; i++) {
			obj = HTS_PStream_calc_derivative(pst, m);
			if (obj > prev)
				step *= STEPDEC;
			if (obj < prev)
				step *= STEPINC;
			for (t = 0; t < pst->length; t++)
				pst->gv_buff[t] -= step * pst->sm.g[t];
			prev = obj;
		}
	}
	for (t = 0; t < pst->length; t++)
		pst->par[t][m] += pst->gv_weight * (pst->gv_buff[t] - pst->par[t][m]);
}

/* p_HTS_PStream_mlpg: generate sequence of speech parameter vector maximizing its output probability for given pdf sequence */
static void p_HTS_PStream_mlpg(HTS_PStream *pst, int past)
{
	int m = pst->static_length - past;
	
	if(m < 0)
		m = 0;
	
	for (m = 0; m < pst->static_length; m++)
	{
		HTS_PStream_calc_wuw_and_wum(pst, m);
		HTS_PStream_ldl_factorization(pst);       /* LDL factorization */
		HTS_PStream_forward_substitution(pst);    /* forward substitution   */
		HTS_PStream_backward_substitution(pst, m);        /* backward substitution  */
		
		if (pst->gv_buff != NULL)
			HTS_PStream_gv_parmgen(pst, m);
	}
}

/* p_HTS_PStreamSet_create: parameter generation using GV weight */
void p_HTS_PStreamSet_create(HTS_PStreamSet *pss, HTS_SStreamSet *sss, double *msd_threshold, double *gv_weight, 
							 HTS_Label *label, HTS_ModelSet *ms, int *total_frame_array, int **length_array, int past)
{
	int i, j, k, l, m;
	int frame, frame_init, msd_frame, msd_frame_init, state, state_init;
	
	HTS_PStream *pst;
	HTS_Boolean not_bound;
	
	/* initialize */
	pss->nstream = HTS_SStreamSet_get_nstream(sss);
	pss->total_frame = HTS_SStreamSet_get_total_frame(sss);
	
	if(HTS_Label_get_size(label) == 1)
		pss->pstream = (HTS_PStream *) HTS_calloc(pss->nstream, sizeof(HTS_PStream));
	
	state = HTS_ModelSet_get_nstate(ms)*(HTS_Label_get_size(label)-1);
	state_init = state;
	
	frame_init = total_frame_array[HTS_Label_get_size(label)-1];

	/* create */
	for (i = 0; i < pss->nstream; i++)
	{
		pst = &pss->pstream[i];
		
		if(HTS_Label_get_size(label) == 1)
		{
			pst->msd_flag = NULL;
			pst->sm.mean = NULL;
			pst->sm.ivar = NULL;
			pst->sm.wum = NULL;
			pst->sm.wuw = NULL;
			pst->sm.g = NULL;
			pst->par = NULL;
			pst->gv_buff = NULL;
			pst->gv_mean = NULL;
			pst->gv_vari = NULL;
			pst->win_l_width = NULL;
			pst->win_r_width = NULL;
			pst->win_coefficient = NULL;
			
			pst->length = 0;
			*length_array = NULL;
		}
		
		*length_array = (int *) realloc(*length_array, HTS_Label_get_size(label)*sizeof(int));
		
		if (HTS_SStreamSet_is_msd(sss, i))
		{      /* for MSD */
			msd_frame_init = pst->length;

			*(*length_array+HTS_Label_get_size(label)-1) = pst->length;
			
			for (state = state_init; state < HTS_SStreamSet_get_total_state(sss); state++)
			{
				if (HTS_SStreamSet_get_msd(sss, i, state) > msd_threshold[i])
					pst->length += HTS_SStreamSet_get_duration(sss, state);
			}
			
			pst->msd_flag = (HTS_Boolean *) realloc(pst->msd_flag, pss->total_frame*sizeof(HTS_Boolean));
			
			for (state = state_init, frame = frame_init; state < HTS_SStreamSet_get_total_state(sss); state++)
			{
				if (HTS_SStreamSet_get_msd(sss, i, state) > msd_threshold[i])
					for (j = 0; j < HTS_SStreamSet_get_duration(sss, state); j++)
					{
						pst->msd_flag[frame] = TRUE;
						frame++;
					}
				else
					for (j = 0; j < HTS_SStreamSet_get_duration(sss, state); j++)
					{
						pst->msd_flag[frame] = FALSE;
						frame++;
					}
			}
		}
		else
		{  /* for non MSD */
			pst->length = pss->total_frame;
			
			*(*length_array+HTS_Label_get_size(label)-1) =  total_frame_array[HTS_Label_get_size(label)-1];			

			pst->msd_flag = NULL;
			msd_frame_init = HTS_Label_get_size(label);
		}
		
		pst->vector_length = HTS_SStreamSet_get_vector_length(sss, i);
		pst->width = HTS_SStreamSet_get_window_max_width(sss, i) * 2 + 1; /* band width of R */
		pst->win_size = HTS_SStreamSet_get_window_size(sss, i);
		pst->static_length = pst->vector_length / pst->win_size;
		
		pst->sm.wum = (double *) realloc(pst->sm.wum, pst->length*sizeof(double));
		pst->sm.g = (double *) realloc(pst->sm.g, pst->length*sizeof(double));
		
		pst->sm.mean = (double **) realloc(pst->sm.mean, pst->length*sizeof(double *));
		pst->sm.ivar = (double **) realloc(pst->sm.ivar, pst->length*sizeof(double *));
		pst->sm.wuw = (double **) realloc(pst->sm.wuw, pst->length*sizeof(double *));
		pst->par = (double **) realloc(pst->par, pst->length*sizeof(double *));
		
		if (HTS_SStreamSet_is_msd(sss, i)) /* for MSD */
			j = msd_frame_init;
		else /* for non MSD */
			j = frame_init;
		
		for (; j < pst->length; j++)
		{
			pst->sm.mean[j] = (double *) calloc( (pst->vector_length ), sizeof(double));
			pst->sm.ivar[j] = (double *) calloc( (pst->vector_length ), sizeof(double));
			pst->sm.wuw[j] = (double *) calloc( (pst->vector_length ), sizeof(double));
			pst->par[j] = (double *) calloc( (pst->vector_length ), sizeof(double));
		}
		
		/* copy dynamic window */
		pst->win_l_width = (int *) realloc(pst->win_l_width, pst->win_size*sizeof(int));
		pst->win_r_width = (int *) realloc(pst->win_r_width, pst->win_size*sizeof(int));
		pst->win_coefficient = (double **) realloc(pst->win_coefficient, pst->win_size*sizeof(double));
		
		for (j = 0; j < pst->win_size; j++)
		{
			pst->win_l_width[j] = HTS_SStreamSet_get_window_left_width(sss, i, j);
			pst->win_r_width[j] = HTS_SStreamSet_get_window_right_width(sss, i, j);
			
			if (pst->win_l_width[j] + pst->win_r_width[j] == 0)
				pst->win_coefficient[j] = (double *) HTS_calloc(-2 * pst->win_l_width[j] + 1, sizeof(double));
			else
				pst->win_coefficient[j] = (double *) HTS_calloc(-2 * pst->win_l_width[j], sizeof(double));
			
			pst->win_coefficient[j] -= pst->win_l_width[j];
			
			for (k = pst->win_l_width[j]; k <= pst->win_r_width[j]; k++)
				pst->win_coefficient[j][k] = HTS_SStreamSet_get_window_coefficient(sss, i, j, k);
		}
		
		/* copy GV */
		if (HTS_SStreamSet_use_gv(sss, i))
		{
			pst->gv_mean = (double *) realloc(pst->gv_mean, pst->static_length*sizeof(double));
			pst->gv_vari = (double *) realloc(pst->gv_vari, pst->static_length*sizeof(double));
			pst->gv_weight = gv_weight[i];
			pst->gv_buff = (double *) realloc(pst->gv_buff, pst->length*sizeof(double));
			
			for (j = 0; j < pst->static_length; j++)
			{
				pst->gv_mean[j] = HTS_SStreamSet_get_gv_mean(sss, i, j);
				pst->gv_vari[j] = HTS_SStreamSet_get_gv_vari(sss, i, j);
			}
		} 
		else
		{
			pst->gv_mean = NULL;
			pst->gv_vari = NULL;
			pst->gv_buff = NULL;
		}
		
		/* copy pdfs */
		if (HTS_SStreamSet_is_msd(sss, i))
		{  	/* for MSD */
			for (state = state_init, frame = frame_init, msd_frame = msd_frame_init; state < HTS_SStreamSet_get_total_state(sss); state++)
			{             
				for (j = 0; j < HTS_SStreamSet_get_duration(sss, state); j++)
				{
					if (pst->msd_flag[frame])
					{
						/* check current frame is MSD boundary or not */
						for (k = 0; k < pst->win_size; k++)
						{
							not_bound = TRUE;
							for (l = pst->win_l_width[k]; l <= pst->win_r_width[k]; l++)
							{
								if (frame + l < 0 || pss->total_frame <= frame + l || !pst->msd_flag[frame + l])
								{
									not_bound = FALSE;
									break;
								}
							}
							for (l = 0; l < pst->static_length; l++)
							{
								m = pst->static_length * k + l;
								pst->sm.mean[msd_frame][m] = HTS_SStreamSet_get_mean(sss, i, state, m);
								
								if (not_bound || k == 0)
									pst->sm.ivar[msd_frame][m] = HTS_finv(HTS_SStreamSet_get_vari(sss, i, state, m));
								else
									pst->sm.ivar[msd_frame][k] = 0.0;                        
							}
						}
						msd_frame++;
					}
					frame++;
				}
			}
		}
		else
		{  /* for non MSD */
			for (state = state_init, frame = frame_init; state < HTS_SStreamSet_get_total_state(sss); state++)
			{
				for (j = 0; j < HTS_SStreamSet_get_duration(sss, state); j++)
				{
					for (k = 0; k < pst->vector_length; k++)
					{
						pst->sm.mean[frame][k] = HTS_SStreamSet_get_mean(sss, i, state, k);
						pst->sm.ivar[frame][k] = HTS_finv(HTS_SStreamSet_get_vari(sss, i, state, k));
					}
					frame++;
				}
			}
		}   
		/* parameter generation */
		p_HTS_PStream_mlpg(pst, past);
		//HTS_PStream_mlpg(pst);
	}
}

/* p_HTS_PStreamSet_set: set different set of PDF streams */
void p_HTS_PStreamSet_set(HTS_PStreamSet *pss,  HTS_PStream *pstream, int nstream, int total_frame)
{
	pss->pstream = pstream;
	pss->nstream = nstream;
	pss->total_frame = total_frame;
}
