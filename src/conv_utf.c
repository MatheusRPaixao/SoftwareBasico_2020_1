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
 * 1        08-04       0.0.1           Question comprehension and base code to develop the functions utf8_32 and utf32_8 
 * 2        10-04       0.0.2           Minors updates and Headers creation
 * 3        11-04       0.1.0           Minors updates and function utf8_32 implemented
 * 4        13-04       0.1.1           Minors updates and optmizations, readable code 
 * 5        14-04       0.2.0           Enum for Errors, add regions,
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "../headers/conv_utf.h" 

#define MAXPATHSIZE 1024
#define TRUE 1
#define FALSE 0

/* Global */

unsigned int BigBOOM = 0x0000feff ; 

/* Private methods */
//Criar enum para erros
/**
 *  Method to verify if given file is a 8UTF coded file
 *  
 *  Parameters:
 *      inFile - the input file to be checked
 * 
 *  Return:
 *  boolean result 
 *  if the file is a 8UTF type file return 1
 *  else 0  
 *  
 * */
int Is8Utf(FILE* inAuxFile); /* main */

/* add Description */
int IdentifyCharSize(unsigned char inbyte);

/* add Description */
unsigned int GetUnicodeFromUtf8(unsigned char inByte, unsigned char auxInByte[3], int n);

/* add Description */
unsigned int BigToLittle(unsigned int outByte);


/* ************** */

#pragma region Public Methods 

int utf32_8(FILE* inFile, FILE* outFile)
{
    puts("its in");
    return 0 ;
}



int utf8_32(FILE* inFile, FILE* outFile)
{
    unsigned char inByte;
    unsigned char auxInByte[3];
    unsigned int outByte;

    int tamInOutByte;
    unsigned int little =  BigToLittle(BigBOOM);

    /* Introduces BOOM Byte in Utf32 File */
    fwrite(&little, sizeof(little) , 1, outFile);

    while(fread(&inByte, sizeof(inByte), 1, inFile) != EOF)
    {
        if (tamInOutByte = IdentifyCharSize(inByte)) 
        { 
            if (!fread(auxInByte, tamInOutByte, 1, inFile))
            {
                perror("Fail to identify size of char ");
                exit(1);
            }
        }

        outByte = BigToLittle(GetUnicodeFromUtf8(inByte, auxInByte, tamInOutByte));

        fwrite(&outByte, tamInOutByte, 1, outFile);
    }

}

#pragma endregion


#pragma region Private Methods

int IdentifyCharSize(unsigned char inByte)
{
    unsigned char aux ;
    int ret = FALSE ;
    int loop = TRUE ;

    while (loop)
    {
        aux = inByte ;
        printf("%x, %x, %d\n", inByte,(aux >> (8-loop)), (aux >> (8-loop)) && 0x01 );
        if ((aux >> (8-loop)) && 0x01 )
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

    return ret ;
}

unsigned int GetUnicodeFromUtf8(unsigned char inByte, unsigned char auxInByte[3], int n)
{
    unsigned int ret = 0x00 ;
    unsigned char aux;
    int desloc = 6;
    int cont;

    ret += ((inByte<<(n+1))>>(8 - (n+1)));

    /* if n is positive then is a multiple byte char */
    if (n)
    {
        ret = ret<<desloc;

        for (cont = 0; cont < n; cont++)
        {
            aux = (auxInByte[cont]<<2)>>2;

            ret+=aux;
            ret=ret<<desloc;
        }
    }

    return ret;

}

unsigned int BigToLittle(unsigned int outByte)
{
    unsigned int ret = 0x00;
    unsigned int aux ;
    int cont, desloc = 4 ;

    for (cont = 0; cont < sizeof(outByte)*8; cont += desloc)
    {
        aux = outByte;
        aux = (aux >> cont) & 0x0f;

        ret += aux ;
        ret = ret << desloc;
    }
    
    return ret;

}

int Is8Utf(FILE* inAuxFile) /* main */
{
    unsigned int firstChar;

    fread(&firstChar, sizeof(firstChar), 1, inAuxFile);

    return firstChar != BigToLittle(BigBOOM) && firstChar != BigBOOM;
}

#pragma endregion


int main ()
{
    /* Variables */
    DIR* filesToConvert;
    struct dirent* inFileName;
    char cwd[MAXPATHSIZE];
    char inFilesDir[MAXPATHSIZE], outFilesDir[MAXPATHSIZE] ;
    char inFilePath[MAXPATHSIZE], outFilePath[MAXPATHSIZE] ; 
    FILE* inFile, *outFile, *inAuxFile;

    /* get current directory */
    getcwd(cwd, sizeof(cwd));

    /* defines origin of files to be converted and destination for the converted files */
    sprintf(inFilesDir, "%s/%s", cwd, "toConvert");
    sprintf(outFilesDir, "%s/%s", cwd, "converted");

    /*  list of all arq's to be converted */
    if (!(filesToConvert = opendir(inFilesDir)))
    {
        perror("Error open directory \"/toConvert\", please read the README.md file");
        exit(1);
    }

    /* loop through the files to be converted  */
    while ( ( inFileName = readdir(filesToConvert) ))
    {
        if (strcmp (inFileName->d_name, ".") && strcmp(inFileName->d_name, ".."))
        {
            /* Define path of input file and out file */
            sprintf(inFilePath, "%s/%s" ,inFilesDir,inFileName->d_name);
            sprintf(outFilePath, "%s/%s", outFilesDir,inFileName->d_name);

            /* Needed to verify BOOM char */
            inAuxFile = fopen(inFilePath, "rb");

            /* Open respective in and out files */
            inFile = fopen(inFilePath, "rb");
            outFile = fopen(outFilePath, "wb");

            if (Is8Utf(inAuxFile))
            {
                utf8_32(inFile, outFile);
            }
            else 
            {
                utf32_8(inFile,outFile); /* TODO */
            }

            /* Close respective in and out files */
            fclose(inFile);
            fclose(outFile);
            fclose(inAuxFile);

        }
    }

    /* Close directory */
    closedir(filesToConvert);

   
   
   
    /***
     * 
     *  
     * To make a UTF32 file use https://onlineutf8tools.com/convert-utf8-to-utf32 add the hexadecimal code at the var bytes and compile
        
        FILE* file;

        unsigned int bytes[] = {  https://onlineutf8tools.com/convert-utf32-to-utf8};

        unsigned int outBytes[26];
        int cont = 0;

        file = fopen("utf32.txt", "wb");
        fwrite(&bytes, sizeof(bytes),1, file);
        fclose(file);

        file = fopen("utf32.txt", "rb");
        fread(&outBytes, sizeof(outBytes),1, file);
        while(cont<26)
        {
            printf("%x\n",outBytes[cont++]);
        }
        fclose(file);

    *
    *
    *
    * */
    
}
