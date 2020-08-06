sudo apt install cmake gcc-arm-none-eabi gcc-arm-linux-gnueabi
wget "https://releases.llvm.org/3.7.0/clang+llvm-3.7.0-armv7a-linux-gnueabihf.tar.xz"
tar xf clang+llvm-3.7.0-armv7a-linux-gnueabihf.tar.xz
cd clang+llvm-3.7.0-armv7a-linux-gnueabihf/
sudo cp -r bin/* /usr/bin/
sudo cp -r include/* /usr/include/
sudo cp -r lib/* /usr/lib/
sudo cp -r share/* /usr/share/
sudo apt install llvm-3.7
rm -r clang+llvm-3.7.0-armv7a-linux-gnueabihf*
