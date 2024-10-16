# FileEncryptor
libakrypt library is needed

## Building

### Debian
```bash
git clone https://github.com/Pafffnutiy/FileEncryptor.git
mkdir build
cd build
cmake -DCMAKE_C_FLAGS="-march=native" ..
mske
```

### Windows
```bash
git clone https://github.com/Pafffnutiy/FileEncryptor.git
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_C_FLAGS="-march=native" ..
mingw32-make.exe
```

## Usage
```bash
./FileEncrypt -f ..\example.txt -p 2003
```
