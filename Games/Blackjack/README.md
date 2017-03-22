# Blackjack (Custom)
This is a program that facilitates playing Blackjack with bots or allows for watching bots playing amongst themselves. The game is currently functional, although it assumes the user knows how to use the program i.e. it doesn't handle invalid input.

Rules:
- Players may hit or pass upon reaching their turn.
- When hitting, the player is given a card.
- Each card has a certain value.
    - For numbered cards, value = their rank.
    - For face cards, their value is 10.
    - For ace, the value can be 1 or 11.
- When passing, nothing happens but the next player now has to hit or pass.
- The goal is to reach a hand value of 21.
- No players can see another player's hand, only the number of cards they've received (or number of hits).
- If a player reaches over 21, they're out for the game.
- If there are less than 2 active players, the game is over.
- If a round passes without anyone hitting after a previous player only passes, the game is over.
- When the game is over, victors are declared based on who got closer to 21. In case 2+ players have the same winning value, each of them is granted a win.
- There is no dealer as per the usual rules of Blackjack.

TODO:
- Make use of Y/N for yes/no questions and H/P for hit/pass decisions instead of 1/0.
- Handle invalid input (e.g. writing 'g' to H/P scenarios).
