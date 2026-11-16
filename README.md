# Disaster Relief Resource Allocator

**Priority-Based Disaster Relief Resource Allocation & Emergency Tracking System**

A menu-driven C++ console application (B.Tech CSE PBL project, Graphic Era Hill University) that registers disaster-affected areas, ranks them by a priority score, and allocates relief resources against a stored inventory.

> This is an academic prototype. The priority score is a project-defined formula, not an official emergency-response standard.

## Features
- Register disaster areas (name, disaster type, severity 1-10, affected population)
- Show areas in priority order (`severity * 10 + population / 100`)
- Add and process emergency calls in arrival order (FIFO)
- Allocate a severity-based relief package (food, medical kits, water, blankets, tents, clothes, first-aid kits) to the highest-priority or a named area
- Check and update inventory (add stock or set total)
- View allocation history (newest first)
- Data is saved to text files and reloaded on the next run

## Data structures used
| Structure | Use |
|---|---|
| Singly linked list | Registry of disaster areas |
| Linked-list priority queue | Orders areas by priority score |
| Queue (FIFO) | Emergency calls |
| Stack | Allocation history |
| File handling | Saves areas, inventory, history and calls |

## Build and run
```bash
g++ -std=c++11 -o relief main.cpp
./relief          # on Windows: relief.exe
```

## Relief packages (by severity)
| Severity | Food | Medical | Water (L) | Blankets | Tents | Clothes | First-aid |
|---|---|---|---|---|---|---|---|
| 1-3 | 50 | 25 | 250 | 25 | 5 | 25 | 10 |
| 4-6 | 100 | 50 | 500 | 50 | 10 | 50 | 25 |
| 7-8 | 150 | 75 | 750 | 75 | 15 | 75 | 35 |
| 9-10 | 200 | 100 | 1000 | 100 | 20 | 100 | 50 |

Default starting inventory: 300 food, 150 medical, 1500 L water, 200 blankets, 50 tents, 200 clothes, 100 first-aid kits.

## Team (team name: singhpawanm100)
- Pawan Singh Mehta (Team Lead)
- Nischay Singh Negi
- Mohit Kandwal
- Parmjeet Singh
