# Project Overview

Did you know that the most renowned casinos use background algorithms to analyze gameplay for signs of cheating? Well, this project flips the script! It’s an interactive Blackjack experience built with **raylib**, featuring a **player vs. dealer** hand. But here's the twist: there’s a running algorithm in the background that doesn’t catch you cheating; instead, it helps you play better. It offers suggestions on the best moves according to **basic strategy**, highlights deviations from the standard approach, and includes a card-counting tool to enhance your gameplay.

## Card Counting and Basic Strategy Explained

**Basic strategy** is a mathematically-proven approach to playing Blackjack, providing optimal decisions for any given hand scenario. It’s the foundation for effective card counting, a technique that offers players an edge when done correctly.

The most widely used card-counting method is the **high-low count**. Here's how it works:

- Cards 2-6 increase the running count by +1
- Cards 10, Jack, Queen, King, and Ace decrease the running count by -1

A higher running count indicates a greater advantage for the player.

In most casinos, Blackjack is played with multiple decks in the shoe. To account for this, the running count is divided by the estimated number of decks remaining. This gives you the **true count**, which helps you gauge when the player holds an advantage over the house.

If the true count is equal to one, the game is considered "fair," meaning the house generally has an edge. However, if the true count is higher, it suggests that the deck has more high cards left, shifting the advantage to the player.

**Deviations from basic strategy** occur when certain card combinations, in conjunction with a high true count, make it more beneficial for the player to take an alternative action—like standing or doubling down instead of hitting.

This project aims to help you understand and practice these concepts while you play, providing both the tools and the guidance necessary for mastering Blackjack strategy.
