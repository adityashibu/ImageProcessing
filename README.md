# Image processing in C

**Coursework by Aditya Shibu - H00432130, BSc Computer Science with specialization in AI**

## Overview of the Program

- **apply_NOISE:** Applies random noise to the Image, adding a random value to the RGB components of the Image.

- **apply_CODE:** reveals the source of the image in R,G,B format including the width and height, so you can copy the code on to another program if needed.

## Using makefile to compile the code

1. Make sure you're in the directory where the makefile is located.

2. To run the make file, run `make` in your terminal (Assuming that the makefile and the process are both located at the same directory)

3. Running the above command should create a new executable called **process** in the directory you're currently at.

4. If you want to know how to compile the program using GCC, follow the below steps.

## Compiling the program using GCC

1. Make sure you have (GNU C Compiler) installed on your machine
2. To compile the proces file, use:
   ```c
   gcc process.c -o process
   ```
3. To run process on a given image use:
   ```c
   ./process input_file output_file noise_strength
   ```
   For example:
   ```c
   ./process coffee.hs16 coffee_processed.hs16 10000
   ```
4. Running the above given command will create a file with the given output file name, which will be the processed image.

## Giving in multiple files to process

1. To pass in multiple input files to process, follow the below steps:
   - Pass in multiple files to process using the below given syntax:
     ```c
     ./process input_file1 output_file1 noise_strength1 input_file2 output_file2 noise_strength2 input_file3 output_file3 noise_strength3 ...
     ```
   - For example:
     ```c
     ./process coffee.hs16 coffee_processed.hs16 10000 music.hs16 music_processed.hs16 50000
     ```

## Note

- To view the hs16 formatted image, use **hsconvert**
- Use hsconvert like below:
  1. Ensure you have the hsconvert file on the machine
  2. Ensure you have a program or website to view PPM files, it's recommended to install the VSCode extension called **"PBM/PPM/PGM Viewer for Visual Studio Code"**
  3. After making sure you have both of the above, Run:
     ```c
     ./hsconvert -f PPM inputfile.hs16 outputfile.ppm
     ```
     For example:
     ```c
     ./hsconvert -f PPM coffee.hs16 coffee.ppm
     ```

## Additional Notes

- Memory Usage for the program, the program loads in all images into memory before processing. Ensure that
