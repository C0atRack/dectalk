/*
 ***********************************************************************
 *                                                                       
 *                           Coryright (c)                              
 *    � Digital Equipment Corporation 1996, 1997. All rights reserved.    
 *                                                                      
 *    Restricted Rights: Use, duplication, or disclosure by the U.S.    
 *    Government is subject to restrictions as set forth in subparagraph
 *    (c) (1) (ii) of DFARS 252.227-7013, or in FAR 52.227-19, or in FAR
 *    52.227-14 Alt. III, as applicable.                                
 *                                                                      
 *    This software is proprietary to and embodies the confidential     
 *    technology of Digital Equipment Corporation and other parties.    
 *    Possession, use, or copying of this software and media is authorized
 *    only pursuant to a valid written license from Digital or an        
 *    authorized sublicensor.                                            
 *                                                                       
 *********************************************************************** 
 *    File Name:	ls_proc2.c
 *    Author:		JDB                                         
 *    Creation Date:06/24/96                                                   
 *                                                                             
 *    Functionality:                                                           
 * 	  Heuristic output routines
 * 	  used by letter to sound to speak
 *    numbers, part numbers, and other more
 *    complex items. Routines are common to English & German processing.
 *                                                                             
 ***********************************************************************       
 *                                                                             
 * Rev	Who  	Date		    Description                    
 * ---	-----	------------	---------------------------------------
 * 001  JDB     05/31/1996      first creation
 * 001  GL      08/28/1996      us ls_util_lookup to replace ls_dict_blook()
 *                                                                             
 */


 
/*
 *      Function Name: ls_proc_do_sign()      
 *
 *  	Description: Speak a "sign" like character, followed
 * 		by a word boundry. The "-" is pronounced "minus"
 * 		without using the dictionary, just in case some future
 * 		edit decides that it should be "dash". All the others
 * 		are just handed to the dictionary code.
 *
 *      Arguments: LPTTS_HANDLE_T phTTS,
 *				   int sign
 *
 *      Return Value: void
 *
 *      Comments:
 *
 */
void ls_proc_do_sign(LPTTS_HANDLE_T phTTS,int sign)
{
	LETTER	lbuf[2];

	if (sign == '-')
	{
		/* Special case.	*/
		ls_util_send_phone_list(phTTS,pminus);
		ls_util_send_phone(phTTS,WBOUND);
	}
	else	
	{
	 	if (sign == '+')
		{			/* Special case.	*/
			ls_util_send_phone_list(phTTS,pplus);
			ls_util_send_phone(phTTS,WBOUND);
		}
	 	else 
	 	{
	 		if (sign != 0)
			{		/* Dictionary case.	*/
				lbuf[0].l_ch = ' ';
				lbuf[0].l_ip = NULL;
				lbuf[1].l_ch = sign;
				lbuf[1].l_ip = NULL;
#ifdef ENGLISH_US
                                if (ls_util_lookup(phTTS,&lbuf[0], &lbuf[2], FIRST) == MISS)
					ls_util_send_phone(phTTS,EY);
#endif
				ls_util_send_phone(phTTS,WBOUND);
			}
		}
	}
}

/*
 * Speak a part number, and any
 * index markers that are imbedded in the
 * string of characters. The code works best if
 * the part number has DECtalk's standard part number
 * syntax, but it won't blow up if it doesn't.
 * I don't really like the way that the three case
 * arms all check if the inter-chunk WBOUND is
 * needed.
 *
 * The Kurzweil spelling rules are a little
 * bit different. The number processing rules never
 * wake up. The "-" and "/" characters are never
 * spoken. I could not decide if there should be a
 * pause on these characters, so I made it an
 * option, controlled by a new bit in the modeflag
 * option word.
 */
 
/*
 *      Function Name: ls_proc_do_part_number()      
 *
 *  	Description: Speak a part number, and any
 * 					 index markers that are imbedded in the
 * 					 string of characters. The code works best if
 * 					 the part number has DECtalk's standard part number
 * 					 syntax, but it won't blow up if it doesn't.
 * 					 I don't really like the way that the three case
 * 					 arms all check if the inter-chunk WBOUND is
 * 					 needed.
 *
 * 					 The Kurzweil spelling rules are a little
 * 					 bit different. The number processing rules never
 * 					 wake up. The "-" and "/" characters are never
 * 					 spoken. I could not decide if there should be a
 * 					 pause on these characters, so I made it an
 * 					 option, controlled by a new bit in the modeflag
 * 					 option word. 
 *
 *      Arguments: LPTTS_HANDLE_T phTTS,LETTER *llp, LETTER *rlp
 *
 *      Return Value: void 
 *
 *      Comments:
 *
 */
void ls_proc_do_part_number(LPTTS_HANDLE_T phTTS,LETTER *llp, LETTER *rlp)
{
	LETTER *blp;
	int	speed;

	int	nd;

	while (llp != rlp) 
	{
		blp = llp;
		++llp;
		if (blp->l_ch=='-' || blp->l_ch=='/') 
		{
			ls_spel_spell(phTTS,blp, llp);
			if (llp != rlp)
				ls_util_send_phone(phTTS,WBOUND);
		} 
		else
		{ 
			if (blp->l_ch>='0' && blp->l_ch<='9') 
			{
				while (llp!=rlp && llp->l_ch>='0' && llp->l_ch<='9')
					++llp;
				if ((nd = llp-blp) == 2)
				{
					ls_proc_do_2_digits(phTTS,blp);
				}
				else
				{ 
					if (nd == 3)
					{
						ls_proc_do_3_digits(phTTS,blp);
					}
					else
					{ 
						if (nd == 4)
							ls_proc_do_4_digits(phTTS,blp);
						else				
							ls_spel_spell(phTTS,blp, llp);
					}
				}
				if (llp != rlp)
				{
					ls_util_send_phone(phTTS,WBOUND);
				}
			} 
			else 
			{
				while (llp!=rlp && ls_proc_is_a_part(llp->l_ch)!=FALSE)
					++llp;
				if (llp-blp<3 || ls_util_lookup(phTTS, blp, llp, FALSE)==MISS) 
				{
					speed = ls_spel_spell_speed(blp, llp);
					ls_spel_spell(phTTS,blp, llp);
					if (speed == FAST)
						ls_util_send_phone(phTTS,WBOUND);
					else
						ls_util_send_phone(phTTS,COMMA);
				} 
				else 
					if (llp != rlp)
						ls_util_send_phone(phTTS,WBOUND);
			}
		}
	}
}

 
/*
 *      Function Name: ls_proc_is_a_part()      
 *
 *  	Description: Return TRUE if the character
 * 					 "c" can be part of a part number alpha
 * 					 string. All characters but digits,
 * 					 the "-" and the "/" are this.
 *
 *      Arguments: int c
 *
 *      Return Value: int
 *
 *      Comments:
 *
 */
int ls_proc_is_a_part(int c)
{
	if (c=='-' || c=='/' || (c>='0' && c<='9'))
		return (FALSE);
	return (TRUE);
}

 
/*
 *      Function Name:       
 *
 *  	Description: 0X	spell.
 * 					 1X	speak X (as a "teen", "10" is a "teen").
 * 					 X0	speak X (as tens).
 * 					 XY	speak X (as tens), speak Y (as units).
 *
 *      Arguments: LPTTS_HANDLE_T phTTS,LETTER *lp
 *
 *      Return Value: void
 *
 *      Comments:
 *
 */ 
void ls_proc_do_2_digits(LPTTS_HANDLE_T phTTS,LETTER *lp)
{
	if (lp->l_ch == '0')
		ls_spel_spell(phTTS,lp, lp+2);
	else 
	{
		if (lp->l_ch == '1')
			ls_util_send_phone_list(phTTS,pteens[(lp+1)->l_ch-'0']);
		else 
		{
#ifdef ENGLISH_US
			ls_util_send_phone_list(phTTS,ptens[lp->l_ch-'0']);
#endif
#ifdef GERMAN
			ls_util_send_phone_list(phTTS, punits[(lp+1)->l_ch-'0']);
#endif
			if ((lp+1)->l_ch != '0') 
			{
				ls_util_send_phone(phTTS,WBOUND);
#ifdef ENGLISH_US
				ls_util_send_phone_list(phTTS,punits[(lp+1)->l_ch-'0']);
#endif
#ifdef GERMAN
				ls_util_send_phone_list (phTTS, pand);
				ls_util_send_phone_list(phTTS, ptens[lp->l_ch-'0']);
#endif
			}
		}
	}
}

/*
 * 
 */
 
/*
 *      Function Name: ls_proc_do_3_digits()      
 *
 *  	Description: 0XX	spell.
 * 					 X00	speak X, "hundred".
 * 					 XYY	speak X, speak YY.
 *
 *      Arguments: LPTTS_HANDLE_T phTTS,LETTER *lp
 *
 *      Return Value: void
 *
 *      Comments:
 *
 */
void ls_proc_do_3_digits(LPTTS_HANDLE_T phTTS,LETTER *lp)
{
	if (lp->l_ch == '0')
		ls_spel_spell(phTTS,lp, lp+3);
	else 
	{
		ls_util_send_phone_list(phTTS,punits[lp->l_ch-'0']);
		ls_util_send_phone(phTTS,WBOUND);
		if ((lp+1)->l_ch=='0' && (lp+2)->l_ch=='0') 
		{
			ls_util_send_phone_list(phTTS,phundred);
		} else
			ls_proc_do_2_digits(phTTS,lp+1);
	}
}

 
/*
 *      Function Name: ls_proc_do_4_digits()      
 *
 *  	Description: 0XXX	spell.
 * 					 X000 speak X, "thousand".
 * 					 XX00 speak XX, "hundred".
 * 					 XXYY	speak XX, speak YY.
 *
 *      Arguments: LPTTS_HANDLE_T phTTS,LETTER *lp
 *
 *      Return Value: int
 *
 *      Comments:
 *
 */                   
int ls_proc_do_4_digits(LPTTS_HANDLE_T phTTS,LETTER *lp)
{
	if (lp->l_ch == '0')
		ls_spel_spell(phTTS,lp, lp+4);
	else 
	{
		if ((lp+2)->l_ch=='0' && (lp+3)->l_ch=='0') 
		{
			if ((lp+1)->l_ch == '0') 
			{
				ls_util_send_phone_list(phTTS,punits[lp->l_ch-'0']);
				ls_util_send_phone(phTTS,WBOUND);
				ls_util_send_phone_list(phTTS,pthousand);
			} 
			else 
			{
/* JDB: don't say "12 hundred" for German, say "one thousand, 2 hundred"... */
#ifdef GERMAN
				ls_util_send_phone_list(phTTS, punits[lp->l_ch-'0']);
				ls_util_send_phone(phTTS,WBOUND);
				ls_util_send_phone_list(phTTS,pthousand);
				ls_util_send_phone(phTTS, WBOUND);
				ls_util_send_phone_list(phTTS, punits[(lp+1)->l_ch-'0']);
#endif
#ifdef ENGLISH_US
				ls_proc_do_2_digits(phTTS,lp);
#endif
				ls_util_send_phone(phTTS,WBOUND);
				ls_util_send_phone_list(phTTS,phundred);
			}
		} 
		else 
		{

/* JDB: for German, don't speak a 4-digit word as 2 2-digit words, speak
   it as one 4-digit word!
*/
#ifdef GERMAN
			ls_util_send_phone_list(phTTS, punits[lp->l_ch-'0']);
			ls_util_send_phone(phTTS, WBOUND);
			ls_util_send_phone_list(phTTS, pthousand);
			ls_util_send_phone(phTTS, WBOUND);
			if ((lp+1)->l_ch != '0') {
	            ls_util_send_phone_list(phTTS, punits[(lp+1)->l_ch-'0']);
				ls_util_send_phone(phTTS, WBOUND);
				ls_util_send_phone_list(phTTS, phundred);
			}
#endif
#ifdef ENGLISH_US
			ls_proc_do_2_digits(phTTS,lp+0);
#endif
			ls_util_send_phone(phTTS,WBOUND);
			ls_proc_do_2_digits(phTTS,lp+2); 
		}
	}
	return 0; /* return value never used, BUT left in to keep 
	             consistent prototype with Spanish! */
}

 
/*
 *      Function Name: ls_proc_do_digit_group()
 *
 *  	Description: Speak a three digit group.
 * 		The number is in the supplied array of three
 * 		digits, with leading zeros. The caller must arrange
 * 		to put out the required index markers. This may be a
 * 		"bug", because the index marks will happen at the
 * 		end of the group, not as the digits get
 * 		spoken.
 *
 *      Arguments: LPTTS_HANDLE_T phTTS,
 *				   char buf[3], 
 *				   int oflag
 *
 *      Return Value: void
 *
 *      Comments:
 *
 */
void ls_proc_do_digit_group(LPTTS_HANDLE_T phTTS,char buf[3], int oflag)
{
	if (buf[0] != '0') 
	{
#ifdef GERMAN
		if (buf[0] != '1' && buf[0] != '0')
		{
		   ls_util_send_phone_list(phTTS,punits[buf[0]-'0']);
		   ls_util_send_phone(phTTS,WBOUND);
		}
#endif
#ifdef ENGLISH_US
		ls_util_send_phone_list(phTTS,punits[buf[0]-'0']);
		ls_util_send_phone(phTTS,WBOUND);
#endif
		ls_util_send_phone_list(phTTS,phundred);
		if (buf[1]=='0' && buf[2]=='0') 
		{
			if (oflag != FALSE)
			{
#ifdef ENGLISH_US
				ls_util_send_phone(phTTS,TH);
#endif
			}
			return;
		}
#ifdef ENGLISH_US
		ls_util_send_phone_list(phTTS,pand);
#endif
	}
	if (buf[1] == '1') 
	{
		ls_util_send_phone_list(phTTS,pteens[buf[2]-'0']);

		if (oflag != FALSE)
		{
#ifdef ENGLISH_US
			ls_util_send_phone(phTTS,TH);
#endif
		}
		return;
	}
	if (buf[1] != '0') 
	{

#ifdef GERMAN
		if (buf[2] != '0')
		{
			if (oflag != FALSE)
			{
				if (buf[1] == '0')
				   ls_util_send_phone_list(phTTS,pordin[buf[2]-'0']);
				else
				   ls_util_send_phone_list(phTTS,punits[buf[2]-'0']);
			}
			else
			{
				ls_util_send_phone_list(phTTS,punits[buf[2]-'0']);
			}
			ls_util_send_phone_list (phTTS, pand);
		}
#endif

		ls_util_send_phone_list(phTTS,ptens[buf[1]-'0']);
		if (buf[2] == '0') 
		{
			if (oflag != FALSE) 
			{
#ifdef ENGLISH_US
				ls_util_send_phone(phTTS,IX);
				ls_util_send_phone(phTTS,TH);
#endif
			}
			return;
		}
		ls_util_send_phone(phTTS,WBOUND);
#ifdef GERMAN
		return;
#endif
	}
	/* 
	 * if German is still here, we have a 0 in tens place and a unit that must 
     * be spoken...
	 */
	if (oflag != FALSE)
		ls_util_send_phone_list(phTTS,pordin[buf[2]-'0']);
	else
		ls_util_send_phone_list(phTTS,punits[buf[2]-'0']);
}

 
/*
 *      Function Name: ls_proc_do_number()
 *
 *  	Description: This is a general number speaking routine.
 * 					 It understands integers, fractional digits, and powers
 * 					 of 10. Long integers are spoken in groups. Shorter integers
 * 					 are spoken more cleverly. The "oflag" asks to have the number
 * 					 spoken as an ordinal (for dates, etc.); it only works right if
 * 					 the number is a small integer. Return TRUE if the number is
 * 					 plural, and FALSE if it is singular. This function calls
 * 					 itself recursively to speak the exponent of a floating
 * 					 point number; it cannot get stuck, because of the
 * 					 syntax of such numbers.
 *
 *      Arguments: LPTTS_HANDLE_T phTTS,
 *				   LETTER *llp, 
 *				   LETTER *rlp, 
 *				   int oflag
 *
 *      Return Value: int
 *
 *      Comments:
 *
 */         
int ls_proc_do_number(LPTTS_HANDLE_T phTTS,LETTER *llp, LETTER *rlp, int oflag)
{
	LETTER *tlp1;
	LETTER *tlp2;
	int	c;
	int	n;
	int	pflag;
	int	ndig;
	int	sflag;
	char		buf[18];
	PLTS_T pLts_t;
	pLts_t = phTTS->pLTSThreadData;

	tlp1  = llp;
	/* This handles integer parts like "1/2".			*/
	if (tlp1!=rlp && (tlp1->l_ch==0xBC || tlp1->l_ch==0xBD)) 
	{
		ls_spel_spell(phTTS,tlp1, rlp);
		return (FALSE);
	}
	pflag = FALSE;				/* Not plural.			*/
	sflag = FALSE;				/* No user "," seen.	*/
	ndig  = 0;
#ifdef LS2DEBUG
	printf("In ls_proc_do_number\n");

#endif
	while (tlp1!=rlp && (ls_proc_digit(tlp1)!=FALSE || tlp1->l_ch==pLts_t->schar)) 
	{
		if (tlp1->l_ch == pLts_t->schar)
			sflag = TRUE;
		else
			++ndig;
		++tlp1;
	}
	if (ndig>18 && sflag!=FALSE) 
	{
		/* Long, commas.	*/
		tlp2 = llp;
		while (tlp2 != tlp1) 
		{
			/* Pause where you are	*/
			c = tlp2->l_ch;		/* told to do so.	*/
			++tlp2;
			if (c == pLts_t->schar)
				ls_util_send_phone(phTTS,COMMA);
			else 
			{
				ls_util_send_phone_list(phTTS,punits[c-'0']);
				if (tlp2!=tlp1 && tlp2->l_ch!=pLts_t->schar)
					ls_util_send_phone(phTTS,WBOUND);
			}
		}
		pflag = TRUE;
	} 
	else
	{ 
		if (ndig>18 || (ndig>1 && llp->l_ch=='0')) 
		{
			tlp2 = llp;
			while (ndig >= 6) 
			{
				/* At least 1 group.	*/
				n = 0;
				while (n < 3) 
				{
					if ((c=tlp2->l_ch) != pLts_t->schar) 
					{
						if (n != 0)
							ls_util_send_phone(phTTS,WBOUND);
						++n;
						ls_util_send_phone_list(phTTS,punits[c-'0']);
					}
					++tlp2;
				}
				ls_util_send_phone(phTTS,COMMA);
				ndig -= 3;
			}
			n = 0;				/* Last group.		*/
			while (tlp2 != tlp1) 
			{
				if ((c=tlp2->l_ch) != pLts_t->schar) 
				{
					if (n != 0)
						ls_util_send_phone(phTTS,WBOUND);
					++n;
					ls_util_send_phone_list(phTTS,punits[c-'0']);
				}
				++tlp2;
			}
			pflag = TRUE;			/* Long => plural.	*/
		} 
		else 
		{
			if (ndig != 0) 
			{
				n = 18;				/* Right justify	*/

				tlp2 = tlp1;
				while (tlp2 != llp)
				{
					c = (--tlp2)->l_ch;
					if (c != pLts_t->schar)
						buf[--n] = c;
				}
				if (n!=17 || buf[17]!='1')	/* Watch for "1".	*/
					pflag = TRUE;
				while (n != 0)
					buf[--n] = '0';
				if (ls_proc_non_zero(&buf[0], 3) != FALSE) 
				{	/* Quadrillions		*/
					ls_proc_do_digit_group(phTTS,&buf[0], FALSE);
					ls_util_send_phone(phTTS,WBOUND);
					ls_util_send_phone_list(phTTS,pquadrillion);
					if (ls_proc_non_zero(&buf[3], 15) == FALSE) 
					{

						if (oflag != FALSE)
						{
#ifdef ENGLISH_US
							ls_util_send_phone(phTTS,TH);
#endif
						}
						goto out;
					}
					if (ls_proc_non_zero(&buf[4], 14) == FALSE)
						ls_util_send_phone(phTTS,VPSTART);
#ifdef ENGLISH_US
					else if (ls_proc_non_zero(&buf[3], 1) == FALSE)
						ls_util_send_phone_list(phTTS,pand);
					else
						ls_util_send_phone(phTTS,COMMA);
#endif
				}
				if (ls_proc_non_zero(&buf[3], 3) != FALSE) 
				{	/* Trillions		*/
					ls_proc_do_digit_group(phTTS,&buf[3], FALSE);
					ls_util_send_phone(phTTS,WBOUND);
					ls_util_send_phone_list(phTTS,ptrillion);
					if (ls_proc_non_zero(&buf[6], 12) == FALSE) 
					{

						if (oflag != FALSE)
						{
#ifdef ENGLISH_US					
							ls_util_send_phone(phTTS,TH);
#endif
						}
						goto out;
					}
					if (ls_proc_non_zero(&buf[7], 11) == FALSE)
						ls_util_send_phone(phTTS,VPSTART);
#ifdef ENGLISH_US
					else 
					if (ls_proc_non_zero(&buf[6], 1) == FALSE)
						ls_util_send_phone_list(phTTS,pand);
					else
						ls_util_send_phone(phTTS,COMMA);
#endif
				}
				if (ls_proc_non_zero(&buf[6], 3) != FALSE) 
				{	/* Billions		*/
					ls_proc_do_digit_group(phTTS,&buf[6], FALSE);
					ls_util_send_phone(phTTS,WBOUND);
					ls_util_send_phone_list(phTTS,pbillion);
					if (ls_proc_non_zero(&buf[9], 9) == FALSE) 
					{

						if (oflag != FALSE)
						{
#ifdef ENGLISH_US
							ls_util_send_phone(phTTS,TH);
#endif
						}
						goto out;
					}
					if (ls_proc_non_zero(&buf[10], 8) == FALSE)
						ls_util_send_phone(phTTS,VPSTART);
#ifdef ENGLISH_US
					else 
					if (ls_proc_non_zero(&buf[9], 1) == FALSE)
						ls_util_send_phone_list(phTTS,pand);
					else
						ls_util_send_phone(phTTS,COMMA);
#endif
				}
				if (ls_proc_non_zero(&buf[9], 3) != FALSE) 
				{	/* Millions		*/
					ls_proc_do_digit_group(phTTS,&buf[9], FALSE);
					ls_util_send_phone(phTTS,WBOUND);
					ls_util_send_phone_list(phTTS,pmillion);
					if (ls_proc_non_zero(&buf[12], 6) == FALSE) 
					{

						if (oflag != FALSE)
						{
#ifdef ENGLISH_US
							ls_util_send_phone(phTTS,TH);
#endif
						}
						goto out;
					}
					if (ls_proc_non_zero(&buf[13], 5) == FALSE)
						ls_util_send_phone(phTTS,VPSTART);
#ifdef ENGLISH_US
					else 
					if (ls_proc_non_zero(&buf[12], 1) == FALSE)
						ls_util_send_phone_list(phTTS,pand);
					else
						ls_util_send_phone(phTTS,COMMA);
#endif
				}

				if (ls_proc_non_zero(&buf[12], 3) != FALSE) 
				{	/* Thousands		*/

					ls_proc_do_digit_group(phTTS,&buf[12], FALSE);
					ls_util_send_phone(phTTS,WBOUND);
					ls_util_send_phone_list(phTTS,pthousand);
					if (ls_proc_non_zero(&buf[15], 3) == FALSE) 
					{
						if (oflag != FALSE)
						{
#ifdef ENGLISH_US
							ls_util_send_phone(phTTS,TH);
#endif
						}
						goto out;
					}
					if (ls_proc_non_zero(&buf[16], 2) == FALSE)
						ls_util_send_phone(phTTS,VPSTART);
#ifdef ENGLISH_US
					else if (ls_proc_non_zero(&buf[15], 1) == FALSE)
						ls_util_send_phone_list(phTTS,pand);
					else
						ls_util_send_phone(phTTS,COMMA);
#endif
				}
		
				ls_proc_do_digit_group(phTTS,&buf[15], oflag);	/* Units		*/
#ifdef GERMAN
				if (oflag != FALSE)
				{
					if (buf[16] >= '2' ||  (buf[15] != '0' && buf[16] == '0' && buf[17] == '0'))
					   ls_util_send_phone_list(phTTS, pstex);
					else
					{
					   if (buf[16] != '0')
					      ls_util_send_phone_list(phTTS, ptex);
					}
				}
				else
				{
					if (buf[16] == '0' &&  buf[17] == '1')
					   ls_util_send_phone_list(phTTS, p1s);

				}
#endif


			out:	;
			}
		}
	}
	/* This code handles integer parts like "1 1/2".		*/
	if (tlp1!=rlp && (tlp1->l_ch==0xBC || tlp1->l_ch==0xBD)) 
	{
		ls_util_send_phone_list(phTTS,pand);
		ls_spel_spell(phTTS,tlp1, tlp1+1);
		++tlp1;
		pflag = TRUE;			/* Always plural.	*/
	}
	if (tlp1!=rlp && tlp1->l_ch==pLts_t->fchar) 
	{	/* Fraction digits.	*/
		if (llp != tlp1)
			ls_util_send_phone(phTTS,WBOUND);
		ls_util_send_phone_list(phTTS,ppoint);
		tlp2 = tlp1;
		++tlp1;
		while (tlp1!=rlp && tlp1->l_ch!='e') 
		{
			c = tlp1->l_ch;
			if (c != pLts_t->schar) 
			{
				ls_util_send_phone(phTTS,WBOUND);
				ls_util_send_phone_list(phTTS,punits[c-'0']);
			}
			++tlp1;
		}
		pflag = TRUE;								/* 1.01 is plural.	*/
	}
	if (tlp1 != rlp) 
	{												/* Must be an "e".	*/
		ls_util_send_phone_list(phTTS,ptt2tp);		/* " " on end.		*/
		tlp2 = tlp1;
		++tlp1;										/* Skip "e"			*/
		if (tlp1 != rlp) 
		{											/* Handle signs.	*/
			c = tlp1->l_ch;
			if (c=='-' || c=='+') 
			{
				ls_proc_do_sign(phTTS,c);
				++tlp1;
			}
		}
		ls_proc_do_number(phTTS,tlp1, rlp, FALSE);	/* Cannot recur on "e".	*/
		pflag = TRUE;								/* 1E01 is plural.		*/
	}
	return (pflag);
}

 
/*
 *      Function Name: ls_proc_non_zero()      
 *
 *  	Description: Check to see if the supplied
 * 					 array of characters, with length "n",
 * 					 is non zero. Return TRUE if it is.
 * 					 Return FALSE if all zero.
 *
 *      Arguments: char *p, int n
 *
 *      Return Value: int
 *
 *      Comments:
 *
 */
int ls_proc_non_zero(char *p, int n)
{
	while (n--) 
	{
		if (*p != '0')
			return (TRUE);
		++p;
	}
	return (FALSE);
}




/*
 *	Function Name:
 *		ls_proc_is_date	
 *
 *	Description:       
 *		this function checks weather the number is ir isn't a date
 *
 *	Arguments:
 *		LETTER *llp		The left bounding pointer to the word
 *		LETTER *rlp     The right bounding pointer to the word
 *
 *	Return Value:
 *		TRUE	if the word is a date
 *		FALSE	otherwise
 *
 *	Comments:
 *
 */
int ls_proc_is_date(LETTER *llp, LETTER *rlp)
{
	char *cp;
	 int	i;
	short		buf[3];

	if (ls_proc_digit(llp)==FALSE || ++llp==rlp)	/* First digit.		*/
		return (FALSE);
	if (llp->l_ch != '-') 
	{												/* Optional digit.	*/
		if (ls_proc_digit(llp)==FALSE || ++llp==rlp)
			return (FALSE);
		if (llp->l_ch != '-')						/* Must be "-" now!	*/
			return (FALSE);
	}
	if (++llp==rlp || ls_proc_alpha(llp)==FALSE)	/* Three alphas.	*/
		return (FALSE);
	buf[0] = llp->l_ch;
	if (++llp==rlp || ls_proc_alpha(llp)==FALSE)
		return (FALSE);
	buf[1] = llp->l_ch;
	if (++llp==rlp || ls_proc_alpha(llp)==FALSE)
		return (FALSE);
	buf[2] = llp->l_ch;
	for (i=0; i<12; ++i) 
	{			/* Validate.		*/
		cp = months[i];
		if (buf[0]==cp[0] && buf[1]==cp[1] && buf[2]==cp[2])
			break;
	}
	if (i == 12)				/* Loss!		*/
		return (FALSE);
	if (++llp == rlp)			/* 23-Aug		*/
		return (TRUE);
	if (llp->l_ch != '-')		/* Must be a year.	*/
		return (FALSE);
	if (++llp==rlp || ls_proc_digit(llp)==FALSE)
		return (FALSE);
	if (++llp==rlp || ls_proc_digit(llp)==FALSE)
		return (FALSE);
	if (++llp == rlp)			/* 23-Aug-84		*/
		return (TRUE);
	if (ls_proc_digit(llp)==FALSE || ++llp==rlp)	/* Need 2 more digits.	*/
		return (FALSE);
	if (ls_proc_digit(llp)==FALSE || ++llp!=rlp)
		return (FALSE);
	return (TRUE);				/* 23-Aug-1984		*/
}

/*
 *	Function Name:
 *		ls_proc_do_date	
 *
 *	Description:       
 *		this function sends the correct phonemes for a date
 *
 *	Arguments:                              
 *		LETTER *llp		The left bounding pointer to the word
 *		LETTER *rlp     The right bounding pointer to the word
 *
 *	Return Value:
 *		None
 *
 *	Comments:
 *
 */
void ls_proc_do_date(LPTTS_HANDLE_T phTTS,LETTER *llp, LETTER *rlp)
{
	LETTER	*lp1;
	 int	i;
	char *cp;

	lp1 = llp;				/* Find end of day.	*/
	while (lp1->l_ch != '-')
		++lp1;
	for (i=0; i<12; ++i) 
	{						/* Get month.		*/
		cp = months[i];
		if ((lp1+1)->l_ch == cp[0]
		&&  (lp1+2)->l_ch == cp[1]
		&&  (lp1+3)->l_ch == cp[2])
			break;
	}
	ls_util_send_phone_list(phTTS,pmonths[i]);			/* Speak the month		*/
	ls_util_send_phone(phTTS,WBOUND);
	if (lp1!=llp+1 && llp->l_ch=='0')					/* Get "01-Jan-84" ok.	*/
		ls_proc_do_number(phTTS,llp+1, lp1, TRUE);
	else
		ls_proc_do_number(phTTS,llp, lp1, TRUE);
	lp1 += 4;
	if (lp1 != rlp) {
		ls_util_send_phone(phTTS,COMMA);
		if (lp1+3 == rlp) 
		{
			ls_util_send_phone_list(phTTS,pteens[9]);	/* 19XX					*/
			ls_util_send_phone(phTTS,WBOUND);
			ls_proc_do_2_digits(phTTS,lp1+1);
		} 
		else
			ls_proc_do_4_digits(phTTS,lp1+1);			/* YYXX					*/		
	}
}

/*
 *	Function Name:
 *		ls_proc_is_frac	
 *
 *	Description:       
 *		this function determines if the word is a fraction
 *
 *	Arguments:
 *		LETTER *llp		The left bounding pointer to the word
 *		LETTER *rlp     The right bounding pointer to the word
 *
 *	Return Value:
 *		TRUE	if the word is a fraction
 *		FALSE	otherwise
 *
 *	Comments:
 *
 */
int ls_proc_is_frac(LETTER *llp, LETTER *rlp)
{
	int	n;

	if (ls_proc_digit(llp)==FALSE || llp->l_ch=='0' || ++llp==rlp)
		return (FALSE);				/* Non digit or "0".	*/
	if (llp->l_ch != '/') 
	{								/* Optional digit.		*/
		if (ls_proc_digit(llp)==FALSE || ++llp==rlp)
			return (FALSE);
		if (llp->l_ch != '/')
			return (FALSE);
	}
	n = 0;							/* Count digits.		*/
	while (++llp!=rlp && ls_proc_digit(llp)!=FALSE) 
	{
		if (n==0 && llp->l_ch=='0')	/* Leading "0" is bad.	*/
			return (FALSE);
		++n;
	}
	if (n==0 || n>3)				/* 1 to 3 digits.		*/
		return (FALSE);
	if (n == 3) 
	{								/* Limit is 100.		*/
		if ((llp-1)->l_ch != '0'
		||  (llp-2)->l_ch != '0'
		||  (llp-3)->l_ch != '1')
			return (FALSE);
	}
	if (llp != rlp) 
	{								/* Allow "%".			*/
		if (llp->l_ch!='%' || llp+1!=rlp)
			return (FALSE);
	}
	return (TRUE);
}

/*
 *	Function Name:
 *		ls_proc_do_frac	
 *
 *	Description:    
 *		this function sends the phonemes for a fraction
 *
 *	Arguments:                                        
 *		LETTER *llp		The left bounding pointer to the word
 *		LETTER *rlp     The right bounding pointer to the word
 *
 *	Return Value:
 *		None
 *
 *	Comments:
 *
 */
void ls_proc_do_frac(LPTTS_HANDLE_T phTTS,LETTER *llp, LETTER *rlp)
{
	 LETTER *tlp1;
	 LETTER *tlp2;
	 int	pflag;
	 int	ud;

	tlp1 = llp;											/* Scan to "/".			*/
	while (tlp1->l_ch != '/')
		++tlp1;
	pflag = ls_proc_do_number(phTTS,llp, tlp1, FALSE);	/* Numerator.			*/
	ls_util_send_phone(phTTS,WBOUND);					/* Gap.					*/
	++tlp1;												/* Skip "/".			*/
	tlp2 = tlp1;										/* Scan to end or "%".	*/
	while (tlp2!=rlp && tlp2->l_ch!='%')
		++tlp2;
	if (tlp1+1==tlp2 && tlp1->l_ch=='2') 
	{
		ls_util_send_phone_list(phTTS,pflag!=FALSE ? phalves : phalf);
	} 
	else 
	{
		ls_proc_do_number(phTTS,tlp1, tlp2, TRUE);		/* As an ordinal.		*/
		if (pflag != FALSE) 
		{												/* Make plural.			*/
			ud = (tlp2-1)->l_ch;
			if (tlp2>tlp1+1 && (tlp2-2)->l_ch=='1')
				ud = '0';
			ls_util_send_phone(phTTS,ud=='2'||ud=='3' ? Z : S);
		}
	}
	if (tlp2 != rlp) 
	{													/* Must be "%".			*/
		ls_util_send_phone_list(phTTS,ppercent);
	}          
}

/*
 *	Function Name:
 *		ls_proc_alpha	
 *
 *	Description:     
 *		this function checks the character in the letter structure lp
 *		to see if it is an alpha 
 *
 *	Arguments:                   
 *		LETTER *lp		a pointer to a letter structure
 *
 *	Return Value:                                      
 *		TRUE	if it is an aplha character
 *		FALSE	otherwise
 *
 *	Comments:                                          
 *		This seems redundant with the macro IS_ALPHA(C)
 *
 */
int ls_proc_alpha(LETTER *lp)
{
	if (lp->l_ch>='a' && lp->l_ch<='z')
		return (TRUE);
	return (FALSE);
}

/*
 *	Function Name:	
 *		ls_proc_digit
 *
 *	Description:     
 *		checks the character pointed to by lp to  see if it is a digit
 *
 *	Arguments:                                                        
 *		LETTER *lp		a pointer to a letter structure that contains the letter to be checked
 *
 *	Return Value:
 *		TRUE	if the chracter is a digit
 *		FALSE	otherwise
 *
 *	Comments:            
 *		this seems redundant with the nacro IS_DIGIT(C)
 *
 */
int ls_proc_digit(LETTER *lp)
{

	if (lp->l_ch>='0' && lp->l_ch<='9')
	{
		return (TRUE);
	}
	return (FALSE);
}



/*
 *	Function Name:
 *		ls_proc_is_time	
 *
 *	Description:       
 *		checks to see if the surrent word is a time
 *
 *	Arguments:                                     
 *		LETTER *llp		The left bounding pointer to the word
 *		LETTER *rlp     The right bounding pointer to the word
 *
 *	Return Value:                                             
 *		TRUE 	if ti is a time
 *		FALSE	otherwise 
 *
 *	Comments:
 *
 */
int ls_proc_is_time(PLTS_T pLts_t,LETTER *llp, LETTER *rlp)
{
	if (ls_proc_digit(llp)==FALSE || ++llp==rlp)	/* First digit.		*/
		return (FALSE);

//#ifdef ENGLISH_US
	if (llp->l_ch != ':') 							/* Optional digit.	*/
//#endif
//#ifdef GERMAN
//	if (llp->l_ch != ':' && llp->l_ch != '.') 
//#endif
	{
		if (ls_proc_digit(llp)==FALSE || ++llp==rlp)
			return (FALSE);
//#ifdef ENGLISH_US
		if (llp->l_ch != ':')						/* Must be ":" now!	*/
//#endif
//#ifdef GERMAN
//		if (llp->l_ch != ':' && llp->l_ch != '.')
//#endif
			return (FALSE);
	}
	
	if (++llp==rlp || ls_proc_digit(llp)==FALSE)	/* Two digits.		*/
		return (FALSE);
	if (++llp==rlp || ls_proc_digit(llp)==FALSE)
		return (FALSE);
	if (++llp == rlp)								/* 00:00			*/
	{
		return (TRUE);
	}

//#ifdef ENGLISH_US
	if (llp->l_ch == ':')
//#endif
//#ifdef GERMAN
//	if (llp->l_ch == ':' || llp->l_ch == '.')
//#endif
	{												/* 00:00:00			*/
		if (++llp==rlp || ls_proc_digit(llp)==FALSE)
			return (FALSE);
		if (++llp==rlp || ls_proc_digit(llp)==FALSE)
			return (FALSE);
		++llp;
	}
	if (llp!=rlp && llp->l_ch==pLts_t->fchar)
	{	/* Fractional digits.	*/
		if (++llp==rlp || ls_proc_digit(llp)==FALSE)
			return (FALSE);
		while (++llp!=rlp && ls_proc_digit(llp)!=FALSE);
	}

	return (TRUE);				/* Looks good!		*/
}



/*
 *	Function Name:	     
 *		ls_proc_do_time
 *
 *	Description:       
 *		this function sends the phonemes for the time given in the word
 *
 *	Arguments:
 *		LETTER *llp		The left bounding pointer to the word
 *		LETTER *rlp     The right bounding pointer to the word
 *
 *	Return Value:
 *		None
 *
 *	Comments:
 *
 */
void ls_proc_do_time(LPTTS_HANDLE_T phTTS,LETTER *llp, LETTER *rlp)
/*  LETTER	*llp; */
/*  LETTER	*rlp; */
{
	int flag= 0;
//#ifdef ENGLISH_US
	if ((llp+1)->l_ch == ':')
//#endif
//#ifdef GERMAN
//	if ((llp+1)->l_ch == ':' || (llp+1)->l_ch== '.')
//#endif
	{		/* Initial 1 digit.	*/
		ls_util_send_phone_list(phTTS,punits[llp->l_ch-'0']);
		llp += 2;
	}
	else
	{				/* Initial 2 digit.	*/
		ls_proc_do_2_digits(phTTS,llp);
		llp += 3;
	}
#ifdef GERMAN
	ls_util_send_phone_list (phTTS, phour);
#endif

	ls_util_send_phone(phTTS,VPSTART);			/* Middle.		*/
	if ( !( llp->l_ch == '0' && (llp+1)->l_ch == '0'))
		ls_proc_do_2_digits(phTTS,llp);
	llp += 2;
#ifdef GERMAN
	ls_util_send_phone_list(phTTS, pminutes);
#endif
//#ifdef ENGLISH_US
	if (llp!=rlp && llp->l_ch==':')
//#endif
//#ifdef GERMAN
//	if (llp!=rlp && (llp->l_ch==':' || llp->l_ch=='.'))
//#endif
	{	/* End.			*/
		ls_util_send_phone(phTTS,VPSTART);
		ls_proc_do_2_digits(phTTS,llp+1);
		llp += 3;
#ifdef GERMAN
		if (llp == rlp)
	       ls_util_send_phone_list(phTTS, pseconds);
#endif
	}
    
	if (llp != rlp)
	{			/* Final fractions.	*/
		if(llp->l_ch  == '.')
		{
			ls_util_send_phone(phTTS,WBOUND);
			ls_util_send_phone_list(phTTS,ppoint);
			while (++llp != rlp)
			{
				ls_util_send_phone(phTTS,WBOUND);
				ls_util_send_phone_list(phTTS,punits[llp->l_ch-'0']);
			}
		}
		else
		{
			ls_spel_spell(phTTS,llp,rlp);
		}
#ifdef GERMAN
		ls_util_send_phone_list(phTTS, pseconds);
#endif
	}
}

