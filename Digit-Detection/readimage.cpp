#include <iostream>
using namespace std;

unsigned char *readBMP(char *filename)
{
    int i;
    FILE *f = fopen(filename, "rb");
    unsigned char info[54];

    // read the 54-byte header
    fread(info, sizeof(unsigned char), 54, f);

    // extract image height and width from header
    int width = *(int *)&info[18];
    int height = *(int *)&info[22];

    // allocate 3 bytes per pixel
    int size = 3 * width * height;
    unsigned char *data = new unsigned char[size];

    // read the rest of the data at once
    fread(data, sizeof(unsigned char), size, f);
    fclose(f);

    for (i = 0; i < size; i += 3)
    {
        // flip the order of every 3 bytes
        unsigned char tmp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = tmp;
    }

    return data;
}
int main()
{
    // unsigned char *data;
    // char *filename =
    // data = readBMP("./tmp/p11.bmp");
    // cout << data << endl;
}