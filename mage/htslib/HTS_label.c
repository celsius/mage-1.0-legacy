/* ----------------------------------------------------------------- */
/*           The HMM-Based Speech Synthesis System (HTS)             */
/*           developed by HTS Working Group                          */
/*           http://hts.sp.nitech.ac.jp/                             */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2001-2008  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/*                2001-2008  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the HTS working group nor the names of its  */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/* $id: HTS_label.c,v 1.1.2.1 2008/02/12 10:47:43 uratec Exp $ */

#include <stdlib.h>             /* for atoi() */
#include <ctype.h>              /* for isgraph(),isdigit() */

/* hts_engine libraries */
#include "HTS_hidden.h"

/* HTS_Label_initialize: initialize label */
void HTS_Label_initialize(HTS_Label *label)
{
	label->head = NULL;
	label->tail = NULL;
	label->size = 0;
	label->speech_speed = 1.0;
}

/* HTS_Label_load_from_fn: load label from file name */
void HTS_Label_load_from_fn(HTS_Label *label, int sampling_rate, int fperiod, char *fn)
{
	FILE *fp = HTS_get_fp(fn, "r");
	HTS_Label_load_from_fp(label, sampling_rate, fperiod, fp);
	fclose(fp);
}

/* HTS_Label_load_from_fp: load label from file pointer */
void HTS_Label_load_from_fp(HTS_Label *label, int sampling_rate, int fperiod, FILE *fp)
{
	char buff[HTS_MAXBUFLEN];
	HTS_LabelString *lstring = NULL;
	int start, end;
	const double rate = sampling_rate / (fperiod * 1e+7);
	
	if (label->head || label->size != 0)
		HTS_error(1, "HTS_Label_load_from_fp: label is not initialized.\n");
	/* parse label file */
	while (HTS_get_token(fp, buff))
	{
		if (!isgraph(buff[0]))
			break;
		label->size++;
		
		if (lstring)
		{
			lstring->next =
			(HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
			lstring = lstring->next;
		} 
		else
		{                  /* first time */
			lstring = (HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
			label->head = lstring;
		}
		if (isdigit(buff[0])) {   /* has frame infomation */
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
	}
}

/* HTS_Label_load_from_string: load label from string */
void HTS_Label_load_from_string(HTS_Label *label, int sampling_rate, int fperiod, char *data)
{
	char buff[HTS_MAXBUFLEN];
	HTS_LabelString *lstring = NULL;
	int data_index = 0;          /* data index */
	int start, end;
	const double rate = sampling_rate / (fperiod * 1e+7);
	
	if (label->head || label->size != 0)
		HTS_error(1, "HTS_Label_load_from_fp: label list is not initialized.\n");
	/* copy label */
	while (HTS_get_token_from_string(data, &data_index, buff)) {
		if (!isgraph(buff[0]))
			break;
		label->size++;
		
		if (lstring) {
			lstring->next =
			(HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
			lstring = lstring->next;
		} else {                  /* first time */
			lstring = (HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
			label->head = lstring;
		}
		if (isdigit(buff[0])) {   /* has frame infomation */
			start = atoi(buff);
			HTS_get_token_from_string(data, &data_index, buff);
			end = atoi(buff);
			HTS_get_token_from_string(data, &data_index, buff);
			lstring->frame_flag = TRUE;
			lstring->frame = (int) (rate * (end - start));
			if (lstring->frame < 1)
				lstring->frame = 1;
		} else {
			lstring->frame_flag = FALSE;
			lstring->frame = 0;
		}
		lstring->next = NULL;
		lstring->name = HTS_strdup(buff);
	}
}

/* HTS_Label_load_from_string_list: load label from string list */
void HTS_Label_load_from_string_list(HTS_Label *label, int sampling_rate,
                                     int fperiod, char **data, int size)
{
	char buff[HTS_MAXBUFLEN];
	HTS_LabelString *lstring = NULL;
	int i;
	int data_index;
	int start, end;
	const double rate = sampling_rate / (fperiod * 1e+7);
	
	if (label->head || label->size != 0)
		HTS_error(1, "HTS_Label_load_from_fp: label list is not initialized.\n");
	/* copy label */
	for (i = 0; i < size; i++) {
		if (!isgraph(data[i][0]))
			break;
		label->size++;
		
		if (lstring) {
			lstring->next =
			(HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
			lstring = lstring->next;
		} else {                  /* first time */
			lstring = (HTS_LabelString *) HTS_calloc(1, sizeof(HTS_LabelString));
			label->head = lstring;
		}
		data_index = 0;
		if (isdigit(data[i][0])) {        /* has frame infomation */
			HTS_get_token_from_string(data[i], &data_index, buff);
			start = atoi(buff);
			HTS_get_token_from_string(data[i], &data_index, buff);
			end = atoi(buff);
			HTS_get_token_from_string(data[i], &data_index, buff);
			lstring->name = HTS_strdup(&buff[data_index]);
			lstring->frame_flag = TRUE;
			lstring->frame = (int) (rate * (end - start));
			if (lstring->frame < 1)
				lstring->frame = 1;
		} else {
			lstring->frame_flag = FALSE;
			lstring->frame = 0;
			lstring->name = HTS_strdup(data[i]);
		}
		lstring->next = NULL;
	}
}

/* HTS_Label_set_frame: set frame length */
void HTS_Label_set_frame(HTS_Label *label, int string_index, int i)
{
	HTS_LabelString *lstring = label->head;
	
	while (string_index-- && lstring)
		lstring = lstring->next;
	if (!lstring)
		return;
	if (i >= 1) {
		lstring->frame_flag = TRUE;
		lstring->frame = i;
	} else {
		lstring->frame_flag = FALSE;
		lstring->frame = 0;
	}
}

/* HTS_Label_set_speech_speed: set speech speed rate */
void HTS_Label_set_speech_speed(HTS_Label *label, double f)
{
	if (f > 0.0 && f <= 10.0)
		label->speech_speed = f;
}

/* HTS_Label_get_size: get number of label string */
int HTS_Label_get_size(HTS_Label *label)
{
	return label->size;
}

/* HTS_Label_get_string: get label string */
char *HTS_Label_get_string(HTS_Label *label, int string_index)
{
	HTS_LabelString *lstring = label->head;
	
	while (string_index-- && lstring)
		lstring = lstring->next;
	if (!lstring)
		return NULL;
	return lstring->name;
}

/* HTS_Label_get_frame_specified_flag: get frame specified flag */
HTS_Boolean HTS_Label_get_frame_specified_flag(HTS_Label *label,
                                               int string_index)
{
	HTS_LabelString *lstring = label->head;
	
	while (string_index-- && lstring)
		lstring = lstring->next;
	if (!lstring)
		return FALSE;
	return lstring->frame_flag;
}

/* HTS_Label_get_frame: get frame length */
int HTS_Label_get_frame(HTS_Label *label, int string_index)
{
	HTS_LabelString *lstring = label->head;
	
	while (string_index-- && lstring)
		lstring = lstring->next;
	if (!lstring)
		return 0;
	return lstring->frame;
}

/* HTS_Label_get_speech_speed: get speech speed rate */
double HTS_Label_get_speech_speed(HTS_Label *label)
{
	return label->speech_speed;
}

/* HTS_Label_clear: free label */
void HTS_Label_clear(HTS_Label *label)
{
	HTS_LabelString *lstring, *next_lstring;
	
	for (lstring = label->head; lstring; lstring = next_lstring) {
		next_lstring = lstring->next;
		HTS_free(lstring->name);
		HTS_free(lstring);
	}
	HTS_Label_initialize(label);
}
