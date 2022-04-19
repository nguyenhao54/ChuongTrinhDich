#! /bin/bash
./scanner ./example1.kpl | diff ./result1.txt -
./scanner ./example2.kpl | diff ./result2.txt -
# ./scanner ./example3.kpl | diff ./result3.txt -
./scanner ./example4.kpl | diff ./result4.txt -
./scanner ./example5.kpl | diff ./result5.txt -
./scanner ./example6.kpl | diff ./result6.txt -
./scanner ./example7.kpl | diff ./result7.txt -

