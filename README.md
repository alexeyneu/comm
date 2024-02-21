# client and server

```sh
c++ k.cpp -L. -l:libraknet.a -o t
c++ x.cpp -L. -l:libraknet.a -o b
```

## Dependencies  
- libraknet compiled, `.a` in repo root
```sh
git clone https://github.com/PerikiyoXD/ezraknet
cd ezraknet
mkdir build
cd build
cmake .. -G"Unix Makefiles"
make
```
