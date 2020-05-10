/**
 * Version control:
 * Id       Day         Version         Description
 * 1        08-04       0.0.1           Question comprehension and base code to develop the functions utf8_32 and utf32_8 
 * 2        10-04       0.0.2           Minors updates and Headers creation
 * 3        11-04       0.1.0           Minors updates and function utf8_32 implemented
 * 4        13-04       0.1.1           Minors updates and optmizations, readable code 
 * 5        14-04       0.2.0           Enum for Errors and add regions
 * 6        01-05       0.3.0           Conversion utf8 -> utf32 - OK
 * 7        02-05       0.4.0           Conversion utf32 -> utf8 - OK
 * 8        10-05       1.0.0           Finalization and README
 */

#include "../headers/conv_utf.h"

#define TRUE 1
#define FALSE 0

/* Global variable*/

unsigned int BigBOOM = 0x0000feff;

#pragma region Private methods declaration

/**
 *  Identify the size of a character UTF8
 *  
 *  Parameters:
 *      inByte - first byte of an UTF8 character 
 * 
 *  Return:
 *      boolean result 
 */
int IdentifyCharSize(unsigned char inbyte);

/**
 *  Parameters:
 *      inByte      - first byte of an UTF8 character
 *      auxInByte   - 3 remains btyes of the UTF8 character
 *      n           - size of bytes of the character (1,2,3,4)
 * 
 *  Return:
 *      the unicode from the character 
 */
unsigned int GetUnicodeFromUtf8(unsigned char inByte, unsigned char auxInByte[3], int n);

/**
 *  Parameters:
 *      inByte      - BOOM byte
 * 
 *  Return:
 *      boolean result 
 */
int IsLittle(unsigned int inByte);

/**
 *  Parameters:
 *      inByte      - character of UTF32 format
 *      outByte     - adress to be fill in with utf8 charcter
 *      size        - size of the outByte output
 * 
 */
void GetUf8FromUnicode(unsigned int inByte, unsigned char *outByte, int size);

/**
 *  Parameters:
 *      outByte     - unsigned int to be converted
 * 
 *  Return:
 *      the outByte with inverted edian (if little -> big) 
 * 
 */
unsigned int InvertEdian(unsigned int outByte);

/**
 *  Parameters:
 *      inByte      - unsigned int in UTF32 format
 * 
 *  Return:
 *      size of the UTF8 character
 * 
 */
int SetUtf8Size(unsigned int inByte);

/**
 *  Parameters:
 *      outByte     - array to be reset
 * 
 */
void ResetOut(unsigned char *outByte);

#pragma endregion

#pragma region Public Methods

int utf32_8(FILE *inFile, FILE *outFile)
{
    unsigned int inByte;

    unsigned char outByte[4];

    unsigned int isLittle;

    int tamInOutByte;

    fread(&inByte, sizeof(inByte), 1, inFile);
    isLittle = IsLittle(inByte);

    while (fread(&inByte, sizeof(inByte), 1, inFile))
    {
        if (isLittle)
        {
            inByte = InvertEdian(inByte);
        }

        tamInOutByte = SetUtf8Size(inByte);

        GetUf8FromUnicode(inByte, outByte, tamInOutByte);

        fwrite(outByte, tamInOutByte + 1, 1, outFile);
    }

    return 0;
}

int utf8_32(FILE *inFile, FILE *outFile)
{
    unsigned char inByte;
    unsigned char auxInByte[3];
    unsigned int outByte;
    int tamInOutByte;
    unsigned int little = InvertEdian(BigBOOM);

    /* Introduces BOOM Byte in Utf32 File */
    fwrite(&little, sizeof(little), 1, outFile);

    while (fread(&inByte, sizeof(inByte), 1, inFile))
    {
        if (tamInOutByte = IdentifyCharSize(inByte))
        {
            if (!fread(auxInByte, tamInOutByte-1, 1, inFile))
            {
                perror("Fail to identify size of char ");
                exit(1);
            }
        }
        outByte = GetUnicodeFromUtf8(inByte, auxInByte, tamInOutByte);
        outByte = InvertEdian(outByte);

        fwrite(&outByte, sizeof(outByte), 1, outFile);
    }
}

#pragma endregion

#pragma region Private Methods

int IdentifyCharSize(unsigned char inByte)
{
    unsigned char aux;
    int ret = FALSE;
    int loop = TRUE;
    while (loop)
    {
        aux = inByte;
        if ((aux >> (8 - loop)) & 0x01)
        {
            ret++;
            loop++;
        }
        else
        {
            break;
        }

        if (loop > 5)
        {
            perror("Erro to identify given byte");
            exit(1);
        }
    }

    return ret;
}

unsigned int GetUnicodeFromUtf8(unsigned char inByte, unsigned char auxInByte[3], int n)
{
    unsigned int ret = 0x00;
    unsigned int aux;
    unsigned char a;
    int desloc = 6;
    int cont;

    ret += ((inByte << (n + 1)) >> (n + 1));

    /* if n is positive then is a multiple byte char */
    if (n>0)
    {
        a = inByte << (n+1);
        a = a >> (n+1);
        ret = a;
        for (cont = 0; cont < n-1; cont++)
        {
            ret = ret << 6;
            aux = auxInByte[cont] & 0x3f;
            ret = ret + aux;
        }
    }

    return ret;
}

unsigned int InvertEdian(unsigned int outByte)
{
    unsigned int ret = 0x00;
    unsigned int aux;
    int cont, desloc = 8;

    for (cont = 0; cont < 4; cont++)
    {
        aux = outByte;
        aux = (aux >> (cont * desloc)) & 0xff;

        ret += aux;
        if (cont != 3)
        {
            ret = ret << desloc;
        }
    }

    return ret;
}

int Is8Utf(FILE *inAuxFile) /* main */
{
    unsigned int firstChar;

    fread(&firstChar, sizeof(firstChar), 1, inAuxFile);

    return firstChar != InvertEdian(BigBOOM) && firstChar != BigBOOM;
}

int SetUtf8Size(unsigned int inByte)
{
    int ret = 0;

    if (inByte > 0x7f)
    {
        ret += 1;

        if (inByte > 0x7ff)
        {
            ret += 1;

            if (inByte > 0xffff)
            {
                ret += 1;
            }
        }
    }

    return ret;
}

void GetUf8FromUnicode(unsigned int inByte, unsigned char *outByte, int size)
{

    ResetOut(outByte);

    if (size == 1)
    {
        outByte[1] = 0x80 + (inByte & 0x3f);
        outByte[0] = 0xc0 + (inByte >> 6);
    }
    else if (size == 2)
    {
        outByte[2] = 0x80 + (inByte & 0x3f);
        outByte[1] = 0x80 + ((inByte >> 6) & 0x3f);
        outByte[0] = 0xe0 + (inByte >> 12);
    }
    else if (size == 3)
    {
        outByte[3] = 0x80 + (inByte & 0x3f);
        outByte[2] = 0x80 + ((inByte >> 6) & 0x3f);
        outByte[1] = 0x80 + ((inByte >> 12) & 0x3f);
        outByte[0] = 0xf0 + (inByte >> 18);
    }
    else
    {
        outByte[0] = inByte;
    }
}

void ResetOut(unsigned char *outByte)
{
    int cont;
    for (cont = 0; cont < 4; cont++)
    {
        outByte[cont] = 0x00;
    }
}

int IsLittle(unsigned int inByte)
{
    int ret = TRUE;
    if (inByte == BigBOOM)
    {
        ret = FALSE;
    }

    return ret;
}

#pragma endregion