# Script to retrieve the software that is installed in the user context instead of the machine context
# see original c++ implementation here: 
# https://github.com/fmuecke/WinApiHelper/blob/master/Examples/ExamineUserRegHives/ExamineUserRegHives.cpp
# (c) Florian Mücke 2023, MIT license

class UserProfile {
    [string] $sid
    [string] $path
    [string] $account
    [bool] $wasLoaded
}

function Get-UserProfiles {
    $accounts = Get-WmiObject Win32_UserAccount
    $profiles = Get-WmiObject Win32_UserProfile | Where-Object { $_.Special -eq $false} # skip special/service accounts
   
    $userProfiles = New-Object 'System.Collections.Generic.List[UserProfile]'

    foreach ($profile in $profiles) {
        
        # match account names with profiles
        $account = $accounts | Where-Object { $_.SID -eq $profile.SID }  
        
        # get required profile data
        $userProfile = [UserProfile]::new()
        $userProfile.sid = $account.SID
        $userProfile.account = $account.Caption
        $userProfile.path = $profile.LocalPath
        $userProfile.wasLoaded = $profile.loaded
        $userProfiles.Add($userProfile)
    }

    return $userProfiles
}

class UninstallData {
    [string] $user
    [string] $key
    [string] $displayName
    [string] $displayVersion
    [string] $publisher
}

function Get-UnistallData([UserProfile] $userProfile)
{
    # define a temporary registry path where the hive will be loaded.
    $tempRegPath = "HKU\$($userProfile.sid)"
    
    # load the user's registry hive.
    if (-not $userProfile.wasLoaded) {
        $profileFile = (Join-Path $userProfile.path "NTUSER.DAT")
        if (-not (Test-Path $profileFile)) {
            Write-Error "Profile hive could not be accessed: $profileFile"
            continue
        }
        reg load $tempRegPath $profileFile
    }

    # evaluate uninstall keys...
    $uninstallKeys = Get-ChildItem -LiteralPath "Registry::HKEY_USERS\$($userProfile.sid)\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
    foreach ($item in $uninstallKeys) {
        $sw = [UninstallData]::new()
        $sw.user = $userProfile.account
        $sw.key = (Split-Path -leaf $item.Name)
        $sw.displayName = (Get-ItemPropertyValue -Path "Registry::$($item.Name)" -Name DisplayName)
        $sw.displayVersion = (Get-ItemPropertyValue -Path "Registry::$($item.Name)" -Name DisplayVersion)
        $sw.publisher = (Get-ItemPropertyValue -Path "Registry::$($item.Name)" -Name Publisher)
        $sw
    }

    # unload the user's registry hive
    if (-not $userProfile.wasLoaded) {
        reg unload $tempRegPath
    }
}

###
### MAIN
###

$userSW = Get-UserProfiles | ForEach-Object { Get-UnistallData $_ }
$userSW