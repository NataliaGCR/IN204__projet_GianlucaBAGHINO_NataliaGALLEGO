// ImGuiExample.h

#pragma once
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"

#undef CreateWindow
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ImGuiExample {
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void ResetDevice();
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    int RunImGuiExample(int argc, char** argv);

}  // namespace ImGuiExample
