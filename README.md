To run this, paste the input files.
run.sh assumes your input file is named 0.txt. Otherwise, you may change the name of the file.
Run it by by running ./run.sh.

The logic here is that it makes sense to take higher satisfaction values in smaller intervals.
Therefore, I divide each satisfaction score by the length of the interval. I match clients by taking these events into account in decreasing order of this value.
