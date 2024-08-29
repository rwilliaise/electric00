# electric00

An (incorrect) simulation of charged particles floating around in a vacuum,
exerting force on each other through only their electric field.

![A video of the executable running](.github/img/animation.gif)

## why is it incorrect?

The simulation uses action-at-a-distance, which is not actually what happens in
real life. In real life, the electric field is time-varying and potentials would
propagate outwards from a particle long before the field would interact with
other particles.

## building

This unfortunately only compiles on Unix systems that have `gcc` and
`pkg-config` as well as `raylib` installed.

```
make
./electric00
```

To install required packages on Arch Linux (you will need a graphical environment):
```
pacman -S base-devel raylib
```
