#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/file_browser.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int nospace_flag = 0;
    int debug_flag = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--nospace") == 0)
            nospace_flag = 1;
        else if (strcmp(argv[i], "--debug") == 0)
            debug_flag = 1;
    }


    char *path = select_file();
    if (!path) {
        return 1;
    }

    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if (!data) {
        printf("Erreur de chargement : %s\n", stbi_failure_reason());
        free(path);
        return 1;
    }

    // Palette
    const char *grayscale = "@%#*+=:- .";
    int paletteSize = strlen(grayscale);
    float char_aspect = 2.0f;
    int effective_cols = w.ws_col;
    int effective_rows = w.ws_row - 1;

    if (!nospace_flag) {
        effective_cols /= 2;
    }

    float scaleX = (float)width / effective_cols;
    float scaleY = (float)height / (effective_rows * char_aspect);
    float scale = scaleX > scaleY ? scaleX : scaleY;

    int newWidth = width / scale;
    int newHeight = height / scale;
    if (newWidth < 1) {
        newWidth = 1;
    }
    if (newHeight < 1) {
        newHeight = 1;
    }

    int padding = (w.ws_col - (!nospace_flag ? newWidth * 2 : newWidth)) / 2;
    if (padding < 0) {
        padding = 0;
    }

    system("clear");
    // Draw
    for (int y = 0; y < newHeight; y++) {
        for (int p = 0; p < padding; p++) {
            putchar(' ');
        }
        for (int x = 0; x < newWidth; x++) {
            int srcX = x * scale;
            int srcY = y * scale;
            int i = (srcY * width + srcX) * channels;

            unsigned char r = data[i];
            unsigned char g = data[i+1];
            unsigned char b = data[i+2];
            unsigned char pixelValue = (r + g + b) / 3;
            unsigned char a = (channels == 4) ? data[i+3] : 255;

            char symbol;
            if (a == 0) {
                symbol = ' '; // transparent
            } else {
                int index = (pixelValue * (paletteSize - 1)) / 255;
                symbol = grayscale[index];
            }

            // Color Draw
            if (a == 0) {
                putchar(' '); // transparent
            } else {
                printf("\033[1;38;2;%d;%d;%dm%c\033[0m", r, g, b, symbol);
            }

            if (!nospace_flag) putchar(' ');
        }
        putchar('\n');
    }


    if (debug_flag) {
        printf("\n=== Pixel Details ===\n");
        for (int i = 0; i < width * height * channels; i += channels) {
            unsigned char r = data[i];
            unsigned char g = data[i + 1];
            unsigned char b = data[i + 2];
            unsigned char a = (channels == 4) ? data[i + 3] : 255;
    
            printf("Pixel %-5d → R=%3d G=%3d B=%3d A=%3d\n",
                   i / channels, r, g, b, a);
        }
    }

    stbi_image_free(data);
    free(path);
    return 0;
}
