KTelephone
----------------------

![image](https://raw.githubusercontent.com/StefanYohansson/ktelephone/master/screenshots/ktelephone-kdeneon.png)

## Repositories

### AUR

- https://aur.archlinux.org/packages/ktelephone-git/ (special thanks to [Scrumplex](https://github.com/Scrumplex))

## Docker

### Build

if you prefer to build using docker instead of use your local machine.

1. Debian

```
$ mkdir build
$ docker build \
    -t build \
    -f Dockerfile.debian \
    .
$ docker create \
    --name build \
    build
$ docker cp build:/usr/src/ktelephone/build/ktelephone ./build/ktelephone
```

2. Arch

```
$ mkdir build
$ docker build \
    -t build \
    -f Dockerfile.arch \
    .
$ docker create \
    --name build \
    build
$ docker cp build:/usr/src/ktelephone/build/ktelephone ./build/ktelephone
```

### Telephony Platform

if you don't have any way to test your calls using a remote provider, you can go for a local telephony platform.

```
$ docker-compose -f docker-compose.telephony-platform.yml up
$ docker-compose -f docker-compose.telephony-platform.yml exec telephony fs_cli
```

## Build instructions

You need `Qt5` and `CMake 2.8.11` or higher

```
$ mkdir -p /your/ktelephone/build
$ cd /your/ktelephone/build
$ cmake ../
$ make
```

To compile for debugging, use

```
$ mkdir -p /your/ktelephone/build
$ cd /your/ktelephone/build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
```

Executing

```
./ktelephone
```

## Tutorials and resources

Qt online docs
https://doc.qt.io/
