# Doodle Jump Game

## Video

<a href="http://www.youtube.com/watch?feature=player_embedded&v=AiG6eQfz66o
" target="_blank"><img src="http://img.youtube.com/vi/AiG6eQfz66o/0.jpg" 
alt="Doodle Jump Game" width="800" height="600" border="10" /></a>

## About the game

To run the game type in console:

`
./game.exe
`

- for open the game in standard window size (640 x 1024)

`
./game.exe -window 600 800
`

- for open the game in custom window size (in this example 600 x 800)

Use left and right arrows to move the player.
Player scores and som infos are displayed in a console.

### These features are implemented:

- player's speed is not constant, player speeds up after another jump on the same platform and slows down after a jump on the other platform,
- there are two types of platform - the green ones are real and the blue ones are fake,
- immunity from falling down for the first 20 seconds of a game.

### Known bugs:

- from time to time, an unknown error causes the program to crush even right after starting it.
