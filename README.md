Smart Waste Management Simulator

A menu-driven waste management application developed in C for the LTPS (Learning to Program with C) project. It simulates waste-bin registration, waste tracking, collection operations, maintenance, reporting, and basic environmental impact estimates.

Features

* Add and register waste bins
* Assign waste categories: biodegradable, recyclable, hazardous, and general
* Add waste while checking bin capacity
* Display an ASCII waste-level dashboard
* Collect waste from one bin or all eligible bins
* Rank collection priority by bin fill percentage
* Generate a collection priority route (priority order, not geographic routing)
* Record collection history with timestamps
* Calculate daily, weekly, and lifetime collection statistics
* Calculate recyclable waste percentage
* Search bins by ID
* Edit bin location and capacity
* Mark bins for maintenance or return them to service
* Delete bins (soft-delete; the ID record remains)
* Estimate landfill diversion and CO2-equivalent impact
* Save data between program runs using text files

Requirements

* C compiler such as GCC or MinGW
* Windows, Linux, or macOS terminal
* No external libraries required

Compile

Windows (GCC / MinGW)

gcc waste_management.c -o waste_management

Run:

waste_management.exe

Linux / macOS

gcc waste_management.c -o waste_management
./waste_management

Data files

The program creates these files in the same directory where it is run:

* waste_data.txt — bin details and cumulative totals
* waste_history.txt — collection records and timestamps

Keep these files if you want your data to remain available when you restart the program.

How to use

1. Run the program.
2. Select 1 to register a bin.
3. Enter a single-word location such as Library, Cafeteria, or Parking.
4. Choose a waste category and set the bin capacity.
5. Use 2 to add waste to a bin.
6. Use 3 to view the dashboard.
7. Use 6 to see collection priorities.
8. Use 4 or 5 to collect waste.
9. Use 8 and 9 to review collection history and statistics.
10. Select 15 to save and exit.

Menu

|Option|Function                     |
|------|-----------------------------|
|1     |Add Waste Bin                |
|2     |Add Waste                    |
|3     |View Dashboard               |
|4     |Collect Waste from Bin       |
|5     |Collect Waste from All Bins  |
|6     |Smart Collection Priority    |
|7     |Collection Route Planner     |
|8     |Collection History           |
|9     |Statistics & Recycling Report|
|10    |Search Bin                   |
|11    |Edit Bin                     |
|12    |Delete Bin                   |
|13    |Maintenance Management       |
|14    |Environmental Impact         |
|15    |Exit                         |

Concepts used

* Structures (struct)
* Arrays
* Functions
* Loops and conditional statements
* Pointers
* File handling (fopen, fprintf, fscanf, fclose)
* String handling (strcmp, strcpy)
* Sorting
* Time handling (time, ctime, difftime)

Limitations

* Bin locations are single words; spaces are not supported.
* The route planner ranks bins by fill percentage and does not calculate actual distance or GPS routes.
* The CO2-equivalent value is a simplified illustrative estimate using a factor of 0.5 kg CO2e per kg of recyclable waste. It is not a verified environmental measurement.
* The program is a simulation and does not connect to real sensors or collection vehicles.
* The program expects numeric menu input.

Project details

Project: Smart Waste Management Simulator
Language: C
Type: Console-based simulation
Course: LTPS (Learning to Program with C)

Author

Add your name, university, and course details here before submitting.

License

This project is intended for educational use.
