#include <stdio.h>
#include <math.h>

#define FLOAT_FUDGE 0.00001
#define FP_BASE 16
#if !__GNUC_PREREQ (2, 7)

/* These types are defined by the ISO C99 header <inttypes.h>. */
# ifndef __int8_t_defined
#  define __int8_t_defined
typedef	char int8_t;
typedef	short int int16_t;
typedef	int int32_t;
#  if __WORDSIZE == 64
typedef long int int64_t;
#  else
__extension__ typedef long long int int64_t;
#  endif
# endif

/* But these were defined by ISO C without the first `_'.  */
typedef	unsigned char u_int8_t;
typedef	unsigned short int u_int16_t;
typedef	unsigned int u_int32_t;
# if __WORDSIZE == 64
typedef unsigned long int u_int64_t;
# else
__extension__ typedef unsigned long long int u_int64_t;
# endif

typedef int register_t;

#else

/* For GCC 2.7 and later, we can use specific type-size attributes.  */
# define __intN_t(N, MODE)					\
  typedef int int##N##_t __attribute__ ((__mode__ (MODE)))
# define __u_intN_t(N, MODE)						\
  typedef unsigned int u_int##N##_t __attribute__ ((__mode__ (MODE)))

# ifndef __int8_t_defined
#  define __int8_t_defined
__intN_t (8, __QI__);
__intN_t (16, __HI__);
__intN_t (32, __SI__);
__intN_t (64, __DI__);
# endif

__u_intN_t (8, __QI__);
__u_intN_t (16, __HI__);
__u_intN_t (32, __SI__);
__u_intN_t (64, __DI__);

typedef int register_t __attribute__ ((__mode__ (__word__)));
#endif

enum charmKind {
  CHARM_HEALTH,
  CHARM_PROTECTION,
  CHARM_HASTE,
  CHARM_FIRE_IMMUNITY,
  CHARM_INVISIBILITY,
  CHARM_TELEPATHY,
  CHARM_LEVITATION,
  CHARM_SHATTERING,
  CHARM_GUARDIAN,
  CHARM_TELEPORTATION,
  CHARM_RECHARGING,
  CHARM_NEGATION,
  NUMBER_CHARM_KINDS
};

const char * charmNames[] = { "health", "protection", "haste", "fire immunity", "invisibility", "telepathy", "levitation", "shattering", "guardian", "teleportation", "recharging", "negation" };


#define min(x, y)		(((x) < (y)) ? (x) : (y))
#define max(x, y)		(((x) > (y)) ? (x) : (y))
#define clamp(x, low, hi)	(min(hi, max(x, low))) // pins x to the [y, z] interval

short charmEffectDuration174(short charmKind, short enchant) {
  const short duration[NUMBER_CHARM_KINDS] = {
    3,  // Health
    20, // Protection
    7,  // Haste
    10, // Fire immunity
    5,  // Invisibility
    25, // Telepathy
    10, // Levitation
    0,  // Shattering
    18, // Guardian
    0,  // Teleportation
    0,  // Recharging
    0,  // Negation
  };
  const short increment[NUMBER_CHARM_KINDS] = {
    0,  // Health
    0,  // Protection
    20, // Haste
    25, // Fire immunity
    20, // Invisibility
    25, // Telepathy
    25, // Levitation
    0,  // Shattering
    0,  // Guardian
    0,  // Teleportation
    0,  // Recharging
    0,  // Negation
  };
    
  return duration[charmKind] * (pow((double) (100 + (increment[charmKind])) / 100, enchant) + FLOAT_FUDGE);
}

short charmRechargeDelay174(short charmKind, short enchant) {
  const short duration[NUMBER_CHARM_KINDS] = {
    2500,   // Health
    1000,   // Protection
    800,    // Haste
    800,    // Fire immunity
    800,    // Invisibility
    800,    // Telepathy
    800,    // Levitation
    2500,   // Shattering
    700,    // Guardian
    1000,   // Teleportation
    10000,  // Recharging
    2500,   // Negation
  };
  const short increment[NUMBER_CHARM_KINDS] = {
    45, // Health
    40, // Protection
    35, // Haste
    40, // Fire immunity
    35, // Invisibility
    35, // Telepathy
    35, // Levitation
    40, // Shattering
    30, // Guardian
    45, // Teleportation
    45, // Recharging
    40, // Negation
  };
    
  return charmEffectDuration174(charmKind, enchant) + duration[charmKind] * (pow((double) (100 - (increment[charmKind])) / 100, enchant) + FLOAT_FUDGE);
}



short charmEffectDuration(short charmKind, short enchant) {
  const int64_t POW_0_CHARM_INCREMENT[50] = {0};
  const int64_t POW_120_CHARM_INCREMENT[] = { // 1.20^x << FP_BASE, with x from 1 to 50 in increments of 1:
    78643, 94371, 113246, 135895, 163074, 195689, 234827, 281792, 338151, 405781, 486937, 584325, 701190, 841428, 1009714, 1211657,
    1453988, 1744786, 2093744, 2512492, 3014991, 3617989, 4341587, 5209905, 6251886, 7502263, 9002716, 10803259, 12963911, 15556694,
    18668032, 22401639, 26881967, 32258360, 38710033, 46452039, 55742447, 66890937, 80269124, 96322949, 115587539, 138705047, 166446056,
    199735268, 239682321, 287618785, 345142543, 414171051, 497005262, 596406314, 715687577};
  const int64_t POW_125_CHARM_INCREMENT[] = { // 1.25^x << FP_BASE, with x from 1 to 50 in increments of 1:
    81920, 102400, 128000, 160000, 200000, 250000, 312500, 390625, 488281, 610351, 762939, 953674, 1192092, 1490116, 1862645, 2328306,
    2910383, 3637978, 4547473, 5684341, 7105427, 8881784, 11102230, 13877787, 17347234, 21684043, 27105054, 33881317, 42351647, 52939559,
    66174449, 82718061, 103397576, 129246970, 161558713, 201948391, 252435489, 315544362, 394430452, 493038065, 616297582, 770371977,
    962964972, 1203706215, 1504632769, 1880790961, 2350988701, 2938735877, 3673419846, 4591774807, 5739718509};
  const short duration[NUMBER_CHARM_KINDS] = {
    3,  // Health
    20, // Protection
    7,  // Haste
    10, // Fire immunity
    5,  // Invisibility
    25, // Telepathy
    10, // Levitation
    0,  // Shattering
    18, // Guardian
    0,  // Teleportation
    0,  // Recharging
    0,  // Negation
  };
  const int64_t *increment[NUMBER_CHARM_KINDS] = {
    POW_0_CHARM_INCREMENT,      // Health
    POW_0_CHARM_INCREMENT,      // Protection
    POW_120_CHARM_INCREMENT,    // Haste
    POW_125_CHARM_INCREMENT,    // Fire immunity
    POW_120_CHARM_INCREMENT,    // Invisibility
    POW_125_CHARM_INCREMENT,    // Telepathy
    POW_125_CHARM_INCREMENT,    // Levitation
    POW_0_CHARM_INCREMENT,      // Shattering
    POW_0_CHARM_INCREMENT,      // Guardian
    POW_0_CHARM_INCREMENT,      // Teleportation
    POW_0_CHARM_INCREMENT,      // Recharging
    POW_0_CHARM_INCREMENT,      // Negation
  };
  enchant = clamp(enchant, 1, 50) - 1;
  return duration[charmKind] * increment[charmKind][enchant] >> FP_BASE;
}

short charmEffectDurationFix(short charmKind, short enchant) {
  const int64_t POW_0_CHARM_INCREMENT[50] = { 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536 };
  const int64_t POW_120_CHARM_INCREMENT[] = { // 1.20^x << FP_BASE, with x from 1 to 50 in increments of 1:
    78643, 94371, 113246, 135895, 163074, 195689, 234827, 281792, 338151, 405781, 486937, 584325, 701190, 841428, 1009714, 1211657,
    1453988, 1744786, 2093744, 2512492, 3014991, 3617989, 4341587, 5209905, 6251886, 7502263, 9002716, 10803259, 12963911, 15556694,
    18668032, 22401639, 26881967, 32258360, 38710033, 46452039, 55742447, 66890937, 80269124, 96322949, 115587539, 138705047, 166446056,
    199735268, 239682321, 287618785, 345142543, 414171051, 497005262, 596406314, 715687577};
  const int64_t POW_125_CHARM_INCREMENT[] = { // 1.25^x << FP_BASE, with x from 1 to 50 in increments of 1:
    81920, 102400, 128000, 160000, 200000, 250000, 312500, 390625, 488281, 610351, 762939, 953674, 1192092, 1490116, 1862645, 2328306,
    2910383, 3637978, 4547473, 5684341, 7105427, 8881784, 11102230, 13877787, 17347234, 21684043, 27105054, 33881317, 42351647, 52939559,
    66174449, 82718061, 103397576, 129246970, 161558713, 201948391, 252435489, 315544362, 394430452, 493038065, 616297582, 770371977,
    962964972, 1203706215, 1504632769, 1880790961, 2350988701, 2938735877, 3673419846, 4591774807, 5739718509};
  const short duration[NUMBER_CHARM_KINDS] = {
    3,  // Health
    20, // Protection
    7,  // Haste
    10, // Fire immunity
    5,  // Invisibility
    25, // Telepathy
    10, // Levitation
    0,  // Shattering
    18, // Guardian
    0,  // Teleportation
    0,  // Recharging
    0,  // Negation
  };
  const int64_t *increment[NUMBER_CHARM_KINDS] = {
    POW_0_CHARM_INCREMENT,      // Health
    POW_0_CHARM_INCREMENT,      // Protection
    POW_120_CHARM_INCREMENT,    // Haste
    POW_125_CHARM_INCREMENT,    // Fire immunity
    POW_120_CHARM_INCREMENT,    // Invisibility
    POW_125_CHARM_INCREMENT,    // Telepathy
    POW_125_CHARM_INCREMENT,    // Levitation
    POW_0_CHARM_INCREMENT,      // Shattering
    POW_0_CHARM_INCREMENT,      // Guardian
    POW_0_CHARM_INCREMENT,      // Teleportation
    POW_0_CHARM_INCREMENT,      // Recharging
    POW_0_CHARM_INCREMENT,      // Negation
  };
  enchant = clamp(enchant, 1, 50) - 1;
  return duration[charmKind] * increment[charmKind][enchant] >> FP_BASE;
}


short charmRechargeDelay(short charmKind, short enchant) {
  const int64_t POW_30_CHARM_DECREMENT[] = { // 0.7^x << FP_BASE, with x from 1 to 50 in increments of 1:
    45875, 32112, 22478, 15735, 11014, 7710, 5397, 3778, 2644, 1851, 1295, 907, 634, 444, 311, 217, 152, 106, 74, 52, 36, 25,
    17, 12, 8, 6, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const int64_t POW_35_CHARM_DECREMENT[] = { // 0.65^x << FP_BASE, with x from 1 to 50 in increments of 1:
    42598, 27688, 17997, 11698, 7604, 4942, 3212, 2088, 1357, 882, 573, 372, 242, 157, 102, 66, 43, 28, 18, 11, 7, 5, 3, 2, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const int64_t POW_40_CHARM_DECREMENT[] = { // 0.60^x << FP_BASE, with x from 1 to 50 in increments of 1:
    39321, 23592, 14155, 8493, 5096, 3057, 1834, 1100, 660, 396, 237, 142, 85, 51, 30, 18, 11, 6, 3, 2, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const int64_t POW_45_CHARM_DECREMENT[] = { // 0.55^x << FP_BASE, with x from 1 to 50 in increments of 1:
    36044, 19824, 10903, 5996, 3298, 1814, 997, 548, 301, 165, 91, 50, 27, 15, 8, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const short duration[NUMBER_CHARM_KINDS] = {
    2500,   // Health
    1000,   // Protection
    800,    // Haste
    800,    // Fire immunity
    800,    // Invisibility
    800,    // Telepathy
    800,    // Levitation
    2500,   // Shattering
    700,    // Guardian
    1000,   // Teleportation
    10000,  // Recharging
    2500,   // Negation
  };
  const int64_t *increment[NUMBER_CHARM_KINDS] = {
    POW_45_CHARM_DECREMENT, // Health
    POW_40_CHARM_DECREMENT, // Protection
    POW_35_CHARM_DECREMENT, // Haste
    POW_40_CHARM_DECREMENT, // Fire immunity
    POW_35_CHARM_DECREMENT, // Invisibility
    POW_35_CHARM_DECREMENT, // Telepathy
    POW_35_CHARM_DECREMENT, // Levitation
    POW_40_CHARM_DECREMENT, // Shattering
    POW_30_CHARM_DECREMENT, // Guardian
    POW_45_CHARM_DECREMENT, // Teleportation
    POW_45_CHARM_DECREMENT, // Recharging
    POW_40_CHARM_DECREMENT, // Negation
  };
  enchant = clamp(enchant, 1, 50) - 1;
  short inc_part =  (duration[charmKind] * increment[charmKind][enchant] >> FP_BASE);
  short delay = charmEffectDuration(charmKind, enchant)
    + (duration[charmKind] * increment[charmKind][enchant] >> FP_BASE);
  return max(1, delay);
}

short charmRechargeDelayFix(short charmKind, short enchant) {
  const int64_t POW_30_CHARM_DECREMENT[] = { // 0.7^x << FP_BASE, with x from 1 to 50 in increments of 1:
    45875, 32112, 22478, 15735, 11014, 7710, 5397, 3778, 2644, 1851, 1295, 907, 634, 444, 311, 217, 152, 106, 74, 52, 36, 25,
    17, 12, 8, 6, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const int64_t POW_35_CHARM_DECREMENT[] = { // 0.65^x << FP_BASE, with x from 1 to 50 in increments of 1:
    42598, 27688, 17997, 11698, 7604, 4942, 3212, 2088, 1357, 882, 573, 372, 242, 157, 102, 66, 43, 28, 18, 11, 7, 5, 3, 2, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const int64_t POW_40_CHARM_DECREMENT[] = { // 0.60^x << FP_BASE, with x from 1 to 50 in increments of 1:
    39321, 23592, 14155, 8493, 5096, 3057, 1834, 1100, 660, 396, 237, 142, 85, 51, 30, 18, 11, 6, 3, 2, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const int64_t POW_45_CHARM_DECREMENT[] = { // 0.55^x << FP_BASE, with x from 1 to 50 in increments of 1:
    36044, 19824, 10903, 5996, 3298, 1814, 997, 548, 301, 165, 91, 50, 27, 15, 8, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const short duration[NUMBER_CHARM_KINDS] = {
    2500,   // Health
    1000,   // Protection
    800,    // Haste
    800,    // Fire immunity
    800,    // Invisibility
    800,    // Telepathy
    800,    // Levitation
    2500,   // Shattering
    700,    // Guardian
    1000,   // Teleportation
    10000,  // Recharging
    2500,   // Negation
  };
  const int64_t *increment[NUMBER_CHARM_KINDS] = {
    POW_45_CHARM_DECREMENT, // Health
    POW_40_CHARM_DECREMENT, // Protection
    POW_35_CHARM_DECREMENT, // Haste
    POW_40_CHARM_DECREMENT, // Fire immunity
    POW_35_CHARM_DECREMENT, // Invisibility
    POW_35_CHARM_DECREMENT, // Telepathy
    POW_35_CHARM_DECREMENT, // Levitation
    POW_40_CHARM_DECREMENT, // Shattering
    POW_30_CHARM_DECREMENT, // Guardian
    POW_45_CHARM_DECREMENT, // Teleportation
    POW_45_CHARM_DECREMENT, // Recharging
    POW_40_CHARM_DECREMENT, // Negation
  };
  enchant = clamp(enchant, 1, 50) - 1;
  short delay = charmEffectDurationFix(charmKind, enchant + 1)
    + (duration[charmKind] * increment[charmKind][enchant] >> FP_BASE);
  return max(1, delay);
}

short fp_charmProtection(int64_t enchant) {
  const int64_t POW_CHARM_PROTECTION[] = {
    // 1.35^x << FP_BASE, with x from 0 to 50 in increments of 1:
    65536, 88473, 119439, 161243, 217678, 293865, 396718, 535570, 723019, 976076, 1317703, 1778899, 2401514, 3242044, 4376759, 5908625, 7976644, 10768469,
    14537434, 19625536, 26494473, 35767539, 48286178, 65186341, 88001560, 118802106, 160382844, 216516839, 292297733, 394601940, 532712620, 719162037, 970868750,
    1310672812, 1769408297, 2388701201, 3224746621, 4353407939, 5877100717, 7934085969, 10711016058, 14459871678, 19520826766, 26353116134, 35576706781,
    48028554155, 64838548109, 87532039948, 118168253930, 159527142806, 215361642788};
    
  enchant = (enchant >> FP_BASE);
  return 150 * POW_CHARM_PROTECTION[enchant] >> FP_BASE;
}

int fp_charmProtection_fix(int64_t enchant) {
  const int64_t POW_CHARM_PROTECTION[] = {
    // 1.35^x << FP_BASE, with x from 0 to 50 in increments of 1:
    65536, 88473, 119439, 161243, 217678, 293865, 396718, 535570, 723019, 976076, 1317703, 1778899, 2401514, 3242044, 4376759, 5908625, 7976644, 10768469,
    14537434, 19625536, 26494473, 35767539, 48286178, 65186341, 88001560, 118802106, 160382844, 216516839, 292297733, 394601940, 532712620, 719162037, 970868750,
    1310672812, 1769408297, 2388701201, 3224746621, 4353407939, 5877100717, 7934085969, 10711016058, 14459871678, 19520826766, 26353116134, 35576706781,
    48028554155, 64838548109, 87532039948, 118168253930, 159527142806, 215361642788};
    
  enchant = (enchant >> FP_BASE) - 1;
  return 150 * POW_CHARM_PROTECTION[enchant] >> FP_BASE;
}

#define charmProtection(enchant)			((int) (150 * pow(1.35, (double) (enchant) - 1) + FLOAT_FUDGE))

int fp_ringWisdomMultiplier_fix(int64_t enchant) {
  const int64_t POW_WISDOM[] = {
    // 1.3^x << FP_BASE, with x from -10 to 30 in increments of 1:
    4753, 6180, 8034, 10444, 13577, 17650, 22945, 29829, 38778, 50412, 65536, 85196, 110755, 143982, 187177, 243330, 316329, 411228, 534597, 694976, 903469,
    1174510, 1526863, 1984922, 2580398, 3354518, 4360874, 5669136, 7369877, 9580840, 12455093, 16191620, 21049107, 27363839, 35572991, 46244888, 60118355,
    78153861, 101600020, 132080026, 171704034};
    
  enchant = min(27, enchant >> FP_BASE) + 10;
  return 10 * POW_WISDOM[enchant] >> FP_BASE;
}

short fp_ringWisdomMultiplier(int64_t enchant) {
  const int64_t POW_WISDOM[] = {
    // 1.3^x << FP_BASE, with x from -10 to 30 in increments of 1:
    4753, 6180, 8034, 10444, 13577, 17650, 22945, 29829, 38778, 50412, 65536, 85196, 110755, 143982, 187177, 243330, 316329, 411228, 534597, 694976, 903469,
    1174510, 1526863, 1984922, 2580398, 3354518, 4360874, 5669136, 7369877, 9580840, 12455093, 16191620, 21049107, 27363839, 35572991, 46244888, 60118355,
    78153861, 101600020, 132080026, 171704034};
    
  enchant = min(27, enchant >> FP_BASE) + 10;
  return 10 * POW_WISDOM[enchant] >> FP_BASE;
}

#define ringWisdomMultiplier(enchant)       (int) (10 * pow(1.3, min(27, (enchant))) + FLOAT_FUDGE)

void main(void) {

  int i;
  int max_enchant = 20;

  printf("---healing amount---\n");
  for(i=1;i<min(max_enchant,6);i++) {
  
    int base_enchant = i;
    int enchant = base_enchant << FP_BASE;

    int v174_healing_amount = ((int) (clamp(20 * (base_enchant), 0, 100) + FLOAT_FUDGE));
    int v175_healing_amount = ((int) clamp(20 * (enchant) >> FP_BASE, 0, 100));

    printf("healing +%i 174: %i,\t175:%i\n", base_enchant, v174_healing_amount, v175_healing_amount);
  }
  printf("---protection amount---\n");
  for(i=1;i<min(max_enchant,999);i++) {
  
    int base_enchant = i;
    int enchant = base_enchant << FP_BASE;

    int v174_protection_amount = charmProtection(base_enchant);
    int v175_protection_amount = fp_charmProtection(enchant);
    int v175_protection_amount_fix = fp_charmProtection_fix(enchant);

    printf("protection +%i 174: %i,\t175:%i,\tfix:%i\n", base_enchant, v174_protection_amount, v175_protection_amount, v175_protection_amount_fix);

  }

  printf("---wisdom multiplier---\n");
  for(i=1;i<max_enchant;i++) {
  
    int base_enchant = i;
    int enchant = base_enchant << FP_BASE;

    int v174_wisdom_multi = ringWisdomMultiplier(base_enchant);
    int v175_wisdom_multi = fp_ringWisdomMultiplier(enchant);

    printf("wisdom +%i 174: %i,\t175:%i\n", base_enchant, v174_wisdom_multi, v175_wisdom_multi);

  }

  //All delays

  int c;

  const char *fixed_str = "SAME";
  const char *not_fixed_str = "DIFFERENT!";
  
  for(c=0;c<NUMBER_CHARM_KINDS;c++) {
    printf("---%s delay---\n", charmNames[c]);
    for(i=1;i<min(max_enchant,20);i++) {
  
      int base_enchant = i;
      int enchant = base_enchant << FP_BASE;

      int v174_healing_delay = charmRechargeDelay174(c, base_enchant);
      int v175_healing_delay = charmRechargeDelay(c, base_enchant);
      int v175_healing_delay_fix = charmRechargeDelayFix(c, base_enchant);

      const char *fix_str_to_use = not_fixed_str;
    
      if(abs(v175_healing_delay_fix - v174_healing_delay) <= 1) {
	fix_str_to_use = fixed_str;
      }
    
      printf("%s delay +%i 174: %i,\t175:%i,\tfix:%i\t%s\n", charmNames[c], base_enchant, v174_healing_delay, v175_healing_delay, v175_healing_delay_fix,fix_str_to_use);
    }
  }
  
  //All durations
  
  for(c=0;c<NUMBER_CHARM_KINDS;c++) {
    printf("---%s duration---\n", charmNames[c]);
    for(i=1;i<min(max_enchant,20);i++) {
  
      int base_enchant = i;
      int enchant = base_enchant << FP_BASE;

      int v174_haste_duration = charmEffectDuration174(c, base_enchant);
      int v175_haste_duration = charmEffectDuration(c, base_enchant);
      int v175_haste_duration_fix = charmEffectDurationFix(c, base_enchant);

    
      const char *fix_str_to_use = not_fixed_str;
    
      if(abs(v175_haste_duration_fix - v174_haste_duration) <= 1) {
	fix_str_to_use = fixed_str;
      }

      printf("%s duration +%i 174: %i,\t175:%i,\tfix:%i\t%s\n", charmNames[c], base_enchant, v174_haste_duration, v175_haste_duration, v175_haste_duration_fix,fix_str_to_use);

    }
  }


}
