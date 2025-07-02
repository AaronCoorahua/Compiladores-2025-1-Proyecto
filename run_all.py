import os
import shutil
import subprocess
from pathlib import Path

# Rutas base
PROJECT_ROOT = Path(__file__).parent.resolve()
BUILD_DIR = PROJECT_ROOT / "build"
TEST_CASES_DIR = PROJECT_ROOT / "test_cases"
ASSEMBLY_DIR = PROJECT_ROOT / "assembly"
OUTPUTS_DIR = PROJECT_ROOT / "outputs"
EXECUTABLE = BUILD_DIR / "Proyecto"

# Función auxiliar para correr comandos con output
def run_command(cmd, cwd=None):
    try:
        result = subprocess.run(cmd, cwd=cwd, check=True, capture_output=True, text=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        return f"❌ Error:\n{e.stderr}"

# Paso 1: Compilar el compilador Pascal
def build_compiler():
    print("🔧 Compilando el compilador Pascal...")
    if (BUILD_DIR / "CMakeCache.txt").exists():
        with open(BUILD_DIR / "CMakeCache.txt", "r") as f:
            if "c:/" in f.read().lower():  # Detecta mezcla de Windows y Linux
                print("⚠️ Conflicto detectado en CMakeCache.txt (Windows/Linux mix)")
                print("🧹 Limpiando y reconstruyendo `build/`...")
                shutil.rmtree(BUILD_DIR)

    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    configure = subprocess.run(["cmake", ".."], cwd=BUILD_DIR)
    if configure.returncode != 0:
        print("❌ Error al configurar CMake")
        return False

    build = subprocess.run(["cmake", "--build", "."], cwd=BUILD_DIR)
    if build.returncode != 0:
        print("❌ Error al compilar el proyecto")
        return False

    print("✅ Compilador listo\n")
    return True

# Paso 2: Ejecutar pruebas
def run_tests():
    ASSEMBLY_DIR.mkdir(exist_ok=True)
    OUTPUTS_DIR.mkdir(exist_ok=True)

    for txt_file in TEST_CASES_DIR.glob("*.pas"):
        print(f"🧪 Procesando {txt_file.name}...")

        # Ejecutar compilador Pascal
        compile_output = run_command([str(EXECUTABLE), str(txt_file)])
        asm_file = ASSEMBLY_DIR / (txt_file.stem + ".s")

        if not asm_file.exists():
            print(f"⚠️ No se generó ensamblador para {txt_file.name}")
            continue

        # Compilar ensamblador con gcc
        binary_path = asm_file.with_suffix("")
        gcc_result = subprocess.run(["gcc", "-no-pie", "-o", str(binary_path), str(asm_file)],
                                    capture_output=True, text=True)
        if gcc_result.returncode != 0:
            print(f"❌ Error de gcc en {txt_file.name}:\n{gcc_result.stderr}")
            continue

        # Ejecutar binario y guardar output
        output_txt = OUTPUTS_DIR / f"{txt_file.stem}.txt"
        with open(output_txt, "w") as f:
            run_result = subprocess.run([str(binary_path)], stdout=f, stderr=subprocess.STDOUT)

        # Mostrar resumen
        with open(output_txt) as f:
            output_preview = f.read().strip()
        print(f"📤 Output del programa:\n{output_preview}\n")

if __name__ == "__main__":
    if build_compiler():
        run_tests()
