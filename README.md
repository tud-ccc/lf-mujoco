# Lingua-Franca Mujoco Simulator

**Contact:** <tassilo.tanneberger@tu-dresden.de>

This is a very shallow wrapper around mujoco to interface with lingua-franca programs.

## Build

LF-Mujoco can only be built with a version greater then 4.1 of LF-Lang.

```bash
    $ lingo build
    $ lfc src/Main.lf
```

## To-Do

- [ ] Feed reactor-cpp the time from the simulator


## Change between robo and simulator

In order to change from the robo to the simulator change the exmaple/XArm7.lf and its mujoco cmake ('target sources')