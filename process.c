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
    int width;
    int height;
    struct Pixel *pixels;
};

/* Linked list for processing multiple files from the CLI */
struct ImageNode
{
    struct Image *img;
    char *input_file;
    char *output_file;
    int noise_strength;
    struct ImageNode *next;
};

/* Free a struct Image */
void free_image(struct Image *img)
{
    free(img->pixels);
    free(img);
}

/* Opens and reads an image file, returning a pointer to a new struct Image.
 * On error, prints an error message and returns NULL. */
struct Image *load_image(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    struct Image *img = malloc(sizeof(struct Image));
    if (img == NULL)
    {
        fclose(f);
        fprintf(stderr, "Memory allocation for %s failed \n", filename);
        return NULL;
    }

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

    img->width = width;
    img->height = height;

    img->pixels = malloc(sizeof(struct Pixel) * width * height);
    if (img->pixels == NULL)
    {
        fclose(f);
        free(img);
        fprintf(stderr, "Memory allocation for pixels of %s failed \n", filename);
    }

    size_t num_Pixels = width * height;
    size_t pixel_Data = fread(img->pixels, sizeof(struct Pixel), num_Pixels, f);
    if (pixel_Data != num_Pixels)
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
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing to \n");
        return false;
    }

    fprintf(file, "HS16 %d %d\n", img->width, img->height);

    size_t num_Pixels = img->width * img->height;
    size_t pixels_Written = fwrite(img->pixels, sizeof(struct Pixel), num_Pixels, file);
    if (pixels_Written != num_Pixels)
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

    copy->width = source->width;
    copy->height = source->height;

    copy->pixels = malloc(sizeof(struct Pixel) * source->width * source->height);
    if (copy->pixels == NULL)
    {
        fprintf(stderr, "Error allocating memory for pixels");
        free(copy);
        return NULL;
    }

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
struct Image *apply_NOISE(const struct Image *source, int noise_strength)
{
    struct Image *copy = copy_image(source);
    if (copy == NULL)
    {
        fprintf(stderr, "Error copying image to apply Noise");
        return NULL;
    }

    srand(time(NULL));

    for (int i = 0; i < source->width * source->height; i++)
    {
        int noise_Red = rand() % (2 * noise_strength + 1) - noise_strength;
        int noise_Green = rand() % (2 * noise_strength + 1) - noise_strength;
        int noise_Blue = rand() % (2 * noise_strength + 1) - noise_strength;

        int new_Red = copy->pixels[i].red + noise_Red;
        if (new_Red < 0)
        {
            new_Red = 0;
        }
        else if (new_Red > 65535)
        {
            new_Red = 65535;
        }

        int new_Green = copy->pixels[i].green + noise_Green;
        if (new_Green < 0)
        {
            new_Green = 0;
        }
        else if (new_Green > 65535)
        {
            new_Green = 65535;
        }

        int new_Blue = copy->pixels[i].blue + noise_Blue;
        if (new_Blue < 0)
        {
            new_Blue = 0;
        }
        else if (new_Blue > 65535)
        {
            new_Blue = 65535;
        }

        copy->pixels[i].red = (unsigned short)new_Red;
        copy->pixels[i].green = (unsigned short)new_Green;
        copy->pixels[i].blue = (unsigned short)new_Blue;
    }
    return copy;
}

/* Perform your second task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns true on success, or false on error. */
bool apply_CODE(const struct Image *source)
{
    if (source == NULL)
    {
        printf("Error reading file \n");
        return false;
    }

    printf("const in image_width = %d;\n", source->width);
    printf("const int image_height = %d;\n", source->height);
    printf("const struct Pixel image_data = {\n");

    printf("    ");
    for (int i = 0; i < source->width * source->height; i++)
    {
        printf("{%hu, %hu, %hu}", source->pixels[i].red, source->pixels[i].green, source->pixels[i].blue);

        if (i != source->width * source->height - 1)
        {
            printf(", ");
        }

        if ((i + 1) % 3 == 0)
        {
            printf("\n");
            printf("    ");
        }
    }

    printf("\n");
    printf("} \n");

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: process INPUTFILE OUTPUTFILE NOISE_STRENGTH\n");
        return 1;
    }

    struct Image *in_img = load_image(argv[1]);
    if (in_img == NULL)
    {
        return 1;
    }

    int noise_strength;
    if (sscanf(argv[3], "%d", &noise_strength) != 1)
    {
        fprintf(stderr, "Invalid noise strength: %s\n", argv[3]);
        free_image(in_img);
        return 1;
    }

    struct Image *out_img = apply_NOISE(in_img, noise_strength);
    if (out_img == NULL)
    {
        fprintf(stderr, "First process failed.\n");
        free_image(in_img);
        return 1;
    }

    if (!apply_CODE(out_img))
    {
        fprintf(stderr, "Second process failed.\n");
        free_image(in_img);
        free_image(out_img);
        return 1;
    }

    if (!save_image(out_img, argv[2]))
    {
        fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
        free_image(in_img);
        free_image(out_img);
        return 1;
    }

    free_image(in_img);
    free_image(out_img);
    return 0;
}
