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
Launch multi-thread engine with:
flecspong -mt 

Few fun enhancements:
- type f to switch to azerty keyboard (Z/S)
- type n to restart the game
- type b to add a ball (AI track the coming nearest ball)
- type v to switch Paddle2 from AI to Player (UP/DOWN)
- (testing) type k to create 50k balls

## draft code (multi-thread branch)

Testing some tricks around threading Flecs (internal tables spliting) and systems. 
Few changes in v2 to procure same tests: ball is a Rect and loop delay is reduced.
Code is a lot more effecient than v2.
