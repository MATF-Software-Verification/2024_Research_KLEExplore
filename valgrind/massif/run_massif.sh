#!/bin/bash

EXEC_PATH="../../alat-za-analitiku/alatZaAnalitiku/build/Desktop_Qt_6_9_3-Debug/alatZaAnalitiku"

OUTPUT_DIR="massif_out"
mkdir -p $OUTPUT_DIR

OUTFILE="./$OUTPUT_DIR/massif.out.$$"

echo "Pokrećem Massif za $EXEC_PATH ..."
# valgrind --tool=massif --massif-out-file=$OUTFILE $EXEC_PATH
valgrind \
  --tool=massif \
  --time-unit=ms \
  --threshold=5.0 \
  --detailed-freq=200 \
  --massif-out-file="$OUTFILE" \
  "$EXEC_PATH"


echo ""
echo "Profil memorije sačuvan u $OUTFILE"

echo ""
echo "Za lepši ispis koristite: ms_print $OUTFILE"
echo "Za GUI pregled koristite: massif-visualizer $OUTFILE"
