import os
import subprocess
from pathlib import Path

# Rutas
TEST_CASES_DIR = Path("test_cases")
ASSEMBLY_DIR = Path("assembly")
OUTPUTS_DIR = Path("outputs")
BUILD_DIR = Path("build")
EXECUTABLE = BUILD_DIR / "Debug" / "Proyecto.exe"  # Ajustar según tu configuración

# Asegurar carpetas
for folder in [ASSEMBLY_DIR, OUTPUTS_DIR, BUILD_DIR]:
    folder.mkdir(parents=True, exist_ok=True)

# 1. 🛠️ Compilar el compilador con CMake
print("🔧 Compilando el compilador Pascal...")
try:
    subprocess.run(["cmake", ".."], cwd=BUILD_DIR, check=True)
    subprocess.run(["cmake", "--build", "."], cwd=BUILD_DIR, check=True)
except subprocess.CalledProcessError as e:
    print("❌ Error al compilar el compilador:\n", e)
    exit(1)

if not EXECUTABLE.exists():
    print(f"❌ No se encontró {EXECUTABLE}")
    exit(1)

# 2. 📂 Procesar cada archivo .txt
for txt_file in TEST_CASES_DIR.glob("*.txt"):
    print(f"\n🧪 Procesando {txt_file.name}...")

    # Llamar al compilador
    result = subprocess.run([str(EXECUTABLE), str(txt_file)], capture_output=True, text=True)
    if result.returncode != 0:
        print(f"❌ Error al compilar {txt_file.name}:\n{result.stderr}")
        continue

    # Verifica ensamblador generado
    asm_file = ASSEMBLY_DIR / (txt_file.stem + ".s")
    if not asm_file.exists():
        print(f"⚠️  No se generó {asm_file}")
        continue

    # 3. 🔧 Compilar con gcc
    binary_path = asm_file.with_suffix(".exe")
    compile_result = subprocess.run(["gcc", "-no-pie", "-o", str(binary_path), str(asm_file)],
                                    capture_output=True, text=True, shell=True)
    if compile_result.returncode != 0:
        print(f"❌ Error de gcc en {asm_file.name}:\n{compile_result.stderr}")
        continue

    # 4. 💻 Ejecutar el binario
    output_txt = OUTPUTS_DIR / (txt_file.stem + ".txt")
    with open(output_txt, "w") as out_file:
        run_result = subprocess.run([str(binary_path)], stdout=out_file, stderr=subprocess.STDOUT, shell=True)

    print(f"✅ Output guardado en {output_txt.name}")

print("\n🎉 Todo completado.")
