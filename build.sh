#!/bin/bash
set -euo pipefail

PROJECT_DIR="$(
    cd "$(dirname "${BASH_SOURCE[0]}")"
    pwd
)"

CORE_SOURCE_DIR="$PROJECT_DIR/source/ZellnoComboLock"
RAG_COMPAT_SOURCE_DIR="$PROJECT_DIR/source/ZellnoComboLock_RaGCompat"
BUILD_ROOT="$PROJECT_DIR/build"
BUILD_MOD="$BUILD_ROOT/@ZellnoComboLock"
BUILD_ADDONS="$BUILD_MOD/addons"
BUILD_OPTIONAL="$BUILD_MOD/optional"
BUILD_KEYS="$BUILD_MOD/keys"

DAYZ_TOOLS="$HOME/.local/share/Steam/steamapps/common/DayZ Tools/Bin"
FILEBANK="$DAYZ_TOOLS/PboUtils/FileBank.exe"
BANKREV="$DAYZ_TOOLS/PboUtils/BankRev.exe"
DSSIGNFILE="$DAYZ_TOOLS/DsUtils/DSSignFile.exe"
DSCHECK="$DAYZ_TOOLS/DsUtils/DSCheckSignatures.exe"

PRIVATE_KEY="$HOME/dayz/modding/keys/Zellno/Zellno.biprivatekey"
PUBLIC_KEY="$HOME/dayz/modding/keys/Zellno/Zellno.bikey"

EXPECTED_BUILD_MOD="$PROJECT_DIR/build/@ZellnoComboLock"

if [ "$BUILD_MOD" != "$EXPECTED_BUILD_MOD" ]; then
    echo "Unexpected build destination: $BUILD_MOD" >&2
    exit 1
fi

for required in \
    "$CORE_SOURCE_DIR/config.cpp" \
    "$RAG_COMPAT_SOURCE_DIR/config.cpp" \
    "$PROJECT_DIR/mod.cpp" \
    "$PROJECT_DIR/meta.cpp" \
    "$PROJECT_DIR/README.md" \
    "$FILEBANK" \
    "$BANKREV" \
    "$DSSIGNFILE" \
    "$DSCHECK" \
    "$PRIVATE_KEY" \
    "$PUBLIC_KEY"
do
    if [ ! -f "$required" ]; then
        echo "Required file not found:" >&2
        echo "$required" >&2
        exit 1
    fi
done

rm -rf -- "$BUILD_MOD"
mkdir -p "$BUILD_ADDONS" "$BUILD_OPTIONAL" "$BUILD_KEYS"

wine "$FILEBANK" \
    -property prefix=ZellnoComboLock \
    -dst "$(winepath -w "$BUILD_ADDONS")" \
    "$(winepath -w "$CORE_SOURCE_DIR")"

wine "$FILEBANK" \
    -property prefix=ZellnoComboLock_RaGCompat \
    -dst "$(winepath -w "$BUILD_OPTIONAL")" \
    "$(winepath -w "$RAG_COMPAT_SOURCE_DIR")"

CORE_PBO="$BUILD_ADDONS/ZellnoComboLock.pbo"
RAG_COMPAT_PBO="$BUILD_OPTIONAL/ZellnoComboLock_RaGCompat.pbo"

for pbo in "$CORE_PBO" "$RAG_COMPAT_PBO"
do
    if [ ! -f "$pbo" ]; then
        echo "FileBank did not create the expected PBO:" >&2
        echo "$pbo" >&2
        exit 1
    fi

    wine "$DSSIGNFILE" \
        "$(winepath -w "$PRIVATE_KEY")" \
        "$(winepath -w "$pbo")"
done

cp "$PUBLIC_KEY" "$BUILD_KEYS/Zellno.bikey"
cp "$PROJECT_DIR/mod.cpp" "$BUILD_MOD/mod.cpp"
cp "$PROJECT_DIR/meta.cpp" "$BUILD_MOD/meta.cpp"
cp "$PROJECT_DIR/README.md" "$BUILD_MOD/README.md"

CORE_SIGNATURE="$CORE_PBO.Zellno.bisign"
RAG_COMPAT_SIGNATURE="$RAG_COMPAT_PBO.Zellno.bisign"

for signature in "$CORE_SIGNATURE" "$RAG_COMPAT_SIGNATURE"
do
    if [ ! -f "$signature" ]; then
        echo "The expected signature was not created:" >&2
        echo "$signature" >&2
        exit 1
    fi
done

CORE_VERIFY_OUTPUT="$(
    wine "$DSCHECK" \
        "$(winepath -w "$BUILD_ADDONS")" \
        "$(winepath -w "$(dirname "$PUBLIC_KEY")")" \
        2>&1
)"

RAG_VERIFY_OUTPUT="$(
    wine "$DSCHECK" \
        "$(winepath -w "$BUILD_OPTIONAL")" \
        "$(winepath -w "$(dirname "$PUBLIC_KEY")")" \
        2>&1
)"

echo "$CORE_VERIFY_OUTPUT"
echo "$RAG_VERIFY_OUTPUT"

if ! grep -q 'is OK' <<< "$CORE_VERIFY_OUTPUT" || \
   ! grep -q 'is OK' <<< "$RAG_VERIFY_OUTPUT"; then
    echo "One or more signatures failed validation." >&2
    exit 1
fi

echo
echo "PBO properties:"
wine "$BANKREV" \
    -properties \
    "$(winepath -w "$CORE_PBO")"
wine "$BANKREV" \
    -properties \
    "$(winepath -w "$RAG_COMPAT_PBO")"

echo
echo "Produced files:"
find "$BUILD_MOD" -type f -printf '%P\n' | sort

echo
echo "Hashes:"
sha256sum \
    "$CORE_PBO" \
    "$CORE_SIGNATURE" \
    "$RAG_COMPAT_PBO" \
    "$RAG_COMPAT_SIGNATURE"

echo
echo "Validated build:"
echo "$BUILD_MOD"

echo
echo "No installation was performed."
