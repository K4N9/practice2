#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int ip[64] = {
	58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

const int ep[48] = {
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 01
};

const int sBox[8][4][16] = { {
        14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
         0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
         4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
    },{
        15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
         3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
         0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
    },{
        10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
         1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
    },{
         7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
         3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
    },{
         2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
         4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
    },{
        12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
         9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
         4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
    },{
         4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
         1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
         6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
    },{
        13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
         1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
         7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
         2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
    } };

const int sp[32] = {
	16,  7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 05, 18, 31, 10,
    2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};
	
const int ipinverse[64] = {
	40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30, 
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

const int parity[56] = {
	57, 49, 41, 33, 25, 17,  9,  1,
    58, 50, 42, 34, 26, 18, 10,  2,
    59, 51, 43, 35, 27, 19, 11,  3,
    60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15,  7, 62, 54, 46, 38,
    30, 22, 14,  6, 61, 53, 45, 37,
    29, 21, 13,  5, 28, 20, 12,  4
};

const int leftshift[16] = {
	 1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
};

const int compress[48] = {
	14, 17, 11, 24,  1,  5,  3, 28,
    15,  6, 21, 10, 23, 19, 12,  4,
    26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

uint64_t des(uint64_t input, uint64_t key){
	uint64_t subkey[16] = {0}; // 배열 초기화 
	uint64_t ip_result = 0;
	uint64_t ipinverse_result = 0;
	uint64_t parity_result = 0;
	uint64_t leftshift_result = 0;
	uint64_t afterallround;
	uint64_t row,column;
	
	uint32_t left,right,temp;
	uint32_t ep_left,ep_right;
	uint32_t key_left,key_right,key_restore; 
	
	
	//key generation 수행
	for(int i=0; i<56; i++){
		parity_result <<= 1; // 왼쪽으로 한 자리 옮김.
		parity_result = parity_result|((key >> (64-parity[i])) & 0x0000000000000001);
		// i+1번째 bit를 맨 오른쪽(64번째) 자리까지 끌고옴
		// key parity permutation
	}
	
	key_left = (uint32_t)((parity_result>>28)&0x000000000fffffff);
	key_right = (uint32_t)(parity_result&0x000000000fffffff);
	
	for(int i=0; i<16; i++){
		uint64_t comp_result = 0;
		
		key_restore = key_left >> (28-leftshift[i]);
		key_left = (key_left << leftshift[i]) & 0x0fffffff;
		key_left = key_left | key_restore;
		
		key_restore = key_right >> (28-leftshift[i]);
		key_right = (key_right << leftshift[i]) & 0x0fffffff;
		key_right = key_right | key_restore;
		
		leftshift_result = ((uint64_t)key_left << 28) | (uint64_t)key_right;
		
		for(int j=0; j<48; j++){
			comp_result <<= 1; // 왼쪽으로 한 자리 옮김.
			comp_result = comp_result|((leftshift_result >> (56-compress[j])) & 0x0000000000000001);
			// i+1번째 bit를 맨 오른쪽(64번째) 자리까지 끌고옴
		} // key compress permutation
		subkey[i] = comp_result;
	} // leftshift 수행 후 subkey에 저장 
	 
	
	
	
	//des 과정 수행 
	for(int i=0; i< 64; i++){
		ip_result <<= 1; // 왼쪽으로 한 자리 옮김.
		ip_result = ip_result|((input >> (64-ip[i])) & 0x0000000000000001);
		// i+1번째 bit를 맨 오른쪽(64번째) 자리까지 끌고옴
	} // initial permutation 
	
	left = (uint32_t)((ip_result>>32)&0x00000000ffffffff); // ip_result 왼쪽 32자리 
	right = (uint32_t)(ip_result&0x00000000ffffffff); // ip_result 오른쪽 32자리
	
	for(int i=0; i<16; i++){
		uint64_t ep_result = 0;
		uint64_t xor_result = 0;
		uint64_t sBoxtemp = 0;
		uint32_t sBox_result = 0;
		uint32_t sp_result = 0;
		
		for(int j=0; j< 48; j++){
			ep_result <<= 1; // 왼쪽으로 한 자리 옮김.
			ep_result = ep_result|(uint64_t)((right >> (32-ep[j])) & 0x00000001);
			// i+1번째 bit를 맨 오른쪽(64번째) 자리까지 끌고옴
		} // expand permutation
		xor_result = ep_result ^ subkey[i]; // subkey와 xor연산
		for(int j=0; j< 8; j++){
			sBox_result <<= 4;
			sBoxtemp = (xor_result >> (42 - 6*j)) & 0x000000000000003f;
			//6개씩 분해
			row = ((sBoxtemp >> 5) << 1)|(sBoxtemp & 0x0000000000000001);
			column = (sBoxtemp >> 1) & 0x000000000000000f;
			sBox_result = sBox_result | ((uint32_t)sBox[j][row][column]);
		} // sbox
		for(int j=0; j < 32; j++){
			sp_result <<= 1; // 왼쪽으로 한 자리 옮김.
			sp_result = sp_result|((sBox_result >> (32-sp[j])) & 0x00000001);
		} // straight permutation
		
		temp = right;
		right = left ^ sp_result;
		left = temp; //switch 연산
		
		
	}//round 수행
	
	
	afterallround = ((uint64_t)right << 32) | (uint64_t)left;
	
	for(int i=0; i< 64; i++){
		ipinverse_result <<= 1;
		ipinverse_result = ipinverse_result|((afterallround >> (64-ipinverse[i])) & 0x0000000000000001);
	} // inverse initial permutation
	
	return ipinverse_result;
}

int main(){
	uint64_t input;
	uint64_t key;
	uint64_t output;

	
	printf("input of des : ");
	scanf("%016llx",&input);
	printf("\n");
	printf("input of key : ");
	scanf("%016llx",&key);
	printf("\n");


	output = des(input, key);
	printf("output of des : %016llx\n",output);
	
	return 0;
}
