---
title: Building Moai with Steam Runtime
out_of_date: true
---

To build moai with the steam runtime you can follow the steps here.

I used a fresh ubuntu 14.04 (might work with others)

```bash
 sudo apt-get install git
 git clone https://github.com/ValveSoftware/steam-runltime
 cd steam-runtime
 ./setup_chroot.sh --outputdir ~/steam-runtime-build --i386
```
 waiting waiting waiting
```bash
 cd ~/steam-runtime-build
 chroot steamrt_scout_i386
 cd ~
 apt-get install git
 wget v
 bash cmake-2.8.12.2-Linux-i386.sh (say yes)
 rm /usr/bin/cmake
 ln -s ~/cmake-2.8.12.2-Linux-i386/bin/cmake /usr/bin
 git clone http://github.com/moai/moai-dev
 cd moai-dev
 git checkout 1.5-stable 
 ./bin/build-linux-sdl.sh
```
your output is in `~/moai-dev/cmake/build/host-sdl` and is called `moai`

To build again in future it should just be:

```bash
cd ~/steam-runtime-build
chroot steamrt_scout_i386
cd ~/moai-dev
(checkout new source here)
./bin/build-linux-sdl.sh
```