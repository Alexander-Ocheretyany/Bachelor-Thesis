echo "XOverLevelScript has been started"
Copy-Item 'Input.txt' -Destination '01-XOver (0.9)'
Copy-Item 'Input_VC_IS.txt' -Destination '01-XOver (0.9)'
Copy-Item 'GA.exe' -Destination '01-XOver (0.9)'
Copy-Item 'Parser.exe' -Destination '01-XOver (0.9)'
Copy-Item 'PopulationLevelScript.ps1' -Destination '01-XOver (0.9)'
Copy-Item 'MutationLevelScript.ps1' -Destination '01-XOver (0.9)'

cd '01-XOver (0.9)'
& .\'MutationLevelScript.ps1'
cd ..
Rename-Item '01-XOver (0.9)' '01-XOver (0.9)+'



Copy-Item 'Input.txt' -Destination '02-XOver (0.8)'
Copy-Item 'Input_VC_IS.txt' -Destination '02-XOver (0.8)'
Copy-Item 'GA.exe' -Destination '02-XOver (0.8)'
Copy-Item 'Parser.exe' -Destination '02-XOver (0.8)'
Copy-Item 'PopulationLevelScript.ps1' -Destination '02-XOver (0.8)'
Copy-Item 'MutationLevelScript.ps1' -Destination '02-XOver (0.8)'

cd '02-XOver (0.8)'
& .\'MutationLevelScript.ps1'
cd ..
Rename-Item '02-XOver (0.8)' '02-XOver (0.8)+'

Remove-Item 'Input.txt'
Remove-Item 'Input_VC_IS.txt'
Remove-Item 'GA.exe'
Remove-Item 'Parser.exe'
Remove-Item 'PopulationLevelScript.ps1'
Remove-Item 'MutationLevelScript.ps1'
Remove-Item 'XOverLevelScript.ps1'
echo "Done!"