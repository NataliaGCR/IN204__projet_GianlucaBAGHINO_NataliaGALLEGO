#ifdef assert
#undef assert
#endif

#include <cassert> // Include this
#include "Application.h"
#include "xml_reader.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>
#include <fstream>
#include <iostream>

#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")



ImTextureID        myImage    = NULL;
LPDIRECT3DTEXTURE9 newTexture = NULL;


void cSprite(IDirect3DDevice9* m_pD3Ddev, LPCSTR szFilePath)
{   
    D3DXIMAGE_INFO pInfo; // Declare pInfo here
    ZeroMemory(&pInfo, sizeof(D3DXIMAGE_INFO));

    if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pInfo)))
        MessageBox(NULL, "Cannot get image info", "Error", MB_ICONERROR);
    if (FAILED(D3DXCreateTextureFromFileEx(m_pD3Ddev, szFilePath, pInfo.Width, pInfo.Height,
        D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &pInfo,
        NULL,  &newTexture)))
            MessageBox(NULL, "Cannot create image texture", "Error", MB_ICONERROR);
}


    
namespace MyApp
{   

    int my_image_width = 0;
    int my_image_height = 0;

    bool loadXMLFlag = false;
    int loadRenderFlag = 0;
    bool imageReady = false;

    void LoadImage(LPDIRECT3DDEVICE9 device)
    {
        if (!myImage && imageReady && current_item!=0)
        {   
            std::cout << "Loading Image" << std::endl;
            cSprite(device, "C:\\Users\\natyo\\OneDrive - Universidad EIA\\Escritorio\\POOH\\RayTracer\\output.png");
            myImage = newTexture;
        }
    }

    std::vector<CameraInfo> sceneInfo;
    std::vector<ObjectInfo> objectInfo;

    int current_item = 0;
    bool renderPressed = false;

    void SaveXML(const char* text)
    {
        std::ofstream file("C:\\Users\\natyo\\OneDrive - Universidad EIA\\Escritorio\\POOH\\output.xml");
        if (file.is_open())
        {
            file << "\xEF\xBB\xBF"; // UTF-8 BOM
            file << text;
            file.close();
        }
    }

    void RenderUI()
    {
        ImGui::Begin("Settings");


        for (int i = 0; i < 3; ++i)
            ImGui::Spacing();

        static const char* items[] = {" ", "Low Render", "Medium Render", "High Render"};
        
        ImGui::Text("Render Type");
        ImGui::Combo("##RenderType", &current_item, items, IM_ARRAYSIZE(items));

        for (int i = 0; i < 7; ++i)
            ImGui::Spacing();
        
        float windowWidth = ImGui::GetWindowWidth();
        float buttonWidth = 180; // Define the width of the button
        float buttonHeight = 25; // Define the height of the button

        ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f); // Center the button
 

        if (ImGui::Button("Render", ImVec2(buttonWidth, buttonHeight)))
        {   
            loadRenderFlag = 1;
            renderPressed = true;
        }

        
        for (int i = 0; i < 10; ++i)
            ImGui::Spacing();

        ImGui::Text("Scene Description");
        static char text1[1024 * 4] = "";
        ImGui::InputTextMultiline("##SceneDescription", text1, IM_ARRAYSIZE(text1), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 35));
        if (ImGui::Button("Save XML", ImVec2(buttonWidth, buttonHeight)))
        {
            SaveXML(text1);
        }

        if (ImGui::Button("Load XML", ImVec2(buttonWidth, buttonHeight)))
        {
            loadXMLFlag = true;
            std::cout << "Loaded XML" << std::endl;
        }

        ImGui::End();

        ImGui::Begin("DockSpace");
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id);
        ImGui::End();

        float aspect_ratio = (float)400 / (float)225; // Calcula la relación de aspecto original

        ImGui::Begin("RayTracer");
        if (renderPressed and loadRenderFlag==2)
        {
            float my_image_width = ImGui::GetWindowWidth(); // Ajusta el ancho de la imagen al ancho de la ventana
            float my_image_height = my_image_width / aspect_ratio; // Calcula la altura en función del ancho y la relación de aspecto original
            ImGui::ImageButton(myImage, ImVec2(my_image_width, my_image_height));
        }
        ImGui::End();
    }
}  // Fin del namespace MyApp
