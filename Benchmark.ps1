$root = Get-Item -Path ".\"

$debugOrRelease = "release"
$secretPath = Get-Item -Path "$root\cmake-build-$debugOrRelease\src\secret"
$publicPath = Get-Item -Path "$root\cmake-build-$debugOrRelease\src\public"
$benchmarkPath = Get-Item -Path "$root\cmake-build-$debugOrRelease\src\benchmark"

$scheme = 2
$batchingEnabled = "true"
$timeUnit = 2  # enum TimeUnit { kNanosecond, kMicrosecond, kMillisecond, kSecond };
$numberOfRequest = 1000
$isSecret = $true

if($isSecret){
    $reportFile = "$root\report\bench_'$scheme'_$batchingEnabled_$numberOfRequest"
} else {
    $reportFile = "$root\report\bench_public_$numberOfRequest"
}

$schemeArg = "--st $scheme"
$batchingEnabledArg = "--ee $batchingEnabled"
$timeUnitArg = "--tu $timeUnit"
$numberOfRequestArg = "--nor $numberOfRequest"
$isSecretArg = "--is $isSecret"
$benchmarkArgs = "$schemeArg $batchingEnabledArg $timeUnitArg $numberOfRequestArg $isSecretArg --benchmark_format=console --benchmark_out_format=json --benchmark_out=$reportFile"

Start-Sleep -Seconds 5

function runSecrets {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true, Position = 0)]
        [string]$schemeArg,

        [Parameter(Mandatory = $true, Position = 1)]
        [string]$batchingEnabledArg
    )

    cd "$secretPath\invoice_clerk\"
    Start-Process -NoNewWindow ".\secretInvoiceClerk.exe" -ArgumentList $schemeArg, $batchingEnabledArg
    Start-Sleep -Seconds 8
    cd "$secretPath\mobility_provider\"
    Start-Process -NoNewWindow ".\secretMobilityProvider.exe" -ArgumentList $schemeArg, $batchingEnabledArg
    Start-Sleep -Seconds 8
    cd "$secretPath\transporter\"
    Start-Process -NoNewWindow ".\secretTransporter.exe" -ArgumentList $schemeArg, $batchingEnabledArg
    Start-Sleep -Seconds 8
    cd "$secretPath\platform\"
    Start-Process -NoNewWindow ".\secretPlatform.exe" -ArgumentList $schemeArg, $batchingEnabledArg
    cd $root
    Start-Sleep -Seconds 10

    cd $root
}

function runPublics {
    cd "$publicPath\invoice_clerk\"
    Start-Process -NoNewWindow ".\publicInvoiceClerk.exe"
    Start-Sleep -Seconds 8
    cd "$publicPath\mobility_provider\"
    Start-Process -NoNewWindow ".\publicMobilityProvider.exe"
    Start-Sleep -Seconds 8
    cd "$publicPath\transporter\"
    Start-Process -NoNewWindow ".\publicTransporter.exe"
    Start-Sleep -Seconds 8
    cd "$publicPath\platform\"
    Start-Process -NoNewWindow ".\publicPlatform.exe"
    Start-Sleep -Seconds 8
    
    cd $root
}

function runBenchmark {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true, Position = 0)]
        [string]$benchmarkArgs
    )

    Start-Sleep -Seconds 10
    cd "$benchmarkPath\"
    Start-Process -NoNewWindow ".\benchmark.exe" -ArgumentList $benchmarkArgs

    cd $root
}

if ($isSecret) {
    Write-Host "Running Secret Benchmark....."
    runSecrets -schemeArg $schemeArg -batchingEnabledArg $batchingEnabledArg
}
else {
    Write-Host "Running Public Benchmark....."
    runPublics
}

runBenchmark -benchmarkArgs $benchmarkArgs
