
This tool helps calculate which expeditions to run
for the highest resource gain.

Simply run the tool, put in which resources you want,
how long you will be gone at a time,
and the tool will tell you which expeditions to run.

## Feature List

    Recommends best expeditions, based on given factors
    Bases information on time afk, if relevant
    Factors in buckets

## Todo List

    Factor in ship consumption
    Factor in great success/sparkling
    Think of things to put on the Todo List

Visit http://kancolle.wikia.com/wiki/Expedition for detailed information
on each expidition.

Visit http://kancollepoi.weebly.com/ for the original, web-based expedition
tool.

## Acknowledgments

    StackOverflow user dorserg, for recursive calculation of combinations
    kancollepoi.weebly.com, for original idea
    Luke Sathrum for StringToInt function
    Google for the style checking python script

## Reading expedition_tables.txt

Should it be necessary to read expedition_tables.txt, each row corresponds to
an expedition. 1-40 are expedtions 1-40, 41-43 are expeditions A1-A3, and 44 is
B1. The columns are laid out as follows:

    Time required in minutes
    Ammo
    Fuel
    Iron
    Bauxite
    Chance for bucket (decimal representation)
