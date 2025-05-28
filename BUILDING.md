## Installation

### Windows

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Nano.git
    cd Nano
    ```

2. Navigate to the scripts folder:
    ```sh
    cd scripts/windows
    ```

3. (Optional) If you haven't already installed the premake5 build system, open an admin command prompt:
    ```sh
    ./install-premake5.bat
    ```

4. Choose what you want it build to:
    - Visual Studio 17 2022:
        ```sh
        ./gen-vs2022.bat
        ```
    - MinGW make files:
        ```sh
        ./gen-make-%compiler%.bat
        ```

### Linux

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Nano.git
    cd Nano
    ```

2. Navigate to the scripts folder:
    ```sh
    cd scripts/linux
    ```

3. (Optional) If you haven't already installed the premake5 build system you can install it like this:
    ```sh
    chmod +x install-premake5.sh
    ./install-premake5.sh
    ```

4. Generate make files:
    ```sh
    chmod +x gen-make-%compiler%.sh
    ./gen-make-%compiler%.sh
    ```

### MacOS

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Nano.git
    cd Nano
    ```

2. Navigate to the scripts folder:
    ```sh
    cd scripts/macos
    ```

3. (Optional) If you haven't already installed the premake5 build system you can install it like this:
    ```sh
    chmod +x install-premake5.sh
    ./install-premake5.sh
    ```

4. Generate make files:
    ```sh
    chmod +x gen-xcode.sh
    ./gen-xcode.sh
    ```

## Building

### Windows
- Visual Studio 17 2022:
    1. Navigate to the root of the directory
    2. Open the Nano.sln file
    3. Start building in your desired configuration
    4. Build files can be in the bin/%Config%-windows/Sandbox/ folder.
    5. (Optional) Open a terminal and run the Sandbox project:

        ```sh
        ./Sandbox.exe 
        ```

- MinGW Make:
    1. Navigate to the root of the directory
    2. Open a terminal.
    3. Call make with desired configuration (debug, release or dist):

        ```sh
        make config=release
        ```

    5. Build files can be in the bin/%Config%-linux/Sandbox/ folder.
    6. (Optional) Open a terminal and run the Sandbox project:
        ```sh
        ./Sandbox.exe 
        ```

### Linux

1. Navigate to the root of the directory
2. Open a terminal
3. Call make with desired configuration (debug, release or dist):

    ```sh
    make config=release
    ```

5. Build files can be in the bin/%Config%-linux/Sandbox/ folder.
6. (Optional) Open a terminal and run the Sandbox project:

    ```sh
    chmod +x Sandbox
    ./Sandbox
    ```

### MacOS
1. Navigate to the root of the directory
2. Open the Nano.xcworkspace file
3. Start building in your desired configuration
4. Build files can be in the bin/%Config%-macosx/Sandbox/ folder.
5. (Optional) Open a terminal and run the Sandbox project:

    ```sh
    ./Sandbox
    ```