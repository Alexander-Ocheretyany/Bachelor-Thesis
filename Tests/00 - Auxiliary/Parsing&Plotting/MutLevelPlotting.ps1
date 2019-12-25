echo "Start!"

Copy-item 'Parser.exe' -Destination '01-Mut(0.4)+/Parser.exe' -force
Copy-item 'Plot.plt' -Destination '01-Mut(0.4)+/Plot.plt' -force
Copy-item 'Parse&Plot.ps1' -Destination '01-Mut(0.4)+/Parse&Plot.ps1' -force

Copy-item 'Parser.exe' -Destination '02-Mut(0.2)+/Parser.exe' -force
Copy-item 'Plot.plt' -Destination '02-Mut(0.2)+/Plot.plt' -force
Copy-item 'Parse&Plot.ps1' -Destination '02-Mut(0.2)+/Parse&Plot.ps1' -force

cd '01-Mut(0.4)+'
& .\'Parse&Plot.ps1'
cd..

cd '02-Mut(0.2)+'
& .\'Parse&Plot.ps1'
cd..

Remove-item 'MutLevelPlotting.ps1'
Remove-item 'Parser.exe'
Remove-item 'Plot.plt'
Remove-item 'Parse&Plot.ps1'

echo "Done!"