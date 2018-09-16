                               ******************INPUT FILE FORMATTING******************
This program does NOT work with OUTER walls that are connected diagonally with no other shared adjacent wall. Invalid walls will result in a segmentation fault.

INVALID outer walls:
########       #####
#       #     #     #
#        #    #     #
#       #      #####
########  

VALID outer walls:

##############       ###########
#            #       #         #
##           #       #         #
 #       #####       #         #
 #########           ###########
 
Diagonal INNER walls that are connected with no other shared adjacent wall will be counted as walls, but the robots will be able to move as follows:

##############
#E  #        #
# S#         #
# #S         #
##  SSSS     #
##############

(Note: by removing the last four if statements of the while loop of the BFS (lines 201-231) the pathfinding will switch from Manhattan to Euclidean, which may allow for valid, impermiable diagonal walls.)

The input file MUST contain both robots starting positions and finish goals. If it doesn't the program will exit with an erorr code.

The input file MUST NOT contain any characters other than " " "#" "S" "E" "F" "L" or "(newline)." If it does the program will exit with an error code.

If the input file has duplicate start and finish places, the program will take the rightmost, bottommost duplicate and turn the others into empty spaces.

                                    ******************EXECUTION******************
This program accepts the the executible, the input file, and the output file as command line arguments. As the project is uploaded as a netbeans project, it is reccomended that it is run as a netbeans project.

Example(terminal): luke@luke-VirtualBox:~$ ./a.out input.txt output.txt
Example(Netbeans): Project Proporties -> Run -> Run Command: "${OUTPUT_PATH}" "input.txt" "out.txt"

The robots will move all the way from start to finish one at a time. For example, if robot one finishes at time = 5, robot two will start moving for the first time at t = 6.
If the goal is unreachible for one of the robots, the program will terminate.

                                      ******************OUTPUT******************

The program will print two versions of the room: one with the path taken by the first robot displayed, and one with the path of the second robot displayed. We decided to display the paths this way rather than superimposing them to improve path visibility. The paths taken are represented by that robot's start symbol, and the finish spot by that robot's finish spot.