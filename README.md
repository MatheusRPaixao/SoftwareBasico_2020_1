# SoftwareBasico_2020_1

## Author: 

    Matheus Romero Paixao

## Trabalho de Software Basico 2020.1

## Description:

    The current software has the objective to do a conversion of files from utf8 to utf32 and utf32 to utf8.
    It'll be made by reading a preexisting file, identify the type (utf8, utf32 big-endian or utf32 little-endian) of the
    inputed file and do the convertion .

## Convertions:

    UTF8   ----------------------------- UTF32 (little-endian)
    UTF32 (little-endian) -------------- UTF8
    UTF32 (big-endian) ----------------- UTF8

## Setup:

In the Terminal:

    cd [Path]/SoftwareBasico_2020_1     # go to SoftwareBasico_2020_1 folder in your computer

Then place all files to be converted at the folder "toConvert", after that, in the Terminal:
OBS: Already have some files at the folder as example 

    cd src                              #go to src folder 
    gcc -o exec main.c conv_utf.c       #crete a executable call exec

## Excution

Inside 'src' folder:
    
    ./exec                              #execute de executable

## Result 

After that all done, you must go in the "converted" folder and it should have all converted files 

Obs: the converted file have the same name of the original file in the "toConvert" folder
