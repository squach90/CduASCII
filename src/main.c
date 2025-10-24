#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/file_browser.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int space_flag = 0;
    if (argc > 1 && strcmp(argv[1], "--space") == 0)
        space_flag = 1;

    char *path = select_file();
    if (!path) return 1;

    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if (!data) {
        printf("Erreur de chargement : %s\n", stbi_failure_reason());
        free(path);
        return 1;
    }

    const char *grayscale = "@%#*+=:-. ";
    int paletteSize = strlen(grayscale);

    float scaleX = (float)width / w.ws_col;
    float scaleY = (float)height / (w.ws_row - 1);
    float scale = scaleX > scaleY ? scaleX : scaleY;

    int newWidth = width / scale;
    int newHeight = height / scale;

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int srcX = x * scale;
            int srcY = y * scale;
            int i = (srcY * width + srcX) * channels;
            unsigned char pixelValue = (data[i] + data[i+1] + data[i+2]) / 3;
            int index = (pixelValue * (paletteSize - 1)) / 255;
            char symbol = grayscale[index];
            putchar(symbol);
            if (space_flag) putchar(' ');
        }
        putchar('\n');
    }

    stbi_image_free(data);
    free(path);
    return 0;
}
