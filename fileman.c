#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <errno.h> 

void logAction(char* fileName, char* message);


/**This method takes the name of the file that is entered by 
 * the user. It is designed to return 1 if the file name has
 * the txt file extention. The newline and line terminating 
 * characters are removed. Then the strstr function is used
 * to check if the '.txt' is a substring of the file name. 
 * This would confirm that the file is indeed a text file. 
 * Just checking for 'txt' would not be sufficient since the name
 * of the file might contain 'txt'. Instead, checking for '.txt'
 * would mean that the file extension is searched for.
 * @param fileName The name of the file being validated
 * @return 1 if the file name belongs to a text file, 0 if the 
 * file name is not a text file.  
 */
int checkTxt(char *fileName){
    strtok(fileName, "\n"); //Removes next line character
    strtok(fileName, "\0"); //Removes the string terminating character
    char txt[] = ".txt"; //Defines search string
    char illegalChar[] = "£#%&{}/?*><$!':@+'|="; //String of illegal characters
    int add = strlen(fileName)-4;
    int istxt = 1; //Boolean variable stores wheter the file ending is .txt
    for (int a=0; a<4;a++){
        //Iterates through the last 4 character
        if((fileName[a+add]) != (txt[a])){
            istxt = 0; //If it does not match corresponding characters in .txt 
            break; //Stops the for loop
        }
    }
    int valid = 1; //Boolean to store whether the name is valid or not
    if (istxt){ //If .txt is in the file name
        for (int i=0; i<strlen(fileName); i++){ //Iterates through fileName
            char currentChar = fileName[i]; //Extracts every character from the fileName
            for (int j=0; j<strlen(illegalChar); j++){ //Iterates through list of illegal file characters
                char currentIllegalChar = illegalChar[j]; 
                if (currentIllegalChar == currentChar){ //Checks if character matches illegal file character
                    valid = 0; //File name is no longer valid 
                    break;
                }
            }
        }
    }else{
        //if it does not end in .txt
        valid = 0;
    }
    if (valid==1){//If the file name is not valid
        return 1;
    }else{
        //Displays error message
        printf("Please enter a valid name of a text file under 20 characters\n"); 
        return 0; //Returns false
    }
}

/**This method is designed to read the contents of a text file.
 * The name of which is the parameter source. The str parameter
 * is a pointer to the string passed from the caller method which 
 * will contain the file contents. The method parses through the text
 * file until it has reached the end (recognised by using the EOF condiiton
 * that will determine whether or not the string end of the file has 
 * been reached while reading data. Characters are read individually 
 * into the string complete_file and then copied to the str pointer
 * @param source The name of the text file that will be read
 * @param str Pointer variable to the string containing the data in the 
 * caller method. 
 */
void getString(FILE *source, char* str){
    char ch; //Character variable to hold each character of a tetx file while parsing it
    char complete_file[1000]; //String to store all the text of the text file
    int count = 0; //Counter variable for the current position in the text file
    memset(complete_file,0,1000); //Resets complete_file
    while( ( ch = fgetc(source) ) != EOF ){ //While end of the text file has not been reached
        complete_file[count] = ch; //Adds current character to the next position in complete_file
        count += 1; //Increments the character counter by 1 
    }
    fclose(source); //Closes the source file
    strcpy(str, complete_file); //Copies the contents of the file to the str pointer
    
}


/**The countLine method takes in the name of a file and then counts
 * the number of lines in it. It first checks if the file exists, if 
 * it does not the user is alerted. Otherwise, the contents of the file
 * are read and stored in complete_file. Then each character is iterated
 * and the number of newline characters are counted. This indicates the 
 * number of lines present as for every new line there is a newline
 * character in the text file. 
 * @param fileName The name of the file for which the number of lines
 * is to be counted 
 */
int countLine(char *fileName){
    FILE *source; //File pointer to the file
    source = fopen(fileName, "r"); //Opens the file in read mode
    if (source == NULL){ //If the file does not exist
        printf("File %s does not exist", fileName);
        return 0; //Returns 0 back to the program
    }else{
        char complete_file[1000]; //Stores all the text read from the file
        getString(source, complete_file); //Retrieves the contents of the file
        int count = strlen(complete_file); //Counts the number of characters in the file
        int line_count = 1; //Declares line counter variable
        for(int i=0; i<count; i++){ //Iterates over each character 
            if (complete_file[i] == '\n'){ //If character is a newline character
                line_count +=1; //Increments the line counter
            }
        }
        printf("There are %i line(s) in the text file",line_count);
        return line_count; //Returns number of lines for other methods 
    }
    fclose(source); //Closes the text file
    
}


/**The createFile method will take the name of a file
 * and then create a text file for it. However, it first 
 * checks whether a text file with the same name already exists.
 * If it does, then the user is asked whether they want to overwrite
 * the old file or not. If they choose not to, then the method 
 * terminates, however if the user decides to go ahead then the
 * text file with the given name is created. Furthermore, 
 * the name of the file is saved in the log file so that 
 * any operations performed on it by the file manager are 
 * recorded. 
 * @param name The name of the text file to be created.
 * @return 1 if a file was successfully created
 */
int createFile(char *name){
    FILE *file; //File pointer to the file to be created
    file = fopen(name, "r"); //Opens in read mode to see if it exists
    if (file != NULL){ //If it does exist
        //Ask user if they want to overwrite it
        printf("File %s already exists, do you wish to overwrite? y/n\n", name);
        char ans[5];
        fgets(ans,5, stdin);// Reads the answer from the user
        strtok(ans, "\n"); //Removes newline character
        if (strcmp(ans, "y")==0){ //If the user wants to overwrite
            fclose(file); //Closes the already opened file
            file = fopen(name, "w"); //Opens file in write mode which overwrites the file
            printf("File overwritten\n");
            char message[] = "File overwritten"; //Constructs message 
            logAction(name, message); //Message inserted into the log file
            return 1;
        }else{
            printf("File not overwritten\n"); 
        }
    }else{ //If file does not exist
        FILE *file;
        file = fopen(name, "w"); //Opens a new file in write mode
        fclose(file); //Closes the file
        printf("File %s has been created\n", name);
        FILE *log_file; //Log file pointer
        log_file = fopen("log.txt", "r"); //Opens log file in read mode
        if (log_file == NULL){ //If log file does not exist
            FILE *file; //Declares a new file pointer
            file = fopen("log.txt", "w"); //Opens a new text file called log.txt
            fclose(file); //Closes the text file
        }
        log_file = fopen("log.txt", "a"); //Opens the log file ini append mode
        strtok(name, "\n"); //Removes the newline character
        fprintf(log_file, "%s\n", name); //Inserts the text file name into log.txt
        fprintf(log_file, "\n"); //Inserts an empty line after the text files name
        fclose(log_file); //Closes log.txt
        return 1;
    }
}


/** The method is designed to fetch the line number in the log file 
 * where the name of the file currently being edited is stored. This 
 * is so that the logging information can then be written directly 
 * underneath the file name which is easier for the user to understand
 * wheras otherwise all the logging information for all the files would 
 * be in one block. 
 * log.txt is first read into completeFile. The text in each file needs
 * to be isolated to be compared with the fileName. I used a start pointer
 * to point to the beginning of the line and an end pointer to point to 
 * the end of each line. The end pointer would be updated when a newline
 * character was found to point to this location. The start pointer takes 
 * points to the next value after the newline character. The size of the
 * line is then calculated by doing end-start and the memcpy function
 * is used to extract the text for each line from completeFile. The strcmp
 * method is then used to compare fileName and the text on that line. 
 * If there is a match, then the next line number is returned to the caller
 * method. If the fileName is not found, then 0 is returned. 
 * @paran fileName the name of the file to be searched for in log.txt 
 * @return The next line number after the name of the file in log.txt
 */
int getLineNo(char* fileName){
    FILE* source; //File pointer to the source file
    char complete_file[1000];//Will hold the text from the file
    char *ptr = complete_file; //Sets pointer to the beginning of the string
    source = fopen("log.txt", "r"); //Opens log.txt in read mode
    getString(source, complete_file); //Copies the data into complete_file
    int count = strlen(complete_file); //Calculates the number of characters in the text file
    int line_count = 1; //line counter variable
    int start=0;//Intialises an index location variable to point to the beginning of a line
    int end = 0; //Initialies an index location variable to point to the end of a line
    int found = 0; //Boolean variable to store whether or not the fileName was found or not
    strtok(fileName, "\n"); //Removes the newline character from the fileName
    char line_string[100]; //String to the hold the contents of each line
    for (int i=0; i<=count; i++){ //Iterates through every character in complete_file
        if (complete_file[i]=='\n'){ //If a newline chaeracter is found
           end = i; //Sets the end index location to the location of the newline character
           int size = end-start; //Calculates the size of the line
           memcpy(line_string, &ptr[start],size); //Copies the text in that line to line_string
           line_string[size] = '\0'; //Adds a string terminating character
           start = i+1; //Beginning of the next line is the index after the newLine character
           
           //Checks if the line_text is the same as the fileName
           if (strcmp(line_string, fileName)==0){
               found = 1; //Sets boolean variable to True
               line_count = line_count+1; //Increments the line counter by 1
               break; //Stops the for loop as it is no longer necessary
           }
           line_count +=1; //Increments line counter by 1 
           memset(line_string, 0, size); //Resets the line_string to an empty string
        }
    }
    if (found==1){ //If the file name has been found
        return line_count; //Return the line number the message should be written to
    }else{
        return 0; //Return false
    }
}



/**This method takes in the name of the file, the text that needs to inserted 
 * and the line number that the text should be inserted to. The method will first 
 * check if the file exists, if it does not, the user is alerted. Then the contents
 * of the file are copied into complete_file. The text file is then overwritten so 
 * that it is blank. Each character is then  iterated over, counting
 * the number of newline characters which indicates the current line number. If the 
 * current line number matches the line_no, then the text is inserted at this position. 
 * The rest of the text in complete_file is subsequently added into the text file too.
 * If it is the case that the line number the text needs to be inserted into exceeds 
 * the total number of lines in the text file, then more lines are added and the text
 * is inserted.
 * This method is also used by the logAction method to insert logging messages into the 
 * log file. If this is the case, then the methods own logging methods will not be required.
 * Therefore an if statement is used to check the fileName at the end before the logging 
 * statement for the method is inserted into the relevant file. 
 * @param fileName  String containing the name of the file the text is to be inserted into 
 * @param insert_text String. The text that will be inserted into the file
 * @param line_no Integer containing the line number the text will be inserted into
 */
void insertLine(char *fileName, char *insert_text, int line_no){
    FILE *source; //File pointer to the source file
    source = fopen(fileName, "r"); //Opens the file in read mode
    if (source == NULL){ //If the file does not exist
        printf("File %s does not exist", fileName);
    }else{
        char complete_file[1000]; //String to hold the text in the text file
        getString(source, complete_file); //Retrieves the data in the text file
        int count = strlen(complete_file); //Counts the number of character in the file
        remove(fileName);//Deletes the file from the directory
        source = fopen(fileName, "w"); //Creates an empty file with the same name
        int line_count = 1; //Initialises the line counter variable
        int total_line_count = 1; //Initialises the total line count variable
        for (int i=0; i<count; i++){ //Iterates through every character
            if (complete_file[i] == '\n'){ //Counts number of newline characters
                total_line_count +=1;
            }
        }
        for (int i=0; i<count; i++){ //Iterates through every character
            //If the current line matches the line number entered by the user.
            if (line_count == line_no){ 
                //Inserts the user entered text into the line
                for (int a=0; a<strlen(insert_text); a++){ 
                    fprintf(source, "%c", insert_text[a]);
                }
                line_count +=1; //Increments the line_count by 1 
                fprintf(source, "\n"); //Enters a newline character
            }//Inserts the already existing text into the file
            fprintf(source, "%c", complete_file[i]); 
            if (complete_file[i] == '\n'){ //If a newline is detected
                line_count +=1; //Increments the line counter by 1
            }
            //If the end of the file is reached
            if (line_count == total_line_count+1){ 
                break; //Breaks the for loop
            }
        }
        //if the line number entered by the user exceeds the number of existing lines
        if (line_count<line_no){
            //Calculates the number of extra_lines
            int extra_lines = line_no - line_count;
            //Inserts extra lines into the text file
            for (int a=0; a<=extra_lines-1; a++){
                fprintf(source,"\n");
            }
            //Inserts the user entered text into the text file
            for (int a=0; a<strlen(insert_text); a++){
                fprintf(source, "%c", insert_text[a]);
            }
        }
        memset(complete_file, 0, 1000);
        fclose(source);
        if (strcmp(fileName,"log.txt")!=0){ //If not inserting text into log.txt
            char message[] = "Line inserted |"; //Constructs the message
            char inserted_text[20];
            strcpy(inserted_text, insert_text); //Concatenates the text entered by the user to string
            strcat(message, inserted_text); //Concatenates the text entered by the user to string
            char buffer[10];
            sprintf(buffer, " to line %d" , line_no); //Creates string containing meta information
            strcat(message, buffer); //Concatenates the meta information to the message
            logAction(fileName, message); //Saves the information in the log file
        }
    }
}


/** The logAction method will take the name of the file that has been edited 
 * and the logging message about the operation just performed and insert the message
 * into the correct location in the log file. It makes used of the getLineNo method
 * to retrieve the lne number in the log file where the message should be inserted. 
 * All logging messages will contain the date and time the file operation was carried
 * out. This information is also added here as it is the only static content in all
 * operation logging messages. 
 * @param fileName. String containing the file name of the text file that was edited
 * @param message. String containing the message that the user will need to be inserted 
 * into the log file. 
 */
void logAction(char* fileName, char* message){
    FILE *log;
    log = fopen("log.txt", "r");
    if (log == NULL){
        printf("Unable to log action as log.txt does not exist\n");
    }else{
        fclose(log);
        int line_no = getLineNo(fileName); //Retrieves the line number that the fileName is in log.txt  
        if (line_no!=0){//If a record of the text file is found in log.txt
            char finalMessage[1000];
            memset(finalMessage,0,1000);
            char lineNo[10]; // Initialises variable to store the time and date information
            int file_lines = countLine(fileName); //Counts number of lines after the operation
            sprintf(lineNo, " | %d" , file_lines); //Concatenates number of lines in file to lineNo
            time_t rawtime; //Creates a time_t object
            struct tm *info; //Retrieves time information
            time(&rawtime); 
            info = localtime(&rawtime); //Retrieves the date and time
            strftime(finalMessage,sizeof(finalMessage)," %x - %X | ",  info); //Formats the time into DD/MM/YY - HH:MM:SS
            strcat(finalMessage, message); //Concatenates the message to final message
            strcat(finalMessage, lineNo); //Concatenates the line information to the 
            strcat(finalMessage, " lines after operation. "); //Concatenates terminating text
            printf("%s", finalMessage);
            //Uses insertLine method to insert the finalMessage into log.txt
            insertLine("log.txt", finalMessage, line_no); 
        }
    }
}        
            


/**The copyFile function will take the name of the file that needs
 * to be copied 'oldFile' and copy the contents to a file called
 * 'newFile' which is another parameter passed to the function. 
 * It intially checks whether the oldFile exists or not. If it does
 * not, the user is shown an error message. Otherwise the newFile
 * text file name is verified to ensure that it does have the .txt 
 * extension using the checkTxt function. If the file name is a text files,
 * then the contents of it is copied into complete_file. A new tetx file
 * called newFile is also created using the createFile function. If the
 * text file was successfully created, it is opened and all the text
 * in complete_file is inserted into the text file. The message for the log
 * file is also created and inserted using the logAction method. 
 * @param oldFile The name of the file for the contents to be copied from 
 * @param newFile The name of the file for text to be copied to
 */
void copyFile(char *oldFile, char* newFile){
    FILE *source, *target; //Defines the destination and source file pointer
    char ch; //Character to hold the values of each character in a text file
    int copy; ///Boolean variable to store whether the file was copied or not
    char complete_file[1000];
    source = fopen(oldFile, "r"); //Opens the text file in read mode
    if (source == NULL){ //If the file to be copied does not exist
        printf("File %s does not exist", oldFile); //Prints the message
    }else{
        getString(source, complete_file);
        copy = createFile(newFile); //Creates a new file with the user entered name
        if (copy==1){ //If the file was created successfully
            target = fopen(newFile, "w"); //Opens the file in write mode
            //inserts the text into the new text file from the old text file
            for (int a=0; a<strlen(complete_file); a++){ 
                    fprintf(target, "%c", complete_file[a]); //Inserts character into the file
            }
            printf("File successfully copied %s\n", newFile);
            fclose(source); //Closes the source file
            fclose(target);//Closes the target file            
            char message[] = "File copied to ";//Construct message for logging
            strcat(message, newFile); //Adds the file name that was copied to
            logAction(oldFile, message); //Logs the file operation into oldFile
            memset(message,0,strlen(message)); //Resets the message string
        }
    }
}

/**The deleteFile function will take a file name as a parameter and 
 * delete it from its directory. The remove method is used which
 * deletes a file. The corresponding message is created and written
 * to the log file. If the file to be deleted does not exist, then
 * the user is alerted. 
 * @param fileName The name of the file to be deleted
 */
void deleteFile(char *fileName){
    if (remove(fileName) ==0){ //If the file was deleted
        printf("File deleted\n"); //The user is told that the file is deleted
        char* message = "File deleted"; //Message for the log is constructed
        logAction(fileName, message); //The message is inserted into the log file
    }else{
        printf("File %s does not exist", fileName); //If file does not exist
    }
}

/** The showFile function takes the fileName as a parameter. This will be the name
 * of the text file, the contents of which will be printed out by the function. The 
 * fileName is checked to see if it belongs to a file that exists. If it does not, 
 * the user is alerted. If the file exists, each character of the text file is 
 * parsed over and printed. 
 * @param fileName The name of the file whose content is to be printed out. 
 */
void showFile(char *fileName){
    FILE *source; //File pointer pointing to the file to be read
    char ch; //Used to contain the current character being read
    source = fopen(fileName, "r"); //Opens file in read mode
    if (source == NULL){ //If such a file does not exist
        printf("File %s does not exist", fileName);
    }else{
        printf("\nThe file contains the following: \n");
        //Reads the whole file and prints every character
        while( ( ch = fgetc(source) ) != EOF ){
            printf("%c", ch);
        }
        fclose(source); //Closes the text file
    }
}

/** The appendFile method takes in the file name for the file to
 * be appended to. It will then ask the user for the text they want
 * to append into the text file. The source variable contains the 
 * file pointer to the file. The fprintf function is then used to 
 * append the text into the end of the text file.
 * @param fileName The name of the file to be appended to.
 */
void appendFile(char *fileName){
    FILE *source; //Declares a file pointer to point to the source file
    source = fopen(fileName, "r"); //Opens the file in read mode
    if (source == NULL){ //If the file does not exist
        printf("File %s does not exist", fileName); //Prints message alerting user
    }else{
        char message[50];
        memset(message,0,50);
        fclose(source); //Closes the file 
        source = fopen(fileName, "a"); //Re-opens the file in append mode
        printf("Enter the text you would like to add\n"); //Asks for user to enter the text they want to append
        char text[30]; //Holds the text the user has entered
        fgets(text,30,stdin); //Retrieves the text that the user entered 
        fprintf(source,"%s",&text);//Adds the text to the end of the text file
        fclose(source);//Closes the text file
        strtok(text,"\n"); //Removes the newline character from the end of the string
        strcat(message, "Line appended to file:  "); //Creates message for logging purposes
        if (strcmp(text,"\n")!=0){ //Checks if an empty line was entered by the user to append
            strcat(message, text); //If not, then the user entered text is concatendated to message
        }else{
            strcat(message, "'newline'");//If an empty line was entered, then the message will instead hold newline
        }
        logAction(fileName, message); //Logs the message into log.txt
        printf("Text has been appended\n");//Prints message for the user to say the text has been appended
        
    }
}

/** The check_integer method takes a string input and evaluates whether it
 * is an integer or not. This is used by multiple functions in order to 
 * verify whether an input that is supposed to be an integer (eg: lineNo to 
 * insert data into a text file). A flag 'is_int' keeps track of whether the input
 * is deemed to be an integer or not. If it is not, then during the testing operationThe method iterates  through all the characters
 * of the input and checks whether they are a digit or not. If one of them is not, then the is_int flag is
 * set to 0 which means the input is not an integer. The for loop is terminated. If the input is
 * still deemed to be an integer, two final checks are performed. Firstly, the input should not be 0
 * and also not be empty. If one of these tests fail,then the is_int flag is set to 0. At the end, if the in_int
 * flag is still 1, then 1 is returned back to the main program indicating the input was indeed an integer.
 * @param input. String containing the input that needs to be verified
 * @param input_len. Integer that contains the length of the string
 * @return is_int. Returns the flag variable indicating whether the input is an integer or not
 */
int check_integer(char input[], int input_len){
    int is_int=1; //Flag variable indicating whether the input is an integer or not 
    for (int i=0;i<input_len;i++){ ////Iterates through every character
       //Checks if character is a digit or not
       if (isdigit(input[i])){
           ;
       }else{
           is_int = 0; //Sets flag variable to False
           break; //Stops the for loop
       }
    }    
    if (is_int){ //If input only contains digits
        //If the length is greater than 9
        if(atoi(input)==0){ //Checks if input is 0
            is_int =0; //Sets flag to False
        }
    }    
    if (input_len==0){ //Checks if nothing was entered//
        is_int = 0;
    }
    return(is_int);
}

/**The deleteLine function is designed to remove a line from the text file named
 * in the parameter. The file name will be passed into the method and checked to 
 * see whether it exists or not. If it does not, then the user is informed. Otherwise
 * the user is asked what line number they would like to delete. After this is
 * validated to be an integer using the check_integer method, the program will first 
 * read in all the text from the source file. The file is then iterated over while
 * keeping a count of the current line using the line_count variable. This is incremented 
 * each time a newline character is found. Each character is used to overwrite the same 
 * file, however when the line_count variable contains the same value as line_no 
 * (the line number that needs to be deleted), the characters are not written into
 * the file. This means at the end, the characters from that line are no longer in
 * the text file. If the line is deleted, then the delete flag is set to True. If at the
 * end of the method, the delete flag is still false, then the line has not been deleted
 * as the line number they entered did not exist. The user is alerted to this. 
 * @param fileName The name of the text file from which the data needs to be deleted
 */
void deleteLine(char *fileName){
    FILE *source; //File pointer to file that needs the line deleting
    source = fopen(fileName, "r"); //Opens file in read mode
    if (source == NULL){
        printf("File %s does not exist", fileName); //If the file is not present in the directory
    }else{
        printf("Enter the line number which you would like to delete: \n"); 
        char line[10];
        fgets(line, 10, stdin); //Stores the line number that needs deleting
        strtok(line,"\n");
        int line_no = atoi(line); //Converts the input into an integer
        if (check_integer(line, strlen(line))==0){ //If the input is a valid integer
            printf("You did not enter an integer greater than 1 as the line number you would like to delete\n");
        }else{
            char complete_file[1000];
            getString(source, complete_file); //Retrieves the contents of the file
            int count = strlen(complete_file); //Counts the number of characters in the file
            remove(fileName);//Deletes the file 
            source = fopen(fileName, "w"); //Re-opens an empty file with the same name 
            int line_count = 1; //Counter variable to store the line number
            int delete = 0; //Boolean integer value, stores whether the line was deleted or not
            for (int i=0; i<=count; i++){ //Iterates through each the length of the entire contents
                if (line_count != line_no){ //If current line not the line that needs deleting
                    fprintf(source, "%c", complete_file[i]); //Insert the character into the text file
                }else{
                    delete = 1; //Set delete to 1 to indicate the text has been deleted
                }
                if (complete_file[i] == '\n'){ //If current character is a new line character
                    line_count +=1; //Increments the counter variable
                }
            }
            if (delete==1){ //If the line was actually deleted
                char messagep1[] = "Line "; //Starts creating the message 
                strcat(messagep1, line);//Concatenates the line number to the message
                strcat(messagep1, " deleted");//Concatenates 'deleted' text to message
                char* message = messagep1; //Creates a pointer to the message string
                fclose(source);
                logAction(fileName, message);//Logs the message into log.txt
            }else{
                //If the text was not deleted, prints message to tell the user the this was the case
                printf("Line no %i, does not exist\n", line_no);  
                fclose(source);
            }
        }
    }
}


/**The showLine method takes a file name and asks the user
 * for the line number they would like to see. All the contents 
 * of the text file are read in from the text file and then iterated
 * over. Each time a newline character is come across, the total_line_count
 * counter is incremented. When this number is equal to the line number 
 * the user wanted to see, each character is printed out. The total
 * number of lines in the file is retrieved using the countLines method. 
 * If this value is greater than the line number the user entered, 
 * then the user is alerted to the fact that the line number they 
 * entered was out of bounds. 
 * @param fileName The name of the file that the text needs to be displayed from
 * 
 */
void showLine(char *fileName){
    FILE *source; //File pointer to file that needs the line deleting
    source = fopen(fileName, "r"); //Opens the file in read mode
    if (source == NULL){ //If file does not exist
        printf("File %s does not exist", fileName);
    }else{
        char complete_file[1000]; //Stores the text from the text file
        getString(source, complete_file); //Retrieves the text from the text file
        int count = strlen(complete_file); //Counts the number of characters 
        int total_line_count = 1; //Stores the total number of lines in the text file
        //Iterates through the text file
        for (int i=0; i<count; i++){ 
            //Counts the number of newline characters
            if (complete_file[i] == '\n'){
                total_line_count +=1; //Increments line counter
            }
        }
        printf("Enter the line number which you would like to show: ");
        char line[10]; //Stores the line number which the user wants displayed
        fgets(line, 10, stdin);
        strtok(line, "\n");
        int line_no = atoi(line); //Converts the input into an integer
        if (check_integer(line, strlen(line))==0){ //If the input is a valid integer
            printf("You did not enter an integer greater than 1 as the line number you would like to see\n");
        }else{
            //If line number entered exceeds number of lines in the file
            if (total_line_count<line_no){
                printf("Line %i does not exist in the text file\n", line_no);
            }else{
                int line_count = 1; //Counter for the current line number
                for(int i=0; i<count; i++){ //Iterates through every character 
                    if (line_count == line_no){// If the current line number is the line the user wants to see
                        printf("%c", complete_file[i]); //Prints the characters on that line
                    }
                    if (complete_file[i] == '\n'){ //If new line
                        line_count +=1; //Increments the line counter variable
                    }
                }
            }
        }
    }
}

/**The searchF method will take a fileName which will be searched for.
 * The method will ask for the text that the user wants to search for.
 * The process starts by reading the text in the text file to string
 * called 'complete_file'. Each line is iterated through, and the 
 * strstr method will be used to check for the existence of substring 
 * in the string 'complete_file'. The line number is also kept a count 
 * of by checking how many '\n' characters have been parsed. If the
 * text is found, the location of the first character of the search term
 * in the text file is stored in the location pointer.  
 * @param fileName The name of the text file from which the user wants the 
 * text to be searched from. 
 */
void searchF(char *fileName){
    FILE *source; //File pointer to the search file
    source = fopen(fileName, "r"); //Opens file in read mode
    if (source == NULL){ //If the file does not exist
        printf("File %s does not exist", fileName); //Informs the user
    }else{
        //Receives the search string from the user
        printf("Enter the text you would like to search for: ");
        char line[100];
        fgets(line,100, stdin); //Fetches input from the user
        strtok(line, "\n");
        char complete_file[1000]; //Holds the total contents of the text file being searched
        char* start = &complete_file[0]; //Sets a pointer to the start of completeFile
        getString(source, complete_file); //Retrieves contents of the file being searched
        char *location = strstr(complete_file, line); //holds location of the beginning of the substring
        if (location){ //If search term is found
            int line_count = 1; //Holds the line number
            int count = 0; //Holds the character number
            while(start<location){ //while the substring location is not reached
                //Keeps a count of the number of lines until the substring location
                if (complete_file[count]=='\n'){ //If current character is a newline
                    line_count+=1; //Increment new line counter
                }
                start++; //Increment the current location pointer
                count++; //Increment the current character index location
            }
            //Prints message saying data was found along with the line number
            if (strcmp(line, "\n")==0){ //If an empty line is entered
                printf("You entered nothing to be searched");
            }else{
                printf("The string '%s' was found on line %i", line, line_count);
            }
        }else{
            printf("The string '%s' was not found", line); //If the data is not found    
        }
    }
}


/**
 * The moveF function takes the file name that needs to be moved
 * and asks the user for the directory that the file should be moved
 * too. The directory the user enters might contain a leading forward
 * or it might be not. To handle this potential problem, if a leading 
 * forward slash is present, then this is removed from the string. 
 * Afterwards, the directory is checked to see if it exists. If it does
 * not, then the the user is showed an error message. If the directory 
 * does exist, the file name will be concatentated to the directory. The 
 * copyFile function is then used to copy the original file to the destination
 * file.
 * @param fileName The name of the file that should be copied. 
 */
void moveF(char *fileName){
    FILE *source; //File pointer to the text file to be moved
    source = fopen(fileName, "r"); //Opens the file in read mode
    char complete_file[1000]; //To store the contents of the file
    if (source == NULL){ //If file does not exist
        printf("File %s does not exist", fileName);
    }else{
        getString(source, complete_file); //Retrieves the contents of the file
        printf("Enter the path you would like to move the file to: \n");
        char line[100]; //To store the directory entered by the user
        char full_directory[1000]; //To store the final copy directory 
        fgets(line, 1000, stdin);
        strtok(line, "\n");
        DIR* dir = opendir(line); //Opens directory from the user input
        if (dir){ //Checks if the directory exists
            closedir(dir);//Closes the directory
            //Removes the last forward slash if entered by the user
            if (line[strlen(line)-1]=='/'){ 
                line[strlen(line)-1] = '\0'; //Replaces with string terminating character
            }
            strcat(line, "/");//Adds another forward slash
            strcat(full_directory, line);//Concatenates the file directory to the final directory
            strcat(full_directory, fileName); //Concatenates the file name to the final directory
            copyFile(fileName, full_directory); //Creates a copy of the file 
            memset(complete_file,0,1000); //Resets the memory
            printf("File moved \n");
            remove(fileName);
        }else{
            printf("directory does not exist"); //If directory existed, but did not open
        }
    }
}


/**The showLog method displays the contents of the log file. The 
 * whole file is read in and then printed out onto the terminal.
 */
void showLog(){
    FILE *source; //Declares file pointer for source file
    source = fopen("log.txt", "r"); //Opens log file
    char complete_file[1000];//Character array to hold log file contents
    getString(source, complete_file); //Retrieves the log file contents
    printf("%s",complete_file); //Prints the contents of the file
    }
    
    
int main(void) {
    //Prints menu for the user to choose the operation from
    printf("Menu: \n");
    printf(" Description | Command \n");
    printf("1) Creating a file: create\n");
    printf("2) Copying a file: copy \n");
    printf("3) Deleting a file: delete \n");
    printf("4) Showing a file: show \n");
    printf("5) Appending to a file: append\n");
    printf("6) Deleting a line from a file: deleteL\n");
    printf("7) Inserting to a line in a file: insertL\n");
    printf("8) Showing a line in a file: showL\n");
    printf("9) Show change log: showLog\n");
    printf("10) Show number of lines: countL\n");
    printf("11) Move directory: moveF \n");
    printf("12) Search a file: searchF\n");
    printf("Please enter the operation you would like to execute: \n");
    char opcode[100]; //Stores the inctruction being performed
    fgets(opcode, 100, stdin); //Retrieves the instruction entered by the user
    strtok(opcode, "\n"); //Removes the 
    
    //Checks if the command is to show the log file
    if(strcmp(opcode, "showLog")==0){
        char file[] = "log.txt"; //Shows the file contents in the log file
        showFile(file); 
    }else{
        //Asks for the file name the operation should be carried out on
        printf("Enter the file you would like to carry out the operation on:\n");
        char operand[20];
        char fileName[20];
        fgets(operand,20, stdin);         
        //Checks if the file name is valid
        if (checkTxt(operand)==1){
            //If operation is to create a file
            if (strcmp(opcode, "create")==0){
                createFile(operand);
            }else if (strcmp(opcode, "copy")==0){
                //Asks for the name of the file to be copied to
                printf("Enter the file name you would like to copy to: \n");
                char fileName[20];
                fgets(fileName, 20, stdin);
                strtok(fileName, "\n");
                if (checkTxt(fileName)==1){
                    copyFile(operand, fileName);
                }
            }else if(strcmp(opcode, "delete")==0){
                deleteFile(operand);
            }else if(strcmp(opcode, "show")==0){
                showFile(operand);
            }else if(strcmp(opcode, "append")==0){
                appendFile(operand);
            }else if(strcmp(opcode, "deleteL")==0){
                deleteLine(operand);
            }else if(strcmp(opcode, "insertL")==0){
                //Asks for the text to be inserted
                printf("Enter the text that you would like to insert: \n");
                char insert_text[20];
                fgets(insert_text, 20, stdin);
                strtok(insert_text, "\n");
                //Asks for the line number to insert the text into
                printf("Enter the line number which you would like to insert into: ");
                char line[10]; //The line number that the text will be inserted into
                fgets(line, 10, stdin); //Retrieves the user input
                strtok(line, "\n"); //Removes the new line character
                int line_no = atoi(line); //Converts the input into an integer
                if (check_integer(line, strlen(line))==0){ //If the input is a valid integer
                    printf("You did not enter an integer greater than 1 as the line number you would like to insert\n");
                }else{
                    //If the line number exceeds the maximum number of lines
                    if(line_no>20){
                        printf("Maximum number of 20 lines exceeded\n");
                    }else{
                        //Inserts the line of text into the requested line
                        insertLine(operand, insert_text, line_no);
                    }
                }
            }else if(strcmp(opcode, "showL")==0){
                showLine(operand);
            }else if(strcmp(opcode, "countL")==0){
                countLine(operand);
            }else if(strcmp(opcode, "searchF")==0){
                searchF(operand);
            }else if(strcmp(opcode, "moveF")==0){
                moveF(operand);
            //If the entered text does not match any operations 
            }else{
                printf("incorrect operation");
            }
        }
    }
}
