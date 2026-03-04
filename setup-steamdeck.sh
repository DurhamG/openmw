#!/bin/bash
# Set up Steam Deck build environment for OpenMW
# Re-run this after SteamOS updates, which wipe pacman-installed packages.
#
# Prerequisites:
#   - Set a password with: passwd
#   - Bullet with double precision must be built separately (see below)

set -euo pipefail

echo "=== OpenMW Steam Deck Environment Setup ==="

# Enable developer mode: unlocks read-only filesystem, populates pacman
# keyring, and restores headers/includes stripped from the base SteamOS image.
echo "Enabling developer mode..."
sudo steamos-devmode enable

# Restore development headers that SteamOS strips from its minimized image.
# Without this, glibc headers like features.h are missing even though
# pacman reports glibc as installed.
echo "Restoring development headers..."
sudo steamos-unminimize /usr/include

# Install build dependencies
# SteamOS updates replace the root filesystem image (A/B partitioning),
# wiping all pacman-installed packages. Re-run this script after updates.
echo "Installing build dependencies..."
sudo pacman -Syu --noconfirm --needed \
    base-devel cmake git \
    openscenegraph sdl2 boost ffmpeg luajit sqlite yaml-cpp \
    qt6-base qt6-tools qt6-svg \
    openal lz4 recastnavigation freetype2 \
    mesa libglvnd icu libxkbcommon vulkan-headers zlib \
    glibc linux-api-headers

# Build and install Bullet with double precision if not already present.
# Bullet installs to /usr/local which lives on the user partition and
# survives SteamOS updates.
if [ -f /usr/local/lib/libBulletDynamics.so ] && \
   grep -q "BT_USE_DOUBLE_PRECISION" /usr/local/include/bullet/btBulletDynamicsCommon.h 2>/dev/null; then
    echo "Bullet with double precision already installed in /usr/local, skipping"
    sudo ldconfig
else
    echo ""
    echo "Building Bullet with double precision..."
    BULLET_DIR=$(mktemp -d)
    git clone --depth 1 https://github.com/bulletphysics/bullet3.git "${BULLET_DIR}"
    mkdir -p "${BULLET_DIR}/build"
    cd "${BULLET_DIR}/build"
    cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DUSE_DOUBLE_PRECISION=ON \
        -DBUILD_SHARED_LIBS=ON \
        -DBUILD_EXTRAS=OFF \
        -DBUILD_DEMOS=OFF \
        -DBUILD_EXAMPLES=OFF \
        -DBUILD_UNIT_TESTS=OFF \
        -DBUILD_CPU_DEMOS=OFF \
        -DBUILD_OPENGL3_DEMOS=OFF \
        -DBUILD_BULLET2_DEMOS=OFF
    make -j"$(nproc)"
    sudo make install
    sudo ldconfig
    rm -rf "${BULLET_DIR}"
    echo "Bullet installed to /usr/local"
fi

echo ""
echo "=== Setup complete ==="
echo "Now run: ./build-steamdeck.sh --clean"
