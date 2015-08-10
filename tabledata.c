#include "arm_math.h"

/*
 * Envelope decay map
 *
 * FS: 192000
 * MOD_FS: 750
 * Q: 31
 * t0: 0.005
 * t1: 5.0
 *
 */
const q31_t envDecayTable[256] = {
0x6666666,0x4db0f03a,0x617bf8b3,0x6a24df81,0x6efba0de,0x7211d8ce,0x74359b42,0x75c796ee,0x76fb214e,0x77ee001f,0x78b2a717,0x79552160,0x79dda0c0,0x7a51ea12,0x7ab62b22,0x7b0d7dde,
0x7b5a3bdc,0x7b9e3544,0x7bdad5ef,0x7c113f1e,0x7c4259a4,0x7c6ee309,0x7c977726,0x7cbc974e,0x7cdeafb9,0x7cfe1ba6,0x7d1b288f,0x7d3618a9,0x7d4f24e1,0x7d667e68,0x7d7c5001,0x7d90befd,
0x7da3ec17,0x7db5f421,0x7dc6f098,0x7dd6f816,0x7de61ebf,0x7df4768e,0x7e020fa1,0x7e0ef875,0x7e1b3e17,0x7e26ec53,0x7e320dd9,0x7e3cac5d,0x7e46d0b5,0x7e5082ed,0x7e59ca63,0x7e62add5,
0x7e6b3373,0x7e7360ec,0x7e7cc415,0x7e84417f,0x7e8b7626,0x7e92661a,0x7e99151f,0x7e9f86b4,0x7ea5be1a,0x7eabbe57,0x7eb18a3c,0x7eb7246c,0x7ebc8f5b,0x7ec1cd54,0x7ec6e080,0x7ecbcae1,
0x7ed08e5d,0x7ed52cb9,0x7ed9a7a1,0x7ede00a8,0x7ee23948,0x7ee652e5,0x7eea4ed2,0x7eee2e4c,0x7ef1f27f,0x7ef59c87,0x7ef92d73,0x7efca640,0x7f0007e0,0x7f035338,0x7f068922,0x7f09aa6b,
0x7f0cb7d9,0x7f0fb226,0x7f129a03,0x7f15701a,0x7f18350c,0x7f1ae973,0x7f1d8de1,0x7f2022e1,0x7f22a8fb,0x7f2520ac,0x7f278a70,0x7f29e6ba,0x7f2c35fb,0x7f2e789d,0x7f30af06,0x7f32d99a,
0x7f34f8b5,0x7f370cb1,0x7f3915e7,0x7f3b14a7,0x7f3d0943,0x7f3ef406,0x7f40d53b,0x7f42ad28,0x7f447c11,0x7f464238,0x7f47ffdc,0x7f49b539,0x7f4b628a,0x7f4d0808,0x7f4ea5e7,0x7f503c5e,
0x7f51cb9e,0x7f5353d9,0x7f54d53c,0x7f564ff7,0x7f57c433,0x7f59321c,0x7f5ae115,0x7f5c41a2,0x7f5d9c5a,0x7f5ef162,0x7f6040dd,0x7f618aed,0x7f62cfb5,0x7f640f54,0x7f6549ea,0x7f667f94,
0x7f67b070,0x7f68dc9a,0x7f6a042f,0x7f6b2748,0x7f6c45ff,0x7f6d606e,0x7f6e76ae,0x7f6f88d5,0x7f7096fb,0x7f71a135,0x7f72a79b,0x7f73aa40,0x7f74a939,0x7f75a49a,0x7f769c76,0x7f7790e1,
0x7f7881ec,0x7f796fa9,0x7f7a5a2a,0x7f7b417e,0x7f7c25b8,0x7f7d06e5,0x7f7de517,0x7f7ec05c,0x7f7f98c3,0x7f806e59,0x7f81412e,0x7f82114e,0x7f82dec7,0x7f83a9a6,0x7f8471f7,0x7f8537c7,
0x7f85fb21,0x7f86bc11,0x7f877aa4,0x7f8836e2,0x7f88f0d9,0x7f89a891,0x7f8a5e16,0x7f8b1171,0x7f8bc2ac,0x7f8c71d2,0x7f8d1eea,0x7f8dca00,0x7f8e731a,0x7f8f1a43,0x7f8fbf82,0x7f9062e0,
0x7f910466,0x7f91a41a,0x7f924206,0x7f92de30,0x7f9378a0,0x7f94115e,0x7f94a870,0x7f953ddd,0x7f95d1ac,0x7f9663e5,0x7f96f48c,0x7f97a01b,0x7f982d68,0x7f98b938,0x7f994392,0x7f99cc7b,
0x7f9a53f9,0x7f9ada11,0x7f9b5eca,0x7f9be228,0x7f9c6431,0x7f9ce4ea,0x7f9d6458,0x7f9de280,0x7f9e5f66,0x7f9edb10,0x7f9f5583,0x7f9fcec2,0x7fa046d2,0x7fa0bdb9,0x7fa13379,0x7fa1a818,
0x7fa21b99,0x7fa28e00,0x7fa2ff52,0x7fa36f93,0x7fa3dec6,0x7fa44cef,0x7fa4ba12,0x7fa52632,0x7fa59153,0x7fa5fb79,0x7fa664a6,0x7fa6ccdf,0x7fa73427,0x7fa79a80,0x7fa7ffef,0x7fa86476,
0x7fa8c818,0x7fa92ad9,0x7fa98cbb,0x7fa9edc1,0x7faa4dee,0x7faaad46,0x7fab0bca,0x7fab697e,0x7fabc663,0x7fac227e,0x7fac7dd0,0x7facd85c,0x7fad3225,0x7fad8b2c,0x7fade375,0x7fae3b02,
0x7fae91d5,0x7faee7f0,0x7faf3d55,0x7faf9208,0x7fafe60a,0x7fb0395c,0x7fb08c03,0x7fb0ddfe,0x7fb12f51,0x7fb17ffe,0x7fb1d006,0x7fb21f6c,0x7fb26e31,0x7fb2bc57,0x7fb309e1,0x7fb36620,
};

/*
 * Filter cutoff map
 *
 * FS: 192000
 * MOD_FS: 750
 * Q: 31
 * f0: 100
 * f1: 80000
 *
 */
const q31_t fcTable[256] = {
0x6b3b9b,0x6e11da,0x70fb50,0x73f87f,0x7709ec,0x7a3020,0x7d6ba8,0x80bd15,0x8424fb,0x87a3f1,0x8b3a95,0x8ee986,0x92b16a,0x9692e8,0x9a8eaf,0x9ea570,
0xa2d7e2,0xa726c1,0xab92ce,0xb01ccd,0xb4c589,0xb98dd3,0xbe7681,0xc3806d,0xc8ac7a,0xcdfb8d,0xd36e94,0xd90684,0xdec454,0xe4a907,0xeab5a3,0xf0eb37,
0xf74ad8,0xfdd5a2,0x1048cbb,0x10b714e,0x112848f,0x119c7ba,0x1213c13,0x128e2e8,0x130bd8f,0x138cd65,0x14113d4,0x149924d,0x1524a4c,0x15b3d55,0x1646cf9,0x16ddad2,
0x1778885,0x18177c2,0x18baa46,0x19621d7,0x1a0e049,0x1abe77b,0x1b7395b,0x1c2d7e1,0x1cec515,0x1db030c,0x1e793e6,0x1f479d7,0x201b71d,0x20f4e08,0x21d40f6,0x22b9256,
0x23a44a8,0x2495a7a,0x258d66e,0x268bb38,0x2790b9d,0x289ca74,0x29afaa8,0x2ac9f39,0x2bebb3a,0x2d151d2,0x2e4663f,0x2f7fbd3,0x30c15f8,0x320b82e,0x335e60c,0x34ba344,
0x361f39d,0x378dafb,0x3905d5a,0x3a87ed1,0x3c14393,0x3daaff0,0x3f4c853,0x40f9146,0x42b0f71,0x447479c,0x4643eae,0x481f9b0,0x4a07dcc,0x4bfd04f,0x4dff6a8,0x500f66c,
0x522d556,0x5459945,0x5694840,0x58de876,0x5b38042,0x5da1625,0x601b0cd,0x62a5715,0x6541006,0x67ee2d5,0x6aad6ea,0x6d7f3db,0x7064171,0x735c7ab,0x7668eb9,0x7989f02,
0x7cc0124,0x800bdf7,0x836de8c,0x86e6c2d,0x8a77064,0x8e1f4f6,0x91e03ea,0x95ba786,0x99aea53,0x9dbd71d,0xa1e78f6,0xa62db38,0xaa90983,0xaf10fc2,0xb3afa2b,0xb86d543,
0xbd4addb,0xc249115,0xc768c65,0xccaad92,0xd2102ba,0xd799a4d,0xdd48318,0xe31cc3e,0xe91853f,0xef3bdf5,0xf58869c,0xfbfefcc,0x102a0a7f,0x1096e811,0x11069a41,0x11793332,
0x11eec56d,0x126763e1,0x12e321e4,0x13621335,0x13e44bfa,0x1469e0c2,0x14f2e685,0x157f72a6,0x160f9aed,0x16a3758f,0x173b1924,0x17d69cae,0x18761794,0x1919a19d,0x19c152f7,0x1a6d4429,
0x1b1d8e1c,0x1bd24a0c,0x1c8b918b,0x1d497e7d,0x1e0c2b0c,0x1ed3b1a9,0x1fa02d00,0x2071b7f4,0x21486d92,0x2224690b,0x2305c5a8,0x23ec9ebd,0x24d90f9b,0x25cb3384,0x26c32597,0x27c100c0,
0x28c4dfa4,0x29cedc8a,0x2adf1144,0x2bf59714,0x2d128690,0x2e35f781,0x2f6000c2,0x3090b81d,0x31c8321b,0x330681e0,0x344bb8f6,0x3597e718,0x36eb19ff,0x38455d1e,0x39a6b962,0x3b0f34eb,
0x3c7ed2be,0x3df5926f,0x3f736fc8,0x40f86267,0x42845d55,0x44174e92,0x45b11ea0,0x4751affc,0x48f8de94,0x4aa67f2d,0x4c5a5ec6,0x4e1441ea,0x4fd3e3f6,0x5198f657,0x53631fb5,0x5531fb18,
0x570516f8,0x58dbf443,0x5ab6054f,0x5c92acc6,0x5e713c78,0x6050f421,0x62310024,0x6410782f,0x65ee5dd0,0x67c99b02,0x69a100a1,0x6b7344db,0x6d3f0189,0x6f02b289,0x70bcb407,0x726b40c6,
0x740c7060,0x759e358d,0x771e5c70,0x788a88e7,0x79e034fa,0x7b1caf5b,0x7c3d1a08,0x7d3e6920,0x7e1d61ee,0x7ed69a33,0x7f6677d2,0x7fc930dc,0x7ffacc10,0x7ff721f8,0x7fb9dea5,0x7f3e8430,
0x7e806e1b,0x7d7ad5ba,0x7c28d7b6,0x7a857ae3,0x788bb88b,0x76368651,0x7380e1f1,0x7065deee,0x6ce0b67d,0x68ecd9cf,0x648606e9,0x5fa8604a,0x5a508774,0x547bba94,0x4e27f559,0x4754150f,
};

/*
 * Oscillator phase step map (notes)
 *
 * FS: 192000
 * MOD_FS: 750
 * Q: 31
 * f0: 0.02685546875
 *
 */
const q31_t noteTable[256] = {
0x00012c,0x00013e,0x000151,0x000165,0x00017a,0x000190,0x0001a8,0x0001c2,0x0001dc,0x0001f9,0x000217,0x000237,0x000258,0x00027c,0x0002a2,0x0002ca,
0x0002f4,0x000321,0x000351,0x000384,0x0003b9,0x0003f2,0x00042e,0x00046e,0x0004b1,0x0004f8,0x000544,0x000594,0x0005e9,0x000643,0x0006a3,0x000708,
0x000773,0x0007e4,0x00085c,0x0008dc,0x000962,0x0009f1,0x000a89,0x000b29,0x000bd3,0x000c87,0x000d46,0x000e10,0x000ee6,0x000fc9,0x0010b9,0x0011b8,
0x0012c5,0x0013e3,0x001512,0x001653,0x0017a7,0x00190f,0x001a8c,0x001c20,0x001dcd,0x001f92,0x002173,0x002370,0x00258b,0x0027c7,0x002a25,0x002ca6,
0x002f4e,0x00321e,0x003519,0x003841,0x003b9a,0x003f25,0x0042e6,0x0046e0,0x004b17,0x004f8f,0x00544a,0x00594d,0x005e9c,0x00643c,0x006a32,0x007083,
0x007734,0x007e4a,0x0085cd,0x008dc1,0x00962f,0x009f1e,0x00a894,0x00b29a,0x00bd39,0x00c879,0x00d465,0x00e106,0x00ee68,0x00fc95,0x010b9a,0x011b83,
0x012c5f,0x013e3c,0x015128,0x016534,0x017a72,0x0190f3,0x01a8ca,0x01c20d,0x01dcd0,0x01f92a,0x021734,0x023707,0x0258bf,0x027c78,0x02a250,0x02ca69,
0x02f4e4,0x0321e6,0x035195,0x03841a,0x03b9a0,0x03f254,0x042e68,0x046e0f,0x04b17e,0x04f8f0,0x0544a1,0x0594d3,0x05e9c9,0x0643cd,0x06a32b,0x070834,
0x077340,0x07e4a9,0x085cd1,0x08dc1e,0x0962fc,0x09f1e0,0x0a8942,0x0b29a6,0x0bd392,0x0c879a,0x0d4656,0x0e1069,0x0ee680,0x0fc953,0x10b9a2,0x11b83c,
0x12c5f9,0x13e3c0,0x151285,0x16534c,0x17a725,0x190f34,0x1a8cac,0x1c20d2,0x1dcd01,0x1f92a6,0x217345,0x237078,0x258bf2,0x27c780,0x2a250b,0x2ca698,
0x2f4e4b,0x321e68,0x351958,0x3841a5,0x3b9a03,0x3f254d,0x42e68a,0x46e0f0,0x4b17e4,0x4f8f01,0x544a17,0x594d30,0x5e9c96,0x643cd1,0x6a32b0,0x70834b,
0x773407,0x7e4a9b,0x85cd15,0x8dc1e0,0x962fc9,0x9f1e02,0xa8942e,0xb29a61,0xbd392c,0xc879a3,0xd46561,0xe10697,0xee680e,0xfc9536,0x10b9a2a,0x11b83c1,
0x12c5f92,0x13e3c05,0x151285c,0x16534c3,0x17a7259,0x190f346,0x1a8cac3,0x1c20d2e,0x1dcd01d,0x1f92a6c,0x2173455,0x2370783,0x258bf25,0x27c780b,0x2a250b9,0x2ca6986,
0x2f4e4b3,0x321e68d,0x3519586,0x3841a5d,0x3b9a03a,0x3f254d9,0x42e68ab,0x46e0f06,0x4b17e4b,0x4f8f016,0x544a173,0x594d30d,0x5e9c967,0x643cd1a,0x6a32b0c,0x70834ba,
0x7734074,0x7e4a9b2,0x85cd157,0x8dc1e0d,0x962fc96,0x9f1e02d,0xa8942e6,0xb29a61a,0xbd392cf,0xc879a35,0xd465619,0xe106974,0xee680e9,0xfc95364,0x10b9a2ae,0x11b83c1a,
0x12c5f92c,0x13e3c05a,0x151285cd,0x16534c34,0x17a7259f,0x190f346a,0x1a8cac33,0x1c20d2e8,0x1dcd01d2,0x1f92a6c8,0x2173455d,0x23707834,0x258bf258,0x27c780b4,0x2a250b9b,0x2ca69869,
};

/*
 * Oscillator phase step map (semitones)
 *
 * FS: 192000
 * MOD_FS: 750
 * Q: 31
 *
 */
const q31_t semitoneTable[256] = {
0x000006,0x000007,0x000007,0x000008,0x000008,0x000009,0x000009,0x00000a,0x00000a,0x00000b,0x00000c,0x00000c,0x00000d,0x00000e,0x00000f,0x000010,
0x000011,0x000012,0x000013,0x000014,0x000015,0x000017,0x000018,0x000019,0x00001b,0x00001d,0x00001e,0x000020,0x000022,0x000024,0x000026,0x000029,
0x00002b,0x00002e,0x000031,0x000033,0x000037,0x00003a,0x00003d,0x000041,0x000045,0x000049,0x00004d,0x000052,0x000057,0x00005c,0x000062,0x000067,
0x00006e,0x000074,0x00007b,0x000082,0x00008a,0x000092,0x00009b,0x0000a4,0x0000ae,0x0000b9,0x0000c4,0x0000cf,0x0000dc,0x0000e9,0x0000f7,0x000105,
0x000115,0x000125,0x000137,0x000149,0x00015d,0x000172,0x000188,0x00019f,0x0001b8,0x0001d2,0x0001ee,0x00020b,0x00022a,0x00024b,0x00026e,0x000293,
0x0002bb,0x0002e4,0x000310,0x00033f,0x000370,0x0003a5,0x0003dc,0x000417,0x000455,0x000497,0x0004dd,0x000527,0x000576,0x0005c9,0x000621,0x00067e,
0x0006e1,0x00074a,0x0007b9,0x00082e,0x0008ab,0x00092f,0x0009bb,0x000a4f,0x000aec,0x000b92,0x000c42,0x000cfd,0x000dc2,0x000e94,0x000f72,0x00105d,
0x001156,0x00125e,0x001376,0x00149e,0x0015d8,0x001724,0x001885,0x0019fa,0x001b85,0x001d28,0x001ee4,0x0020bb,0x0022ad,0x0024bd,0x0026ec,0x00293d,
0x002bb0,0x002e49,0x00310a,0x0033f5,0x00370b,0x003a51,0x003dc9,0x004176,0x00455a,0x00497a,0x004dd9,0x00527a,0x005761,0x005c93,0x006215,0x0067ea,
0x006e17,0x0074a3,0x007b93,0x0082ec,0x008ab5,0x0092f5,0x009bb2,0x00a4f4,0x00aec3,0x00b927,0x00c42a,0x00cfd4,0x00dc2f,0x00e947,0x00f726,0x0105d9,
0x01156b,0x0125ea,0x013764,0x0149e8,0x015d86,0x01724f,0x018854,0x019fa8,0x01b85f,0x01d28f,0x01ee4d,0x020bb2,0x022ad6,0x024bd4,0x026ec8,0x0293d0,
0x02bb0c,0x02e49e,0x0310a8,0x033f50,0x0370bf,0x03a51e,0x03dc9b,0x041764,0x0455ac,0x0497a8,0x04dd91,0x0527a1,0x057619,0x05c93c,0x062150,0x067ea1,
0x06e17f,0x074a3d,0x07b936,0x082ec8,0x08ab58,0x092f51,0x09bb22,0x0a4f43,0x0aec33,0x0b9279,0x0c42a1,0x0cfd43,0x0dc2fe,0x0e947b,0x0f726d,0x105d91,
0x1156b1,0x125ea2,0x137644,0x149e87,0x15d867,0x1724f2,0x188543,0x19fa87,0x1b85fd,0x1d28f7,0x1ee4da,0x20bb23,0x22ad63,0x24bd44,0x26ec88,0x293d0e,
0x2bb0cf,0x2e49e4,0x310a86,0x33f50e,0x370bfa,0x3a51ee,0x3dc9b5,0x417647,0x455ac7,0x497a88,0x4dd911,0x527a1c,0x57619f,0x5c93c9,0x62150d,0x67ea1c,
0x6e17f4,0x74a3dc,0x7b936b,0x82ec8f,0x8ab58f,0x92f511,0x9bb222,0xa4f438,0xaec33e,0xb92793,0xc42a1a,0xcfd439,0xdc2fe9,0xe947b9,0xf726d7,0x105d91f,
};