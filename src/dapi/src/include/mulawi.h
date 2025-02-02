/**********************************************************************/
/*                                                                    */
/*  Include File: mulawi.h                                            */
/*  Author: Bill Hallahan                                             */
/*  Date: February 24, 1992                                           */
/*                                                                    */
/*  This include file defines a table which is used to convert 8 bit  */
/*  mu-law encoded samples to 16 bit linear samples.                  */
/*                                                                    */
/**********************************************************************/

  S16 wMulawTable[256] =
  {
    -8031*4,
    -7775*4,
    -7519*4,
    -7263*4,
    -7007*4,
    -6751*4,
    -6495*4,
    -6239*4,
    -5983*4,
    -5727*4,
    -5471*4,
    -5215*4,
    -4959*4,
    -4703*4,
    -4447*4,
    -4191*4,
    -3999*4,
    -3871*4,
    -3743*4,
    -3615*4,
    -3487*4,
    -3359*4,
    -3231*4,
    -3103*4,
    -2975*4,
    -2847*4,
    -2719*4,
    -2591*4,
    -2463*4,
    -2335*4,
    -2207*4,
    -2079*4,
    -1983*4,
    -1919*4,
    -1855*4,
    -1791*4,
    -1727*4,
    -1663*4,
    -1599*4,
    -1535*4,
    -1471*4,
    -1407*4,
    -1343*4,
    -1279*4,
    -1215*4,
    -1151*4,
    -1087*4,
    -1023*4,
    -975*4,
    -943*4,
    -911*4,
    -879*4,
    -847*4,
    -815*4,
    -783*4,
    -751*4,
    -719*4,
    -687*4,
    -655*4,
    -623*4,
    -591*4,
    -559*4,
    -527*4,
    -495*4,
    -471*4,
    -455*4,
    -439*4,
    -423*4,
    -407*4,
    -391*4,
    -375*4,
    -359*4,
    -343*4,
    -327*4,
    -311*4,
    -295*4,
    -279*4,
    -263*4,
    -247*4,
    -231*4,
    -219*4,
    -211*4,
    -203*4,
    -195*4,
    -187*4,
    -179*4,
    -171*4,
    -163*4,
    -155*4,
    -147*4,
    -139*4,
    -131*4,
    -123*4,
    -115*4,
    -107*4,
    -99*4,
    -93*4,
    -89*4,
    -85*4,
    -81*4,
    -77*4,
    -73*4,
    -69*4,
    -65*4,
    -61*4,
    -57*4,
    -53*4,
    -49*4,
    -45*4,
    -41*4,
    -37*4,
    -33*4,
    -30*4,
    -28*4,
    -26*4,
    -24*4,
    -22*4,
    -20*4,
    -18*4,
    -16*4,
    -14*4,
    -12*4,
    -10*4,
    -8*4,
    -6*4,
    -4*4,
    -2*4,
    0*4,
    8031*4,
    7775*4,
    7519*4,
    7263*4,
    7007*4,
    6751*4,
    6495*4,
    6239*4,
    5983*4,
    5727*4,
    5471*4,
    5215*4,
    4959*4,
    4703*4,
    4447*4,
    4191*4,
    3999*4,
    3871*4,
    3743*4,
    3615*4,
    3487*4,
    3359*4,
    3231*4,
    3103*4,
    2975*4,
    2847*4,
    2719*4,
    2591*4,
    2463*4,
    2335*4,
    2207*4,
    2079*4,
    1983*4,
    1919*4,
    1855*4,
    1791*4,
    1727*4,
    1663*4,
    1599*4,
    1535*4,
    1471*4,
    1407*4,
    1343*4,
    1279*4,
    1215*4,
    1151*4,
    1087*4,
    1023*4,
    975*4,
    943*4,
    911*4,
    879*4,
    847*4,
    815*4,
    783*4,
    751*4,
    719*4,
    687*4,
    655*4,
    623*4,
    591*4,
    559*4,
    527*4,
    495*4,
    471*4,
    455*4,
    439*4,
    423*4,
    407*4,
    391*4,
    375*4,
    359*4,
    343*4,
    327*4,
    311*4,
    295*4,
    279*4,
    263*4,
    247*4,
    231*4,
    219*4,
    211*4,
    203*4,
    195*4,
    187*4,
    179*4,
    171*4,
    163*4,
    155*4,
    147*4,
    139*4,
    131*4,
    123*4,
    115*4,
    107*4,
    99*4,
    93*4,
    89*4,
    85*4,
    81*4,
    77*4,
    73*4,
    69*4,
    65*4,
    61*4,
    57*4,
    53*4,
    49*4,
    45*4,
    41*4,
    37*4,
    33*4,
    30*4,
    28*4,
    26*4,
    24*4,
    22*4,
    20*4,
    18*4,
    16*4,
    14*4,
    12*4,
    10*4,
    8*4,
    6*4,
    4*4,
    2*4,
    0
  };
