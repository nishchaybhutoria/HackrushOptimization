g++-11 -Wall -Wextra -Wconversion -static -Ofast -std=c++17 -Wshadow -Wparentheses a.cpp
time ./a.out < 0.txt > output.txt
python3 checker.py 0.txt output.txt