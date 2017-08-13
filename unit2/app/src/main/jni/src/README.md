--enable-asm 开启汇编
--enable-neon 针对armv7-a平台的专属指令集进行的优化，可以让编码效率提升
--enable-small 可以尽量的缩小编译出来的库大小，比如我没打开它，编译出来的.a有80多M，开启后就只有57M左右
--disable-ffmpeg 不要ffmpeg这个工具，ffmpeg是一个命令行工具，非常强大，在官网也可以看到，但我们通过代码开发，就不需要它了
--disable-ffplay --disable-ffprobe --disable-ffserver 这些都是不需要的工具，当然也可以编译出来看看
--disable-avdevice 这个库是用来操作一些硬件设备的，如摄像头这些，但Android中感觉不需要它，也可以编译出来试试能不能使用在Android上
--disable-symver 禁用 symbol versioning
PREFIX=
(pwd)/android/
CPU 指定编译成功的库所在的目录，这个配置是：当前目录下的android目录下的指定的cpu平台目录下