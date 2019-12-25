echo "Script has been started"

Copy-Item 'Parser.exe' -Destination '01-Pop(200)+/01 - Kill parents and transfer 10%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '01-Pop(200)+/02 - Kill parents and transfer 5%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '01-Pop(200)+/03 - Kill parents, no transfer' -Recurse -force
Copy-Item 'Parser.exe' -Destination '01-Pop(200)+/04 - Best and transfer 10%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '01-Pop(200)+/05 - Best and transfer 5%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '01-Pop(200)+/06 - Best, no transfer' -Recurse -force
New-Item -Name "01-Pop(200)+/Data" -ItemType "directory" -force
Copy-Item 'Plot.plt' -Destination '01-Pop(200)+/Data' -Recurse -force

Copy-Item 'Parser.exe' -Destination '02-Pop(100)+/01 - Kill parents and transfer 10%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '02-Pop(100)+/02 - Kill parents and transfer 5%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '02-Pop(100)+/03 - Kill parents, no transfer' -Recurse -force
Copy-Item 'Parser.exe' -Destination '02-Pop(100)+/04 - Best and transfer 10%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '02-Pop(100)+/05 - Best and transfer 5%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '02-Pop(100)+/06 - Best, no transfer' -Recurse -force
New-Item -Name "02-Pop(100)+/Data" -ItemType "directory" -force
Copy-Item 'Plot.plt' -Destination '02-Pop(100)+/Data' -Recurse -force

Copy-Item 'Parser.exe' -Destination '03-Pop(60)+/01 - Kill parents and transfer 10%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '03-Pop(60)+/02 - Kill parents and transfer 5%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '03-Pop(60)+/03 - Kill parents, no transfer' -Recurse -force
Copy-Item 'Parser.exe' -Destination '03-Pop(60)+/04 - Best and transfer 10%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '03-Pop(60)+/05 - Best and transfer 5%' -Recurse -force
Copy-Item 'Parser.exe' -Destination '03-Pop(60)+/06 - Best, no transfer' -Recurse -force
New-Item -Name "03-Pop(60)+/Data" -ItemType "directory" -force
Copy-Item 'Plot.plt' -Destination '03-Pop(60)+/Data' -Recurse -force

cd '01-Pop(200)+'

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

Move-Item -Path '01 - Kill parents and transfer 10%/Table.txt' -Destination 'Data/01 - Kill parents and transfer 10%.txt' -force
Move-Item -Path '02 - Kill parents and transfer 5%/Table.txt' -Destination 'Data/02 - Kill parents and transfer 5%.txt' -force
Move-Item -Path '03 - Kill parents, no transfer/Table.txt' -Destination 'Data/03 - Kill parents, no transfer.txt' -force
Move-Item -Path '04 - Best and transfer 10%/Table.txt' -Destination 'Data/04 - Best and transfer 10%.txt' -force
Move-Item -Path '05 - Best and transfer 5%/Table.txt' -Destination 'Data/05 - Best and transfer 5%.txt' -force
Move-Item -Path '06 - Best, no transfer/Table.txt' -Destination 'Data/06 - Best, no transfer.txt' -force

echo "Done!"

echo "Removing auxiliary files..."

Remove-Item '01 - Kill parents and transfer 10%/Parser.exe'
Remove-Item '02 - Kill parents and transfer 5%/Parser.exe'
Remove-Item '03 - Kill parents, no transfer/Parser.exe'
Remove-Item '04 - Best and transfer 10%/Parser.exe'
Remove-Item '05 - Best and transfer 5%/Parser.exe'
Remove-Item '06 - Best, no transfer/Parser.exe'

echo "Done!"

echo "Plotting..."

cd 'Data'
Start-Process gnuplot 'Plot.plt' -Wait
Remove-Item 'Plot.plt'

echo "Done!"

cd ..
cd ..

cd '02-Pop(100)+'

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

Move-Item -Path '01 - Kill parents and transfer 10%/Table.txt' -Destination 'Data/01 - Kill parents and transfer 10%.txt' -force
Move-Item -Path '02 - Kill parents and transfer 5%/Table.txt' -Destination 'Data/02 - Kill parents and transfer 5%.txt' -force
Move-Item -Path '03 - Kill parents, no transfer/Table.txt' -Destination 'Data/03 - Kill parents, no transfer.txt' -force
Move-Item -Path '04 - Best and transfer 10%/Table.txt' -Destination 'Data/04 - Best and transfer 10%.txt' -force
Move-Item -Path '05 - Best and transfer 5%/Table.txt' -Destination 'Data/05 - Best and transfer 5%.txt' -force
Move-Item -Path '06 - Best, no transfer/Table.txt' -Destination 'Data/06 - Best, no transfer.txt' -force
echo "Done!"

Remove-Item '01 - Kill parents and transfer 10%/Parser.exe'
Remove-Item '02 - Kill parents and transfer 5%/Parser.exe'
Remove-Item '03 - Kill parents, no transfer/Parser.exe'
Remove-Item '04 - Best and transfer 10%/Parser.exe'
Remove-Item '05 - Best and transfer 5%/Parser.exe'
Remove-Item '06 - Best, no transfer/Parser.exe'

echo "Done!"

echo "Plotting..."

cd 'Data'
Start-Process gnuplot 'Plot.plt' -Wait
Remove-Item 'Plot.plt'

echo "Done!"

cd ..
cd ..

cd '03-Pop(60)+'

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

Move-Item -Path '01 - Kill parents and transfer 10%/Table.txt' -Destination 'Data/01 - Kill parents and transfer 10%.txt' -force
Move-Item -Path '02 - Kill parents and transfer 5%/Table.txt' -Destination 'Data/02 - Kill parents and transfer 5%.txt' -force
Move-Item -Path '03 - Kill parents, no transfer/Table.txt' -Destination 'Data/03 - Kill parents, no transfer.txt' -force
Move-Item -Path '04 - Best and transfer 10%/Table.txt' -Destination 'Data/04 - Best and transfer 10%.txt' -force
Move-Item -Path '05 - Best and transfer 5%/Table.txt' -Destination 'Data/05 - Best and transfer 5%.txt' -force
Move-Item -Path '06 - Best, no transfer/Table.txt' -Destination 'Data/06 - Best, no transfer.txt' -force
echo "Done!"

Remove-Item '01 - Kill parents and transfer 10%/Parser.exe'
Remove-Item '02 - Kill parents and transfer 5%/Parser.exe'
Remove-Item '03 - Kill parents, no transfer/Parser.exe'
Remove-Item '04 - Best and transfer 10%/Parser.exe'
Remove-Item '05 - Best and transfer 5%/Parser.exe'
Remove-Item '06 - Best, no transfer/Parser.exe'

echo "Done!"

echo "Plotting..."
cd 'Data'
Start-Process gnuplot 'Plot.plt' -Wait
Remove-Item 'Plot.plt'
cd..
cd..
echo "Done!"

echo "Moving plots..."
New-Item -Name "Plots" -ItemType "directory" -force
Move-Item -Path '01-Pop(200)+/Data/Plot.png' -Destination 'Plots/Pop 200.png' -force
Move-Item -Path '02-Pop(100)+/Data/Plot.png' -Destination 'Plots/Pop 100.png' -force
Move-Item -Path '03-Pop(60)+/Data/Plot.png' -Destination 'Plots/Pop 60.png' -force
Rename-Item 'Plots' 'Plots+' -force
echo "Done!"

echo "Cleaning..."
Remove-Item "Plot.plt"
Remove-Item "Parser.exe"
Remove-Item "Parse&Plot.ps1"
echo "Done!"