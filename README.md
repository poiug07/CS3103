# CS3103 Project

This project is a part of CS3103 Operating System course at my university. The task was to do multithreaded Top-K search. Report can be found [here](./Report.pdf).

## Problem

Input file format:
```
1657459905, ALKDJSDFSF
1657459803, DFINLKSD
... //and so on
```
Timestamp range is fixed between 1645491600 and 1679046032. There can be multiple files(around ~1GB). We need to find Top-K most frequent hours.

Outputs:
```
Top K frequently accessed hour:
Fri Mar 17 16:00:00 2023		3600
Fri Mar 17 15:00:00 2023		3600
Fri Mar 17 14:00:00 2023		3600
Fri Mar 17 13:00:00 2023		3600
Fri Mar 17 12:00:00 2023		3600
```

## Results

As a result of this project my team got highest score and **least execution time**(fastest code) by quite big margin from most of other teams.
