#ifndef H_ERRORS_H
#define H_ERRORS_H

/* 
 * These errors should be handled properly by detecting them and
 * returning the proper status code by calling exit(enum error)
 *
 * NOTE:
 * An invalid format error should be raised if you encounter a 
 * character that is NOT a non-negative integer, comma, left parenthesis, right  
 * parenthesis or a new line character.  
 *  Examples of invalid characters:
 * 		letters
 * 		negative numbers
 * 		tabs
 * 		symbols
 * 
 *  The invalid format error should also be raised when you encounter lines that  
 *  contain valid characters, but are not properly formatted. This could happen
 *  for example, when a line representing an edge between 4 and 5 with weight 2 is written as
 *  (4,5,2 or as 4,5,2) or as (4 5,2.
 *  So, any parsing error that cannot be attributed to the first seven enumerated errors 
 *  should be given the invalid format error. 
 */

enum error {
    INCORRECT_NUMBER_OF_COMMAND_LINE_ARGUMENTS = 1,
    INPUT_FILE_FAILED_TO_OPEN,
    INPUT_FILE_FAILED_TO_CLOSE,
    PARSING_ERROR_EMPTY_INPUT_FILE,
    OUTPUT_FILE_FAILED_TO_OPEN,
    OUTPUT_FILE_FAILED_TO_CLOSE,
    PARSING_ERROR_INVALID_CHARACTER_ENCOUNTERED,
    PARSING_ERROR_MISSING_GOALS,
    GOAL_UNREACHABLE,
};

/*
 * EXAMPLE: If fopen fails to open the file, exit the program with
 * the proper error code in this way:
 * 		exit(FILE_FAILED_TO_OPEN);
 * Check that the code returned is correct by running your
 * program in GDB.  The above example would return exit status 2.
 */ 

#endif