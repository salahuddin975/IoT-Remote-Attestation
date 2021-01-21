make & sudo make install

#Compile Examples
examples/
sudo ln -s /usr/lib/gcc/arm-linux-gnueabihf/8/libgcc.a /usr/lib/arm-linux-gnueabihf/libgcc.a
sudo ln -s /usr/lib/gcc/arm-linux-gnueabihf/8/crtbegin.o /usr/lib/arm-linux-gnueabihf/crtbegin.o
sudo ln -s /usr/lib/gcc/arm-linux-gnueabihf/8/libgcc_s.so /usr/lib/arm-linux-gnueabihf/libgcc_s.so
sudo ln -s /usr/lib/gcc/arm-linux-gnueabihf/8/crtend.o /usr/lib/arm-linux-gnueabihf/crtend.o
make

