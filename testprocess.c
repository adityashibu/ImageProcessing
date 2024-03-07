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
        int noiseRed = rand() % (2 * noise_strength + 1) - noise_strength;
        int noiseGreen = rand() % (2 * noise_strength + 1) - noise_strength;
        int noiseBlue = rand() % (2 * noise_strength + 1) - noise_strength;

        int newRed = copy->pixels[i].red + noiseRed;
        if (newRed < 0)
        {
            newRed = 0;
        }
        else if (newRed > 65535)
        {
            newRed = 65535;
        }

        int newGreen = copy->pixels[i].green + noiseGreen;
        if (newGreen < 0)
        {
            newGreen = 0;
        }
        else if (newGreen > 65535)
        {
            newGreen = 65535;
        }

        int newBlue = copy->pixels[i].blue + noiseBlue;
        if (newBlue < 0)
        {
            newBlue = 0;
        }
        else if (newBlue > 65535)
        {
            newBlue = 65535;
        }

        copy->pixels[i].red = (unsigned short)newRed;
        copy->pixels[i].green = (unsigned short)newGreen;
        copy->pixels[i].blue = (unsigned short)newBlue;
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

// Int main without linked lists and arrays

/*
int main(int argc, char *argv[])
{
    if ((argc - 1) % 3 != 0 || argc < 4)
    {
        fprintf(stderr, "Usage: process INPUTFILE1 OUTPUTFILE1 NOISE_STRENGTH1 INPUTFILE2 OUTPUTFILE2 NOISE_STRENGTH2 ...\n");
        return 1;
    }

    int pairs = (argc - 1) / 3;

    for (int i = 0; i < pairs; i++)
    {
        char *input_file = argv[i * 3 + 1];
        char *output_file = argv[i * 3 + 2];
        int noise_strength;

        if (sscanf(argv[i * 3 + 3], "%d", &noise_strength) != 1)
        {
            fprintf(stderr, "Invalid noise strength: %s\n", argv[i * 3 + 3]);
            return 1;
        }

        // Load input image
        struct Image *in_img = load_image(input_file);
        if (in_img == NULL)
        {
            fprintf(stderr, "Error loading in the input image");
            return 1;
        }

        // Apply noise to the input image
        struct Image *out_img = apply_NOISE(in_img, noise_strength);
        if (out_img == NULL)
        {
            fprintf(stderr, "Error applying noise to the loaded image");
            return 1;
        }

        // Apply code to the input image
        if (!apply_CODE(out_img))
        {
            fprintf(stderr, "Error applying code to the loaded image");
            free_image(in_img);
            free_image(out_img);
            return 1;
        }

        // Save the output image
        if (!save_image(out_img, output_file))
        {
            fprintf(stderr, "Failed to save image to %s\n", output_file);
            free_image(in_img);
            free_image(out_img);
            return 1=-
        }

        free_image(in_img);
        free_image(out_img);
    }

    return 0;
}
*/

// Int main using linked list
int main(int argc, char *argv[])
{
    if ((argc - 1) % 3 != 0 || argc < 4)
    {
        fprintf(stderr, "Usage: ./process inputfile1 outputfile1 noise_strength1 inputfile2 outputfile2 noise_strength2 ...\n");
        return 1;
    }

    int pairs = (argc - 1) / 3;

    // Initialize the linked list head
    struct ImageNode *head = NULL;
    struct ImageNode *current = NULL;

    // Populate the linked list with image data
    for (int i = 0; i < pairs; i++)
    {
        char *input_file = argv[i * 3 + 1];
        char *output_file = argv[i * 3 + 2];
        int noise_strength;

        if (sscanf(argv[i * 3 + 3], "%d", &noise_strength) != 1)
        {
            fprintf(stderr, "Invalid noise strength: %s\n", argv[i * 3 + 3]);
            return 1;
        }

        // Create a new node for the linked list
        struct ImageNode *new_node = malloc(sizeof(struct ImageNode));
        if (new_node == NULL)
        {
            fprintf(stderr, "Memory allocation failed for new node\n");
            return 1;
        }

        // Load input image
        struct Image *in_img = load_image(input_file);
        if (in_img == NULL)
        {
            fprintf(stderr, "Error loading in the input image: %s\n", input_file);
            return 1;
        }

        // Populate the node with image data
        new_node->img = in_img;
        new_node->input_file = input_file;
        new_node->output_file = output_file;
        new_node->noise_strength = noise_strength;
        new_node->next = NULL;

        // Add the node to the linked list
        if (head == NULL)
        {
            head = new_node;
            current = new_node;
        }
        else
        {
            current->next = new_node;
            current = new_node;
        }
    }

    // Process all images in the linked list
    current = head;
    while (current != NULL)
    {
        struct Image *out_img = apply_NOISE(current->img, current->noise_strength);
        if (out_img == NULL)
        {
            fprintf(stderr, "Error applying noise to the loaded image\n");
            return 1;
        }

        if (!apply_CODE(out_img))
        {
            fprintf(stderr, "Error applying code to the loaded image\n");
            free_image(out_img);
            return 1;
        }

        if (!save_image(out_img, current->output_file))
        {
            fprintf(stderr, "Failed to save image to %s\n", current->output_file);
            free_image(out_img);
            return 1;
        }

        free_image(out_img);

        // Move to the next node in the linked list
        current = current->next;
    }

    // Free the linked list
    current = head;
    while (current != NULL)
    {
        struct ImageNode *temp = current;
        current = current->next;
        free_image(temp->img);
        free(temp);
    }

    return 0;
}