Sorry this readme is trash...

# Anyway This Repo
just changes the "Content created" metadata (different than the "Created" metadata) of all photos in a repo. Sorry so much of this was ai generated cause wtf is jhead...

# Make Usage

| Command | What it does |
|---|---|
| `make` | Compile the program only |
| `make compile` | Compile the program only |
| `make run DIR=/path/to/photos` | Update photos using the current `.date` |
| `make incr` | Increment `.date` by one day |
| `make process DIR=/path/to/photos` | Increment `.date` and update photos |
| `make clean` | Delete the compiled program |


# CHANGE METADATA
    USE THIS Content Created: jhead -ts1980:04:11-0:0:00 *.JPG
        g++ -std=c++17 -o modify_timestamps modify_timestamps.cpp
        ./modify_timestamps <dir_name>
    
    Modified: touch -mt 198004110000 *.JPG
    Created: SetFile -d '04/11/1980 00:00:00' *.JPG
