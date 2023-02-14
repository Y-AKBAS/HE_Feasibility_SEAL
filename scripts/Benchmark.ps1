$root = Get-Item -Path "..\"

$debugOrRelease = "release"
$secretPath = Get-Item -Path "$root\cmake-build-$debugOrRelease\src\secret"
$publicPath = Get-Item -Path "$root\cmake-build-$debugOrRelease\src\public"
$benchmarkPath = Get-Item -Path "$root\cmake-build-$debugOrRelease\src\benchmark"

$scheme = 3
$batchingEnabled = $false
$timeUnit = 2  # enum TimeUnit { kNanosecond, kMicrosecond, kMillisecond, kSecond };
$numberOfRequest = 5000
$isSecret = $false
$isContext = $false

if ($isSecret) {
    if ($isContext) {
        $reportFile = "$root\report\bench_contex_$numberOfRequest"
    }
    else {
        $reportFile = "$root\report\bench_'$scheme'_'$batchingEnabled'_$numberOfRequest"
    }
}
else {
    $reportFile = "$root\report\bench_public_$numberOfRequest"
}

$schemeArg = "--st $scheme"
$batchingEnabledArg = "--ee $batchingEnabled"

$benchmarkArgs = @(
    $schemeArg,
    $batchingEnabledArg,
    "--tu $timeUnit",
    "--nor $numberOfRequest",
    "--is $isSecret",
    "--ic $isContext",
    "--benchmark_format=console",
    "--benchmark_out_format=json",
    "--benchmark_out=$reportFile"
)

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
        [array]$benchmarkArgs
    )

    Start-Sleep -Seconds 10
    cd "$benchmarkPath\"
    Start-Process -NoNewWindow ".\benchmark.exe" -ArgumentList $benchmarkArgs

    cd $root
}

if ($isSecret) {
    if ($isContext) {
        Write-Host "Running Context Benchmarks....."
    }
    else {
        Write-Host "Running Secret Benchmarks....."
        runSecrets -schemeArg $schemeArg -batchingEnabledArg $batchingEnabledArg    
    }
}
else {
    Write-Host "Running Public Benchmarks....."
    runPublics
}

runBenchmark -benchmarkArgs $benchmarkArgs
