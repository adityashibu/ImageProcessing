#include <stdio.h>
#include <stdbool.h>
#include "testprocess.c"

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
/*
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
*/

// Main to test copy_image
/*
int main()
{
    const char *filename = "coffee.hs16";

    // Load the original image
    struct Image *original = load_image(filename);
    if (original == NULL)
    {
        fprintf(stderr, "Failed to load the original image from file %s\n", filename);
        return 1;
    }

    // Create a copy of the original image
    struct Image *copy = copy_image(original);
    if (copy == NULL)
    {
        fprintf(stderr, "Failed to create a copy of the original image\n");
        free_image(original);
        return 1;
    }

    // Compare each pixel of the original image with the corresponding pixel in the copied image
    bool identical = true;
    for (int i = 0; i < original->width * original->height; i++)
    {
        if (original->pixels[i].red != copy->pixels[i].red ||
            original->pixels[i].green != copy->pixels[i].green ||
            original->pixels[i].blue != copy->pixels[i].blue)
        {
            identical = false;
            break;
        }
    }

    if (identical)
    {
        printf("The copy operation is successful. The original and copied images are identical.\n");
    }
    else
    {
        printf("The copy operation failed. The original and copied images are not identical.\n");
    }

    // Free memory allocated for the original and copied images
    free_image(original);
    free_image(copy);

    return 0;
}
*/

// Main to test apply_NOISE
int main()
{
    const char *input_filename = "coffee.hs16";
    const char *output_filename = "noisy_coffee.hs16";
    int noise_strength = 0; // Adjust noise strength as needed

    // Load the original image
    struct Image *original = load_image(input_filename);
    if (original == NULL)
    {
        fprintf(stderr, "Failed to load the original image.\n");
        return 1;
    }

    // Apply noise to the original image
    struct Image *noisy_image = apply_NOISE(original, noise_strength);
    if (noisy_image == NULL)
    {
        fprintf(stderr, "Failed to apply noise to the image.\n");
        free_image(original);
        return 1;
    }

    // Save the noisy image
    if (!save_image(noisy_image, output_filename))
    {
        fprintf(stderr, "Failed to save the noisy image.\n");
        free_image(original);
        free_image(noisy_image);
        return 1;
    }

    // Free memory
    free_image(original);
    free_image(noisy_image);

    printf("Noisy image saved successfully as %s.\n", output_filename);
    return 0;
}