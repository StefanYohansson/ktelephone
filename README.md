KTelephone
----------------------

![image](https://raw.githubusercontent.com/StefanYohansson/ktelephone/master/screenshots/ktelephone-kdeneon.png)

## User-friendly setup using repositories

- AUR: https://aur.archlinux.org/packages/ktelephone-git/ (special thanks to [Scrumplex](https://github.com/Scrumplex))
- Other distros: for now, manually compile it

## Manually build application: Docker

If you prefer to build using Docker instead of use your local machine, just find an available `Dockerfile.*`
and use it for the commands below (current options are: `arch`, `debian` and `ubuntu20`):

```sh
mkdir build

# example: replace `Dockerfile.debian` by `Dockerfile.ubuntu20` if it makes sense
docker build \
    -t build \
    -f Dockerfile.debian \
    .

docker create \
    --name build \
    build

docker cp build:/usr/src/ktelephone/build/ktelephone ./build/ktelephone

docker cp build:/usr/src/ktelephone/build/data ./build/data

(cd build && ./ktelephone)
```

## Manually build application

### Requirements

You need `Qt5`, `CMake 2.8.11` or higher and `pjproject 2.10` before anything else.

For example, on Ubuntu:

```
sudo apt install -y git cmake g++ qtbase5-dev qtmultimedia5-dev libasound2-dev
```

Inspect `Dockerfile.*` files for other instructions if you want.

### PJProject

(This set of commands in particular should be runned outside the KTelephone folder, differently than the others.)

```sh
# as normal user
git clone https://github.com/pjsip/pjproject.git
cd pjproject
git fetch origin 2.10
git checkout 2.10
./configure --prefix=/usr/lib --enable-static --disable-resample --disable-video --disable-opencore-amr
make dep
make
# as root or with `sudo`
sudo make install
sudo ldconfig
```

### KTelephone

Compile:

```sh
mkdir -p ./build
cd ./build
cmake ../
make
```

To compile for debugging, use:

```sh
mkdir -p ./build
cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug ../
make
```

Executing (if you're still inside `build` folder.)

```sh
./ktelephone
```

### For WSL

You'll need something [like X-Server and Terminator](https://medium.com/javarevisited/using-wsl-2-with-x-server-linux-on-windows-a372263533c3)
while trying to run the binary.

## Build your own telephony platform

if you don't have any way to test your calls using a remote provider, you can go for a local telephony platform (FreeSWITCH).

```
docker-compose -f docker-compose.telephony-platform.yml up
docker-compose -f docker-compose.telephony-platform.yml exec telephony fs_cli
```

## License

GNU General Public License v3.0: https://github.com/StefanYohansson/ktelephone/blob/master/LICENSE
