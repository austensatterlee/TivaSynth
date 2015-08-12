#include "arm_math.h"

/*
 * Filter cutoff map
 *
 * FS: 192000
 * MOD_FS: 750
 * Q: (1, 31)
 * f0: 100
 * f1: 50000
 *
 */
const q31_t fcTable[256] = {
0x6b3b9b,0x6dde2b,0x70914e,0x73556d,0x762af2,0x79124c,0x7c0be9,0x7f183e,0x8237bf,0x856ae6,0x88b22d,0x8c0e14,0x8f7f1c,0x9305cb,0x96a2a7,0x9a563e,
0x9e211d,0xa203d8,0xa5ff04,0xaa133b,0xae411b,0xb28944,0xb6ec5e,0xbb6b10,0xc00608,0xc4bdf8,0xc99397,0xce879f,0xd39ad0,0xd8cded,0xde21bf,0xe39714,
0xe92ebf,0xeee998,0xf4c87b,0xfacc4c,0x100f5f3,0x107465e,0x10dbe80,0x1145f54,0x11b29d9,0x1221f16,0x1294017,0x1308def,0x13809ba,0x13fb496,0x1478fad,0x14f9c2e,
0x157db50,0x1604e50,0x168f675,0x171d50e,0x17aeb71,0x1843afb,0x18dc516,0x1978b31,0x1a18ec6,0x1abd156,0x1b6546e,0x1c119a4,0x1cc2298,0x1d770f2,0x1e30668,0x1eee4b9,
0x1fb0dae,0x207831e,0x21446e7,0x2215af8,0x22ec148,0x23c7bdd,0x24a8cc8,0x258f628,0x267ba28,0x276db03,0x2865aff,0x2963c73,0x2a681c2,0x2b72d61,0x2c841d1,0x2d9c1a6,
0x2ebaf82,0x2fe0e18,0x310e02c,0x3242893,0x337ea34,0x34c2808,0x360e51c,0x376248e,0x38be990,0x3a23769,0x3b91173,0x3d07b1e,0x3e877f0,0x4010b82,0x41a3988,0x43405c9,
0x44e7425,0x4698896,0x485472c,0x4a1b411,0x4bed38a,0x4dca9f5,0x4fb3bcd,0x51a8da7,0x53aa435,0x55b8447,0x57d32c9,0x59fb4c7,0x5c30f6d,0x5e74803,0x60c63f7,0x63268d4,
0x6595c4a,0x681442a,0x6aa266a,0x6d40925,0x6fef29b,0x72ae930,0x757f373,0x7861819,0x7b55dff,0x7e5cc2f,0x81769d9,0x84a3e5d,0x87e5146,0x8b3aa4c,0x8ea5156,0x9224e7b,
0x95baa03,0x9966c67,0x9d29e51,0xa1048a1,0xa4f746c,0xa902afa,0xad275ce,0xb165e9e,0xb5bef5e,0xba33238,0xbec3194,0xc36f816,0xc83909d,0xcd20649,0xd22647a,0xd74b6ce,
0xdc90928,0xe1f67ab,0xe77debf,0xed27b12,0xf2f4996,0xf8e5786,0xfefb262,0x105367f5,0x10b98654,0x11221bdd,0x118d3739,0x11fae75e,0x126b3b8e,0x12de4358,0x13540e9a,0x13ccad7f,
0x1448307f,0x14c6a862,0x1548263e,0x15ccbb78,0x165479c2,0x16df731c,0x176db9d5,0x17ff6085,0x18947a12,0x192d19ab,0x19c952c7,0x1a693924,0x1b0ce0c4,0x1bb45ded,0x1c5fc523,0x1d0f2b24,
0x1dc2a4ea,0x1e7a47a1,0x1f3628a5,0x1ff65d7b,0x20bafbd1,0x2184196d,0x2251cc32,0x23242a0e,0x23fb48f8,0x24d73ee4,0x25b821b6,0x269e073d,0x27890520,0x287930d4,0x296e9f8b,0x2a696627,
0x2b699924,0x2c6f4c86,0x2d7a93c7,0x2e8b81bc,0x2fa2287f,0x30be9954,0x31e0e48b,0x33091961,0x343745e1,0x356b76bb,0x36a5b71f,0x37e61095,0x392c8ac9,0x3a792b61,0x3bcbf5c5,0x3d24eae6,
0x3e840900,0x3fe94b5e,0x4154aa0c,0x42c61993,0x443d8aa6,0x45bae9cb,0x473e1f02,0x48c70d5f,0x4a5592a7,0x4be986db,0x4d82bbc6,0x4f20fc7c,0x50c40cd6,0x526ba8e1,0x5417844a,0x55c749bc,
0x577a9a3a,0x59310c69,0x5aea2bd7,0x5ca57832,0x5e62647b,0x60205628,0x61dea443,0x639c9677,0x6559641a,0x67143329,0x68cc173a,0x6a801068,0x6c2f0a33,0x6dd7da5f,0x6f793fc2,0x7111e119,
0x72a04bd0,0x7422f2d1,0x75982d4f,0x76fe3594,0x785327e0,0x79950145,0x7ac19ea3,0x7bd6bbac,0x7cd1f209,0x7db0b8a6,0x7e70631e,0x7f0e215e,0x7f86ff8a,0x7fd7e61f,0x7ffd9a70,0x7ff4bf82,
};

/*
 * Oscillator phase step map (notes)
 *
 * FS: 192000
 * MOD_FS: 750
 * Q: 31
 * f0: 3.4375
 *
 */
const q31_t noteTable[128] = {
0x00962f,0x009f1e,0x00a894,0x00b29a,0x00bd39,0x00c879,0x00d465,0x00e106,0x00ee68,0x00fc95,0x010b9a,
0x011b83,0x012c5f,0x013e3c,0x015128,0x016534,0x017a72,0x0190f3,0x01a8ca,0x01c20d,0x01dcd0,0x01f92a,
0x021734,0x023707,0x0258bf,0x027c78,0x02a250,0x02ca69,0x02f4e4,0x0321e6,0x035195,0x03841a,0x03b9a0,
0x03f254,0x042e68,0x046e0f,0x04b17e,0x04f8f0,0x0544a1,0x0594d3,0x05e9c9,0x0643cd,0x06a32b,0x070834,
0x077340,0x07e4a9,0x085cd1,0x08dc1e,0x0962fc,0x09f1e0,0x0a8942,0x0b29a6,0x0bd392,0x0c879a,0x0d4656,
0x0e1069,0x0ee680,0x0fc953,0x10b9a2,0x11b83c,0x12c5f9,0x13e3c0,0x151285,0x16534c,0x17a725,0x190f34,
0x1a8cac,0x1c20d2,0x1dcd01,0x1f92a6,0x217345,0x237078,0x258bf2,0x27c780,0x2a250b,0x2ca698,0x2f4e4b,
0x321e68,0x351958,0x3841a5,0x3b9a03,0x3f254d,0x42e68a,0x46e0f0,0x4b17e4,0x4f8f01,0x544a17,0x594d30,
0x5e9c96,0x643cd1,0x6a32b0,0x70834b,0x773407,0x7e4a9b,0x85cd15,0x8dc1e0,0x962fc9,0x9f1e02,0xa8942e,
0xb29a61,0xbd392c,0xc879a3,0xd46561,0xe10697,0xee680e,0xfc9536,0x10b9a2a,0x11b83c1,0x12c5f92,0x13e3c05,
0x151285c,0x16534c3,0x17a7259,0x190f346,0x1a8cac3,0x1c20d2e,0x1dcd01d,0x1f92a6c,0x2173455,0x2370783,0x258bf25,
};

/*
 * Oscillator phase step map (semitones)
 *
 * FS: 192000
 * MOD_FS: 750
 * Q: 15
 *
 */
const q31_t semitoneTable[128] = {
0x00032c,0x00035d,0x000390,0x0003c6,0x000400,0x00043c,0x00047d,0x0004c1,0x00050a,0x000556,0x0005a8,
0x0005fe,0x000659,0x0006ba,0x000720,0x00078d,0x000800,0x000879,0x0008fa,0x000983,0x000a14,0x000aad,
0x000b50,0x000bfc,0x000cb2,0x000d74,0x000e41,0x000f1a,0x001000,0x0010f3,0x0011f5,0x001306,0x001428,
0x00155b,0x0016a0,0x0017f9,0x001965,0x001ae8,0x001c82,0x001e34,0x002000,0x0021e7,0x0023eb,0x00260d,
0x002851,0x002ab7,0x002d41,0x002ff2,0x0032cb,0x0035d1,0x003904,0x003c68,0x004000,0x0043ce,0x0047d6,
0x004c1b,0x0050a2,0x00556e,0x005a82,0x005fe4,0x006597,0x006ba2,0x007208,0x0078d0,0x008000,0x00879c,
0x008fac,0x009837,0x00a145,0x00aadc,0x00b504,0x00bfc8,0x00cb2f,0x00d744,0x00e411,0x00f1a1,0x010000,
0x010f38,0x011f59,0x01306f,0x01428a,0x0155b8,0x016a09,0x017f91,0x01965f,0x01ae89,0x01c823,0x01e343,
0x020000,0x021e71,0x023eb3,0x0260df,0x028514,0x02ab70,0x02d413,0x02ff22,0x032cbf,0x035d13,0x039047,
0x03c686,0x040000,0x043ce3,0x047d66,0x04c1bf,0x050a28,0x0556e0,0x05a827,0x05fe44,0x06597f,0x06ba27,
0x07208f,0x078d0d,0x080000,0x0879c7,0x08facd,0x09837f,0x0a1451,0x0aadc0,0x0b504f,0x0bfc88,0x0cb2ff,
};

/*
 * Soft curve shape
 *
 * FS: 192000
 * MOD_FS: 750
 * Q: 31
 *
 */
const q31_t SoftTable[256] = {
0x000000,0x8cbeb9,0x1196152,0x1a5e7d4,0x2325245,0x2bea0af,0x34ad31a,0x3d6e98f,0x462e416,0x4eec2b7,0x57a857a,0x6062c68,0x691b78a,0x71d26e7,0x7a87a87,0x833b274,
0x8beceb5,0x949cf52,0x9d4b455,0xa5f7dc4,0xaea2ba8,0xb74be0a,0xbff34f1,0xc899067,0xd13d072,0xd9df51b,0xe27fe6a,0xeb1ec68,0xf3bbf1c,0xfc5768e,0x104f12c7,0x10d893cf,
0x1161f9ad,0x11eb446b,0x1274740f,0x12fd88a1,0x1386822b,0x140f60b3,0x14982443,0x1520cce0,0x15a95a95,0x1631cd69,0x16ba2562,0x1742628b,0x17ca84ea,0x18528c87,0x18da796b,0x19624b9c,
0x19ea0324,0x1a71a009,0x1af92254,0x1b808a0c,0x1c07d73a,0x1c8f09e5,0x1d162214,0x1d9d1fd1,0x1e240321,0x1eaacc0e,0x1f317a9f,0x1fb80edb,0x203e88cb,0x20c4e875,0x214b2de3,0x21d1591b,
0x22576a25,0x22dd6109,0x23633dce,0x23e9007c,0x246ea91b,0x24f437b2,0x2579ac49,0x25ff06e8,0x26844796,0x27096e5a,0x278e7b3c,0x28136e44,0x2898477a,0x291d06e4,0x29a1ac8a,0x2a263875,
0x2aaaaaaa,0x2b2f0332,0x2bb34215,0x2c376759,0x2cbb7306,0x2d3f6524,0x2dc33db9,0x2e46fcce,0x2ecaa26a,0x2f4e2e94,0x2fd1a154,0x3054fab0,0x30d83ab0,0x315b615c,0x31de6ebb,0x326162d5,
0x32e43daf,0x3366ff53,0x33e9a7c6,0x346c3712,0x34eead3b,0x35710a4b,0x35f34e47,0x36757939,0x36f78b25,0x37798415,0x37fb640e,0x387d2b19,0x38fed93c,0x39806e7e,0x3a01eae8,0x3a834e7f,
0x3b04994b,0x3b85cb53,0x3c06e49e,0x3c87e534,0x3d08cd1b,0x3d899c5a,0x3e0a52f9,0x3e8af0ff,0x3f0b7672,0x3f8be35a,0x400c37be,0x408c73a4,0x410c9714,0x418ca215,0x420c94ae,0x428c6ee5,
0x430c30c3,0x438bda4c,0x440b6b89,0x448ae481,0x450a453a,0x45898dbc,0x4608be0d,0x4687d634,0x4706d638,0x4785be20,0x48048df2,0x488345b6,0x4901e573,0x49806d2f,0x49fedcf1,0x4a7d34c0,
0x4afb74a3,0x4b799ca0,0x4bf7acbf,0x4c75a506,0x4cf3857b,0x4d714e26,0x4deeff0e,0x4e6c9838,0x4eea19ac,0x4f678371,0x4fe4d58d,0x50621007,0x50df32e5,0x515c3e2f,0x51d931eb,0x52560e1f,
0x52d2d2d2,0x534f800c,0x53cc15d1,0x5448942a,0x54c4fb1c,0x55414aaf,0x55bd82e9,0x5639a3d0,0x56b5ad6b,0x57319fc0,0x57ad7ad7,0x58293eb6,0x58a4eb62,0x592080e3,0x599bff40,0x5a17667e,
0x5a92b6a4,0x5b0defb9,0x5b8911c3,0x5c041cc9,0x5c7f10d1,0x5cf9ede1,0x5d74b400,0x5def6335,0x5e69fb85,0x5ee47cf8,0x5f5ee793,0x5fd93b5d,0x6053785c,0x60cd9e97,0x6147ae14,0x61c1a6d9,
0x623b88ee,0x62b55457,0x632f091c,0x63a8a742,0x64222ed0,0x649b9fcd,0x6514fa3e,0x658e3e2a,0x66076b98,0x6680828c,0x66f9830f,0x67726d25,0x67eb40d5,0x6863fe25,0x68dca51d,0x695535c0,
0x69cdb017,0x6a461427,0x6abe61f6,0x6b36998b,0x6baebaeb,0x6c26c61d,0x6c9ebb27,0x6d169a0f,0x6d8e62db,0x6e061592,0x6e7db239,0x6ef538d6,0x6f6ca970,0x6fe4040c,0x705b48b2,0x70d27766,
0x7149902f,0x71c09312,0x72378017,0x72ae5743,0x7325189b,0x739bc427,0x741259eb,0x7488d9ef,0x74ff4437,0x757598cb,0x75ebd7af,0x766200ea,0x76d81482,0x774e127d,0x77c3fae0,0x7839cdb2,
0x78af8af8,0x792532b9,0x799ac4fa,0x7a1041c2,0x7a85a915,0x7afafafa,0x7b703778,0x7be55e93,0x7c5a7051,0x7ccf6cb9,0x7d4453d0,0x7db9259c,0x7e2de223,0x7ea2896b,0x7f171b79,0x7f8b9853,
};
