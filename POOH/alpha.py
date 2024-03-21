import cv2

def check_alpha_channel(image_path):
    try:
        # Cargar la imagen con el canal alpha
        img = cv2.imread(image_path, cv2.IMREAD_UNCHANGED)

        # Verificar si la imagen tiene un canal alpha
        if img.shape[2] == 4:
            print("La imagen tiene un canal alpha.")
        else:
            print("La imagen no tiene un canal alpha.")
    except Exception as e:
        print(f"Error al abrir la imagen: {e}")

# Ruta de la imagen
filename = "mi_imagen.png"

# Llamada a la función
check_alpha_channel("C:\\Users\\natyo\\OneDrive - Universidad EIA\\Escritorio\\POOH\\image_2.png")
