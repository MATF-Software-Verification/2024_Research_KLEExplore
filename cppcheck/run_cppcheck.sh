#!/bin/bash
set -e

export CMAKE_PREFIX_PATH=/home/parallels/Qt/6.9.3/gcc_64/lib/cmake/Qt6

PROJECT_DIR="/media/psf/RemovableDisk/faks/vs/projekat/2024_Research_KLEExplore/alat-za-analitiku/alatZaAnalitiku"
REPORT_DIR="./cppcheck-report"
TXT_REPORT="$REPORT_DIR/cppcheck-output.txt"

mkdir -p "$REPORT_DIR"

cppcheck \
  --enable=all \
  --inconclusive \
  --std=c++17 \
  -I "/home/parallels/Qt/6.9.3/gcc_64/include" \
  -I "$PROJECT_DIR" \
  -DQT_CORE_LIB -DQT_GUI_LIB -DQT_WIDGETS_LIB \
  --suppress=missingIncludeSystem \
  --xml 2> >(tee "$TXT_REPORT" > "$REPORT_DIR/cppcheck-report.xml") \
  "$PROJECT_DIR"

cppcheck-htmlreport \
  --file="$REPORT_DIR/cppcheck-report.xml" \
  --report-dir="$REPORT_DIR" \
  --source-dir="$PROJECT_DIR" \


echo ""
echo "✅ Analiza završena!"
echo "HTML izveštaj: $REPORT_DIR/index.html"
echo "Tekstualni izveštaj: $TXT_REPORT"
