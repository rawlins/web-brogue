#include "Rogue.h"
#include <stdint.h>


int64_t fp_netEnchant(item *theItem) {
  int64_t retval;
  if (theItem->category & (WEAPON | ARMOR)) {
    retval = (((int64_t) theItem->enchant1) << FP_BASE) + fp_strengthModifier(theItem);
  } else {
    retval = (((int64_t) theItem->enchant1) << FP_BASE);
  }
  // Clamp all net enchantment values to [-20, 50].
  return clamp(retval, -20 << FP_BASE, 50 << FP_BASE);
}


int64_t fp_strengthModifier(item *theItem) {
  int64_t difference = 1;//(rogue.strength - player.weaknessAmount) - theItem->strengthRequired;
  if (difference > 0) {
    return (difference << FP_BASE) / 4; // 0.25x
  } else {
    return (difference << FP_BASE) * 5/2; // 2.5x
  }
}



short fp_weaponImageCount(int64_t enchant)			{return (clamp((int) ((enchant) / 3 >> FP_BASE), 1, 7));}

short fp_weaponImageDuration(int64_t enchant)       {return 3;}

/* Computing the number of leading zeros in a word. */
static int32_t clz(uint32_t x)
{
    int n;
    
    /* See "Hacker's Delight" book for more details */
    if (x == 0) return 32;
    n = 0;
    if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
    if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
    if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
    if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
    if (x <= 0x7FFFFFFF) {n = n + 1;}
    
    return n;
}

unsigned fp_sqrt(unsigned val)
{
    unsigned x;
    int bitpos;
    unsigned long long v;

    if(!val)
        return val;

    /* clz = count-leading-zeros. bitpos is the position of the most significant bit,
        relative to "1" or 1 << FP_BASE */
    bitpos = FP_BASE - clz(val);
    
    /* Calculate our first estimate.
        We use the identity 2^a * 2^a = 2^(2*a) or:
         sqrt(2^a) = 2^(a/2)
    */
    if(bitpos > 0) /* val > 1 */
        x = (1u<<FP_BASE)<<(bitpos >> 1u);
    else if(bitpos < 0) /* 0 < val < 1 */
        x = (1u<<FP_BASE)<<((unsigned)(-bitpos) << 1u);
    else /* val == 1 */
        x = (1u<<FP_BASE);
    
    /* We need to scale val with FP_BASE due to the division.
       Also val /= 2, hence the subtraction of one*/
    v = (unsigned long long)val <<  (FP_BASE - 1u);

    /* The actual iteration */
    x = (x >> 1u) + v/x;
    x = (x >> 1u) + v/x;
    x = (x >> 1u) + v/x;
    x = (x >> 1u) + v/x;
    return x;
}


const itemTable weaponTable[NUMBER_WEAPON_KINDS] = {
	{"dagger",				"", "", 10, 190,		12,	{3,	4,	1},		true, false, "A simple iron dagger with a well-worn wooden handle. Daggers will deal quintuple damage upon a successful sneak attack instead of triple damage."},
	{"sword",				"", "", 10, 440,		14, {7,	9,	1},		true, false, "The razor-sharp length of steel blade shines reassuringly."},
	{"broadsword",			"", "", 10, 990,		19,	{14, 22, 1},	true, false, "This towering blade inflicts heavy damage by investing its heft into every cut."},
	
    {"whip",				"", "", 10, 440,		14, {3,	5,	1},		true, false, "The lash from this coil of braided leather can tear bark from trees, and it will reach opponents up to five spaces away."},
    {"rapier",				"", "", 10, 440,		15, {3,	5,	1},		true, false, "This blade is thin and flexible, designed for deft and rapid maneuvers. It inflicts less damage than comparable weapons, but permits you to attack twice as quickly. If there is one space between you and an enemy and you step directly toward it, you will perform a devastating lunge attack, which deals treble damage and never misses."},
    {"flail",				"", "", 10, 440,		17, {10,13,	1},		true, false, "This spiked iron ball can be whirled at the end of its chain in synchronicity with your movement, allowing you a free attack whenever moving between two spaces that are adjacent to an enemy."},
    
	{"mace",				"", "", 10, 660,		16, {16, 20, 1},	true, false, "The iron flanges at the head of this weapon inflict substantial damage with every weighty blow. Because of its heft, it takes an extra turn to recover when it hits, and will push your opponent backward if there is room."},
	{"war hammer",			"", "", 10, 1100,		20, {25, 35, 1},	true, false, "Few creatures can withstand the crushing blow of this towering mass of lead and steel, but only the strongest of adventurers can effectively wield it. Because of its heft, it takes an extra turn to recover when it hits, and will push your opponent backward if there is room."},
	
	{"spear",				"", "", 10, 330,		13, {4, 5, 1},		true, false, "A slender wooden rod tipped with sharpened iron. The reach of the spear permits you to simultaneously attack an adjacent enemy and the enemy directly behind it."},
	{"war pike",			"", "", 10, 880,		18, {11, 15, 1},	true, false, "A long steel pole ending in a razor-sharp point. The reach of the pike permits you to simultaneously attack an adjacent enemy and the enemy directly behind it."},
	
	{"axe",					"", "", 10, 550,		15, {7, 9, 1},		true, false, "The blunt iron edge on this axe glints in the darkness. The arc of its swing permits you to attack all adjacent enemies simultaneously."},
	{"war axe",				"", "", 10, 990,		19, {12, 17, 1},	true, false, "The enormous steel head of this war axe puts considerable heft behind each stroke. The arc of its swing permits you to attack all adjacent enemies simultaneously."},

	{"dart",				"", "",	0,	15,			10,	{2,	4,	1},		true, false, "These simple metal spikes are weighted to fly true and sting their prey with a flick of the wrist."},
	{"incendiary dart",		"", "",	10, 25,			12,	{1,	2,	1},		true, false, "The barbed spike on each of these darts is designed to stick to its target while the compounds strapped to its length explode into flame."},
	{"javelin",				"", "",	10, 40,			15,	{3, 11, 3},		true, false, "This length of metal is weighted to keep the spike at its tip foremost as it sails through the air."},
};

// kindCount is optional
itemTable *tableForItemCategory(enum itemCategory theCat, short *kindCount) {
    itemTable *returnedTable;
    short returnedCount;
	switch (theCat) {
		case WEAPON:
			returnedTable = weaponTable;
            returnedCount = NUMBER_WEAPON_KINDS;
            break;
		default:
			returnedTable = NULL;
            returnedCount = 0;
            break;
	}
    if (kindCount) {
        *kindCount = returnedCount;
    }
    return returnedTable;
}

int64_t fp_enchantIncrement(item *theItem) {
	if (theItem->category & (WEAPON | ARMOR)) {

			return (125 << FP_BASE) / 100;
	} else {
		return (1 + 0) << FP_BASE;
	}
}


short runicWeaponChanceFix(item *theItem, boolean customEnchantLevel, int64_t enchantLevel, int brogue174) {
  const int64_t POW_16_RUNIC_DECREMENT[] = { // (1-0.16)^x << FP_BASE, with x from 0 to 50 in increments of 0.25:
    65536, 62740, 60064, 57502, 55050, 52702, 50454, 48302, 46242, 44269, 42381, 40574, 38843, 37186, 35600, 34082, 32628, 31236, 29904,
    28629, 27407, 26238, 25119, 24048, 23022, 22040, 21100, 20200, 19339, 18514, 17724, 16968, 16244, 15551, 14888, 14253, 13645, 13063,
    12506, 11972, 11462, 10973, 10505, 10057, 9628, 9217, 8824, 8448, 8087, 7742, 7412, 7096, 6793, 6503, 6226, 5961, 5706, 5463, 5230,
    5007, 4793, 4589, 4393, 4206, 4026, 3854, 3690, 3533, 3382, 3238, 3100, 2967, 2841, 2720, 2604, 2492, 2386, 2284, 2187, 2094, 2004,
    1919, 1837, 1759, 1684, 1612, 1543, 1477, 1414, 1354, 1296, 1241, 1188, 1137, 1089, 1042, 998, 955, 914, 875, 838, 802, 768, 735,
    704, 674, 645, 617, 591, 566, 542, 519, 496, 475, 455, 436, 417, 399, 382, 366, 350, 335, 321, 307, 294, 281, 269, 258, 247, 236,
    226, 217, 207, 198, 190, 182, 174, 167, 159, 153, 146, 140, 134, 128, 123, 117, 112, 108, 103, 99, 94, 90, 86, 83, 79, 76, 73, 69,
    66, 64, 61, 58, 56, 53, 51, 49, 47, 45, 43, 41, 39, 37, 36, 34, 33, 31, 30, 29, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 18, 17, 16,
    15, 15, 14, 13, 13, 12, 12, 11, 11, 10};
  const int64_t POW_15_RUNIC_DECREMENT[] = { // (1-0.15)^x << FP_BASE, with x from 0 to 50 in increments of 0.25:
    65536, 62926, 60421, 58015, 55705, 53487, 51358, 49313, 47349, 45464, 43654, 41916, 40247, 38644, 37106, 35628, 34210, 32848, 31540,
    30284, 29078, 27920, 26809, 25741, 24716, 23732, 22787, 21880, 21009, 20172, 19369, 18598, 17857, 17146, 16464, 15808, 15179, 14574,
    13994, 13437, 12902, 12388, 11895, 11421, 10967, 10530, 10111, 9708, 9321, 8950, 8594, 8252, 7923, 7608, 7305, 7014, 6735, 6466, 6209,
    5962, 5724, 5496, 5278, 5067, 4866, 4672, 4486, 4307, 4136, 3971, 3813, 3661, 3515, 3375, 3241, 3112, 2988, 2869, 2755, 2645, 2540,
    2439, 2341, 2248, 2159, 2073, 1990, 1911, 1835, 1762, 1692, 1624, 1559, 1497, 1438, 1380, 1325, 1273, 1222, 1173, 1127, 1082, 1039,
    997, 958, 919, 883, 848, 814, 781, 750, 720, 692, 664, 638, 612, 588, 564, 542, 520, 500, 480, 461, 442, 425, 408, 391, 376, 361, 346,
    333, 319, 307, 294, 283, 271, 261, 250, 240, 231, 221, 213, 204, 196, 188, 181, 173, 166, 160, 153, 147, 141, 136, 130, 125, 120, 115,
    111, 106, 102, 98, 94, 90, 87, 83, 80, 77, 74, 71, 68, 65, 62, 60, 58, 55, 53, 51, 49, 47, 45, 43, 41, 40, 38, 37, 35, 34, 32, 31, 30,
    29, 27, 26, 25, 24, 23, 22, 21, 21, 20, 19};
  const int64_t POW_14_RUNIC_DECREMENT[] = { // (1-0.14)^x << FP_BASE, with x from 0 to 50 in increments of 0.25:
    65536, 63110, 60775, 58526, 56360, 54275, 52267, 50332, 48470, 46676, 44949, 43286, 41684, 40142, 38656, 37226, 35848, 34522, 33244,
    32014, 30829, 29689, 28590, 27532, 26513, 25532, 24587, 23677, 22801, 21958, 21145, 20363, 19609, 18883, 18185, 17512, 16864, 16240,
    15639, 15060, 14503, 13966, 13449, 12952, 12472, 12011, 11566, 11138, 10726, 10329, 9947, 9579, 9224, 8883, 8554, 8238, 7933, 7639,
    7357, 7084, 6822, 6570, 6327, 6092, 5867, 5650, 5441, 5239, 5046, 4859, 4679, 4506, 4339, 4179, 4024, 3875, 3732, 3593, 3460, 3332,
    3209, 3090, 2976, 2866, 2760, 2658, 2559, 2465, 2373, 2285, 2201, 2119, 2041, 1965, 1893, 1823, 1755, 1690, 1628, 1567, 1509, 1454,
    1400, 1348, 1298, 1250, 1204, 1159, 1116, 1075, 1035, 997, 960, 924, 890, 857, 825, 795, 765, 737, 710, 684, 658, 634, 610, 588, 566,
    545, 525, 505, 487, 469, 451, 435, 418, 403, 388, 374, 360, 346, 334, 321, 309, 298, 287, 276, 266, 256, 247, 237, 229, 220, 212, 204,
    197, 189, 182, 176, 169, 163, 157, 151, 145, 140, 135, 130, 125, 120, 116, 111, 107, 103, 99, 96, 92, 89, 85, 82, 79, 76, 73, 71, 68,
    66, 63, 61, 58, 56, 54, 52, 50, 48, 47, 45, 43, 42, 40, 38, 37, 36, 34};
  const int64_t POW_11_RUNIC_DECREMENT[] = { // (1-0.11)^x << FP_BASE, with x from 0 to 50 in increments of 0.25:
    65536, 63654, 61826, 60051, 58327, 56652, 55025, 53445, 51911, 50420, 48972, 47566, 46200, 44874, 43585, 42334, 41118, 39938, 38791,
    37677, 36595, 35544, 34524, 33533, 32570, 31634, 30726, 29844, 28987, 28155, 27346, 26561, 25798, 25058, 24338, 23639, 22960, 22301,
    21661, 21039, 20435, 19848, 19278, 18725, 18187, 17665, 17157, 16665, 16186, 15721, 15270, 14832, 14406, 13992, 13590, 13200, 12821,
    12453, 12095, 11748, 11411, 11083, 10765, 10456, 10155, 9864, 9581, 9305, 9038, 8779, 8527, 8282, 8044, 7813, 7589, 7371, 7159, 6954,
    6754, 6560, 6372, 6189, 6011, 5838, 5671, 5508, 5350, 5196, 5047, 4902, 4761, 4624, 4492, 4363, 4237, 4116, 3997, 3883, 3771, 3663,
    3558, 3456, 3356, 3260, 3166, 3075, 2987, 2901, 2818, 2737, 2658, 2582, 2508, 2436, 2366, 2298, 2232, 2168, 2106, 2045, 1986, 1929,
    1874, 1820, 1768, 1717, 1668, 1620, 1573, 1528, 1484, 1442, 1400, 1360, 1321, 1283, 1246, 1210, 1176, 1142, 1109, 1077, 1046, 1016,
    987, 959, 931, 904, 878, 853, 829, 805, 782, 759, 737, 716, 696, 676, 656, 637, 619, 601, 584, 567, 551, 535, 520, 505, 490, 476, 462,
    449, 436, 424, 412, 400, 388, 377, 366, 356, 345, 336, 326, 317, 307, 299, 290, 282, 274, 266, 258, 251, 243, 236, 230, 223, 217, 210,
    204, 198, 193};
  const int64_t POW_7_RUNIC_DECREMENT[] = { // (1-0.07)^x << FP_BASE, with x from 0 to 50 in increments of 0.25:
    65536, 64357, 63200, 62064, 60948, 59852, 58776, 57719, 56682, 55662, 54662, 53679, 52714, 51766, 50835, 49921, 49024, 48142, 47277,
    46427, 45592, 44772, 43967, 43177, 42401, 41638, 40890, 40155, 39433, 38724, 38027, 37344, 36672, 36013, 35365, 34730, 34105, 33492,
    32890, 32298, 31718, 31147, 30587, 30038, 29497, 28967, 28446, 27935, 27433, 26939, 26455, 25979, 25512, 25054, 24603, 24161, 23726,
    23300, 22881, 22470, 22066, 21669, 21279, 20897, 20521, 20152, 19790, 19434, 19084, 18741, 18404, 18073, 17748, 17429, 17116, 16808,
    16506, 16209, 15918, 15632, 15351, 15075, 14804, 14537, 14276, 14019, 13767, 13520, 13277, 13038, 12804, 12573, 12347, 12125, 11907,
    11693, 11483, 11276, 11074, 10875, 10679, 10487, 10299, 10113, 9931, 9753, 9578, 9405, 9236, 9070, 8907, 8747, 8590, 8435, 8284, 8135,
    7988, 7845, 7704, 7565, 7429, 7296, 7164, 7036, 6909, 6785, 6663, 6543, 6425, 6310, 6196, 6085, 5976, 5868, 5763, 5659, 5557, 5457,
    5359, 5263, 5168, 5075, 4984, 4894, 4806, 4720, 4635, 4552, 4470, 4390, 4311, 4233, 4157, 4082, 4009, 3937, 3866, 3796, 3728, 3661,
    3595, 3531, 3467, 3405, 3344, 3283, 3224, 3166, 3110, 3054, 2999, 2945, 2892, 2840, 2789, 2739, 2689, 2641, 2594, 2547, 2501, 2456,
    2412, 2369, 2326, 2284, 2243, 2203, 2163, 2124, 2086, 2048, 2012, 1975, 1940, 1905, 1871, 1837, 1804, 1772, 1740};
  const int64_t POW_6_RUNIC_DECREMENT[] = { // (1-0.06)^x << FP_BASE, with x from 0 to 50 in increments of 0.25:
    65536, 64530, 63539, 62564, 61603, 60658, 59727, 58810, 57907, 57018, 56143, 55281, 54433, 53597, 52774, 51964, 51167, 50381, 49608,
    48846, 48097, 47358, 46631, 45916, 45211, 44517, 43833, 43161, 42498, 41846, 41203, 40571, 39948, 39335, 38731, 38137, 37551, 36975,
    36407, 35848, 35298, 34756, 34223, 33698, 33180, 32671, 32169, 31676, 31189, 30711, 30239, 29775, 29318, 28868, 28425, 27989, 27559,
    27136, 26719, 26309, 25905, 25508, 25116, 24731, 24351, 23977, 23609, 23247, 22890, 22539, 22193, 21852, 21516, 21186, 20861, 20541,
    20225, 19915, 19609, 19308, 19012, 18720, 18433, 18150, 17871, 17597, 17327, 17061, 16799, 16541, 16287, 16037, 15791, 15549, 15310,
    15075, 14843, 14616, 14391, 14170, 13953, 13739, 13528, 13320, 13116, 12914, 12716, 12521, 12329, 12139, 11953, 11770, 11589, 11411,
    11236, 11063, 10894, 10726, 10562, 10400, 10240, 10083, 9928, 9776, 9625, 9478, 9332, 9189, 9048, 8909, 8772, 8638, 8505, 8374, 8246,
    8119, 7995, 7872, 7751, 7632, 7515, 7400, 7286, 7174, 7064, 6956, 6849, 6744, 6640, 6538, 6438, 6339, 6242, 6146, 6052, 5959, 5867,
    5777, 5688, 5601, 5515, 5430, 5347, 5265, 5184, 5105, 5026, 4949, 4873, 4798, 4725, 4652, 4581, 4510, 4441, 4373, 4306, 4240, 4175,
    4111, 4047, 3985, 3924, 3864, 3805, 3746, 3689, 3632, 3576, 3521, 3467, 3414, 3362, 3310, 3259, 3209, 3160, 3111, 3064, 3017, 2970};
  const int64_t *effectChances[NUMBER_WEAPON_RUNIC_KINDS] = {
    POW_16_RUNIC_DECREMENT,	// W_SPEED
    POW_6_RUNIC_DECREMENT,	// W_QUIETUS
    POW_7_RUNIC_DECREMENT,	// W_PARALYSIS
    POW_15_RUNIC_DECREMENT,	// W_MULTIPLICITY
    POW_14_RUNIC_DECREMENT,	// W_SLOWING
    POW_11_RUNIC_DECREMENT,	// W_CONFUSION
    POW_15_RUNIC_DECREMENT, // W_FORCE
    0,		// W_SLAYING
    0,		// W_MERCY
    0};		// W_PLENTY
  int64_t modifier;
  short runicType = theItem->enchant2;
  short chance, adjustedBaseDamage, tableIndex;
    
  if (runicType == W_SLAYING) {
    return 0;
  }
  if (runicType >= NUMBER_GOOD_WEAPON_ENCHANT_KINDS) { // bad runic
    return 15;
  }
  if (!customEnchantLevel) {
    enchantLevel = fp_netEnchant(theItem);
  }
    
  // Innately high-damage weapon types are less likely to trigger runic effects.
  adjustedBaseDamage = (tableForItemCategory(theItem->category, NULL)[theItem->kind].range.lowerBound
			+ tableForItemCategory(theItem->category, NULL)[theItem->kind].range.upperBound) / 2;
    
  if (theItem->flags & ITEM_ATTACKS_STAGGER) {
    adjustedBaseDamage /= 2; // Normalize as though they attacked once per turn instead of every other turn.
  }
  //    if (theItem->flags & ITEM_ATTACKS_QUICKLY) {
  //		adjustedBaseDamage *= 2; // Normalize as though they attacked once per turn instead of twice per turn.
  //	} // Testing disabling this for balance reasons...
    
  modifier = FP_FACTOR - min((99 << FP_BASE)/100, (adjustedBaseDamage << FP_BASE) / 18);
  //	rootChance = rootChance * modifier >> FP_BASE;
    
  if (enchantLevel < 0) {
    chance = 0;
  } else {
    tableIndex = enchantLevel * modifier * 4 >> FP_BASE >> FP_BASE;
    tableIndex = clamp(tableIndex, 0, 50 * 4);
    chance = 100 - (short) (100LL * effectChances[runicType][tableIndex] >> FP_BASE); // good runic
  }
    
  // Slow weapons get an adjusted chance of 1 - (1-p)^2 to reflect two bites at the apple instead of one.
  if (theItem->flags & ITEM_ATTACKS_STAGGER) {
    chance = 100 - (100 - chance) * (100 - chance) / 100;
  }
  // Fast weapons get an adjusted chance of 1 - sqrt(1-p) to reflect one bite at the apple instead of two.

  if(brogue174 == 0) {
    //174 behaviour
	if (theItem->flags & ITEM_ATTACKS_QUICKLY) {
		chance = 100 * (1.0 - sqrt(1 - ((double)(chance)/100.0)));
	}
	
  }
  else if(brogue174 == 1) {
    //175 behaviour
    if (theItem->flags & ITEM_ATTACKS_QUICKLY) {
      chance = 100 * (FP_FACTOR - fp_sqrt(FP_FACTOR - (chance << FP_BASE)/100)) >> FP_BASE;
    }
  }
  else {
    //175 fix
    if (theItem->flags & ITEM_ATTACKS_QUICKLY) {
      //      printf("debug %i, %i, %i\n", chance, FP_FACTOR - (chance << FP_BASE)/100, fp_sqrt(FP_FACTOR - (chance << FP_BASE)/100));
    //      chance = 100 * ((1 << (FP_BASE / 2)) - fp_sqrt(FP_FACTOR - (chance << FP_BASE)/100)) >> (FP_BASE / 2);
      chance = 100 * (256 - sqrt(FP_FACTOR - (chance << FP_BASE)/100)) / 256;
    }
  }
    
  
  // The lowest percent change that a weapon will ever have is its enchantment level (if greater than 0).
  // That is so that even really heavy weapons will improve at least 1% per enchantment.
  chance = clamp(chance, max(1, (short) (enchantLevel >> FP_BASE)), 100);
    
  return chance;
}


item *initializeItem() {
	short i;
	item *theItem;
	
	theItem = (item *) malloc(sizeof(item));
	memset(theItem, '\0', sizeof(item) );
	
	theItem->category = 0;
	theItem->kind = 0;
	theItem->flags = 0;
	theItem->displayChar = '&';
	//	theItem->foreColor = &itemColor;
	//	theItem->inventoryColor = &white;
    theItem->inventoryLetter = '\0';
	theItem->armor = 0;
	theItem->strengthRequired = 0;
	theItem->enchant1 = 0;
	theItem->enchant2 = 0;
    theItem->timesEnchanted = 0;
	theItem->vorpalEnemy = 0;
	theItem->charges = 0;
	theItem->quantity = 1;
	theItem->quiverNumber = 0;
	theItem->originDepth = 0;
	theItem->inscription[0] = '\0';
	theItem->nextItem = NULL;
	
	for (i=0; i < KEY_ID_MAXIMUM; i++) {
		theItem->keyLoc[i].x = 0;
		theItem->keyLoc[i].y = 0;
		theItem->keyLoc[i].machine = 0;
		theItem->keyLoc[i].disposableHere = false;
	}
    return theItem;
}


void main(void) {


  /*
    if ((theItem->flags & ITEM_IDENTIFIED) || rogue.playbackOmniscience) {
    sprintf(buf2, "%i%% of the time that it hits an enemy, %i spectral %s%s will spring into being with accuracy and attack power equal to your own, and will dissipate %i turns later. (If the %s is enchanted, %i image%s will appear %i%% of the time, and will last %i turns.)",
    runicWeaponChance(theItem, false, 0),
    fp_weaponImageCount(enchant),
    theName,
    (fp_weaponImageCount(enchant) > 1 ? "s" : ""),
    fp_weaponImageDuration(enchant),
    theName,
    fp_weaponImageCount(enchant + fp_enchantIncrement(theItem)),
    (fp_weaponImageCount(enchant + fp_enchantIncrement(theItem)) > 1 ? "s" : ""),
    runicWeaponChance(theItem, true, enchant + fp_enchantIncrement(theItem)),

    fp_weaponImageDuration(enchant + fp_enchantIncrement(theItem)));
  */

  int i;
  int64_t enchant;
  int runic_chance;
  int runic_chance_175;
  int runic_chance_175fix;
  int runic_image_count;
  int runic_weapon_duration;
  int runic_chance_next;
  int runic_chance_next_175;
  int runic_chance_next_175fix;
  int runic_image_count_next;
  int runic_weapon_duration_next;
  item *theItem;

  theItem = initializeItem();
  theItem->category = WEAPON;
  theItem->enchant2 = W_MULTIPLICITY;
  theItem->flags |= ITEM_RUNIC;
  theItem->flags |= (ITEM_ATTACKS_QUICKLY | ITEM_LUNGE_ATTACKS);

  for(i=0;i<20;i++) {

    theItem->enchant1 = i;
        
    enchant = fp_netEnchant(theItem);
    
    runic_chance = runicWeaponChanceFix(theItem, false, 0, 0);
    runic_chance_175 = runicWeaponChanceFix(theItem, false, 0, 1);
    runic_chance_175fix = runicWeaponChanceFix(theItem, false, 0, 2);
    runic_image_count = fp_weaponImageCount(enchant);
    runic_weapon_duration = fp_weaponImageDuration(enchant);
    runic_chance_next = runicWeaponChanceFix(theItem, true, enchant + fp_enchantIncrement(theItem), 0);
    runic_chance_next_175 = runicWeaponChanceFix(theItem, true, enchant + fp_enchantIncrement(theItem), 1);
    runic_chance_next_175fix = runicWeaponChanceFix(theItem, true, enchant + fp_enchantIncrement(theItem), 2);
    runic_image_count_next = fp_weaponImageCount(enchant + fp_enchantIncrement(theItem));
    runic_weapon_duration_next = fp_weaponImageDuration(enchant + fp_enchantIncrement(theItem));

    printf("multiplicity (174) +%i \tchance: %i,\timages:%i,\tduration:%i\n(next 174 +%i)\t\tchance: %i,\timages:%i,\tduration:%i\n",
	   i,
	   runic_chance,
	   runic_image_count,
	   runic_weapon_duration,
	   i+1,
	   runic_chance_next,
	   runic_image_count_next,
	   runic_weapon_duration_next);
    /*
        printf("multiplicity (175) +%i \tchance: %i,\timages:%i,\tduration:%i\n(next 175 +%i)\t\tchance: %i,\timages:%i,\tduration:%i\n",
	   i,
	   runic_chance_175,
	   runic_image_count,
	   runic_weapon_duration,
	   i+1,
	   runic_chance_next_175,
	   runic_image_count_next,
	   runic_weapon_duration_next);   

	    printf("multiplicity (175fix) +%i \tchance: %i,\timages:%i,\tduration:%i\n(next 175fix +%i)\t\tchance: %i,\timages:%i,\tduration:%i\n",
	   i,
	   runic_chance_175fix,
	   runic_image_count,
	   runic_weapon_duration,
	   i+1,
	   runic_chance_next_175fix,
	   runic_image_count_next,
	   runic_weapon_duration_next);   
    */
  }


  //fp_sqrt(65536*4) returns 65536 * sqrt(4) = 131072
  printf("debug %i, %i\n", fp_sqrt(65536*4), sqrt(4) * 65536);
  printf("debug %i, %f\n", fp_sqrt(65536*0.5), (double)sqrt(0.5) * 65536.0);
  printf("debug %i, %f\n", fp_sqrt(65536*0.3), (double)sqrt(0.3) * 65536.0);
  printf("debug %i, %f\n", fp_sqrt(65536*0.1), (double)sqrt(0.1) * 65536.0);
  //  printf("debug %i, %i\n", fp_sqrt(100), fp_sqrt(65536*100));//crashes
}
