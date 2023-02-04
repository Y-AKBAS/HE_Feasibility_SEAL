$root = Get-Item -Path ".\"
$reportFile = "$root\result"
$secretPath = Get-Item -Path "$root\cmake-build-debug\src\secret"
$publicPath = Get-Item -Path "$root\cmake-build-debug\src\public"
$benchmarkPath = Get-Item -Path "$root\cmake-build-debug\src\benchmark"

$scheme = 1
$batchingEnabled = "false"
$timeUnit = 2

$schemeArg = "--st $scheme"
$batchingEnabledArg = "--ee $batchingEnabled"
$timeUnitArg = "--tu $timeUnit"
$benchmarkArgs = "$timeUnitArg --benchmark_format=console --benchmark_out_format=json --benchmark_out=$reportFile"

function runSecrets {
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

cd $root
}

function runPublics {
    cd "$publicPath\invoice_clerk\"
    Start-Process -NoNewWindow ".\publicInvoiceClerk.exe"
    Start-Sleep -Seconds 3
    cd "$publicPath\mobility_provider\"
    Start-Process -NoNewWindow ".\publicMobilityProvider.exe"
    Start-Sleep -Seconds 3
    cd "$publicPath\transporter\"
    Start-Process -NoNewWindow ".\publicTransporter.exe"
    Start-Sleep -Seconds 3
    cd "$publicPath\platform\"
    Start-Process -NoNewWindow ".\publicPlatform.exe"
    Start-Sleep -Seconds 5
    
    cd $root
}


function runBenchmark {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true, Position = 0)]
        [string]$schemeArg,

        [Parameter(Mandatory = $true, Position = 1)]
        [string]$batchingEnabledArg,

        [Parameter(Mandatory = $true, Position = 2)]
        [string]$benchmarkArgs
    )

Start-Sleep -Seconds 10
cd "$benchmarkPath\"
Start-Process -NoNewWindow ".\benchmark.exe" -ArgumentList $schemeArg,$batchingEnabledArg,$benchmarkArgs

cd $root
}

runPublics
#runSecrets -schemeArg $schemeArg -batchingEnabledArg $batchingEnabledArg
#runBenchmark -schemeArg $schemeArg -batchingEnabledArg $batchingEnabledArg -benchmarkArgs $benchmarkArgs
