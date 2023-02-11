$root = Get-Item -Path ".\"
$debugOrRelease = "release"
$publicPath = Get-Item -Path "$root\cmake-build-$debugOrRelease\src\public"

function runApplications {
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
    cd "$publicPath\client\"
    Start-Process -NoNewWindow ".\publicClient.exe"
    
    cd $root
}

runApplications