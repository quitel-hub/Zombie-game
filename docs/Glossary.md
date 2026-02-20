# Project Glossary: Zombie Survival Game

* **Entity:** The base abstract class representing any interactive object in the game world (has coordinates, texture, and basic physics).
* **Player:** The main character controlled by the user. Inherits from Entity and includes health, movement logic, and an inventory system.
* **Enemy:** A hostile NPC (Non-Player Character) that attacks the Player.
* **Zombie:** A standard Enemy variant with basic pathfinding towards the Player.
* **Boss:** An advanced Enemy variant with increased health, damage, and unique attack patterns.
* **Weapon:** An interface/base class for items that deal damage.
* **Sword:** A melee Weapon variant.
* **Gun:** A ranged Weapon variant requiring ammunition.
* **Inventory:** A component that stores the Player's collected items and active Weapons.
* **Map:** The playable area containing obstacles, floor tiles, and boundaries.
* **Game / GameLogic:** The central controller that manages the game loop (processing input, updating states, rendering graphics).
* **State:** Represents the current mode of the application (e.g., MainMenu, Playing, GameOver).