# LuaProjekt

A small SFML based game to show off some Lua scripting.

### Controls:

Arrow keys to move, space to slice with sword.

### Lua Scripts

The Lua scripts Player and Enemy can be customised to change behaviour and attacks of the player and enemies, for instance enemies are set to do more damage but have slower projectiles the bigger the enemy is.

Enemy difficulty is basically calculated by how many corners it's geometric shape has by default but this can be changed during runtime. You can do this by pressing E to pause, make changes in the Lua files and then press R to reload the Lua files and then E to unpause.

# SFML-Editor

A map editor for the game.

### Controls

A: Create an enemy
S: Create a wall
D: Create a door

Click or hold one of the above keys while moving the mouse over the grid to create objects.

Click on an object to change settings in the side panel or delete it by pressing delete.
