
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


#ifndef __p_HTS_engine
#define __p_HTS_engine 1

#include "HTS_hidden.h"

/** 
 *	\file p_HTS_engine.h 
 *	\brief Contains the decleration of all the streaming functions of the HTS_Engine.
 *
 *	The functions here, are the streaming version of the original HTS_Engine functions.
 */

/*  ----------------------- model method --------------------------  */

/** 
 * A streaming model method to get the duration using interpolation weight. 
 *
 * @param ms a HTS_ModelSet pointer to be updated. 
 * @param label_str a character pointer to a phonetic label. 
 * @param mean a pointer to a mean vector sequence.
 * @param vari a pointer to a variance vector sequence.
 * @param iw a pointer to the weights for duration interpolation.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_ModelSet_get_duration: get duration using interpolation weight */
void p_HTS_ModelSet_get_duration(HTS_ModelSet *ms, char *label_str, double *mean, double *vari, double *iw); 


/*  ----------------------- label method --------------------------  */

/** 
 * A streaming label method to load a phonetic label from a string. 
 *
 * @param label a HTS_Label pointer to be updated. 
 * @param sampling_rate an integer for the sampling rate. 
 * @param fperiod an integer for the frame period.
 * @param label_str a character pointer to a phonetic label. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Label_load_from_str: load label from string */
void p_HTS_Label_load_from_str(HTS_Label *label, int sampling_rate, int fperiod, char *label_str);  

/** 
 * A streaming label method to load a phonetic label from file pointer. 
 *
 * @param label a HTS_Label pointer to be updated. 
 * @param sampling_rate an integer for the sampling rate. 
 * @param fperiod an integer for the frame period.
 * @param fp a file pointer to the file containing phonetic labels. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Label_load_from_fp: load label from file pointer */
void p_HTS_Label_load_from_fp(HTS_Label *label, int sampling_rate, int fperiod,  FILE *fp); 

/** 
 * A streaming label method to access the phonetic label string from the last element of the label set. 
 *
 * @param label a HTS_Label pointer to be accessed. 
 * @return a character pointer to the phonetic label string.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Label_get_string: get label string */
char *p_HTS_Label_get_string(HTS_Label *label); 

/** 
 * A streaming label method to get the frame specified flag from the last element of the label set. 
 *
 * @param label a HTS_Label pointer to be accessed. 
 * @return an integer for the frame specified flag, 0 for false or 1 for true.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Label_get_frame_specified_flag: get frame specified flag */
HTS_Boolean p_HTS_Label_get_frame_specified_flag(HTS_Label *label); 

/** 
 * A streaming label method to get the frame length from the last element of the label set. 
 *
 * @param label a HTS_Label pointer to be accessed. 
 * @return an integer for the frame length.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Label_get_frame: get frame length */
int p_HTS_Label_get_frame(HTS_Label *label);  

/** 
 * A streaming label method to set a different set of labels. 
 *
 * @param label a HTS_Label pointer to be updated. 
 * @param head a HTS_LabelString pointer to the head pointer of the HTS_LabelString list. 
 * @param tail a HTS_LabelString pointer to the tail pointer of the HTS_LabelString list.  
 * @param size an integer for the size of the list, number of elements. 
 * @param speech_speed a double for the speech speed rate. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Label_set: set different set of labels */
void p_HTS_Label_set(HTS_Label *label, HTS_LabelString *head, HTS_LabelString *tail, int size, double speech_speed);  

/** 
 * A streaming label method to set the frame length of the last element of the label set. 
 *
 * @param label a HTS_Label pointer to be updated. 
 * @param frame_length an integer for the frame length.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Label_set_frame: set frame length */
void p_HTS_Label_set_frame(HTS_Label *label, int frame_length);  

/** 
 * A streaming label method to set the speech speed rate of the last element of the label set. 
 *
 * @param label a HTS_Label pointer to be updated. 
 * @param speech_speed a double for the speech speed rate.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Label_set_speech_speed: set speech speed rate */
void p_HTS_Label_set_speech_speed(HTS_Label *label, double speech_speed);  


/*  ----------------------- sstream method ------------------------  */

/** 
 * A streaming state stream method to initialize the state stream set. 
 *
 * @param sss a HTS_SStreamSet pointer to be initialized. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_SStreamSet_initialize: initialize state stream set */
void p_HTS_SStreamSet_initialize(HTS_SStreamSet *sss);    

/** 
 * A streaming state stream method to parse label and determine state duration. 
 *
 * @param sss a HTS_SStreamSet pointer to be updated. 
 * @param ms a HTS_ModelSet pointer to the set of duration models, HMMs and GV models. 
 * @param label a HTS_Label pointer to the label set. 
 * @param duration_iw a pointer to the weights for duration interpolation. 
 * @param parameter_iw a pointer to the weights for parameter interpolation. 
 * @param gv_iw a pointer to the weights for GV interpolation. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param duration a double for the duration of the phoneme in the label
 * @param mode an integer to determine the action applied on the duration trajectory, if mode=0, the existing trajectory 
 *             is overwritten with the value of duration, if mode=1, the existing trajectory is shifted up if duration>0 
 *			   or shifted down if duration<0, if mode=2, the existing trajectory is scaled.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_SStreamSet_create: parse label and determine state duration */
void p_HTS_SStreamSet_create(HTS_SStreamSet *sss, HTS_ModelSet *ms,
							 HTS_Label *label, double *duration_iw,
							 double **parameter_iw, double **gv_iw,  int **total_frame_array, float duration, int mode);   

/*  ----------------------- pstream method ------------------------  */

/** 
 * A streaming PDF stream method to generate parameter using GV weight. 
 *
 * @param pss a HTS_PStreamSet pointer to be updated. 
 * @param sss a HTS_SStreamSet pointer to the set of state streams. 
 * @param msd_threshold a pointer to the MSD thresholds. 
 * @param gv_weight a pointer to the GV weight. 
 * @param label a pointer to the label set. 
 * @param ms a pointer to the set of duration models, HMMs and GV models. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param length_array a pointer to the array with the stream lengths. 
 * @param past an integer for the number of past labels to be used. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_PStreamSet_create: parameter generation using GV weight */
void p_HTS_PStreamSet_create(HTS_PStreamSet *pss, HTS_SStreamSet *sss, double *msd_threshold, double *gv_weight, 
							 HTS_Label *label, HTS_ModelSet *ms, int *total_frame_array, int **length_array, int past);  

/** 
 * A streaming PDF stream method to set a different set of PDF streams. 
 *
 * @param pss a HTS_PStreamSet pointer to be updated. 
 * @param pstream a HTS_PStream pointer to the PDF streams. 
 * @param nstream an integer for the number of PDF streams.  
 * @param total_frame an integer for the total number of frames. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_PStreamSet_set: set different set of PDF streams */
void p_HTS_PStreamSet_set(HTS_PStreamSet *pss,  HTS_PStream *pstream, int nstream, int total_frame);

/*  ----------------------- gstream method ------------------------  */

/** 
 * A streaming generated stream method to initialize the set of generated parameter stream. 
 *
 * @param gss a HTS_GStreamSet pointer to be initialized. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_GStreamSet_initialize: initialize generated parameter stream set */
void p_HTS_GStreamSet_initialize(HTS_GStreamSet *gss);  

/** 
 * A streaming generated stream method to generate speech parameters. 
 *
 * @param gss a HTS_GStreamSet pointer to be updated. 
 * @param pss a HTS_PStreamSet pointer to the set of PDF streams. 
 * @param sss a HTS_SStreamSet pointer to the set of state stream. 
 * @param label a HTS_Label pointer to the label set.
 * @param stage an integer for stage, Gamma=-1, if stage=0 then Gamma=0. 
 * @param use_log_gain an integer for the log gain flag (for LSP). 
 * @param sampling_rate an integer for the sampling rate. 
 * @param fperiod an intefer for the frame period. 
 * @param alpha a double for the all-pass constant. 
 * @param beta a double for the postfiltering coefficient. 
 * @param audio_buff_size an integer for the audio buffer size (for audio device). 
 * @param delay an integer for the number of labels used as delay, number of used future labels. 
 * @param reset a boolean integer for the number of labels to be synthesised before refreshing the engine. 
 *              If reset=0 then the delayed frames are not estimated/synthesised, if reset=1 then the delayed framd are estimated/synthesised. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param length_array a pointer to the array with the stream lengths. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_GStreamSet_create_param: generate speech parameters */
void p_HTS_GStreamSet_create_param(HTS_GStreamSet *gss, HTS_PStreamSet *pss, HTS_SStreamSet *sss, HTS_Label *label,
								   int stage, HTS_Boolean use_log_gain, int sampling_rate, int fperiod, double alpha,
								   double beta, int audio_buff_size,  int delay, int *total_frame_array, int *length_array, HTS_Boolean reset);

/** 
 * A streaming generated stream method to generate speech samples. 
 *
 * @param gss a HTS_GStreamSet pointer to be updated. 
 * @param pss a HTS_PStreamSet pointer to the set of PDF streams. 
 * @param sss a HTS_SStreamSet pointer to the set of state stream. 
 * @param label a HTS_Label pointer to the label set.
 * @param stage an integer for stage, Gamma=-1, if stage=0 then Gamma=0. 
 * @param use_log_gain an integer for the log gain flag (for LSP). 
 * @param sampling_rate an integer for the sampling rate. 
 * @param fperiod an intefer for the frame period. 
 * @param alpha a double for the all-pass constant. 
 * @param beta a double for the postfiltering coefficient. 
 * @param audio_buff_size an integer for the audio buffer size (for audio device). 
 * @param delay an integer for the number of labels used as delay, number of used future labels. 
 * @param v a HTS_Vocoder pointer to the structure for setting of vocoder. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_GStreamSet_create_speech_samples: generate speech samples */
void p_HTS_GStreamSet_create_speech_samples(HTS_GStreamSet *gss, HTS_PStreamSet *pss, HTS_SStreamSet *sss, HTS_Label *label,
									int stage, HTS_Boolean use_log_gain, int sampling_rate, int fperiod, double alpha,
									double beta, int audio_buff_size, int delay, HTS_Vocoder *v, int *total_frame_array); 

/** 
 * A streaming generated stream method to generate speech parameters and speech samples. 
 *
 * @param gss a HTS_GStreamSet pointer to be updated. 
 * @param pss a HTS_PStreamSet pointer to the set of PDF streams. 
 * @param sss a HTS_SStreamSet pointer to the set of state stream. 
 * @param label a HTS_Label pointer to the label set.
 * @param stage an integer for stage, Gamma=-1, if stage=0 then Gamma=0. 
 * @param use_log_gain an integer for the log gain flag (for LSP). 
 * @param sampling_rate an integer for the sampling rate. 
 * @param fperiod an intefer for the frame period. 
 * @param alpha a double for the all-pass constant. 
 * @param beta a double for the postfiltering coefficient. 
 * @param audio_buff_size an integer for the audio buffer size (for audio device). 
 * @param delay an integer for the number of labels used as delay, number of used future labels. 
 * @param reset a boolean integer for the number of labels to be synthesised before refreshing the engine. 
 *              If reset=0 then the delayed frames are not estimated/synthesised, if reset=1 then the delayed framd are estimated/synthesised. 
 * @param v a HTS_Vocoder pointer to the structure for setting of vocoder. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param length_array a pointer to the array with the stream lengths. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_GStreamSet_create: generate speech parameters & speech samples */
void p_HTS_GStreamSet_create(HTS_GStreamSet *gss, HTS_PStreamSet *pss, HTS_SStreamSet *sss, HTS_Label *label,
							 int stage, HTS_Boolean use_log_gain, int sampling_rate, int fperiod, double alpha,
							 double beta, int audio_buff_size, int delay, HTS_Vocoder *v, int *total_frame_array, int *length_array, HTS_Boolean reset);  

/** 
 * A streaming generated stream method to set a different set of generated parameter stream. 
 *
 * @param gss a HTS_GStreamSet pointer to be updated. 
 * @param total_nsample an integer for the number of samples.
 * @param total_frame an integer for the total number of frames.
 * @param nstream an integer for the number of stream.
 * @param gstream a HTS_GStream pointer to the generated parameter stream.
 * @param gspeech a short pointer to the generated speech samples.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_GStreamSet_set_gstream: set different set of generated parameter stream */
void p_HTS_GStreamSet_set_gstream(HTS_GStreamSet *gss,  int total_nsample, int total_frame, int nstream, HTS_GStream *gstream, short *gspeech);

/** 
 * A streaming generated stream method to set a different pitch value in the generated speech parameters. 
 *
 * @param gss a HTS_GStreamSet pointer to be updated. 
 * @param sss a HTS_SStreamSet pointer to the set of state stream. 
 * @param label a HTS_Label pointer to the label set.
 * @param delay an integer for the number of labels used as delay, number of used future labels. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param lf0 a double for the new pitch value.
 * @param mode an integer to determine the action applied on the pitch trajectory, if mode=0, the existing trajectory 
 *             is overwritten with the value of lf0, if mode=1, the existing trajectory is shifted up if lf0>0 or shifted down if lf0<0.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_GStreamSet_set_gstream_pitch: set a different pitch value in the generated speech parameters */
void p_HTS_GStreamSet_set_gstream_pitch(HTS_GStreamSet *gss, HTS_SStreamSet *sss, HTS_Label *label, int delay,  int *total_frame_array, double lf0, int mode);

/*  ----------------------- engine method -------------------------  */

/** 
 * A streaming engine method to load a phonetic label from a string. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param label_str character pointer to a phonetic label.  
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Engine_load_label_from_str: load label from string */
void p_HTS_Engine_load_label_from_str(HTS_Engine *engine,  char *label_str); 

/** 
 * A streaming engine method to load a phonetic label from file pointer. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param fp a file pointer to the file containing phonetic labels. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Engine_load_label_from_fp: load label from file pointer */
void p_HTS_Engine_load_label_from_fp(HTS_Engine *engine, FILE *fp); 

/** 
 * A streaming engine method to parse label and determine state duration. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param duration a double for the duration of the phoneme in the label
 * @param mode an integer to determine the action applied on the duration trajectory, if mode=0, the existing trajectory 
 *             is overwritten with the value of duration, if mode=1, the existing trajectory is shifted up if duration>0 
 *			   or shifted down if duration<0, if mode=2, the existing trajectory is scaled.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Engine_create_sstream: parse label and determine state duration */
void p_HTS_Engine_create_sstream(HTS_Engine *engine,  int **total_frame_array, double duration, int mode);  

/** 
 * A streaming engine method to generate parameter using GV weight. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param length_array a pointer to the array with the stream lengths. 
 * @param past an integer for the number of past labels to be used. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Engine_create_pstream: generate speech parameter vector sequence */
void p_HTS_Engine_create_pstream(HTS_Engine *engine, int *total_frame_array, int **length_array, int past); 

/** 
 * A streaming engine method to generate speech parameters. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param delay an integer for the number of labels used as delay, number of used future labels. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param length_array a pointer to the array with the stream lengths. 
 * @param reset a boolean integer for the number of labels to be synthesised before refreshing the engine. 
 *              If reset=0 then the delayed frames are not estimated/synthesised, if reset=1 then the delayed framd are estimated/synthesised. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 * @attention To synthesize speech, you must set stream[0]=spectrum models and spectrum[1]=lf0 models.
 */
/* p_HTS_Engine_create_gstream_param: generate the speech parameters */
void p_HTS_Engine_create_gstream_param(HTS_Engine *engine, int delay, int *total_frame_array, int *length_array, HTS_Boolean reset); 

/** 
 * A streaming engine method to generate speech samples. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param delay an integer for the number of labels used as delay, number of used future labels. 
 * @param v a HTS_Vocoder pointer to the structure for setting of vocoder.  
 * @param total_frame_array a pointer to the array with the total number of frames. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 * @attention To synthesize speech, you must set stream[0]=spectrum models and spectrum[1]=lf0 models.
 */
/* p_HTS_Engine_create_gstream_speech_samples: generate speech samples */
void p_HTS_Engine_create_gstream_speech_samples(HTS_Engine *engine, int delay, HTS_Vocoder *v, int *total_frame_array); 

/** 
 * A streaming engine method to set a different set of labels. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param label a HTS_Label pointer to the new label set. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Engine_set_labels: set different set of labels */
void p_HTS_Engine_set_labels(HTS_Engine *engine, HTS_Label *label);

/** 
 * A streaming enfine method to set a different set of PDF streams. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param pss a HTS_PStreamSet pointer to the new PDF stream set. 
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Engine_set_pstream: set speech parameter vector sequence */
void p_HTS_Engine_set_pstream(HTS_Engine *engine, HTS_PStreamSet *pss); 

/** 
 * A streaming engine method to set a different set of generated parameter stream. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param gss a HTS_GStreamSet pointer to the new set of generated parameter stream.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Engine_set_gstream: set different speech parameters */
void p_HTS_Engine_set_gstream(HTS_Engine *engine,  HTS_GStreamSet *gss); 

/** 
 * A streaming engine method to set a different pitch value in the generated speech parameters. 
 *
 * @param engine a HTS_Engine pointer to be updated. 
 * @param delay an integer for the number of labels used as delay, number of used future labels. 
 * @param total_frame_array a pointer to the array with the total number of frames. 
 * @param pitch a double for the new pitch value.
 * @param mode an integer to determine the action applied on the pitch trajectory, if mode=0, the existing trajectory 
 *             is overwritten with the value of pitch, if mode=1, the existing trajectory is shifted up if pitch>0 
 *			   or shifted down if pitch<0.
 *
 * @attention You must initialize the pHTS environment and the HTS engine before calling this function.
 */
/* p_HTS_Engine_set_gstream_pitch: set different pitch value in the generated speech parameters */
void p_HTS_Engine_set_gstream_pitch(HTS_Engine *engine,  int delay, int *total_frame_array, double pitch, int mode);  

#endif//__p_HTS_engine

