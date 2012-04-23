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


#include <string.h>             /* for strlen(),strstr(),strrchr(),strcmp() */

/* hts_engine libraries */
#include "HTS_hidden.h"

/** 
 *	\file p_HTS_model.c 
 *	\brief Contains the implementation of all the streaming functions of the HTS_Engine for the set of duration models, HMMs and GV models.
 *
 *	The functions implemented here, are the streaming version of the original model functions of the HTS_Engine.
 */

/* HTS_dp_match: recursive matching */
static HTS_Boolean HTS_dp_match(const char *string, const char *pattern,
                                const int pos, const int max)
{
	if (pos > max)
		return FALSE;
	if (string[0] == '\0' && pattern[0] == '\0')
		return TRUE;
	if (pattern[0] == '*') {
		if (HTS_dp_match(string + 1, pattern, pos + 1, max) == 1)
			return TRUE;
		else
			return HTS_dp_match(string, pattern + 1, pos, max);
	}
	if (string[0] == pattern[0] || pattern[0] == '?') {
		if (HTS_dp_match(string + 1, pattern + 1, pos + 1, max + 1) == 1)
			return TRUE;
	}
	
	return FALSE;
}

/* HTS_pattern_match: pattern matching function */
static HTS_Boolean HTS_pattern_match(const char *string, const char *pattern)
{
	int i, j;
	int buff_length, max = 0, nstar = 0, nquestion = 0;
	char buff[HTS_MAXBUFLEN];
	const int pattern_length = strlen(pattern);
	
	for (i = 0; i < pattern_length; i++) {
		switch (pattern[i]) {
			case '*':
				nstar++;
				break;
			case '?':
				nquestion++;
				max++;
				break;
			default:
				max++;
		}
	}
	if (nstar == 2 && nquestion == 0 && pattern[0] == '*'
		&& pattern[i - 1] == '*') {
		/* only string matching is required */
		buff_length = i - 2;
		for (i = 0, j = 1; i < buff_length; i++, j++)
			buff[i] = pattern[j];
		buff[buff_length] = '\0';
		if (strstr(string, buff) != NULL)
			return TRUE;
		else
			return FALSE;
	} else
		return HTS_dp_match(string, pattern, 0, (int) (strlen(string) - max));
}

/* HTS_Question_match: check given string match given question */
static HTS_Boolean HTS_Question_match(const HTS_Question *question,
                                      const char *string)
{
	HTS_Pattern *pattern;
	
	for (pattern = question->head; pattern; pattern = pattern->next)
		if (HTS_pattern_match(string, pattern->string))
			return TRUE;
	
	return FALSE;
}

/* HTS_Node_search: tree search */
static int HTS_Tree_search_node(HTS_Tree *tree, const char *string)
{
	HTS_Node *node = tree->root;
	
	while (node != NULL) {
		if (HTS_Question_match(node->quest, string)) {
			if (node->yes->pdf > 0)
				return node->yes->pdf;
			node = node->yes;
		} else {
			if (node->no->pdf > 0)
				return node->no->pdf;
			node = node->no;
		}
	}
	
	HTS_error(1, "HTS_Tree_search_node: Cannot find node.\n");
	return -1;                   /* make compiler happy */
}

/* p_HTS_ModelSet_get_duration: get duration using interpolation weight */
void p_HTS_ModelSet_get_duration(HTS_ModelSet *ms, char *label_str, double *mean, double *vari, double *iw)
{
	int i, j;
	int pdf_index1, pdf_index2;
	const int vector_length = ms->duration.vector_length;
	HTS_Tree *tree;
	HTS_Pattern *pattern;
	HTS_Boolean find;
	
	for (i = 2; i <= ms->nstate + 1; i++)
	{
		mean[i] = 0.0;
		vari[i] = 0.0;
	}
	
	for (i = 0; i < ms->duration.interpolation_size; i++)
	{
		find = FALSE;
		pdf_index1 = 2;
		pdf_index2 = 1;
		
		for (tree = ms->duration.model[i].tree; tree; tree = tree->next)
		{
			pattern = tree->head;
			
			if (!pattern)
				find = TRUE;
			
			for (; pattern; pattern = pattern->next)
				if (HTS_pattern_match(label_str, pattern->string))
				{
					find = TRUE;
					break;
				}
			
			if (find)
				break;
			
			pdf_index1++;
		}
		
		if (tree)
			pdf_index2 = HTS_Tree_search_node(tree, label_str);
		
		for (j = 0; j < ms->nstate; j++)
		{
			mean[j + 2] += iw[i] * ms->duration.model[i].pdf[pdf_index1][pdf_index2][j];
			
			vari[j + 2] += iw[i] * iw[i] * ms->duration.model[i].pdf[pdf_index1][pdf_index2][j + vector_length];
			
		}
	}
}