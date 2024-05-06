# FlecsPong

FLECS port of EnttPong

https://github.com/reworks-org/EnttPong

https://github.com/SanderMertens/flecs

## Build

cmake -S . -B ./build (-DCMAKE_BUILD_TYPE=Release)

Player1: (W/S) keys to move paddle

## v1.x (enttpong-arch branch)

EnttPong strict port to Flecs (with few fixes)

Entity, Component ans Query natives from Flecs, System are custom as in EnttPong

## v2.x (main branch)

Rewritted code to feat a more Flecs native API (Entity, Component, Query, Iter, Pipeline(mod) and System(mod)), 
reworked Components (add Paddle and Movement, removing Player, modified AI) and adapted Systems

Few fun enhancements:
- type f to switch to azerty keyboard (Z/S)
- type n to restart the game
- type b to add a ball (AI track the coming nearest ball)
- type v to switch Paddle2 from AI to Player (UP/DOWN)
