#!/bin/bash
set -e

# Folder submodule-a koji će se formatirati
SUBMODULE_DIR="../alat-za-analitiku/alatZaAnalitiku"

# Provera da li folder postoji
if [ ! -d "$SUBMODULE_DIR" ]; then
    echo "Submodule folder '$SUBMODULE_DIR' ne postoji!"
    exit 1
fi

# Formatiranje svih .cpp i .h fajlova unutar submodule-a
find "$SUBMODULE_DIR" \( -name "*.cpp" -o -name "*.h" \) \
    -exec clang-format -i -style=file {} +

echo "✅ Formatiranje završeno za submodule: $SUBMODULE_DIR"
