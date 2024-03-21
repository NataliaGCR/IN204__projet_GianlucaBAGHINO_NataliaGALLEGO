cmake_minimum_required(VERSION 3.10)

project(MiProyecto)

# Agrega las rutas a las carpetas de inclusión
include_directories(
    "C:\\VulkanSDK\\1.3.275.0\\Include"
    "C:\\GLFW\\include"
    "C:\\glew\\include"
    "C:\\pugixml-1.14\\src"
    "C:\\Users\\natyo\\OneDrive - Universidad EIA\\Escritorio\\POOH\\RayTracer"
    #"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include"
    "${CMAKE_SOURCE_DIR}"
)

# Agrega las rutas a las bibliotecas
link_directories(
    "C:\\GLFW\\lib"
    "C:\\VulkanSDK\\1.3.275.0\\Lib"
    "C:\\glew\\lib\\Release\\x64"
   #"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64"
)

# Lista de archivos fuente
set(SOURCES
    "ImGui/imgui.cpp"
    "ImGui/imgui_demo.cpp"
    "ImGui/imgui_draw.cpp"
    "ImGui/imgui_impl_dx9.cpp"
    "ImGui/imgui_impl_glfw.cpp"
    "ImGui/imgui_impl_vulkan.cpp"
    "ImGui/imgui_impl_win32.cpp"
    "ImGui/imgui_tables.cpp"
    "ImGui/imgui_impl_opengl3.cpp"
    "ImGui/imgui_widgets.cpp" 
    "RayTracer/RayTracer.cpp"
    Application.cpp
    xml_reader.cpp
    printpng.cpp
    "ImGui/ImGuiExample.cpp"
    main.cpp
    "C:/pugixml-1.14/src/pugixml.cpp"
)

# Compila los archivos fuente y crea el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Enlaza las bibliotecas necesarias
target_link_libraries(${PROJECT_NAME}
    d3d9
    d3dx9
    dwmapi
    gdi32
    vulkan-1
    glfw3
    glew32
    opengl32
)


find_package(OpenMP)

# Si se encuentra OpenMP, enlaza las bibliotecas de OpenMP y agrega las banderas de compilación
if (OPENMP_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()