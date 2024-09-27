#!/bin/bash

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check if Conan is installed
if ! command_exists conan; then
    echo "Conan is not installed. Please install Conan first."
    exit 1
fi

# Detect and create default profile
echo "Detecting system settings and creating default profile..."
conan profile detect

# Get the detected OS
detected_os=$(conan profile show default | grep os= | cut -d'=' -f2)

# Set OS version if macOS
if [ "$detected_os" = "Macos" ]; then
    macos_version=$(sw_vers -productVersion)
    conan profile update settings.os.version="$macos_version" default
fi

# Set compiler to clang if on macOS, otherwise keep the detected compiler
if [ "$detected_os" = "Macos" ]; then
    conan profile update settings.compiler="apple-clang" default
    clang_version=$(clang --version | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -n 1)
    conan profile update settings.compiler.version="$clang_version" default
    conan profile update settings.compiler.libcxx="libc++" default
fi

# Set C++ standard to 17
conan profile update settings.compiler.cppstd="17" default

# Detect architecture
arch=$(uname -m)
conan profile update settings.arch="$arch" default

# Set build type to Release (you can change this if needed)
conan profile update settings.build_type="Release" default

# Display the updated profile
echo "Updated Conan profile for 3Dive project:"
conan profile show default

echo "Conan profile setup complete for 3Dive project!"