#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include <stdio.h>

int main(int argc, char **argv) {
    int width, height, channels;
    char *black = "@";
    char *white = ".";
    char *path = "example/patern16x16.png";
    int widthForPrint = 0;

    if (argc > 1 && strcmp(argv[1], "--space") == 0) {
        black = "@ ";
        white = ". ";
    }

    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if (!data) {
        printf("Erreur de chargement : %s\n", stbi_failure_reason());
        return 1;
    }

    printf("Height: %d \n", height);
    printf("Width: %d \n", width);
    printf("Channels: %d \n", channels);
    printf("First pixel: %d \n", data[0]);

    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char pixelValue = (data[i] + data[i+1] + data[i+2]) / 3;

        if (pixelValue == 255) {
            printf("%s", black);
        } else if (pixelValue == 0) {
            printf("%s", white);
        } else {
            printf("%d", pixelValue);
        }

        widthForPrint++;
        if (widthForPrint >= width) {
            printf("\n");
            widthForPrint = 0;
        }
    }


    stbi_image_free(data); // free memory
    return 0;
}
