FROM ubuntu:22.04

# Instalar gcc y cmake
RUN apt update && apt install -y build-essential cmake

# Crear una carpeta para el código
WORKDIR /app

# Copiar todo el proyecto
COPY . .

