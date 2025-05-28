#!/bin/bash

# Download premake5
wget https://github.com/premake/premake-core/releases/download/v5.0.0-beta6/premake-5.0.0-beta6-linux.tar.gz -O premake5.tar.gz

# Extract the tarball
tar -xvf premake5.tar.gz

# Find the extracted premake5 binary
extracted_premake=$(tar -tf premake5.tar.gz | grep premake5)

# Make the binary executable
chmod +x $extracted_premake

# Move the binary to /usr/local/bin
sudo mv $extracted_premake /usr/local/bin/premake5

# Clean up
rm premake5.tar.gz

echo "premake5 installed successfully."
