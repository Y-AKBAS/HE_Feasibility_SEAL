$root = Get-Item -Path ".\"
$secretPath = Get-Item -Path "$root\cmake-build-debug\src\secret"

$scheme = 3
$batchingEnabled = "false"

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
Start-Sleep -Seconds 8
cd "$secretPath\mobility_provider\"
Start-Process -NoNewWindow ".\secretMobilityProvider.exe" -ArgumentList $schemeArg,$batchingEnabledArg
Start-Sleep -Seconds 8
cd "$secretPath\transporter\"
Start-Process -NoNewWindow ".\secretTransporter.exe" -ArgumentList $schemeArg,$batchingEnabledArg 
Start-Sleep -Seconds 8
cd "$secretPath\platform\"
Start-Process -NoNewWindow ".\secretPlatform.exe" -ArgumentList $schemeArg,$batchingEnabledArg 
cd $root
Start-Sleep -Seconds 10
cd "$secretPath\client\"
Start-Process -NoNewWindow ".\secretClient.exe" -ArgumentList $schemeArg,$batchingEnabledArg 

cd $root
}

runApplications -schemeArg $schemeArg -batchingEnabledArg $batchingEnabledArg
