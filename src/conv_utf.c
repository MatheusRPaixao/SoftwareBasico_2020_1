/**
 * Author : Matheus Romero Paixao
 * Trabalho de Software Basico 2020.1
 * 
 * Description :
 * The current file has the objective to do a conversion of files from utf8 to utf32 and utf32 to utf8.
 * It'll be made by reading a preexisting file , identify the type (utf8, utf32 big-endian or utf32 little-endian) of the
 * inputed file and converting to another type. 
 * 
 * Version control:
 * Id       Day         Version         Description
 * 1        08-04       0.0.1           Question comprehension and did the base coding to develop the functions utf8_32 and utf32_8 
 * 2        10-04       0.0.2           Minors updates and Headers creation
 * 3        11-04       0.1.3           Minors updates and function utf8_32 implemented
 * 4        13-04       0.1.4           Minors updates and optmizations, readable code 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "conv_utf.h"

#define MAXPATHSIZE 1024

/* Private metods */

/**
 *  Function to verify if is a 8UTF file
 *  
 *  Parameters:
 *      inFile - the input file to be checked
 * 
 *  Return:
 *  boolean-ish result 
 *  if the file is a 8UTF type file return 1
 *  else 0  
 *  
 * */
int Is8utf(FILE* inFile); /* main */

/* add Description */
int IdentifyCharSize(unsigned char inbyte);

/* add Description */
unsigned long GetUnicodeFromUtf8(unsigned char inByte, unsigned char auxInByte[3], int n);


/* ************** */



int utf8_32(FILE* inFile, FILE* outFile)
{
    unsigned char inByte;
    unsigned char auxInByte[3];
    unsigned long outByte;

    int numBytesToRead;

    while(fread(inByte, sizeof(inByte), 1, inFile))
    {
        if (numBytesToRead = IdentifyCharSize(inByte)) 
        { 
            if (!fread(auxInByte, numBytesToRead, 1, inFile))
            {
                puts('Fail to identify size of char ');
                exit(0);
            }
        }

        outByte = GetUnicodeFromUtf8(inByte, auxInByte, numBytesToRead);
        /* tranformar para unsigned long  */
        /* retornar 1  */

        if (!fwrite(outByte, sizeof(outByte), 1, outFile))
        {
            puts('Error writting at new file ');
            exit(0);
        }
    }

}


int IdentifyCharSize(unsigned char inByte)
{
    unsigned char aux ;
    int ret = 0;
    int loop = 1;

    while (loop)
    {
        aux = inByte ;
        if ((aux >> (8-loop)) && 0x01 )
        {
            ret++;
            loop++;
        }
        else
        {
            loop = 0;
        }
        
    }

    return ret ;
}

unsigned long GetUnicodeFromUtf8(unsigned char inByte, unsigned char auxInByte[3], int n)
{
    unsigned long ret;
    unsigned char aux;
    int desloc = 6;
    int cont;

    ret += ((inByte<<(n+1))>>(8 - (n+1)));
    ret = ret<<desloc;

    for (cont = 0; cont < n; cont++)
    {
        aux = (auxInByte[cont]<<2)>>2;

        ret+=aux;
        ret=ret<<desloc;
    }
    return ret;



}




int main ()
{
    /* Variables */
    DIR* filesToConvert;
    struct dirent* inFileName;
    char cwd[MAXPATHSIZE];
    char inFilesDir[MAXPATHSIZE], outFilesDir[MAXPATHSIZE] ;
    char inFilePath[MAXPATHSIZE], outFilePath[MAXPATHSIZE]; 
    FILE* inFile, *outFile;

    /* get current directory */
    getcwd(cwd, sizeof(cwd));

    /* defines origin of files to be converted and destination for the converted files */
    sprintf(inFilesDir, "%s/%s", cwd, "toConvert");
    sprintf(outFilesDir, "%s/%s", cwd,"converted");

    /*  list of all arq's to be converted */
    if (!(filesToConvert = opendir(inFilesDir)))
    {
        printf("Fail to open directory %s , please read the README file\n", inFilePath);
        exit(0);
    }

    /* loop through the files to be converted  */
    while ( ( inFileName = readdir(filesToConvert) ))
    {
        if (strcmp (inFileName->d_name, ".") && strcmp(inFileName->d_name, ".."))
        {
            /* Define path of input file and out file */
            sprintf(inFilePath, "%s/%s" ,inFilesDir,inFileName->d_name);
            sprintf(outFilePath, "%s/%s", outFilesDir,inFileName->d_name);

            /* Open respective in and out files */
            inFile = fopen(inFilePath, "rb");
            outFile = fopen(outFilePath, "wb");

            if (Is8utf(inFile))
            {
                utf8_32(inFile, outFile);
            }
            else 
            {
                utf32_8(inFile,outFile);
            }

            /* Close respective in and out files */
            fclose(inFile);
            fclose(outFile);

        }
    }

    /* Close directory */
    closedir(filesToConvert);

    return 0 ;
}
