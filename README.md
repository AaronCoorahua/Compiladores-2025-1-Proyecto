# Compiladores-2025-1-Proyecto

## 🪟 Instrucciones para Windows (Frontend GUI)

1. Instalar Docker Engine (Docker Desktop)

2. Construir la imagen de ejecución (solo una vez):
   "docker build -t pascal-linux-runner -f Dockerfile ."

3. Instalar PyQt5:
   "pip install pyqt5"

4. Ejecutar la interfaz gráfica del compilador:
   "python frontend.py"

5. Si ocurre algún conflicto con la carpeta build/, elimínarla y volver a ejecutar "python frontend.py".

## 🐧 Instrucciones para Linux (CLI Automático)

1. Asegúrate de tener `cmake`, `make`, `g++` y `python3` instalados.

2. Si ocurre algún conflicto con la carpeta `build/`, elimínala:
   "rm -rf build"

3. Ejecuta el script para compilar el proyecto y correr todos los tests:
   "python3 run_all.py"

4. Los archivos ensamblador `.s` se generarán en la carpeta:
   assembly/

5. Las salidas de los programas ejecutados se guardarán en:
   outputs/