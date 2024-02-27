// #include "compression/hashFunction.hh"

// int main (void) {
//     int data_size = 16;
//     u_int8_t data[data_size] = {
//         0b10110010, 0b01010101, 0b10101010, 0b01010101,
//         0b10101010, 0b01010111, 0b10100010, 0b01010101,
//         0b10101010, 0b01010101, 0b10101010, 0b01010101,
//         0b10101010, 0b01000111, 0b10100010, 0b00000000,
//     };
//     // int data_size = 5;
//     // u_int8_t data[data_size] = {
//     //     // 0b11111111, 0b00000000, 0b00000001, 0b00000000, 0b00000000
//     //     0b00000000, 0b00000010, 0b00000000, 0b00000001, 0b00000000
//     // };
//     int set_bits = countSetBits(data, data_size);
//     printf("<vanilla set_bits=%d>\n", set_bits);

//     FullBitShuffleHash * fullBitShuffleHash;
//     fullBitShuffleHash = new FullBitShuffleHash();
//     int full_bit_fp_size;
//     u_int8_t * full_bit_fp = fullBitShuffleHash->hash(data, data_size*8, full_bit_fp_size);
//     int full_bit_set_bits = countSetBits(full_bit_fp, full_bit_fp_size/8);
//     printf("<full_bit_set_bits=%d, size=%d>\n", full_bit_set_bits, full_bit_fp_size);

//     delete fullBitShuffleHash;
//     delete [] full_bit_fp;

//     bool use_same_seed_for_all_byte = true;
//     ByteBitShuffleHash * byteBitShuffleHash;
//     byteBitShuffleHash = new ByteBitShuffleHash(use_same_seed_for_all_byte);
//     int byte_bit_fp_size;
//     u_int8_t * byte_bit_fp = byteBitShuffleHash->hash(data, data_size*8, byte_bit_fp_size);
//     int byte_bit_set_bits = countSetBits(byte_bit_fp, byte_bit_fp_size/8);
//     printf("<byte_bit_set_bits=%d, size=%d>\n", byte_bit_set_bits, byte_bit_fp_size);

//     delete byteBitShuffleHash;
//     delete [] byte_bit_fp;


//     XORFoldingHash * xorFoldingHash;
//     xorFoldingHash = new XORFoldingHash(10);
//     xorFoldingHash->print();
//     int xor_folding_fp_size;
//     u_int8_t * xor_folding_fp = xorFoldingHash->hash(data, data_size*8, xor_folding_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < xor_folding_fp_size/8; i++) {
//         printf("%d ", xor_folding_fp[i]);
//     }
//     printf("\n\n\n");
//     delete xorFoldingHash;
//     delete [] xor_folding_fp;

//     ThesaurusLSHash * thesaurusLSHash;
//     thesaurusLSHash = new ThesaurusLSHash(5, data_size);
//     thesaurusLSHash->print();
//     int thesaurus_ls_fp_size;
//     u_int8_t * thesaurus_ls_fp = thesaurusLSHash->hash(data, data_size*8, thesaurus_ls_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(thesaurus_ls_fp_size/8.0); i++) {
//         printf("%d ", thesaurus_ls_fp[i]);
//     }
//     printf("\n\n\n");
//     delete thesaurusLSHash;
//     delete [] thesaurus_ls_fp;



//     // ThesaurusLSHash * thesaurusLSHash;
//     // thesaurusLSHash = new ThesaurusLSHash(20, 16);
//     // thesaurusLSHash->print();
//     // int thesaurus_ls_fp_size;
//     // u_int8_t * thesaurus_ls_fp = thesaurusLSHash->hash(data, data_size*8, thesaurus_ls_fp_size);
//     // // print the fingerprint
//     // for (int i = 0; i < ceill(thesaurus_ls_fp_size/8.0); i++) {
//     //     printf("%d ", thesaurus_ls_fp[i]);
//     // }
//     // printf("\n\n\n");
//     // delete thesaurusLSHash;
//     // delete [] thesaurus_ls_fp;


//     BitSamplingLSHash * bshash;
//     bshash = new BitSamplingLSHash(4);
//     bshash->print();
//     int bs_fp_size;
//     u_int8_t * bs_fp = bshash->hash(data, data_size*8, bs_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(bs_fp_size/8.0); i++) {
//         printf("%d ", bs_fp[i]);
//     }
//     printf("\n\n\n");
//     delete bshash;
//     delete [] bs_fp;


//     ByteMapHash * bmhash;
//     bmhash = new ByteMapHash();
//     bmhash->print();
//     int bm_fp_size;
//     u_int8_t * bm_fp = bmhash->hash(data, data_size*8, bm_fp_size);
//     printf("bm_fp_size: %d\n", bm_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(bm_fp_size/8.0); i++) {
//         printf("%d ", bm_fp[i]);
//     }
//     printf("\n\n\n");
//     delete bmhash;
//     delete [] bm_fp;



//     int nbyte = 3;
//     NByteMapHash * nbmhash;
//     nbmhash = new NByteMapHash(nbyte);
//     nbmhash->print();
//     int nbm_fp_size;
//     u_int8_t * nbm_fp = nbmhash->hash(data, data_size*8, nbm_fp_size);
//     printf("nbm_fp_size: %d\n", nbm_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(nbm_fp_size/8.0); i++) {
//         printf("%d ", nbm_fp[i]);
//     }
//     printf("\n\n\n");
//     delete nbmhash;
//     delete [] nbm_fp;



//     TernaryByteMapHash * tbmhash;
//     tbmhash = new TernaryByteMapHash();
//     tbmhash->print();
//     int tbm_fp_size;
//     u_int8_t * tbm_fp = tbmhash->hash(data, data_size*8, tbm_fp_size);
//     printf("tbm_fp_size: %d\n", tbm_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(tbm_fp_size/8.0); i++) {
//         printf("%d ", tbm_fp[i]);
//     }
//     printf("\n\n\n");
//     delete tbmhash;
//     delete [] tbm_fp;



//     BitSelectionHash * bitsel;
//     for (int i = 0; i < 8; i++) {
//         bitsel = new BitSelectionHash(i);
//         bitsel->print();
//         int bitsel_fp_size;
//         u_int8_t * bitsel_fp = bitsel->hash(data, data_size*8, bitsel_fp_size);
//         printf("bitsel_fp_size: %d\n", bitsel_fp_size);
//         // print the fingerprint
//         for (int i = 0; i < ceill(bitsel_fp_size/8.0); i++) {
//             printf("%d ", bitsel_fp[i]);
//         }
//         printf("\n");
//         delete bitsel;
//         delete [] bitsel_fp;
//     }
//     printf("\n\n\n");

//     ByteSelectionHash * bytesel;
//     bytesel = new ByteSelectionHash(0);
//     bytesel->print();
//     int bytesel_fp_size;
//     u_int8_t * bytesel_fp = bytesel->hash(data, data_size*8, bytesel_fp_size);
//     printf("tbm_fp_size: %d\n", bytesel_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(bytesel_fp_size/8.0); i++) {
//         printf("%d ", bytesel_fp[i]);
//     }
//     printf("\n\n\n");
//     delete bytesel;
//     delete [] bytesel_fp;

//     SparseByteMapHash * sbmhash;
//     sbmhash = new SparseByteMapHash(8,6);
//     sbmhash->print();
//     int sbm_fp_size;
//     u_int8_t * sbm_fp = sbmhash->hash(data, data_size*8, sbm_fp_size);
//     printf("sbm_fp_size: %d\n", sbm_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(sbm_fp_size/8.0); i++) {
//         printf("%d ", sbm_fp[i]);
//     }
//     printf("\n\n\n");
//     delete sbmhash;
//     delete [] sbm_fp;

//     MaskedBitSamplingLSHash * mbs;
//     mbs = new MaskedBitSamplingLSHash(16, 4, 8);
//     mbs->print();
//     int mbs_fp_size;
//     u_int8_t * mbs_fp = mbs->hash(data, data_size*8, mbs_fp_size);
//     printf("mbs_fp_size: %d\n", mbs_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(mbs_fp_size/8.0); i++) {
//         printf("%d ", mbs_fp[i]);
//     }
//     printf("\n\n\n");
//     delete mbs;
//     delete [] mbs_fp;

//     EPCWordLabelingHash * epc;
//     epc = new EPCWordLabelingHash();
//     // epc->print();
//     int epc_fp_size;
//     u_int8_t * epc_fp = epc->hash(data, data_size*8, epc_fp_size);
//     printf("epc_fp_size: %d\n", epc_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(epc_fp_size/8.0); i++) {
//         printf("%d ", epc_fp[i]);
//     }
//     printf("\n\n\n");
//     delete epc;
//     delete [] epc_fp;

//     StrongWordLabelingHash * swl;
//     swl = new StrongWordLabelingHash();
//     // swl->print();
//     int swl_fp_size;
//     u_int8_t * swl_fp = swl->hash(data, data_size*8, swl_fp_size);
//     printf("swl_fp_size: %d\n", swl_fp_size);
//     // print the fingerprint
//     for (int i = 0; i < ceill(swl_fp_size/8.0); i++) {
//         printf("%d ", swl_fp[i]);
//     }
//     printf("\n\n\n");
//     delete swl;
//     delete [] swl_fp;



// }