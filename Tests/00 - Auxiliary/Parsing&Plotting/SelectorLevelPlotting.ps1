echo "Start!"

Copy-item 'Parser.exe' -Destination '01-RWSel+/Parser.exe' -force
Copy-item 'Plot.plt' -Destination '01-RWSel+/Plot.plt' -force
Copy-item 'Parse&Plot.ps1' -Destination '01-RWSel+/Parse&Plot.ps1' -force
Copy-item 'MutLevelPlotting.ps1' -Destination '01-RWSel+/MutLevelPlotting.ps1' -force
Copy-item 'XOverLevelPlotting.ps1' -Destination '01-RWSel+/XOverLevelPlotting.ps1' -force

Copy-item 'Parser.exe' -Destination '02-TournSel+/Parser.exe' -force
Copy-item 'Plot.plt' -Destination '02-TournSel+/Plot.plt' -force
Copy-item 'Parse&Plot.ps1' -Destination '02-TournSel+/Parse&Plot.ps1' -force
Copy-item 'MutLevelPlotting.ps1' -Destination '02-TournSel+/MutLevelPlotting.ps1' -force
Copy-item 'XOverLevelPlotting.ps1' -Destination '02-TournSel+/XOverLevelPlotting.ps1' -force

cd '01-RWSel+'
& .\'XOverLevelPlotting.ps1'
cd..

cd '02-TournSel+'
& .\'XOverLevelPlotting.ps1'
cd..

Remove-item 'MutLevelPlotting.ps1'
Remove-item 'Parser.exe'
Remove-item 'Plot.plt'
Remove-item 'Parse&Plot.ps1'
Remove-item 'XOverLevelPlotting.ps1'
Remove-item 'SelectorLevelPlotting.ps1'

echo "Done!"