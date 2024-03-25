echo "run executable file"
../cmake-build-debug/MyCCompiler_1stGen ../example/test1.in > source.txt;
echo "run python script"
python3 ./add_source.py;
echo "run gcc"
gcc -m32 -no-pie ./target.s -o target;
./target;
