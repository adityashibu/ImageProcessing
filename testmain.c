#include <stdio.h>
#include "process.c"

// Main to test load_image
/*
int main()
 {
     const char *filename = "bars.hs16";
     struct Image *img = load_image(filename);

     if (img == NULL)
     {
         printf("Failed to load image from file %s\n", filename);
         return 1;
     }

     printf("Image loaded successfully with dimensions: %d x %d\n", img->width, img->height);

     free_image(img);
     return 0;
 }
*/

// Main to test save_image
int main()
{
    const char *input_filename = "coffee.hs16";
    const char *output_filename = "coffee_output.hs16";

    // Load the image
    struct Image *image = load_image(input_filename);
    if (image == NULL)
    {
        fprintf(stderr, "Failed to load the input image.\n");
        return 1;
    }

    // Modify the image if needed (optional)

    // Save the modified image
    if (save_image(image, output_filename))
    {
        printf("Image saved successfully.\n");
    }
    else
    {
        fprintf(stderr, "Failed to save the image.\n");
        return 1;
    }

    // Free memory allocated for the image
    free_image(image);

    return 0;
}