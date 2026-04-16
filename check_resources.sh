#!/bin/bash
# Run this from your project root:
#   chmod +x check_resources.sh && ./check_resources.sh

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
echo "=== Nothing Private Browser — Resource Checker ==="
echo "Project root: $PROJECT_ROOT"
echo ""

REQUIRED=(
    "icons/logomain.png"
    "icons/elysia.jpeg"
    "icons/main_alysia.jpeg"
    "icons/3-dots-bounce.svg"
    "resources.qrc"
    "CMakeLists.txt"
)

ALL_OK=true
for f in "${REQUIRED[@]}"; do
    FULL="$PROJECT_ROOT/$f"
    if [ -f "$FULL" ]; then
        SIZE=$(du -h "$FULL" | cut -f1)
        echo "  ✅  $f  ($SIZE)"
    else
        echo "  ❌  MISSING: $f  <-- THIS IS YOUR PROBLEM"
        ALL_OK=false
    fi
done

echo ""

# Check CMakeLists.txt mentions resources.qrc
CMAKE="$PROJECT_ROOT/CMakeLists.txt"
if [ -f "$CMAKE" ]; then
    if grep -q "resources.qrc" "$CMAKE"; then
        echo "  ✅  CMakeLists.txt references resources.qrc"
    else
        echo "  ❌  CMakeLists.txt does NOT mention resources.qrc — images will never load!"
        echo "      Add one of these to your CMakeLists.txt:"
        echo ""
        echo "      # Modern Qt6 way:"
        echo "      qt_add_resources(RESOURCES resources.qrc)"
        echo "      target_sources(your_target_name PRIVATE \${RESOURCES})"
        echo ""
        echo "      # Or just add it to your source list directly:"
        echo "      qt_add_executable(your_target_name"
        echo "          main.cpp PrivateWindow.cpp resources.qrc)"
        ALL_OK=false
    fi
fi

echo ""
if $ALL_OK; then
    echo "Everything looks good! Do a clean rebuild:"
    echo "  rm -rf build && mkdir build && cd build && cmake .. && make -j\$(nproc)"
else
    echo "Fix the issues above, then rebuild."
fi