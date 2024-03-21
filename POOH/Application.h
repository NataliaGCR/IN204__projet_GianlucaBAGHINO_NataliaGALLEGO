#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <d3d9.h>


namespace MyApp
{
        //void LoadImage();
        extern bool loadXMLFlag;
        extern int loadRenderFlag;
        extern bool imageReady;
        extern int current_item;
        void InitTexture(const char* imagePath);
        void RenderUI();
        void SaveXML();
        void LoadImage(LPDIRECT3DDEVICE9 device);
} // namespace MyApp

#endif // APPLICATION_H