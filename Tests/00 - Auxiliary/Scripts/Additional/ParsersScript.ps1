cd '01 - Kill parents and transfer 10%'
Start-Process Parser -Wait
cd ..
cd '02 - Kill parents and transfer 5%'
Start-Process Parser -Wait
cd ..
cd '03 - Kill parents, no transfer'
Start-Process Parser -Wait
cd ..
cd '04 - Best and transfer 10%'
Start-Process Parser -Wait
cd ..
cd '05 - Best and transfer 5%'
Start-Process Parser -Wait
cd ..
cd '06 - Best, no transfer'
Start-Process Parser -Wait
cd ..

New-Item -Name "Data" -ItemType "directory"

Move-Item -Path '01 - Kill parents and transfer 10%/Table.txt' -Destination 'Data/01 - Kill parents and transfer 10%.txt'
Move-Item -Path '02 - Kill parents and transfer 5%/Table.txt' -Destination 'Data/02 - Kill parents and transfer 5%.txt'
Move-Item -Path '03 - Kill parents, no transfer/Table.txt' -Destination 'Data/03 - Kill parents, no transfer.txt'
Move-Item -Path '04 - Best and transfer 10%/Table.txt' -Destination 'Data/04 - Best and transfer 10%.txt'
Move-Item -Path '05 - Best and transfer 5%/Table.txt' -Destination 'Data/05 - Best and transfer 5%.txt'
Move-Item -Path '06 - Best, no transfer/Table.txt' -Destination 'Data/06 - Best, no transfer.txt'
echo "Done!"

echo "Removing auxiliary files..."
Remove-Item '01 - Kill parents and transfer 10%/GA.exe'
Remove-Item '02 - Kill parents and transfer 5%/GA.exe'
Remove-Item '03 - Kill parents, no transfer/GA.exe'
Remove-Item '04 - Best and transfer 10%/GA.exe'
Remove-Item '05 - Best and transfer 5%/GA.exe'
Remove-Item '06 - Best, no transfer/GA.exe'

Remove-Item '01 - Kill parents and transfer 10%/Parser.exe'
Remove-Item '02 - Kill parents and transfer 5%/Parser.exe'
Remove-Item '03 - Kill parents, no transfer/Parser.exe'
Remove-Item '04 - Best and transfer 10%/Parser.exe'
Remove-Item '05 - Best and transfer 5%/Parser.exe'
Remove-Item '06 - Best, no transfer/Parser.exe'

Remove-Item '01 - Kill parents and transfer 10%/Input.txt'
Remove-Item '02 - Kill parents and transfer 5%/Input.txt'
Remove-Item '03 - Kill parents, no transfer/Input.txt'
Remove-Item '04 - Best and transfer 10%/Input.txt'
Remove-Item '05 - Best and transfer 5%/Input.txt'
Remove-Item '06 - Best, no transfer/Input.txt'

Remove-Item '01 - Kill parents and transfer 10%/Input_VC_IS.txt'
Remove-Item '02 - Kill parents and transfer 5%/Input_VC_IS.txt'
Remove-Item '03 - Kill parents, no transfer/Input_VC_IS.txt'
Remove-Item '04 - Best and transfer 10%/Input_VC_IS.txt'
Remove-Item '05 - Best and transfer 5%/Input_VC_IS.txt'
Remove-Item '06 - Best, no transfer/Input_VC_IS.txt'

Remove-Item 'ParsersScript.ps1'