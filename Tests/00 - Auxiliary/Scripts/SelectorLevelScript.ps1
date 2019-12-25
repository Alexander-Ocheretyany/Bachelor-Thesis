echo "SelectorLevelScript has been started"

Copy-Item 'Graph+/Input.txt' 'Input.txt'
Copy-Item 'Graph+/Input_VC_IS.txt' 'Input_VC_IS.txt'

Copy-Item 'Input.txt' -Destination '01-RWSel'
Copy-Item 'Input_VC_IS.txt' -Destination '01-RWSel'
Copy-Item 'GA.exe' -Destination '01-RWSel'
Copy-Item 'Parser.exe' -Destination '01-RWSel'
Copy-Item 'PopulationLevelScript.ps1' -Destination '01-RWSel'
Copy-Item 'MutationLevelScript.ps1' -Destination '01-RWSel'
Copy-Item 'XOverLevelScript.ps1' -Destination '01-RWSel'

cd '01-RWSel'
& .\'XOverLevelScript.ps1'
cd ..
Rename-Item '01-RWSel' '01-RWSel+'



Copy-Item 'Input.txt' -Destination '02-TournSel'
Copy-Item 'Input_VC_IS.txt' -Destination '02-TournSel'
Copy-Item 'GA.exe' -Destination '02-TournSel'
Copy-Item 'Parser.exe' -Destination '02-TournSel'
Copy-Item 'PopulationLevelScript.ps1' -Destination '02-TournSel'
Copy-Item 'MutationLevelScript.ps1' -Destination '02-TournSel'
Copy-Item 'XOverLevelScript.ps1' -Destination '02-TournSel'

cd '02-TournSel'
& .\'XOverLevelScript.ps1'
cd ..
Rename-Item '02-TournSel' '02-TournSel+'

Remove-Item 'Input.txt'
Remove-Item 'Input_VC_IS.txt'
Remove-Item 'GA.exe'
Remove-Item 'Parser.exe'
Remove-Item 'PopulationLevelScript.ps1'
Remove-Item 'MutationLevelScript.ps1'
Remove-Item 'XOverLevelScript.ps1'
Remove-Item 'SelectorLevelScript.ps1'
echo "Done!"