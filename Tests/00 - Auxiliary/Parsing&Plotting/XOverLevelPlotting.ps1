echo "Start!"

Copy-item 'Parser.exe' -Destination '01-XOver (0.9)+/Parser.exe' -force
Copy-item 'Plot.plt' -Destination '01-XOver (0.9)+/Plot.plt' -force
Copy-item 'Parse&Plot.ps1' -Destination '01-XOver (0.9)+/Parse&Plot.ps1' -force
Copy-item 'MutLevelPlotting.ps1' -Destination '01-XOver (0.9)+/MutLevelPlotting.ps1' -force

Copy-item 'Parser.exe' -Destination '02-XOver (0.8)+/Parser.exe' -force
Copy-item 'Plot.plt' -Destination '02-XOver (0.8)+/Plot.plt' -force
Copy-item 'Parse&Plot.ps1' -Destination '02-XOver (0.8)+/Parse&Plot.ps1' -force
Copy-item 'MutLevelPlotting.ps1' -Destination '02-XOver (0.8)+/MutLevelPlotting.ps1' -force

cd '01-XOver (0.9)+'
& .\'MutLevelPlotting.ps1'
cd..

cd '02-XOver (0.8)+'
& .\'MutLevelPlotting.ps1'
cd..

Remove-item 'MutLevelPlotting.ps1'
Remove-item 'Parser.exe'
Remove-item 'Plot.plt'
Remove-item 'Parse&Plot.ps1'
Remove-item 'XOverLevelPlotting.ps1'

echo "Done!"