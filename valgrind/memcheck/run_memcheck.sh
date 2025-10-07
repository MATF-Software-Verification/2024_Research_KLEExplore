#!/bin/bash
set -e

EXEC_PATH="../../alat-za-analitiku/alatZaAnalitiku/build/Desktop_Qt_6_9_3-Debug/alatZaAnalitiku"
REPORT_FILE="results.txt"
SUPP_FILE="suppressions.supp"

echo "Pokrećem Valgrind na projektu..."
echo ""

valgrind --tool=memcheck \
         --leak-check=summary \
         --track-origins=yes \
         --suppressions="$SUPP_FILE" \
         --log-file="$REPORT_FILE" \
         "$EXEC_PATH"

#  --gen-suppressions=all \

echo ""
echo "✅ Analiza završena! Rezultat se nalazi u $REPORT_FILE"
