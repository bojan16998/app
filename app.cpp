#include <cstdint>
#include <jpeglib.h>
#include <stdio.h>
#include <stdlib.h>

void write_JPEG_file(const char *filename, uint16_t *image, int width, int height) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE *outfile;
    JSAMPROW row_pointer[1];
    int row_stride;

    if ((outfile = fopen(filename, "wb")) == nullptr) {
        fprintf(stderr, "Can't open %s\n", filename);
        exit(EXIT_FAILURE);
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3; // Broj komponenti po pikselu (RGB)
    cinfo.in_color_space = JCS_RGB; // Boja slike

    jpeg_set_defaults(&cinfo);
    jpeg_start_compress(&cinfo, TRUE);

    row_stride = width * 3;

    while (cinfo.next_scanline < cinfo.image_height) {
        // Prikupite podatke za red slike iz niza uint16_t
        row_pointer[0] = reinterpret_cast<JSAMPROW>(&image[cinfo.next_scanline * width]);
        
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}

int main() {
    const int width = 640;
    const int height = 630;

    // Ovde unesite svoj niz uint16_t sa slikom
    uint16_t *image = new uint16_t[width * height * 3];

    // Postavite vrednosti u nizu image prema potrebama

    // Pisanje slike u JPEG format
    write_JPEG_file("output.jpg", image, width, height);

    delete[] image;

    return 0;
}
