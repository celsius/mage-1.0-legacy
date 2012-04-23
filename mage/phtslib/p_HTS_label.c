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


#include <stdlib.h>             /* for atoi() */
#include <ctype.h>              /* for isgraph(),isdigit() */
#include <string.h>				/* strtok() */

/* hts_engine libraries */
#include "HTS_hidden.h"

/** 
 *	\file p_HTS_label.c 
 *	\brief Contains the implementation of all the streaming functions of the HTS_Engine for the labels set.
 *
 *	The functions implemented here, are the streaming version of the original label functions of the HTS_Engine.
 */

/* p_HTS_Label_load_from_str: load label from char pointer */
void p_HTS_Label_load_from_str(HTS_Label *label, int sampling_rate, int fperiod, char *label_str)
{
	char buff[HTS_MAXBUFLEN];
	HTS_LabelString *lstring = NULL;
	char result[HTS_MAXBUFLEN];
	int start, end, i = 0, j = 0;
	const double rate = sampling_rate / (fperiod * 1e+7);
	
	/* parse label file until a specific line */
	strcpy( buff, label_str );
	
	if (!isgraph(buff[0]))
		return;

	label->size++;
	
	if (label->tail)
	{
		lstring = label->tail;
		lstring->next = (HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
		lstring = lstring->next;
	} 
	else
	{                  /* first time */
		lstring = (HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
		label->head = lstring;
	}
	label->tail = lstring;   

	if (isdigit(buff[0]))
	{   /* has frame infomation */
		for (i = 0, j=0; label_str[i] != ' ' && label_str[i] != '\n' && label_str[i] != '\t'; i++, j++) 
			result[j] = label_str[i];		
		result[j] = '\0';
	
		start = atoi(result);
		
		label_str[i++];
		
		for (j=0; label_str[i] != ' ' && label_str[i] != '\n' && label_str[i] != '\t'; i++, j++) 
			result[j] = label_str[i];	
		
		result[j] = '\0';

		end = atoi(result);
		
		lstring->frame_flag = TRUE;
		lstring->frame = (int) (rate * (end - start));
		if (lstring->frame < 1)
			lstring->frame = 1;
	} 
	else
	{
		lstring->frame_flag = FALSE;
		lstring->frame = 0;
	}
	lstring->next = NULL;
	lstring->name = HTS_strdup(buff);
}

/* p_HTS_Label_load_from_fp: load label from file pointer */
void p_HTS_Label_load_from_fp(HTS_Label *label, int sampling_rate, int fperiod, FILE *fp) 
{
	char buff[HTS_MAXBUFLEN];
	HTS_LabelString *lstring = NULL;
	int start, end;
	const double rate = sampling_rate / (fperiod * 1e+7);
	
	/* parse label file until a specific line */
	HTS_get_token(fp, buff);	
	
	if (!isgraph(buff[0]))
		return;
	
	label->size++;
	
	if (label->tail)
	{
		lstring = label->tail;
		lstring->next =
		(HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
		lstring = lstring->next;
	} 
	else
	{                  /* first time */
		lstring = (HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
		label->head = lstring;
		label->tail = lstring;
	}
	if (isdigit(buff[0]))
	{   /* has frame infomation */
		start = atoi(buff);
		HTS_get_token(fp, buff);
		end = atoi(buff);
		HTS_get_token(fp, buff);
		lstring->frame_flag = TRUE;
		lstring->frame = (int) (rate * (end - start));
		if (lstring->frame < 1)
            lstring->frame = 1;
	} 
	else
	{
		lstring->frame_flag = FALSE;
		lstring->frame = 0;
	}
	lstring->next = NULL;
	lstring->name = HTS_strdup(buff);
	label->tail = lstring;   
}

/* p_HTS_Label_get_string: get label string */
char *p_HTS_Label_get_string(HTS_Label *label)
{
	HTS_LabelString *lstring = label->tail;
	return lstring->name;
}

/* p_HTS_Label_get_frame_specified_flag: get frame specified flag */
HTS_Boolean p_HTS_Label_get_frame_specified_flag(HTS_Label *label)
{
	HTS_LabelString *lstring = label->tail;
	return lstring->frame_flag;
}

/* p_HTS_Label_get_frame: get frame length */
int p_HTS_Label_get_frame(HTS_Label *label)
{
	HTS_LabelString *lstring = label->tail;	
	return lstring->frame;
}

/* p_HTS_Label_set: set different set of labels */
void p_HTS_Label_set(HTS_Label *label, HTS_LabelString *head, HTS_LabelString *tail, int size, double speech_speed)
{
	label->head = head;
	label->tail = tail;
	label->size = size;
	label->speech_speed = speech_speed;
}

/* p_HTS_Label_set_frame: set frame length */
void p_HTS_Label_set_frame(HTS_Label *label, int frame_length)
{
	HTS_LabelString *lstring = label->tail;
	
	if (!lstring)
		return;
	if (frame_length >= 1)
	{
		lstring->frame_flag = TRUE;
		lstring->frame = frame_length;
	} 
	else
	{
		lstring->frame_flag = FALSE;
		lstring->frame = 0;
	}
}

/* p_HTS_Label_set_speech_speed: set speech speed rate */
void p_HTS_Label_set_speech_speed(HTS_Label *label, double speech_speed)
{
	if (speech_speed > 0.0)
		label->speech_speed = speech_speed;
}
