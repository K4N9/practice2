#define _CRT_SECURE_NO_WARNINGS

#include"tables.h"
#include<stdio.h>
#include<stdint.h>

void initialPermutation(int plainText[64], int afterIp[64])
{
    for (int i = 0; i < 64; i++)
        afterIp[i] = plainText[initialPermutationTable[i]-1];
}

void splitText(int afterIp[64], int leftInput[32], int rightInput[32])
{
    for (int i = 0; i < 32; i++)
    {
        leftInput[i] = afterIp[i];
        rightInput[i] = afterIp[i + 32];
    }
}

void dropParity(int key[64], int afterDrop[56])
{
    for (int i = 0; i < 56; i++)
        afterDrop[i] = key[parityBitDropTable[i]-1];
}

void splitKey(int afterDrop[56], int keyLeft[28], int keyRight[28])
{
    for (int i = 0; i < 28; i++)
    {
        keyLeft[i] = afterDrop[i];
        keyRight[i] = afterDrop[i + 28];
    }
}

void generateNextKey(int currentKey[48], int keyLeft[28], int keyRight[28], int round, int isEncrypt)
{
    int temp[56];
    int tl, tr;
    if (isEncrypt)
    {
        for (int i = 0; i < leftShiftTable[round]; i++)
        {
            tl = keyLeft[27];
            tr = keyRight[27];
            for (int j = 0; j < 28; j++)
            {
                keyLeft[(28 + j - 1) % 28] = keyLeft[j];
                keyRight[(28 + j - 1) % 28] = keyRight[j];
            }
            keyLeft[26] = tl;
            keyRight[26] = tr;
        }
    }
    else if(round != 0)
    {
        for (int i = 0; i < leftShiftTable[15-round+1]; i++)
        {
            tl = keyLeft[27];
            tr = keyRight[27];
            for (int j = 27; j >= 0; j--)
            {
                keyLeft[j] = keyLeft[(j-1+28)%28];
                keyRight[j] = keyRight[(j-1+28)%28];
            }
            keyLeft[0] = tl;
            keyRight[0] = tr;
        }
    }
    for (int i = 0; i < 28; i++)
    {
        temp[i] = keyLeft[i];
        temp[i + 28] = keyRight[i];
    }

    for (int i = 0; i < 48; i++)
        currentKey[i] = temp[keyCompressionTable[i]-1];
}

void expansion(int rightInput[32], int afterExp[48])
{
    for (int i = 0; i < 48; i++)
        afterExp[i] = rightInput[expansionPermutationTable[i]-1];
}

void exclusiveOr(int afterExp[48], int currentKey[48], int afterXor[48])
{
    for (int i = 0; i < 48; i++)
    {
        afterXor[i] = afterExp[i] ^ currentKey[i];
    }
}

void sBoxFunc(int afterXor[48], int sboxOut[8][4])
{
    for (int i = 0; i < 8; i++)
    {
        int row = 2 * afterXor[0 + i * 6] + afterXor[5 + i * 6];
        int col = 0;
        for (int j = 0; j < 4; j++)
            col += afterXor[1 + j + i * 6]  << (3-j);
       

        int tmp = sBox[i][row][col];
        for (int j = 3; j >= 0; j--)
        {
            sboxOut[i][j] = tmp % 2;
            tmp /= 2;
        }
    }
}

void straightPermutation(int sBoxOut[8][4], int afterSp[32])
{
    int tmp[32];
    
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
            tmp[i * 4 + j] = sBoxOut[i][j];

    for (int i = 0; i < 32; i++)
        afterSp[i] = tmp[straightPermutationTable[i]-1];
}

void inverseInitialPermutaion(int cipherText[64], int leftInput[32], int rightInput[32])
{
    int tmp[64];
    for (int i = 0; i < 32; i++)
    {
        tmp[i] = leftInput[i];
        tmp[i + 32] = rightInput[i];
    }
    for (int i = 0; i < 64; i++)
        cipherText[i] = tmp[inverseInitialPermutaionTable[i]-1];
}

void des(int plainText[64], int key[64], int cipherText[64], int afterIp[64], int leftInput[32], int currentKey[48], int afterDrop[56]
    , int rightInput[32], int afterExp[48], int afterXor[48], int sboxOut[8][4], int afterSp[32], int keyLeft[28], int keyRight[28], int isEncrypt)
{
    initialPermutation(plainText, afterIp);
    splitText(afterIp, leftInput, rightInput);
    dropParity(key, afterDrop);
    splitKey(afterDrop, keyLeft, keyRight);
    for (int k = 0; k < 16; k++)
    {
        generateNextKey(currentKey, keyLeft, keyRight, k, isEncrypt);
        expansion(rightInput, afterExp);
        exclusiveOr(afterExp, currentKey, afterXor);
        sBoxFunc(afterXor, sboxOut);
        straightPermutation(sboxOut, afterSp);
        
        int tmp[32];
        for (int j = 0; j < 32; j++)
        {
            tmp[j] = rightInput[j];
            rightInput[j] = leftInput[j] ^ afterSp[j];
            leftInput[j] = tmp[j];
        }
    }
    inverseInitialPermutaion(cipherText, rightInput, leftInput);
}

void arrToHex(int plainText[], uint64_t* hexPlainTextInput, int size)
{
    uint64_t hex = 0;
    for (int i = 0; i < size/4; i++)
    {
        for (int j = 0; j < 4; j++)
            hex += plainText[i * 4 + j] << (3 - j);
        if(i!= size/4 - 1)
            hex <<= 4;
    }
    *hexPlainTextInput = hex;
}

void hexToArr(uint64_t hexPlainText, int plainText[])
{
    for (int i = 0; i < 64; i++)
    {
        plainText[63 - i] = hexPlainText % 2;
        hexPlainText /= 2;
    }
}

int main()
{
    uint64_t hexPlainText = 0x0123456789ABCDEF;
    uint64_t hexKey = 0x85E813540F0AB405;
    uint64_t hexCipherText = 0;
    int plainText[64];
    int afterIp[64];
    int leftInput[32];
    int rightInput[32];
    int afterExp[48];
    int afterXor[48];
    int sboxOut[8][4];
    int afterSp[32];
    int cipherText[64];
    int key[64];
    int afterDrop[56];
    int keyLeft[28];
    int keyRight[28];
    int currentKey[48];

    hexToArr(hexPlainText, plainText);
    hexToArr(hexKey, key);

    //Encryption
    des(plainText, key, cipherText, afterIp, leftInput, currentKey, afterDrop
        , rightInput, afterExp, afterXor, sboxOut, afterSp, keyLeft, keyRight, 1);

    arrToHex(cipherText, &hexCipherText, 64);
    printf("Encryption : %016llx\n", hexCipherText);

    //Decryption
    des(cipherText, key, plainText, afterIp, leftInput, currentKey, afterDrop
        , rightInput, afterExp, afterXor, sboxOut, afterSp, keyLeft, keyRight, 0);

    arrToHex(plainText, &hexPlainText, 64);
    printf("Decryption : %016llx", hexPlainText);

    return 0;
}