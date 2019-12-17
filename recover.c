#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool jpg_found(unsigned char chunk[]);

int main(int argc, char *argv[])
{
    // ensures proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // stores the name from the card
    char *infile = argv[1];

    //opens card
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not read file %s\n.", infile);
        return 2;
    }

    // buffer to store chunks of 512 bytes
    unsigned int *chunk;
    chunk = malloc(sizeof(int)*128);


    // buffer for sprintf to create the custom name on the recovered JPG files
    char filename[8];

    // counter to keep the count on the number of JPGs created
    int counter = 0;
    //reads the card until a JPG header is found

    while (fread(&chunk, sizeof(chunk), 1, inptr))
    {
        if (chunk[0] == 0xff && chunk[1] == 0xd8 && chunk[2] == 0xff && (chunk[3] & 0xf0) == 0xe0)
        {
            sprintf(filename, "%03i.jpg", counter);
            FILE *img = fopen(filename, "w");
            counter++;
            do
            {
                fwrite(&chunk, sizeof(chunk), 1, img);
            }
            while (fread(&chunk, sizeof(chunk), 1, inptr) && !(chunk[0] == 0xff && chunk[1] == 0xd8 && chunk[2] == 0xff && (chunk[3] & 0xf0) == 0xe0));
            fclose(img);
            fseek(inptr, -512, SEEK_CUR);
        }
    }

    //success
    free(chunk);
    fclose(inptr);
}