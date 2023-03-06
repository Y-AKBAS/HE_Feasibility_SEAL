# Run this script from the root of the application

# General Paths
$root = Get-Item -Path ".\"
$buildType = "relwithdebinfo"
$secretPath = Get-Item -Path "$root\cmake-build-$buildType\src\secret"
$publicPath = Get-Item -Path "$root\cmake-build-$buildType\src\public"
$benchmarkPath = Get-Item -Path "$root\cmake-build-$buildType\src\benchmark"

# App Paths
# vtune
$vtuneAppPath = "C:\Program Files (x86)\Intel\oneAPI\vtune\latest\bin64"
# secret app paths
$secretPlatformAppPath = "$secretPath\platform"
$secretMobilityProviderAppPath = "$secretPath\mobility_provider"
$secretInvoiceClerkAppPath = "$secretPath\invoice_clerk"
$secretTransporterAppPath = "$secretPath\transporter"
# public app paths
$publicPlatformAppPath = "$publicPath\platform"
$publicMobilityProviderAppPath = "$publicPath\mobility_provider"
$publicInvoiceClerkAppPath = "$publicPath\invoice_clerk\"
$publicTransporterAppPath = "$publicPath\transporter"
# benchmark app path
$benchmarkAppPath = $benchmarkPath

# App names
# vtune
$vtuneName = "vtune"
# secret app names
$secretPlatformName = "secretPlatform"
$secretMobilityProviderName = "secretMobilityProvider"
$secretInvoiceClerkName = "secretInvoiceClerk"
$secretTransporterName = "secretTransporter"
# public app names
$publicPlatformName = "publicPlatform"
$publicMobilityProviderName = "publicMobilityProvider"
$publicInvoiceClerkName = "publicInvoiceClerk"
$publicTransporterName = "publicTransporter"
# benchmark app name
$benchmarkName = "benchmark"

# Apps
# vtune
$vtuneApp = "$vtuneAppPath\$vtuneName"
$vtuneApp
# secret apps
$secretPlatformApp = "$secretPlatformAppPath\$secretPlatformName"
$secretMobilityProviderApp = "$secretMobilityProviderAppPath\$secretMobilityProviderName"
$secretInvoiceClerkApp = "$secretInvoiceClerkAppPath\$secretInvoiceClerkName"
$secretTransporterApp = "$secretTransporterAppPath\$secretTransporterName"
# public apps
$publicPlatformApp = "$publicPlatformAppPath\$publicPlatformName"
$publicMobilityProviderApp = "$publicMobilityProviderAppPath\$publicMobilityProviderName"
$publicInvoiceClerkApp = "$publicInvoiceClerkAppPath\$publicInvoiceClerkName"
$publicTransporterApp = "$publicTransporterAppPath\$publicTransporterName"
# benchmark app
$benchmarkApp = "$benchmarkAppPath\$benchmarkName"

# parameters
$scheme = 3 # enum class scheme_type {None, bfv, ckks, bgv}
$batchingEnabled = $false
$timeUnit = 2  # enum TimeUnit { kNanosecond, kMicrosecond, kMillisecond, kSecond };
$numberOfRequest = 500
$isSecret = $true
$isContext = $false

$schemeArg = "--st $scheme"
$batchingEnabledArg = "--ee $batchingEnabled"

# set report file path
if ($isSecret) {
    if ($isContext) {
        $reportFile = "$root\report\bench_context_$numberOfRequest"
    }
    else {
        $reportFile = "$root\report\bench_'$scheme'_'$batchingEnabled'_$numberOfRequest"
    }
}
else {
    $reportFile = "$root\report\bench_public_$numberOfRequest"
}

# set application args
$appArgs = @(
    $schemeArg,
    $batchingEnabledArg
)

# set collection type
$vtuneArgs = @(
    "-collect hotspots"
)

# set benchmark args
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

Write-Host "Benchmark Args: "
$benchmarkArgs

function runSecrets {
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$secretInvoiceClerkAppPath -- $secretInvoiceClerkApp $appArgs"
    Start-Sleep -Seconds 5
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$secretMobilityProviderAppPath -- $secretMobilityProviderApp $appArgs"
    Start-Sleep -Seconds 5
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$secretTransporterAppPath -- $secretTransporterApp $appArgs"
    Start-Sleep -Seconds 5
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$secretPlatformAppPath -- $secretPlatformApp $appArgs"
    Start-Sleep -Seconds 3
}

function runPublics {
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$publicInvoiceClerkAppPath -- $publicInvoiceClerkApp"
    Start-Sleep -Seconds 5
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$publicMobilityProviderAppPath -- $publicMobilityProviderApp"
    Start-Sleep -Seconds 5
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$publicTransporterAppPath -- $publicTransporterApp"
    Start-Sleep -Seconds 5
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$publicPlatformAppPath -- $publicPlatformApp"
    Start-Sleep -Seconds 3
}

function runBenchmark {
    Start-Sleep -Seconds 3
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "$vtuneArgs --app-working-dir=$benchmarkAppPath -- $benchmarkApp $benchmarkArgs"
    Start-Sleep -Seconds 5
}

if ($isSecret) {
    if ($isContext) {
        Write-Host "Running Context Benchmarks....."
    }
    else {
        Write-Host "Running Secret Benchmarks....."
        runSecrets
    }
}
else {
    Write-Host "Running Public Benchmarks....."
    runPublics
}

runBenchmark

Wait-Process -Name $benchmarkName
Start-Sleep -Seconds 5

function stopPublicAppsForced {
    Stop-Process -Name $publicPlatformName
    Stop-Process -Name $publicMobilityProviderName
    Stop-Process -Name $publicTransporterName
    Stop-Process -Name $publicInvoiceClerkName
}

function stopPublicApps {
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "-r $root\r000hs -command stop"
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "-r $root\r001hs -command stop"
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "-r $root\r002hs -command stop"
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "-r $root\r003hs -command stop"
}

function stopSecretAppsForced {
    Stop-Process -Name $secretPlatformName
    Stop-Process -Name $secretMobilityProviderName
    Stop-Process -Name $secretTransporterName
    Stop-Process -Name $secretInvoiceClerkName
}

function stopSecretApps {
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "-r $root\r000hs -command stop"
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "-r $root\r001hs -command stop"
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "-r $root\r002hs -command stop"
    Start-Process $vtuneApp -Verb RunAs -ArgumentList "-r $root\r003hs -command stop"
}

if ($isSecret) {
    Write-Host "Shut down secret apps"
    stopSecretApps
    #stopSecretAppsForced
}
else {
    Write-Host "Shut down public apps"
    stopPublicApps
    #stopPublicAppsForced
}
