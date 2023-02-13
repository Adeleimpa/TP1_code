#!/bin/sh
bindir=$(pwd)
cd /Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Moteur_de_Jeu/TPs/TP1_code/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Moteur_de_Jeu/TPs/TP1_code/cmake-build-debug/TP1 
	else
		"/Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Moteur_de_Jeu/TPs/TP1_code/cmake-build-debug/TP1"  
	fi
else
	"/Users/adeleimparato/Documents/Montpellier/UM/Semestre2/Moteur_de_Jeu/TPs/TP1_code/cmake-build-debug/TP1"  
fi
