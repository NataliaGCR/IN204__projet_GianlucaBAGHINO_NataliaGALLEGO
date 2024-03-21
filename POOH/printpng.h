#ifndef PRINTPNG_H
#define PRINTPNG_H

#include <d3d9.h>

class ImGuiD3D9Wrapper {
public:
    ImGuiD3D9Wrapper(HWND hwnd, LPDIRECT3DDEVICE9 d3dDevice, const char* imagePath);
    ~ImGuiD3D9Wrapper();

    void InitD3D9();
    void InitImGuiAndD3D9();
    void InitTexture(const char* imagePath);
    void CleanupImGuiAndD3D9();
    LPDIRECT3DTEXTURE9 RenderAndGetTexture();

private:
    HWND hwnd_;
    LPDIRECT3DDEVICE9 d3dDevice_;
    LPDIRECT3D9 d3d_;
    LPDIRECT3DTEXTURE9 g_Texture;

    bool LoadTextureFromPNG(const char* filePath, LPDIRECT3DTEXTURE9* texture);
    void ReleaseTexture(LPDIRECT3DTEXTURE9* texture);
};

#endif // PRINTPNG_H