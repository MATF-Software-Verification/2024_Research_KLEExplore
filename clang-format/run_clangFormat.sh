#!/bin/bash
set -e

SUBMODULE_DIR="../alat-za-analitiku/alatZaAnalitiku"
DEST_DIR="formattedFiles"

# clang-format style
CLANG_STYLE="{BasedOnStyle: LLVM, IndentWidth: 4, TabWidth: 4, AlignConsecutiveAssignments: true, AlignEscapedNewlines: Left, AllowShortBlocksOnASingleLine: true, AllowShortCaseLabelsOnASingleLine: true, AllowShortFunctionsOnASingleLine: Inline, AllowShortLambdasOnASingleLine: All, BreakBeforeBraces: Attach, MaxEmptyLinesToKeep: 2, ColumnLimit: 150}"

# Find all C/C++ files, excluding tests, build, and CMakeFiles
find "$SUBMODULE_DIR" \( -name "*.h" -o -name "*.cpp" \) \
    ! -path "*/tests/*" \
    ! -path "*/build/*" \
    ! -path "*/CMakeFiles/*" | while read f; do
  
    REL_PATH="${f#$SUBMODULE_DIR/}"

    TMP_FILE=$(mktemp)
    clang-format -style="$CLANG_STYLE" "$f" > "$TMP_FILE"

    # Compare formatted file with original
    if ! cmp -s "$f" "$TMP_FILE"; then
        # Make directories if they don't exist
        mkdir -p "$(dirname "$DEST_DIR/$REL_PATH")"
        # Move formatted file to destination
        mv "$TMP_FILE" "$DEST_DIR/$REL_PATH"
        echo "Promenjen: $REL_PATH"
    else
        # Remove temp file if no changes
        rm "$TMP_FILE"
    fi
done

echo "✅ Formatirani i izmenjeni fajlovi su kopirani u $DEST_DIR"



