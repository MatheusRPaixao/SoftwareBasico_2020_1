#include "../headers/conv_utf.h"

#define MAXPATHSIZE 512


#pragma region Private methods Declaration

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
int Is8Utf(FILE *inAuxFile); 

#pragma endregion

int main(void)
{
    /* Variables */
    DIR* filesToConvert;
    struct dirent *inFileName;
    char inFilesDir[MAXPATHSIZE], outFilesDir[MAXPATHSIZE];
    char inFilePath[MAXPATHSIZE], outFilePath[MAXPATHSIZE];
    FILE *inFile, *outFile, *inAuxFile;

    /* defines origin of files to be converted and destination for the converted files */
    sprintf(inFilesDir, "../%s", "toConvert");
    sprintf(outFilesDir, "../%s", "converted");

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
                utf32_8(inFile,outFile); 
            }

            /* Close respective in and out files */
            fclose(inFile);
            fclose(outFile);
            fclose(inAuxFile);

        }
    }
    /* Close directory */
    closedir(filesToConvert);
}
