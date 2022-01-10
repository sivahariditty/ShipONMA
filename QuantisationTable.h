#ifndef QUANTISATIONTABLE_H
#define QUANTISATIONTABLE_H
#include<stdint.h>
#include<stdlib.h>


float EnergyQuantisationTable[100]={70,70,70,70,70,71,71,71,71,71,71,71,72,72,72,72,72,72,73,73,73,73,74,74,74,74,75,75,75,
                               76,76,76,77,77,78,78,79,79,80,80,81,81,82,83,83,84,85,85,86,87,88,89,90,91,92,93,94,96,
                               97,98,100,101,103,104,106,108,110,112,114,116,118,120,123,125,128,131,134,137,140,144,
                               147,151,155,159,163,168,173,177,183,188,194,200,206,212,219,226,234,241,250,258
                              };
float SpectrumQuantisationTable[256]={ };
float DelSpectrumQuantisationTable[256]={ };
float CompSpectrumQuantisationTable[256]={ };

int16_t SpectrumQuantisationTable2[500]={33,
                                         66,
                                         98,
                                         131,
                                         164,
                                         197,
                                         229,
                                         262,
                                         295,
                                         328,
                                         360,
                                         393,
                                         426,
                                         459,
                                         492,
                                         524,
                                         557,
                                         590,
                                         623,
                                         655,
                                         688,
                                         721,
                                         754,
                                         786,
                                         819,
                                         852,
                                         885,
                                         918,
                                         950,
                                         983,
                                         1016,
                                         1049,
                                         1081,
                                         1114,
                                         1147,
                                         1180,
                                         1212,
                                         1245,
                                         1278,
                                         1311,
                                         1343,
                                         1376,
                                         1409,
                                         1442,
                                         1475,
                                         1507,
                                         1540,
                                         1573,
                                         1606,
                                         1638,
                                         1671,
                                         1704,
                                         1737,
                                         1769,
                                         1802,
                                         1835,
                                         1868,
                                         1901,
                                         1933,
                                         1966,
                                         1999,
                                         2032,
                                         2064,
                                         2097,
                                         2130,
                                         2163,
                                         2195,
                                         2228,
                                         2261,
                                         2294,
                                         2327,
                                         2359,
                                         2392,
                                         2425,
                                         2458,
                                         2490,
                                         2523,
                                         2556,
                                         2589,
                                         2621,
                                         2654,
                                         2687,
                                         2720,
                                         2753,
                                         2785,
                                         2818,
                                         2851,
                                         2884,
                                         2916,
                                         2949,
                                         2982,
                                         3015,
                                         3047,
                                         3080,
                                         3113,
                                         3146,
                                         3178,
                                         3211,
                                         3244,
                                         3277,
                                         3310,
                                         3342,
                                         3375,
                                         3408,
                                         3441,
                                         3473,
                                         3506,
                                         3539,
                                         3572,
                                         3604,
                                         3637,
                                         3670,
                                         3703,
                                         3736,
                                         3768,
                                         3801,
                                         3834,
                                         3867,
                                         3899,
                                         3932,
                                         3965,
                                         3998,
                                         4030,
                                         4063,
                                         4096,
                                         4129,
                                         4162,
                                         4194,
                                         4227,
                                         4260,
                                         4293,
                                         4325,
                                         4358,
                                         4391,
                                         4424,
                                         4456,
                                         4489,
                                         4522,
                                         4555,
                                         4588,
                                         4620,
                                         4653,
                                         4686,
                                         4719,
                                         4751,
                                         4784,
                                         4817,
                                         4850,
                                         4882,
                                         4915,
                                         4948,
                                         4981,
                                         5014,
                                         5046,
                                         5079,
                                         5112,
                                         5145,
                                         5177,
                                         5210,
                                         5243,
                                         5276,
                                         5308,
                                         5341,
                                         5374,
                                         5407,
                                         5439,
                                         5472,
                                         5505,
                                         5538,
                                         5571,
                                         5603,
                                         5636,
                                         5669,
                                         5702,
                                         5734,
                                         5767,
                                         5800,
                                         5833,
                                         5865,
                                         5898,
                                         5931,
                                         5964,
                                         5997,
                                         6029,
                                         6062,
                                         6095,
                                         6128,
                                         6160,
                                         6193,
                                         6226,
                                         6259,
                                         6291,
                                         6324,
                                         6357,
                                         6390,
                                         6423,
                                         6455,
                                         6488,
                                         6521,
                                         6554,
                                         6586,
                                         6619,
                                         6652,
                                         6685,
                                         6717,
                                         6750,
                                         6783,
                                         6816,
                                         6849,
                                         6881,
                                         6914,
                                         6947,
                                         6980,
                                         7012,
                                         7045,
                                         7078,
                                         7111,
                                         7143,
                                         7176,
                                         7209,
                                         7242,
                                         7274,
                                         7307,
                                         7340,
                                         7373,
                                         7406,
                                         7438,
                                         7471,
                                         7504,
                                         7537,
                                         7569,
                                         7602,
                                         7635,
                                         7668,
                                         7700,
                                         7733,
                                         7766,
                                         7799,
                                         7832,
                                         7864,
                                         7897,
                                         7930,
                                         7963,
                                         7995,
                                         8028,
                                         8061,
                                         8094,
                                         8126,
                                         8159,
                                         8192,
                                         8225,
                                         8258,
                                         8290,
                                         8323,
                                         8356,
                                         8389,
                                         8421,
                                         8454,
                                         8487,
                                         8520,
                                         8552,
                                         8585,
                                         8618,
                                         8651,
                                         8684,
                                         8716,
                                         8749,
                                         8782,
                                         8815,
                                         8847,
                                         8880,
                                         8913,
                                         8946,
                                         8978,
                                         9011,
                                         9044,
                                         9077,
                                         9110,
                                         9142,
                                         9175,
                                         9208,
                                         9241,
                                         9273,
                                         9306,
                                         9339,
                                         9372,
                                         9404,
                                         9437,
                                         9470,
                                         9503,
                                         9535,
                                         9568,
                                         9601,
                                         9634,
                                         9667,
                                         9699,
                                         9732,
                                         9765,
                                         9798,
                                         9830,
                                         9863,
                                         9896,
                                         9929,
                                         9961,
                                         9994,
                                         10027,
                                         10060,
                                         10093,
                                         10125,
                                         10158,
                                         10191,
                                         10224,
                                         10256,
                                         10289,
                                         10322,
                                         10355,
                                         10387,
                                         10420,
                                         10453,
                                         10486,
                                         10519,
                                         10551,
                                         10584,
                                         10617,
                                         10650,
                                         10682,
                                         10715,
                                         10748,
                                         10781,
                                         10813,
                                         10846,
                                         10879,
                                         10912,
                                         10945,
                                         10977,
                                         11010,
                                         11043,
                                         11076,
                                         11108,
                                         11141,
                                         11174,
                                         11207,
                                         11239,
                                         11272,
                                         11305,
                                         11338,
                                         11370,
                                         11403,
                                         11436,
                                         11469,
                                         11502,
                                         11534,
                                         11567,
                                         11600,
                                         11633,
                                         11665,
                                         11698,
                                         11731,
                                         11764,
                                         11796,
                                         11829,
                                         11862,
                                         11895,
                                         11928,
                                         11960,
                                         11993,
                                         12026,
                                         12059,
                                         12091,
                                         12124,
                                         12157,
                                         12190,
                                         12222,
                                         12255,
                                         12288,
                                         12321,
                                         12354,
                                         12386,
                                         12419,
                                         12452,
                                         12485,
                                         12517,
                                         12550,
                                         12583,
                                         12616,
                                         12648,
                                         12681,
                                         12714,
                                         12747,
                                         12780,
                                         12812,
                                         12845,
                                         12878,
                                         12911,
                                         12943,
                                         12976,
                                         13009,
                                         13042,
                                         13074,
                                         13107,
                                         13140,
                                         13173,
                                         13206,
                                         13238,
                                         13271,
                                         13304,
                                         13337,
                                         13369,
                                         13402,
                                         13435,
                                         13468,
                                         13500,
                                         13533,
                                         13566,
                                         13599,
                                         13631,
                                         13664,
                                         13697,
                                         13730,
                                         13763,
                                         13795,
                                         13828,
                                         13861,
                                         13894,
                                         13926,
                                         13959,
                                         13992,
                                         14025,
                                         14057,
                                         14090,
                                         14123,
                                         14156,
                                         14189,
                                         14221,
                                         14254,
                                         14287,
                                         14320,
                                         14352,
                                         14385,
                                         14418,
                                         14451,
                                         14483,
                                         14516,
                                         14549,
                                         14582,
                                         14615,
                                         14647,
                                         14680,
                                         14713,
                                         14746,
                                         14778,
                                         14811,
                                         14844,
                                         14877,
                                         14909,
                                         14942,
                                         14975,
                                         15008,
                                         15041,
                                         15073,
                                         15106,
                                         15139,
                                         15172,
                                         15204,
                                         15237,
                                         15270,
                                         15303,
                                         15335,
                                         15368,
                                         15401,
                                         15434,
                                         15466,
                                         15499,
                                         15532,
                                         15565,
                                         15598,
                                         15630,
                                         15663,
                                         15696,
                                         15729,
                                         15761,
                                         15794,
                                         15827,
                                         15860,
                                         15892,
                                         15925,
                                         15958,
                                         15991,
                                         16024,
                                         16056,
                                         16089,
                                         16122,
                                         16155,
                                         16187,
                                         16220,
                                         16253,
                                         16286,
                                         16318,
                                         16351,
                                         16384,
                              };



float SpectrumZoom1QuantisationTable[250]={
    33554,
67109,
100663,
134218,
167772,
201327,
234881,
268435,
301990,
335544,
369099,
402653,
436208,
469762,
503316,
536871,
570425,
603980,
637534,
671089,
704643,
738197,
771752,
805306,
838861,
872415,
905970,
939524,
973078,
1006633,
1040187,
1073742,
1107296,
1140851,
1174405,
1207959,
1241514,
1275068,
1308623,
1342177,
1375732,
1409286,
1442840,
1476395,
1509949,
1543504,
1577058,
1610613,
1644167,
1677721,
1711276,
1744830,
1778385,
1811939,
1845494,
1879048,
1912602,
1946157,
1979711,
2013266,
2046820,
2080375,
2113929,
2147483,
2181038,
2214592,
2248147,
2281701,
2315256,
2348810,
2382364,
2415919,
2449473,
2483028,
2516582,
2550137,
2583691,
2617245,
2650800,
2684354,
2717909,
2751463,
2785018,
2818572,
2852126,
2885681,
2919235,
2952790,
2986344,
3019899,
3053453,
3087007,
3120562,
3154116,
3187671,
3221225,
3254780,
3288334,
3321888,
3355443,
3388997,
3422552,
3456106,
3489661,
3523215,
3556769,
3590324,
3623878,
3657433,
3690987,
3724542,
3758096,
3791650,
3825205,
3858759,
3892314,
3925868,
3959423,
3992977,
4026531,
4060086,
4093640,
4127195,
4160749,
4194304,
4227858,
4261412,
4294967,
4328521,
4362076,
4395630,
4429184,
4462739,
4496293,
4529848,
4563402,
4596957,
4630511,
4664065,
4697620,
4731174,
4764729,
4798283,
4831838,
4865392,
4898946,
4932501,
4966055,
4999610,
5033164,
5066719,
5100273,
5133827,
5167382,
5200936,
5234491,
5268045,
5301600,
5335154,
5368708,
5402263,
5435817,
5469372,
5502926,
5536481,
5570035,
5603589,
5637144,
5670698,
5704253,
5737807,
5771362,
5804916,
5838470,
5872025,
5905579,
5939134,
5972688,
6006243,
6039797,
6073351,
6106906,
6140460,
6174015,
6207569,
6241124,
6274678,
6308232,
6341787,
6375341,
6408896,
6442450,
6476005,
6509559,
6543113,
6576668,
6610222,
6643777,
6677331,
6710886,
6744440,
6777994,
6811549,
6845103,
6878658,
6912212,
6945767,
6979321,
7012875,
7046430,
7079984,
7113539,
7147093,
7180648,
7214202,
7247756,
7281311,
7314865,
7348420,
7381974,
7415529,
7449083,
7482637,
7516192,
7549746,
7583301,
7616855,
7650410,
7683964,
7717518,
7751073,
7784627,
7818182,
7851736,
7885291,
7918845,
7952399,
7985954,
8019508,
8053063,
8086617,
8120172,
8153726,
8187280,
8220835,
8254389,
8287944,
8321498,
8355053,
8388607,
};

#endif // QUANTISATIONTABLE_H
