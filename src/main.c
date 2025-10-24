#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    struct winsize w;
    int old_row = 0, old_col = 0;

    int width, height, channels;
    char *black = "@";
    char *white = ".";
    char *path = "example/patern16x16.png";
    int widthForPrint = 0;
    
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // terminal size

    if (argc > 1 && strcmp(argv[1], "--space") == 0) {
        black = "@ ";
        white = ". ";
    }
    
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if (!data) {
        printf("Erreur de chargement : %s\n", stbi_failure_reason());
        return 1;
    }
    
    printf("\n");
    while (1) {
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
            perror("ioctl");
            return 1;
        }

        int width_ok = w.ws_col >= width;
        int height_ok = w.ws_row >= height + 1;

        printf("\rTerminal size: ");

        
        if (width_ok)
            printf("\033[32mWidth: %d\033[0m ", w.ws_col); // green
        else
            printf("\033[31mWidth: %d\033[0m ", w.ws_col); // red

        
        if (height_ok)
            printf("\033[32mHeight: %d\033[0m ", w.ws_row);
        else
            printf("\033[31mHeight: %d\033[0m ", w.ws_row);

        printf(" | Required: %dx%d", width, height+1); // height+1 for avoid cut by the prompt line (when the exec is finished)
        fflush(stdout);

        if (width_ok && height_ok)
            break;

        usleep(300000);
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
