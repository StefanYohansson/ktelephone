KTelephone
----------------------

## Docker

### Build

if you prefer to build using docker instead of use your local machine.

```
$ mkdir build
$ docker-compose -f docker-compose.build-debian.yml up
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
