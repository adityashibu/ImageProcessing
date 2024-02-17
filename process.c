/* This coursework specification, and the example code provided during the
 * course, is Copyright 2024 Heriot-Watt University.
 * Distributing this coursework specification or your solution to it outside
 * the university is academic misconduct and a violation of copyright law. */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel
{
    unsigned short red;
    unsigned short green;
    unsigned short blue;
};

/* An image loaded from a file. */
struct Image
{
    /* TODO: Question 1 */
    // Store the dimensions of the Image
    int width;
    int height;
    // Pointer pixels of type struct Pixel, used to dynamically allocate an array of Pixel structs
    struct Pixel *pixels;
};

/* Free a struct Image */
void free_image(struct Image *img)
{
    // Free the dynamically allocated array of pixel structs
    free(img->pixels);
    // Free the image itself
    free(img);
}

/* Opens and reads an image file, returning a pointer to a new struct Image.
 * On error, prints an error message and returns NULL. */
struct Image *load_image(const char *filename)
{
    /* Open the file for reading */
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    /* Allocate the Image object, and read the image from the file */
    /* TODO: Question 2b */
    struct Image *img = malloc(sizeof(struct Image));
    if (img == NULL)
    {
        fclose(f);
        fprintf(stderr, "Memory allocation for %s failed \n", filename);
        return NULL;
    }

    // Reading header
    char header[5];
    int width, height;
    if (fscanf(f, "%4s %d %d\n", header, &width, &height) != 3)
    {
        fclose(f);
        free(img);
        fprintf(stderr, "Errror reading header of %s \n", filename);
        return NULL;
    }
    else if (strcmp(header, "HS16") != 0)
    {
        fclose(f);
        free(img);
        fprintf(stderr, "Errror in header format of %s \n", filename);
        return NULL;
    }

    // Set image dimensions
    img->width = width;
    img->height = height;

    // Allocate memory for the pixels
    img->pixels = malloc(sizeof(struct Pixel) * width * height);
    if (img->pixels == NULL)
    {
        fclose(f);
        free(img);
        fprintf(stderr, "Memory allocation for pixels of %s failed \n", filename);
    }

    // Read pixel data
    size_t numPixels = width * height;
    size_t pixelData = fread(img->pixels, sizeof(struct Pixel), numPixels, f);
    if (pixelData != numPixels)
    {
        fclose(f);
        free_image(img);
        fprintf(stderr, "Error reading pixel data of %s \n", filename);
        return NULL;
    }

    fclose(f);

    return img;
}

/* Write img to file filename. Return true on success, false on error. */
bool save_image(const struct Image *img, const char *filename)
{
    // Open the file to write to
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing to \n");
        return false;
    }

    // Write the header
    fprintf(file, "HS16 %d %d\n", img->width, img->height);

    // Write pixel data
    size_t numPixels = img->width * img->height;
    size_t pixelsWritten = fwrite(img->pixels, sizeof(struct Pixel), numPixels, file);
    if (pixelsWritten != numPixels)
    {
        fprintf(stderr, "Error writing pixel data to %s \n", filename);
        return false;
    }

    fclose(file);
    return true;
}

/* Allocate a new struct Image and copy an existing struct Image's contents
 * into it. On error, returns NULL. */
struct Image *copy_image(const struct Image *source)
{
    struct Image *copy = malloc(sizeof(struct Image));
    if (copy == NULL)
    {
        fprintf(stderr, "Memory allocation failed for the copied image \n");
        return NULL;
    }

    // Copy the image dimensions
    copy->width = source->width;
    copy->height = source->height;

    // Allocate memory for the pixels
    copy->pixels = malloc(sizeof(struct Pixel) * source->width * source->height);
    if (copy->pixels == NULL)
    {
        fprintf(stderr, "Error allocating memory for pixels");
        free(copy);
        return NULL;
    }

    // Use a loop to copy pixel data from source to copy
    for (int i = 0; i < source->width * source->height; i++)
    {
        copy->pixels[i] = source->pixels[i];
    }

    return copy;
}

/* Perform your first task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns a new struct Image containing the result, or NULL on error. */
struct Image *apply_BLUR(const struct Image *source)
{
    /* TODO: Question 3 */
    return NULL;
}

/* Perform your second task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns true on success, or false on error. */
bool apply_NORM(const struct Image *source)
{
    /* TODO: Question 4 */
    return false;
}

// int main(int argc, char *argv[])
// {
//     /* Initialise the random number generator, using the time as the seed */
//     /* TODO: Remove this if you don't need to use rand() */
//     srand(time(NULL));

//     /* Check command-line arguments */
//     if (argc != 3)
//     {
//         fprintf(stderr, "Usage: process INPUTFILE OUTPUTFILE\n");
//         return 1;
//     }

//     /* Load the input image */
//     struct Image *in_img = load_image(argv[1]);
//     if (in_img == NULL)
//     {
//         return 1;
//     }

//     /* Apply the first process */
//     struct Image *out_img = apply_BLUR(in_img);
//     if (out_img == NULL)
//     {
//         fprintf(stderr, "First process failed.\n");
//         free_image(in_img);
//         return 1;
//     }

//     /* Apply the second process */
//     if (!apply_NORM(out_img))
//     {
//         fprintf(stderr, "Second process failed.\n");
//         free_image(in_img);
//         free_image(out_img);
//         return 1;
//     }

//     /* Save the output image */
//     if (!save_image(out_img, argv[2]))
//     {
//         fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
//         free_image(in_img);
//         free_image(out_img);
//         return 1;
//     }

//     free_image(in_img);
//     free_image(out_img);
//     return 0;
// }
