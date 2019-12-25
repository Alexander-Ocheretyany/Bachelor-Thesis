echo "MutationLevelScript has been started"
Copy-Item 'Input.txt' -Destination '01-Mut(0.4)'
Copy-Item 'Input_VC_IS.txt' -Destination '01-Mut(0.4)'
Copy-Item 'GA.exe' -Destination '01-Mut(0.4)'
Copy-Item 'Parser.exe' -Destination '01-Mut(0.4)'
Copy-Item 'PopulationLevelScript.ps1' -Destination '01-Mut(0.4)'

cd '01-Mut(0.4)'
& .\'PopulationLevelScript.ps1'
cd ..
Rename-Item '01-Mut(0.4)' '01-Mut(0.4)+'

Copy-Item 'Input.txt' -Destination '02-Mut(0.2)'
Copy-Item 'Input_VC_IS.txt' -Destination '02-Mut(0.2)'
Copy-Item 'GA.exe' -Destination '02-Mut(0.2)'
Copy-Item 'Parser.exe' -Destination '02-Mut(0.2)'
Copy-Item 'PopulationLevelScript.ps1' -Destination '02-Mut(0.2)'

cd '02-Mut(0.2)'
& .\'PopulationLevelScript.ps1'
cd ..
Rename-Item '02-Mut(0.2)' '02-Mut(0.2)+'

Remove-Item 'Input.txt'
Remove-Item 'Input_VC_IS.txt'
Remove-Item 'GA.exe'
Remove-Item 'Parser.exe'
Remove-Item 'PopulationLevelScript.ps1'
Remove-Item 'MutationLevelScript.ps1'
"Done!"