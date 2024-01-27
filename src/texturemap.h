#ifndef RAYCASTING_TEXTUREMAP_H
#define RAYCASTING_TEXTUREMAP_H

#include <vector>

#define texWidth 64
#define texHeight 64

class TextureMap {
private:
    std::vector<size_t> textures[8];

    void generate() {
        for (auto &texture: textures) {
            texture.resize(texWidth * texHeight);
        }

        for (int x = 0; x < texWidth; x++)
            for (int y = 0; y < texHeight; y++) {
                int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
                //int xcolor = x * 256 / texWidth;
                int ycolor = y * 256 / texHeight;
                int xycolor = y * 128 / texHeight + x * 128 / texWidth;
                textures[0][texWidth * y + x] =
                        65536 * 254 * (x != y && x != texWidth - y); //flat red texture with black cross
                textures[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
                textures[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
                textures[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
                textures[4][texWidth * y + x] = 256 * xorcolor; //xor green
                textures[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
                textures[6][texWidth * y + x] = 65536 * ycolor; //red gradient
                textures[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
            }
    }

public:
    TextureMap() {
        generate();
    }

    std::vector<size_t>& operator[](int index) {
        return textures[index];
    }
};


#endif //RAYCASTING_TEXTUREMAP_H
