# Prisoner's Dilemma Simulation

## References

[Generous Tit For Tat: A Winning Strategy](https://www.forbes.com/sites/rogerkay/2011/12/19/generous-tit-for-tat-a-winning-strategy/#1eb99df466eb)

## Overview

This software simulates opponents (players) competing with each other for 100 days.
Each day each player must decide to either Attack or Defend. The players have
health points added or taken away depending on their decision and the decision
of the opposing player, as follows:
* If both Defend:  No Change in health points.
* If both Attack:  Both lose 5 health points.
* If one Attacks and Other Defends: Attacker gains 2, and Defender loses 10 health points.

To run the simulation:
* ./ctlr \<player\> \<player\> \<player\> ...
* Or, use the 'run' script to run ctlr for the players provided

The ctlr program selects 2 players (opponents) to compete over 100 days period.
Each day the 2 players are faced with prisoner's dilemma, to Attack or Defend. The player's
health scores are updated by the ctlr program at the end of each day.
When 100 days of competitions have been completed, the final health score for each 
player is displayed.

The ctlr program repeats selecting 2 players to compete, for all player combinations.

The ctlr program is written in C. The player programs can be written in most any
language.  The ctlr program executes the player programs and communicates with
them using standard input and output

The player programs receive the following inputs from ctlr:
* "YESTERDAY_YOUR_OPPONENT ATTACK|DEFEND": This input to the player is provided
  to the player at the beginning of each day. Players will often base their 
  decision of whether to Attack or Defend today on what happened yesterday.
* "WHAT_WILL_YOU_DO_TODAY?": This input to the player requires a response back
  to ctlr of either "ATTACK" or "DEFEND".
* "TERM": The player program must terminate.

## Players Provided

The player programs provided are described below. Most are written in python,
except for jesus which is written in bash, and lucifer which is written in C.
* jesus: always defends
* lucifer: always attacks
* tft (tit-for-tat): always responds to an attack with a counter-attack
* gtft (generous-tit-for-tat): almost always responds to an attack with a counter-attack
* mrs (massive retaliatory strike): defends until attacked, once attacked 
  will always respond with attack
* mrs20 (massive retaliatory strike): defends until attacked, once attacked 
  will respond with attack for 20 days
* tester: will occasionally attack, and continue attacking until other player attacks
* tft_tester: combined tft and tester 
* gtft_tester: combined gtft and tester 

## Some Other Uses

Other than simulating the prisoner's dilemma this software provides:
* A fun way to begin learning a new computer language.
* Code which creates a child process, and communicates to it using pipes for stdin and stdout.
