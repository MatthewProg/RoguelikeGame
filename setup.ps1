Write-Host "Setting up project..."
$sfml = Read-Host "Please insert SFML folder path: "
$sfml = $sfml.Replace('/', '\')
if(-not($sfml.EndsWith('\'))){
    $sfml = $sfml+"\"
}

function Get-File-Exists {
    param (
        [Parameter(Mandatory=$true, Position=0)]
        [string] $FilePath, 
        [Parameter(Mandatory=$true, Position=1)]
        [bool] $isFile
    )

    if($isFile) {
        if(-not(Test-Path -Path ($sfml+$FilePath) -PathType Leaf)) {
            Write-Host "File '$FilePath' doesn't exist!`nPress any key to continue..."
            $null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
            exit
        } else {
            Write-Host "'$FilePath' OK"
        }
    } else {
        if(-not(Test-Path -Path ($sfml+$FilePath))) {
            Write-Host "Directory '$FilePath' doesn't exist!`nPress any key to continue..."
            $null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
            exit
        } else {
            Write-Host "'$FilePath' OK"
        }
    }
}

Write-Host "`nChecking path.."
Get-File-Exists "include" $false
Get-File-Exists "lib/sfml-audio.lib" $true
Get-File-Exists "lib/sfml-graphics.lib" $true
Get-File-Exists "lib/sfml-window.lib" $true
Get-File-Exists "lib/sfml-system.lib" $true
Get-File-Exists "lib/sfml-audio-d.lib" $true
Get-File-Exists "lib/sfml-graphics-d.lib" $true
Get-File-Exists "lib/sfml-window-d.lib" $true
Get-File-Exists "lib/sfml-system-d.lib" $true
Get-File-Exists "bin/openal32.dll" $true
Get-File-Exists "bin/sfml-audio-d-2.dll" $true
Get-File-Exists "bin/sfml-graphics-d-2.dll" $true
Get-File-Exists "bin/sfml-window-d-2.dll" $true
Get-File-Exists "bin/sfml-system-d-2.dll" $true

$sfml = $sfml.Substring(0, $sfml.Length-1)
if(Test-Path -Path "./sfml-file" -PathType Leaf) {
    Write-Host "`nRemoving old files.."
    Remove-Item "./sfml-file"
} 
Write-Host "`nSaving sfml path..."
$sfml = $sfml.Replace('\', '\\')
$sfml | Set-Content "./sfml-file"
$sfml = $sfml.Replace('\\', '\')

Write-Host "`nSetting up project file..."
(Get-Content ".\RoguelikeGame.Main\RoguelikeGame.Main.vcxproj") -replace '{SFML_PATH}', $sfml | Out-File -encoding ASCII ".\RoguelikeGame.Main\RoguelikeGame.Main.vcxproj"

$sfml = $sfml + "\"
Write-Host "`nCopying files..."
Copy-Item ($sfml+"bin\openal32.dll") -Destination ".\RoguelikeGame.Main\"
Copy-Item ($sfml+"bin\sfml-audio-d-2.dll") -Destination ".\RoguelikeGame.Main\"
Copy-Item ($sfml+"bin\sfml-graphics-d-2.dll") -Destination ".\RoguelikeGame.Main\"
Copy-Item ($sfml+"bin\sfml-window-d-2.dll") -Destination ".\RoguelikeGame.Main\"
Copy-Item ($sfml+"bin\sfml-system-d-2.dll") -Destination ".\RoguelikeGame.Main\"

Write-Host "`nCreating Git Hooks..."
Write-Host " - Pre-commit"
Set-Content ".\.git\hooks\pre-commit" '#!/bin/sh'
Add-Content ".\.git\hooks\pre-commit" 'if [ -f "sfml-file" ] '
Add-Content ".\.git\hooks\pre-commit" 'then'
Add-Content ".\.git\hooks\pre-commit" 'search=`cat sfml-file`'
Add-Content ".\.git\hooks\pre-commit" 'replace="{SFML_PATH}"'
Add-Content ".\.git\hooks\pre-commit" 'sed -i ''s#''$search''#''$replace''#g'' ./RoguelikeGame.Main/RoguelikeGame.Main.vcxproj'
Add-Content ".\.git\hooks\pre-commit" 'unix2dos ./RoguelikeGame.Main/RoguelikeGame.Main.vcxproj'
Add-Content ".\.git\hooks\pre-commit" 'git add ./RoguelikeGame.Main/RoguelikeGame.Main.vcxproj'
Add-Content ".\.git\hooks\pre-commit" 'fi'

Write-Host " - Post-commit"
Set-Content ".\.git\hooks\post-commit" '#!/bin/sh'
Add-Content ".\.git\hooks\post-commit" 'if [ -f "sfml-file" ] '
Add-Content ".\.git\hooks\post-commit" 'then'
Add-Content ".\.git\hooks\post-commit" 'replace=`cat sfml-file`'
Add-Content ".\.git\hooks\post-commit" 'search="{SFML_PATH}"'
Add-Content ".\.git\hooks\post-commit" 'sed -i ''s#''$search''#''$replace''#g'' ./RoguelikeGame.Main/RoguelikeGame.Main.vcxproj'
Add-Content ".\.git\hooks\post-commit" 'unix2dos ./RoguelikeGame.Main/RoguelikeGame.Main.vcxproj'
Add-Content ".\.git\hooks\post-commit" 'echo "----- INFO -----"'
Add-Content ".\.git\hooks\post-commit" 'echo "SFML values changed in commit, please ignore vcxproj modified flag!"'
Add-Content ".\.git\hooks\post-commit" 'echo "----------------"'
Add-Content ".\.git\hooks\post-commit" 'fi'

Write-Host "`nDone`nPress any key to continue..."
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
exit