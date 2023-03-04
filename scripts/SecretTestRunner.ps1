# Run this script from the root of the application

# General Paths
$root = Get-Item -Path ".\"
$buildType = "relwithdebinfo"
$secretPath = Get-Item -Path "$root\cmake-build-$buildType\src\secret"

$scheme = 3
$batchingEnabled = $false

$schemeArg = "--st $scheme"
$batchingEnabledArg = "--ee $batchingEnabled"

function runApplications {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true, Position = 0)]
        [string]$schemeArg,

        [Parameter(Mandatory = $true, Position = 1)]
        [string]$batchingEnabledArg
    )

cd "$secretPath\invoice_clerk\"
Start-Process -NoNewWindow ".\secretInvoiceClerk.exe" -ArgumentList $schemeArg,$batchingEnabledArg 
Start-Sleep -Seconds 5
cd "$secretPath\mobility_provider\"
Start-Process -NoNewWindow ".\secretMobilityProvider.exe" -ArgumentList $schemeArg,$batchingEnabledArg
Start-Sleep -Seconds 5
cd "$secretPath\transporter\"
Start-Process -NoNewWindow ".\secretTransporter.exe" -ArgumentList $schemeArg,$batchingEnabledArg 
Start-Sleep -Seconds 5
cd "$secretPath\platform\"
Start-Process -NoNewWindow ".\secretPlatform.exe" -ArgumentList $schemeArg,$batchingEnabledArg 
cd $root
Start-Sleep -Seconds 5
cd "$secretPath\client\"
Start-Process -NoNewWindow ".\secretClient.exe" -ArgumentList $schemeArg,$batchingEnabledArg 

cd $root
}

runApplications -schemeArg $schemeArg -batchingEnabledArg $batchingEnabledArg
