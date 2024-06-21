$DebugPreference = "Continue"

function Get-UninstallKeys {
    param ([string] $uninstallKeyPath)
    try {
        Get-ChildItem -Path $uninstallKeyPath | ForEach-Object {
            $keyProperties = Get-ItemProperty -Path $_.PSPath
            $displayName = $keyProperties.DisplayName
            $displayVersion = $keyProperties.DisplayVersion
            if ($displayName) {
                Write-Host "Application: $displayName, Version: $displayVersion"
            }
        }
    }
    catch {
        Write-Warning "An error occurred during Get-UninstallKeys: $_"
    }
}

function Load-RegistryHive {
    param ([string] $tempHiveName, [string] $hivePath)

    reg load "HKU\$tempHiveName" $hivePath | Out-Null
    if ($LASTEXITCODE -eq 0) {
        Write-Debug "Loaded the registry hive: $hivePath"
    } else {
        Write-Warning "Failed to load the registry hive: $hivePath"
    }
}

function Unload-RegistryHive {
    param ([string] $tempHiveName)

    # ensure all handles to the registry hive are properly closed or unloading will fail
    [GC]::Collect()
    [GC]::WaitForPendingFinalizers()

    reg unload "HKU\$tempHiveName" | Out-Null
    if ($LASTEXITCODE -eq 0) {
        Write-Debug "Unloaded the registry hive: $tempHiveName"
    } else {
        Write-Warning "Failed to unload the registry hive."
    }
}

function Read-UninstallKeys {
    param ([string] $username, [string] $userProfilePath, [string] $hivePath, [string] $tempHiveName)

    Load-RegistryHive -tempHiveName $tempHiveName -hivePath $hivePath
    try {
        Get-UninstallKeys -uninstallKeyPath "Registry::HKEY_USERS\$tempHiveName\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"    
    }
    catch {
        Write-Warning "An error occurred during Read-UninstallKeys: $_"
    }
    finally {
        Unload-RegistryHive -tempHiveName $tempHiveName
    }
}

# ----------------------------
# MAIN
# ----------------------------

# Specify the username of the user whose uninstall keys you want to read
$username = "someuser"
$userProfilePath = "C:\Users\$username"
$hivePath = "$userProfilePath\NTUSER.DAT"
$tempHiveName = [System.Guid]::NewGuid().ToString()

# create copy of ntuser.dat file to avoid file lock issues
$tempHivePath = "$env:TEMP\$tempHiveName"
Copy-Item -Path $hivePath -Destination $tempHivePath -Force

Read-UninstallKeys -username $username -userProfilePath $userProfilePath -hivePath $tempHivePath -tempHiveName $tempHiveName

# remove the temporary hive file
Remove-Item -Path $tempHivePath -Force