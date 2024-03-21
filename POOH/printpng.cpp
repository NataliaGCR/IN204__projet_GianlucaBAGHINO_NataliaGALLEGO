#include <d3d9.h>
#include <d3dx9tex.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>

class ImGuiD3D9Wrapper {
public:
    ImGuiD3D9Wrapper(HWND hwnd, LPDIRECT3DDEVICE9 d3dDevice, const char* imagePath) : hwnd_(hwnd), d3dDevice_(d3dDevice) {
        InitD3D9();
        InitImGuiAndD3D9();
        InitTexture(imagePath);
    }

    ~ImGuiD3D9Wrapper() {
        CleanupImGuiAndD3D9();
    }

    // Método para inicializar D3D9
    void InitD3D9() {
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

        d3d_ = Direct3DCreate9(D3D_SDK_VERSION);
        if (!d3d_) {
            // Maneja el error
        }

        HRESULT hr = d3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd_,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice_);
        if (FAILED(hr)) {
            // Maneja el error
        }
    }

    // Método para inicializar ImGui y cargar la textura desde un archivo PNG proporcionado
    // Método para inicializar ImGui y cargar la textura desde un archivo PNG proporcionado
    void InitImGuiAndD3D9() {
        ImGui::CreateContext();
        ImGui_ImplDX9_Init(d3dDevice_);
    }

    // Método para cargar la textura desde un archivo PNG proporcionado
    void InitTexture(const char* imagePath) {
        if (LoadTextureFromPNG(imagePath, &g_Texture)) {
            // La textura se ha cargado correctamente
        } else {
            // Manejar el error si la carga de la textura falla
        }
    }

    // Método para limpiar los recursos de ImGui, D3D9 y liberar la memoria de la textura
    void CleanupImGuiAndD3D9() {
        ReleaseTexture(&g_Texture);
        ImGui_ImplDX9_Shutdown();
        ImGui::DestroyContext();

        if (d3dDevice_) {
            d3dDevice_->Release();
            d3dDevice_ = nullptr;
        }

        if (d3d_) {
            d3d_->Release();
            d3d_ = nullptr;
        }
    }

    // Método para renderizar la interfaz gráfica ImGui y devolver la textura
    LPDIRECT3DTEXTURE9 RenderAndGetTexture() {
        ImGui_ImplDX9_NewFrame();

        ImGui::Begin("RayTracer");

        if (g_Texture) {
            // Renderizar la imagen en una posición predeterminada
            ImGui::Image((ImTextureID)g_Texture, ImVec2(100, 100));
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

        return g_Texture;
    }

private:
    HWND hwnd_;
    LPDIRECT3DDEVICE9 d3dDevice_;
    LPDIRECT3D9 d3d_ = nullptr;
    LPDIRECT3DTEXTURE9 g_Texture = nullptr;

    bool LoadTextureFromPNG(const char* filePath, LPDIRECT3DTEXTURE9* texture) {
        return D3DXCreateTextureFromFileA(d3dDevice_, filePath, texture) == D3D_OK;
    }

    void ReleaseTexture(LPDIRECT3DTEXTURE9* texture) {
        if (*texture) {
            (*texture)->Release();
            *texture = nullptr;
        }
    }
};