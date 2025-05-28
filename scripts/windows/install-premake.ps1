# install-premake.ps1

# Check for Admin Rights
if (-not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()
    ).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "Restarting as Administrator..."
    Start-Process powershell "-ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs
    exit
}

# Variables
$version = '5.0.0-beta6'
$url     = "https://github.com/premake/premake-core/releases/download/v$version/premake-$version-windows.zip"
$tempZip = "$env:TEMP\premake-$version.zip"
$tempDir = "$env:TEMP\premake-$version"
$installDir = 'C:\Program Files\Premake\bin'
$exeName = 'premake5.exe'

Write-Host "Downloading Premake $version..."
Invoke-WebRequest -Uri $url -OutFile $tempZip

Write-Host "Extracting..."
# Clean up any previous temp folder
if (Test-Path $tempDir) { Remove-Item -Recurse -Force $tempDir }
Expand-Archive -Path $tempZip -DestinationPath $tempDir

# Ensure install directory exists
if (-not (Test-Path $installDir)) {
    Write-Host "Creating install directory at $installDir..."
    New-Item -ItemType Directory -Path $installDir -Force | Out-Null
}

# Copy the EXE
$sourceExe = Join-Path $tempDir $exeName
$destExe   = Join-Path $installDir $exeName

Write-Host "Copying $exeName to $installDir..."
Copy-Item -Path $sourceExe -Destination $destExe -Force

# Optional: add to system PATH if not already present
$path = [Environment]::GetEnvironmentVariable('Path', 'Machine')
if ($path -notlike "*$installDir*") {
    Write-Host "Adding $installDir to system PATH..."
    $newPath = "$path;$installDir"
    [Environment]::SetEnvironmentVariable('Path', $newPath, 'Machine')
}

# Cleanup
Write-Host "Cleaning up temporary files..."
Remove-Item $tempZip, $tempDir -Recurse -Force

Write-Host "Premake5 v$version has been installed to $installDir."
Write-Host "You may need to restart your shell for PATH changes to take effect."

Read-Host -Prompt "Press Enter to exit"