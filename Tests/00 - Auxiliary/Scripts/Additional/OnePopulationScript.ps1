cd '01 - Kill parents and transfer 10%'
Start-Process GA
cd ..
cd '02 - Kill parents and transfer 5%'
Start-Process GA
cd ..
cd '03 - Kill parents, no transfer'
Start-Process GA
cd ..
cd '04 - Best and transfer 10%'
Start-Process GA
cd ..
cd '05 - Best and transfer 5%'
Start-Process GA
cd ..
cd '06 - Best, no transfer'
Start-Process GA
cd ..
echo "Running..."

$p = get-process GA
Wait-Process -Id $p.id
echo "Done!"