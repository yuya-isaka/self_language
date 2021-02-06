#!/bin/bash
gcc -std=c11 -o lang lang_isaka.c

runtest() {
    output=$(./lang "$1")
    if [ "$output" != "$2" ]; then
	echo "$1: $2 expected, but got $output"
	exit 1
    fi
    echo "$1 => $output"
}

runtest 0 0
runtest 42 42
runtest '1 2 3' '1
2
3'

runtest '+ 1 2' 3
runtest '+ 13 13' 26
runtest '- 100 3' 97
runtest '- 99 3' 96
runtest '* 9 4' 36
runtest '/ 100 5' 20

runtest 'F[+ a a] F(2)' 4
runtest 'F[* 3 a] F(2)' 6
runtest 'F[/ 100 a] F(2)' 50
runtest 'F[* a a] F(100)' 10000
runtest 'F[* a a] G[+ a a] F(G(2))' 16

runtest 'F[* a b] G[+ a b] F(G(3 2) 5)' 25
runtest 'F[* + a b c] G[+ + a b c] F(G(1 2 3) 2 3)' 24

runtest 'P(2)' '2
2'

runtest 'F[+ * a b * c d] F(1 2 3 4)' 14

echo OK
