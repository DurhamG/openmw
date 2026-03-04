#!/bin/bash
# Build and install OpenMW for Steam Deck
# Run this script from the root of the openmw repository on the Steam Deck.
#
# Prerequisites (run once):
#   sudo steamos-readonly disable
#   sudo pacman-key --init && sudo pacman-key --populate archlinux holo
#   sudo pacman -Syu base-devel cmake openscenegraph sdl2 boost ffmpeg luajit \
#       sqlite yaml-cpp qt6-base qt6-tools qt6-svg openal lz4 recastnavigation \
#       freetype2 mesa libglvnd icu libxkbcommon vulkan-headers zlib glibc linux-api-headers
#
# Bullet must be built separately with double precision:
#   git clone https://github.com/bulletphysics/bullet3.git
#   cd bullet3 && mkdir build && cd build
#   cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo \
#       -DUSE_DOUBLE_PRECISION=ON -DBUILD_SHARED_LIBS=ON \
#       -DBUILD_EXTRAS=OFF -DBUILD_DEMOS=OFF -DBUILD_EXAMPLES=OFF \
#       -DBUILD_UNIT_TESTS=OFF -DBUILD_CPU_DEMOS=OFF \
#       -DBUILD_OPENGL3_DEMOS=OFF -DBUILD_BULLET2_DEMOS=OFF
#   make -j4 && sudo make install

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
INSTALL_DIR="${HOME}/openmw-install"
JOBS=$(nproc)

echo "=== OpenMW Steam Deck Build ==="
echo "Source:  ${SCRIPT_DIR}"
echo "Build:   ${BUILD_DIR}"
echo "Install: ${INSTALL_DIR}"
echo "Jobs:    ${JOBS}"
echo ""

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Only run cmake configure if cache doesn't exist or --clean is passed
if [ ! -f CMakeCache.txt ] || [ "${1:-}" = "--clean" ]; then
    rm -f CMakeCache.txt
    echo "Running cmake configure..."
    cmake "${SCRIPT_DIR}" \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}" \
        -DCMAKE_PREFIX_PATH=/usr/local \
        -DBULLET_ROOT=/usr/local \
        -DOPENMW_USE_SYSTEM_MYGUI=OFF \
        -DGLOBAL_CONFIG_PATH="${INSTALL_DIR}/etc" \
        -DGLOBAL_DATA_PATH="${INSTALL_DIR}/share/games"
else
    echo "Using existing cmake cache (pass --clean to reconfigure)"
fi

cmake --build . -j "${JOBS}"
cmake --install .

echo ""
echo "=== Build complete ==="
echo "Run: ${INSTALL_DIR}/bin/openmw-launcher"
echo "Or:  ${INSTALL_DIR}/bin/openmw"
echo ""
echo "First time? Run openmw-wizard to set up game data:"
echo "  ${INSTALL_DIR}/bin/openmw-wizard"
