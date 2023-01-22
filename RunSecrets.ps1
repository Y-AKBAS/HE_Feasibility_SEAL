$secretPath = Get-Item -Path ".\cmake-build-debug\src\secret"

cd "$secretPath\invoice_clerk\"
Start-Process -NoNewWindow ".\secretInvoiceClerk.exe"
Start-Sleep -Seconds 8
cd "$secretPath\mobility_provider\"
Start-Process -NoNewWindow ".\secretMobilityProvider.exe"
Start-Sleep -Seconds 8
cd "$secretPath\transporter\"
Start-Process -NoNewWindow ".\secretTransporter.exe"
Start-Sleep -Seconds 8
cd "$secretPath\platform\"
Start-Process -NoNewWindow ".\secretPlatform.exe"
Start-Sleep -Seconds 8
cd "$secretPath\client\"
Start-Process -NoNewWindow ".\secretClient.exe"
