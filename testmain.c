#include <stdio.h>
#include "process.c"

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