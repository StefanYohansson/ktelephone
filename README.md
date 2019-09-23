KTelephone
----------------------

## Docker

```
$ docker-compose up
$ docker-compose exec telephony fs_cli
```

You need `Qt5` and `CMake 2.8.11` or higher

## Build instructions

```
$ cd /your/ktelephone/build
$ cmake ../
$ make
```

To compile for debugging, use

```
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
