



#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cstdlib>
using namespace std;


class BtreeNode {
public:
    string filePath;
};

class Btree {
public:
    int order;
    char* rootFile;
    Btree(int o) {
        order = o;
        rootFile = nullptr;
    }
    void myStrcpy(char* dest, const char* src) {
        int i = 0;
        // Copy each character from src to dest until we hit the null terminator
        while (src[i] != '\0') {
            dest[i] = src[i];
            i++;
        }
        // Null-terminate the destination string
        dest[i] = '\0';
    }

    //this function counts the number of rows in the csv
    int countLinesInCSV(const char* filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file!" << endl;
            return -1;
        }

        char line[1024]; // Buffer to hold each line
        int lineCount = 0;

        while (file.getline(line, sizeof(line))) {
            lineCount++;
        }

        file.close();
        return lineCount;
    }
    //this function returns the data on particular row and column
    char* getWord(int column, char* row) {
        column--;
        int count = 0;
        int commaCount = 0;
        int s = 0;
        for (int i = 0; row[i] != '\0'; i++) {
            s++;
        }
        s++;
        int size = s;
        for (int i = 0; i < size; i++) {
            if (commaCount == column) {
                break;
            }
            else if (row[i] == ',') {
                commaCount++;
            }
            count++;
        }
        char* word = new char[100];
        int m;
        for (m = 0; m < size; m++) {
            if (row[count] == ',') {
                break;
            }
            word[m] = row[count++];
        }

        word[m] = '\0';
        return word;
    }

    //
    char* loadData(char* filePath) {
        ifstream file(filePath);
        char* word = new char[100];
        if (file.is_open()) {
            file.getline(word, 100);
            cout << word << endl;
            file.close();
        }
        else {
            cout << "Cannot open " << filePath << endl;
            return nullptr;
        }
        return word;
    }
    char* manualSprintf(int x, char*& root) {
        int index = 0;

        // Append "Tree\\" to the root
        const char* folder = "Tree\\";
        while (*folder != '\0') {
            root[index++] = *folder++;
        }

        // Convert integer x to string and append it to root
        char number[12]; // Buffer for integer conversion
        int numIndex = 0;

        if (x == 0) {
            number[numIndex++] = '0';
        }
        else {
            // Convert number to string in reverse order
            int temp = x;
            while (temp > 0) {
                number[numIndex++] = (temp % 10) + '0';
                temp /= 10;
            }
            // Reverse the number string
            for (int i = 0; i < numIndex / 2; ++i) {
                char swap = number[i];
                number[i] = number[numIndex - i - 1];
                number[numIndex - i - 1] = swap;
            }
        }
        number[numIndex] = '\0'; // Null-terminate the number string

        // Append the number string to root
        for (int i = 0; number[i] != '\0'; ++i) {
            root[index++] = number[i];
        }

        // Append ".txt" to root
        const char* extension = ".txt";
        while (*extension != '\0') {
            root[index++] = *extension++;
        }

        // Null-terminate the final string
        root[index] = '\0';
        return root;
    }

    char* setFilePath(char* filePath, int row, char* rowData, char* rowName) {
        manualSprintf(row, filePath);
        if (filePath[0] == '\0') {
            cout << "sprint not right" << endl;
            return nullptr;
        }
        ofstream file(filePath);
        if (file.is_open()) {
            file << rowName;
            for (int i = 0; i < order - 2; i++) {
                file << ",!";
            }
            file << endl;
            //file << row << endl;

            for (int i = 0; rowData[i] != '\0'; i++) {
                if (rowData[i] == ',') {
                    file << endl;
                }
                else {
                    file << rowData[i];
                }
            }
            //for full row values
            for (int i = 0; i < order - 2; i++) {
                file << '\0' << endl;
            }
            //for children path
            for (int i = 0; i < order; i++) {
                file << '\0' << endl;
            }
            //for parent path
            file << '\0' << endl;
            file.close();
        }
        else {
            cout << "Failed to create file in setFilePath" << filePath << endl;

        }
        if (rootFile == nullptr) {
            rootFile = filePath;
        }
        return filePath;
    }
    int myStrCmp(const char* current, const char* previous) {
        int i = 0;
        int j = 0;
        // Compare characters until a difference is found or one of the strings ends
        while (previous[i] != '\0') {
            while (previous[i] != ',') {
                if (current[j] < previous[i]) {

                }
            }
            if (current[i] > previous[i]) {
                return 1; // Current is larger
            }
            i++;
        }

        // If one string ends before the other
        if (current[i] == '\0' && previous[i] != '\0') {
            return -1; // Current is smaller (it ended first)
        }
        if (current[i] != '\0' && previous[i] == '\0') {
            return 1; // Current is larger (it didn't end yet)
        }

        return 0; // Strings are equal
    }
    bool hasEmptySpace(const char* prev) {
        int i = 0;

        while (prev[i] != '\0') {
            // Check for the pattern '!,'
            if (prev[i] == ',' && prev[i + 1] == '!') {
                return true; // Found an empty space
            }
            i++;
        }

        return false; // No empty space found
    }
    void writeWordAndShift(const char* filePath, const char* word, const char* extraData) {
        const int bufferSize = 1024;
        char buffer[bufferSize];
        ifstream inFile(filePath, ios::in);
        ofstream tempFile("temp.txt", ios::out);

        if (!inFile.is_open() || !tempFile.is_open()) {
            cerr << "Error: Unable to open file!" << endl;
            return;
        }

        bool replaced = false;      // Flag to indicate if `,!` has been replaced
        bool extraWritten = false;  // Flag to indicate if `extraData` has been written

        while (inFile.getline(buffer, bufferSize)) {
            // Look for `,!` manually in the current line
            bool foundCommaExclamation = false;
            int index = 0;

            // Check each character in the buffer
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (buffer[i] == ',' && buffer[i + 1] == '!') {
                    foundCommaExclamation = true;
                    index = i;  // Record the position of `,!`
                    break;
                }
            }

            if (foundCommaExclamation && !replaced) {
                // Write everything before `,!`
                for (int i = 0; i < index; i++) {
                    tempFile.put(buffer[i]);
                }
                // Write the word in place of `,!`
                tempFile.put(',');
                for (int i = 0; word[i] != '\0'; i++) {
                    tempFile.put(word[i]);
                }
                // Write everything after `,!`
                for (int i = index + 2; buffer[i] != '\0'; i++) {
                    tempFile.put(buffer[i]);
                }
                tempFile.put('\n');
                replaced = true;
                continue;
            }

            // If the line is completely empty (`\0`) and we haven't written `extraData`
            if (buffer[0] == '\0' && !extraWritten) {
                for (int i = 0; extraData[i] != '\0'; i++) {
                    tempFile.put(extraData[i]);
                }
                tempFile.put('\n');
                extraWritten = true;
            }
            else {
                // Write the line as is
                for (int i = 0; buffer[i] != '\0'; i++) {
                    tempFile.put(buffer[i]);
                }
                tempFile.put('\n');
            }
        }

        inFile.close();
        tempFile.close();

        // Manually replace the original file with the temporary file
        ifstream tempIn("temp.txt", ios::in);
        ofstream outFile(filePath, ios::out);

        if (!tempIn.is_open() || !outFile.is_open()) {
            std::cerr << "Error: Unable to finalize file replacement!" << std::endl;
            return;
        }

        while (tempIn.getline(buffer, bufferSize)) {
            for (int i = 0; buffer[i] != '\0'; i++) {
                outFile.put(buffer[i]);
            }
            outFile.put('\n');
        }

        tempIn.close();
        outFile.close();

        // Delete the temporary file manually
        ofstream clearTemp("temp.txt", ios::out);
        clearTemp.close();
    }
    void sortAndOrganizeFile(const char* filePath, int columnCount) {
        const int MAX_LINE_LENGTH = 1024; // Maximum line length
        const int MAX_ROWS = 100;        // Maximum number of rows

        // Dynamically allocate arrays
        char** headerRow = new char* [columnCount];
        for (int i = 0; i < columnCount; ++i) {
            headerRow[i] = new char[MAX_LINE_LENGTH];
        }

        char*** rows = new char** [MAX_ROWS];
        for (int i = 0; i < MAX_ROWS; ++i) {
            rows[i] = new char* [columnCount];
            for (int j = 0; j < columnCount; ++j) {
                rows[i][j] = new char[MAX_LINE_LENGTH];
            }
        }

        // Open the file
        ifstream inFile(filePath);
        if (!inFile.is_open()) {
            cerr << "Error: Unable to open file for sorting!" << endl;
            return;
        }

        // Read the header row
        char line[MAX_LINE_LENGTH];
        if (inFile.getline(line, MAX_LINE_LENGTH)) {
            int column = 0, index = 0;
            for (int i = 0; line[i] != '\0'; ++i) {
                if (line[i] == ',') {
                    headerRow[column][index] = '\0'; // Null-terminate the word
                    ++column;
                    index = 0;
                }
                else if (line[i] != '!') { // Skip '!' characters
                    headerRow[column][index++] = line[i];
                }
            }
            headerRow[column][index] = '\0'; // Null-terminate the last word
        }

        // Read the rows beneath the header
        int row = 0;
        while (inFile.getline(line, MAX_LINE_LENGTH) && row < MAX_ROWS) {
            int column = 0, index = 0;
            for (int i = 0; line[i] != '\0'; ++i) {
                if (line[i] == ',') {
                    rows[row][column][index] = '\0'; // Null-terminate the word
                    ++column;
                    index = 0;
                }
                else {
                    rows[row][column][index++] = line[i];
                }
            }
            rows[row][column][index] = '\0'; // Null-terminate the last word
            ++row;
        }
        inFile.close();

        // Sorting indices based on the header
        int* indices = new int[columnCount];
        for (int i = 0; i < columnCount; ++i) {
            indices[i] = i;
        }

        // Manual sorting based on the headerRow array
        for (int i = 0; i < columnCount - 1; ++i) {
            for (int j = i + 1; j < columnCount; ++j) {
                if (strcmp(headerRow[indices[i]], headerRow[indices[j]]) > 0) {
                    // Swap indices
                    int temp = indices[i];
                    indices[i] = indices[j];
                    indices[j] = temp;
                }
            }
        }

        // Apply the sorted order
        char** sortedHeader = new char* [columnCount];
        for (int i = 0; i < columnCount; ++i) {
            sortedHeader[i] = new char[MAX_LINE_LENGTH];

            myStrcpy(sortedHeader[i], headerRow[indices[i]]);
        }

        char*** sortedRows = new char** [MAX_ROWS];
        for (int i = 0; i < MAX_ROWS; ++i) {
            sortedRows[i] = new char* [columnCount];
            for (int j = 0; j < columnCount; ++j) {
                sortedRows[i][j] = new char[MAX_LINE_LENGTH];
                myStrcpy(sortedRows[i][j], rows[i][indices[j]]);
            }
        }

        // Write sorted data back to the file
        ofstream outFile(filePath, ios::trunc);
        if (!outFile.is_open()) {
            cerr << "Error: Unable to open file for writing sorted data!" << endl;
            return;
        }

        // Write sorted header
        for (int i = 0; i < columnCount; ++i) {
            outFile << sortedHeader[i];
            if (i != columnCount - 1) outFile << ',';
        }
        outFile << '\n';

        // Write sorted rows
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < columnCount; ++j) {
                outFile << sortedRows[i][j];
                if (j != columnCount - 1) outFile << ',';
            }
            outFile << '\n';
        }
        outFile.close();

        // Free allocated memory
        for (int i = 0; i < columnCount; ++i) {
            delete[] headerRow[i];
            delete[] sortedHeader[i];
        }
        delete[] headerRow;
        delete[] sortedHeader;

        for (int i = 0; i < MAX_ROWS; ++i) {
            for (int j = 0; j < columnCount; ++j) {
                delete[] rows[i][j];
                delete[] sortedRows[i][j];
            }
            delete[] rows[i];
            delete[] sortedRows[i];
        }
        delete[] rows;
        delete[] sortedRows;
        delete[] indices;
    }

    void appendWordAndRow(const char* filePath, const char* newWord, const char* newRowData) {
        const int bufferSize = 1024; // Buffer size for reading lines
        char buffer[bufferSize];

        ifstream inFile(filePath, ios::in);
        ofstream tempFile("temp.txt", ios::out);

        if (!inFile.is_open() || !tempFile.is_open()) {
            cerr << "Error: Unable to open file!" << endl;
            return;
        }

        // Read the first line and append the new word
        if (inFile.getline(buffer, bufferSize)) {
            // Find the end of the current words in the first line
            int i = 0;
            while (buffer[i] != '\0') {
                i++;
            }
            // Append a comma and the new word
            buffer[i++] = ',';
            for (int j = 0; newWord[j] != '\0'; j++) {
                buffer[i++] = newWord[j];
            }
            buffer[i] = '\0'; // Null-terminate the updated line

            // Write the updated first line to the temp file
            tempFile << buffer << '\n';
        }

        // Process subsequent rows
        int rowCount = 0;
        while (inFile.getline(buffer, bufferSize)) {
            tempFile << buffer << '\n';
            rowCount++;
        }

        // Append the new row
        tempFile << newRowData << '\n';

        // Add empty rows to shift the content below
        for (int i = 0; i < rowCount; i++) {
            tempFile << '\0' << '\n';
        }

        inFile.close();
        tempFile.close();

        // Replace the original file with the updated file
        ifstream tempIn("temp.txt", ios::in);
        ofstream outFile(filePath, ios::out);

        if (!tempIn.is_open() || !outFile.is_open()) {
            cerr << "Error: Unable to finalize file replacement!" << endl;
            return;
        }

        while (tempIn.getline(buffer, bufferSize)) {
            outFile << buffer << '\n';
        }

        tempIn.close();
        outFile.close();

        // Delete the temporary file
        ofstream clearTemp("temp.txt", ios::out);
        clearTemp.close();
    }
    char* setPath2(char* filePath, int row, char* rowData, char* rowName, char* parent) {
        manualSprintf(row, filePath);
        if (filePath[0] == '\0') {
            cout << "sprint not right" << endl;
            return nullptr;
        }
        ofstream file(filePath);
        if (file.is_open()) {
            file << rowName;
            for (int i = 0; i < order - 2; i++) {
                file << ",!";
            }
            file << endl;
            //file << row << endl;

            for (int i = 0; rowData[i] != '\0'; i++) {
                if (rowData[i] == ',') {
                    file << endl;
                }
                else {
                    file << rowData[i];
                }
            }
            for (int i = 0; i < order - 2; i++) {
                file << '\0' << endl;
            }
            for (int i = 0; i < order; i++) {
                file << '\0' << endl;
            }
            file << parent << endl;
            file.close();
        }
        else {
            cout << "Failed to create file in setFilePath" << filePath << endl;

        }
        if (rootFile == nullptr) {
            rootFile = filePath;
        }
        return filePath;
    }
    char* getWord2(char* inputString, int index) {
        int commaCount = 0; // Keeps track of the number of commas encountered.
        int startIdx = 0;   // The starting index of the word to extract.
        int endIdx = 0;     // The ending index of the word.

        // Find the index-th word by counting commas.
        for (int i = 0; inputString[i] != '\0'; i++) {
            if (commaCount == index - 1) {
                startIdx = i; // Mark the start of the word.
                break;
            }
            if (inputString[i] == ',') {
                commaCount++;
            }
        }

        // Find the end of the word.
        for (int i = startIdx; inputString[i] != '\0'; i++) {
            if (inputString[i] == ',' || inputString[i] == '\0') {
                endIdx = i;
                break;
            }
        }

        // Allocate space for the word.
        int wordLength = endIdx - startIdx;
        char* word = new char[wordLength + 1]; // Extra space for the null terminator.

        // Copy the word into the allocated memory.
        for (int i = 0; i < wordLength; i++) {
            word[i] = inputString[startIdx + i];
        }
        word[wordLength] = '\0'; // Null-terminate the string.

        return word;
    }
    void SplitFile(char* filePath) {

        ifstream inFile(filePath);
        if (!inFile.is_open()) {
            cerr << "Error: Unable to open file!" << endl;
            return; // Return an error code if the file cannot be opened
        }

        const int MAX_LINE_LENGTH = 1024;
        char line[MAX_LINE_LENGTH];

        // Read the first line (header row)
        if (!inFile.getline(line, MAX_LINE_LENGTH)) {
            cerr << "Error: Unable to read first line!" << endl;
            return;
        }

        // Count the number of words in the first row
        int wordCount = 0;
        for (int i = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') {
                wordCount++;
            }
        }
        wordCount++; // For the last word after the last comma

        // Calculate the middle word's index

        int middleIndex = (wordCount % 2 == 0) ? (wordCount / 2) : (wordCount / 2); // Middle word index (ceil of wordCount/2 for even case)
        inFile.close();

        char* child1 = new char[100];
        child1[0] = '\0';
        inFile.open(filePath);
        char wordArray[200];
        inFile.getline(wordArray, 200);
        char rowthing[300];
        inFile.getline(rowthing, 300);
        child1 = setPath2(child1, 0, rowthing, getWord2(wordArray, 0), filePath);
        int before = middleIndex - 1;
        int after = order - middleIndex;

        for (int i = 1; i < before; i++) {
            inFile.getline(rowthing, 300);
            writeWordAndShift(child1, getWord2(wordArray, i), rowthing);
            sortAndOrganizeFile(child1, order - 1);
            /*child1 = setPath2(child1, 0, rowthing, getWord2(wordArray, i), filePath);*/
        }

        char* child2 = new char[100];
        child2[0] = '\0';
        inFile.getline(rowthing, 300);
        child2 = setPath2(child2, 0, rowthing, getWord2(wordArray, middleIndex + 1), filePath);
        for (int i = middleIndex + 2; i < order; i++) {
            inFile.getline(rowthing, 300);
            writeWordAndShift(child2, getWord2(wordArray, i), rowthing);
            sortAndOrganizeFile(child2, order - 1);
            /*child1 = setPath2(child1, 0, rowthing, getWord2(wordArray, i), filePath);*/
        }
        inFile.close();
        for (int i = 0; i < order - 1;i++) {

        }


        /*
        first check the first row
        in the first row the words are seperated by a comma
        each word has a respective row below in the same order as the words
        we have to first find the middle word
        if there are even number of words then it will take ceil of words/2 and the total words in this case are equal to order

        now it will call the setfilepath2 function and make a new file giving the first word as the actualData and the first row after row1 as the row data
        for no of words to the left-1 times it should call writeWordAndShift and then sortAndOrganizeFile
        after order-1 rows plus the first row  it should write the path of this new file in that row

        then it will call the setfilepath2 function and make a new file
        for no of words to the right-1 times it should call writeWordAndShift and then sortAndOrganizeFile
        after order-1 rows plus the first row plus one another row it should write the path of this new file in that row

        then it should move the middle word to the first pposition and its respective row to the first place
        and then it should place ! for all other words and remove those words now the total words must be equal to order-1 and their respective rows must be equal to \0 and the num of rows must be order-1

        you can change the function arguments acc to your will

        */
    }
    void insert2(char* filePath, char* actualData, int rowNum, char* rowData, int columnNo) {
        //if no file exists
        if (filePath[0] == '\0') {
            delete filePath;
            filePath = new char[100];
            setFilePath(filePath, rowNum, rowData, actualData);
        }
        else {
            if (hasEmptySpace(loadData(filePath))) {
                writeWordAndShift(filePath, actualData, rowData);
                sortAndOrganizeFile(filePath, order - 1);
            }
            else {
                appendWordAndRow(filePath, actualData, rowData);
                sortAndOrganizeFile(filePath, order);

            }

        }
    }
    void insert(const char* csv, int column) {

        ifstream file;
        file.open(csv);
        if (!file.is_open()) {
            cout << "File cannot be opened in insert" << endl;
            return;
        }
        int row = countLinesInCSV(csv);
        cout << "rows: " << row << endl;
        char* PathArray = new char[100];
        PathArray[0] = '\0';
        char tempRow[200];
        file.getline(tempRow, 200);
        for (int i = 0; i < row - 1; i++) {
            file.getline(tempRow, 200);
            insert2(PathArray, getWord(column, tempRow), i, tempRow, column);
        }
    }
};
//-----------------------------------------------------------------------------------------------------------------------
int countLinesInCSV(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Error: Unable to open file!" << std::endl;
        return -1;
    }

    char* line = new char[1000];
    int lineCount = 0;

    while (file.getline(line, 1000)) {
        lineCount++;
    }

    file.close();
    return lineCount;
}


int getSize(char* row) {
    int size = 0;
    while (row[size] != '\0') {
        size++;
    }
    return size;
}
//get a word in a columns
char* getWord(int column, char* row) {
    column--;
    int count = 0;
    int commaCount = 0;
    int size = getSize(row);

    for (int i = 0; i < size; i++) {
        if (commaCount == column) {
            break;
        }
        else if (row[i] == ',' || row[i] == '\n') {
            commaCount++;
        }
        count++;
    }

    char* word = new char[100];
    int m;
    for (m = 0; m < size; m++) {
        if (row[count] == ',' || row[count] == '\0') {
            break;
        }
        word[m] = row[count++];
    }

    word[m] = '\0';
    return word;
}

int compareString(char* a, char* b) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
        i++;
    }
    if (a[i] == '\0' && b[i] == '\0') return 0;
    return (a[i] == '\0') ? -1 : 1;
}

class AVL {
    int columns;
    int c;
public:
    AVL(int x) {
        c = x;
    }

    int balanceFactor(char* nFilePath) {

        int leftHeight = loadHeight(loadLeftChild(nFilePath));
        int rightHeight = loadHeight(loadRightChild(nFilePath));
        cout << leftHeight << " lh" << endl;
        cout << rightHeight << " rh" << endl;
        return leftHeight - rightHeight;
    }

    int loadHeight(char* filePath) {
        ifstream file(filePath);
        int h = -1;

        if (file.is_open()) {
            char* line = new char[100];
            file.getline(line, 100);
            file.getline(line, 100);
            if (file.getline(line, 100)) {
                h = atoi(line);
            }

            file.close();
        }
        else {
            cout << "Cannot open " << filePath << endl;
            return -1;
        }

        return h;
    }
    void stringcopy(char* destination, char* source, int maxSize) {
        int i;
        for (i = 0; i < maxSize - 1 && source[i] != '\0'; i++) {
            destination[i] = source[i];
        }
        destination[i] = '\0';
    }
    char* setFilePath(char* root, int x, char* array) {
        //create a file and write everything in it
        snprintf(root, 100, "Tree\\%d.txt", x);

        ofstream file(root);
        if (file.is_open()) {
            file << getWord(c, array) << endl;;
            file << x << endl;
            file << 0 << endl;
            int size = getSize(array);
            int commaCount = 0;
            for (int i = 0; i < size; i++) {
                if (array[i] == ',') {
                    commaCount++;
                }
            }
            static int x = 0;
            if (x == 0) {
                columns = commaCount + 1;
                x++;
            }

            for (int i = 0; i <= commaCount; i++) {
                file << getWord(i + 1, array) << endl;
            }


            file << "\0" << endl;
            file << "\0" << endl;

            file.close();
        }
        else {
            cout << "Failed to create file: " << root << endl;
        }
        return root;
    }

    char* loadLeftChild(char* filePath) {
        //loads the left child from the file
        ifstream file(filePath);
        char* word = new char[100];
        if (file.is_open()) {
            for (int i = 0; i < columns + 3; i++) {
                file.getline(word, 100);

            }
            file.getline(word, 100);
            cout << word;
            file.close();
        }
        else {
            cout << "Cannot open " << filePath << endl;
            return nullptr;
        }
        return word;
    }
    char* loadData(char* filePath) {
        //loads the data form the file
        ifstream file(filePath);
        char* word = new char[100];
        if (file.is_open()) {
            file.getline(word, 100);

            file.close();
        }
        else {
            cout << "Cannot open " << filePath << endl;
            return nullptr;
        }
        return word;
    }
    char* loadRightChild(char* filePath) {
        //loads the right child form the file
        ifstream file(filePath);
        char* word = new char[100];
        if (file.is_open()) {
            for (int i = 0; i < columns + 4; i++) {
                file.getline(word, 100);
            }
            file.getline(word, 100);
            cout << "left File Path: " << word << endl;;
            file.close();
        }
        else {
            cout << "Cannot open " << filePath << endl;
            return nullptr;
        }
        return word;
    }

    bool compare(char* current, char* previous) {
        return compareString(current, previous) < 0;
    }


    bool rebalance(char*& root, char* x, char*& parentPath) {
        int balance = balanceFactor(root);
        bool parentUpdated = false;

        if (balance > 1) { // Left heavy
            char* leftChildPath = loadLeftChild(root);

            // Left-Right case:
            if (!compare(x, loadData(leftChildPath))) {
                leftRotation(leftChildPath);
                updateChildPath(root, "left", leftChildPath);
            }

            // Right Case:
            rightRotation(root);

            if (parentPath != nullptr) {
                if (strcmp(loadLeftChild(parentPath), root) == 0) {
                    updateChildPath(parentPath, "left", root);
                }
                else {
                    updateChildPath(parentPath, "right", root);
                }
                parentUpdated = true;
            }
        }
        else if (balance < -1) { // Right heavy
            char* rightChildPath = loadRightChild(root);

            // Right-Left case: 
            if (compare(x, loadData(rightChildPath))) {
                rightRotation(rightChildPath);
                updateChildPath(root, "right", rightChildPath);
            }

            //  Left Rotation:
            leftRotation(root);

            // Update parent
            if (parentPath != nullptr) {
                if (strcmp(loadLeftChild(parentPath), root) == 0) {
                    updateChildPath(parentPath, "left", root);
                }
                else {
                    updateChildPath(parentPath, "right", root);
                }
                parentUpdated = true;
            }
        }

        int leftHeight = loadHeight(loadLeftChild(root));
        int rightHeight = loadHeight(loadRightChild(root));
        updateHeightInFile(root, max(leftHeight, rightHeight) + 1);

        return parentUpdated;
    }



    void updateHeightInFile(char* nFilePath, int newHeight) {
        ifstream file(nFilePath);
        if (!file.is_open()) {
            cout << "Error! cannot i=open this file " << nFilePath << endl;
            return;
        }
        ofstream tempFile("temp.txt");
        if (!tempFile.is_open()) {
            cerr << "Error! cannot i=open this file " << endl;
            return;
        }

        char* line = new char[100];
        file.getline(line, 100);
        tempFile << line << endl;
        cout << line << endl;
        file.getline(line, 100);
        tempFile << line << endl;
        cout << line << endl;
        // Position where height is placed in the file
        tempFile << newHeight << endl;
        cout << newHeight << endl;
        file.getline(line, 100);
        //copying the whole file iinto temp file
        while (file.getline(line, 100)) {
            tempFile << line << endl;
            cout << line << endl;
        }


        file.close();
        tempFile.close();

        remove(nFilePath);
        bool x = rename("temp.txt", nFilePath);
    }
    void updateChildPath(char* parentPath, const char* childType, char* childPath) {
        ifstream file(parentPath);
        ofstream tempFile("temp.txt");//create a temp file to copy all the data of file

        char* line = new char[100];
        if (childType == "left") {
            for (int i = 0; i < columns + 3; i++) {
                file.getline(line, 100);
                cout << line << endl;;
                tempFile << line << endl;
            }
            cout << "hehe" << endl;
            file.getline(line, 100);
            //goto the line where left is located
            tempFile << childPath << endl;
            delete[] line;
            line = new char[100];
            // file.ignore(100,'\0');
            file.getline(line, 100);
            tempFile << line << endl;
        }
        else if (childType == "right") {
            for (int i = 0; i < columns + 4; i++) {
                file.getline(line, 100);
                cout << line << endl;
                tempFile << line << endl;
            }
            //goto the line where right is located
            file.getline(line, 100);
            cout << childPath << endl;
            tempFile << childPath << endl;
        }

        file.close();
        tempFile.close();

        remove(parentPath);
        bool x = rename("temp.txt", parentPath);
    }

    bool insert(char*& root, char* x, int i, char* array, char* parentPath = nullptr) {
        // all paths have \0 initially passed from the main
        if (root[0] == '\0') {
            root = new char[100];
            setFilePath(root, i, array);
            return false; //no rebalance as the last node is alwatys already balanced
        }

        bool stopRebalance = false; // to avoid updating the parent if it is updated during the rebalance donot update it

        if (compare(x, loadData(root))) {
            char* leftChild = loadLeftChild(root);
            if (!stopRebalance) {
                stopRebalance = insert(leftChild, x, i, array, root);
            }
            if (!stopRebalance) {
                updateChildPath(root, "left", leftChild);
            }
        }
        else {
            char* rightChild = loadRightChild(root);
            if (!stopRebalance) {
                stopRebalance = insert(rightChild, x, i, array, root);
            }

            if (!stopRebalance) {
                updateChildPath(root, "right", rightChild);
            }
        }

        if (stopRebalance) {
            return true;
        }

        bool rebalanced = rebalance(root, x, parentPath);
        //update heights
        int leftHeight = loadHeight(loadLeftChild(root));
        int rightHeight = loadHeight(loadRightChild(root));
        updateHeightInFile(root, max(leftHeight, rightHeight) + 1);

        return rebalanced;
    }





    void rightRotation(char*& root, char* parentPath = nullptr) {
        char* leftChild = loadLeftChild(root);
        //no rotate if left not
        if (!leftChild || leftChild[0] == '\0') {
            delete[] leftChild;
            return;
        }

        char* newRightChildPath = loadRightChild(leftChild);
        //rotate by changing paths
        updateChildPath(root, "left", newRightChildPath);
        updateChildPath(leftChild, "right", root);
        stringcopy(root, leftChild, 100);
        //update heights
        int leftHeight = loadHeight(loadLeftChild(root));
        int rightHeight = loadHeight(loadRightChild(root));
        updateHeightInFile(root, max(leftHeight, rightHeight) + 1);

        int newLeftHeight = loadHeight(loadLeftChild(leftChild));
        int newRightHeight = loadHeight(loadRightChild(leftChild));
        updateHeightInFile(leftChild, max(newLeftHeight, newRightHeight) + 1);

    }


    void leftRotation(char*& root, char* parentPath = nullptr) {
        char* rightChild = loadRightChild(root);
        //if there is no riht child no rotation can be made
        if (!rightChild || rightChild[0] == '\0') {
            return;
        }

        char* LeftChild = loadLeftChild(rightChild);
        //rotation by updating the paths in the file
        updateChildPath(root, "right", LeftChild);
        updateChildPath(rightChild, "left", root);
        //swap
        stringcopy(root, rightChild, 100);
        //update the heights
        int leftHeight = loadHeight(loadLeftChild(root));
        int rightHeight = loadHeight(loadRightChild(root));
        updateHeightInFile(root, max(leftHeight, rightHeight) + 1);

        int newLeftHeight = loadHeight(loadLeftChild(rightChild));
        int newRightHeight = loadHeight(loadRightChild(rightChild));
        updateHeightInFile(rightChild, max(newLeftHeight, newRightHeight) + 1);
    }



    void inorder(char* root) {
        //if the root path is null return 
        if (root == NULL || root[0] == '\0') {
            return;
        }
        inorder(loadLeftChild(root));
        cout << loadData(root) << " ";
        inorder(loadRightChild(root));
    }
};

//-----------------------------------------------------------------------------------------------------------------------------
struct Node {
    char color;
    char* coloumnvalue;
    char* data;
    char* filepath;
    Node* leftchild;
    Node* rightchild;
    Node* parent;
    int rownum;

    Node(char* row, int r, const char* colValue, Node* p = nullptr)
        : color('R'), rownum(r), leftchild(nullptr), rightchild(nullptr), parent(nullptr)
    {
        data = new char[strlen(row) + 1];
        strcpy_s(data, strlen(row) + 1, row);

        coloumnvalue = new char[strlen(colValue) + 1];
        strcpy_s(coloumnvalue, strlen(colValue) + 1, colValue);

        string filename = "node_" + string(coloumnvalue) + "_" + to_string(rownum) + ".txt";
        filepath = new char[filename.size() + 1];
        strcpy_s(filepath, filename.size() + 1, filename.c_str());
    }

    ~Node()
    {
        delete[] data;
        delete[] filepath;
        delete[] coloumnvalue;
    }
};

class RedBlackTree
{
private:
    Node* root;

    void saveNodeToFile(Node* node)
    {
        string originalFilepath = node->filepath;
        string filepath = originalFilepath;
        int counter = 1;
        while (ifstream(filepath))
        {
            filepath = originalFilepath + "_" + to_string(counter) + ".txt";
            counter++;
        }

        delete[] node->filepath;
        node->filepath = new char[filepath.size() + 1];
        strcpy_s(node->filepath, filepath.size() + 1, filepath.c_str());

        ofstream file(node->filepath);
        if (file.is_open())
        {
            file << node->data << endl;
            file << (node->parent ? node->parent->filepath : "null") << endl;
            file << (node->leftchild ? node->leftchild->filepath : "null") << endl;
            file << (node->rightchild ? node->rightchild->filepath : "null") << endl;
            file.close();
        }
        else
        {
            cout << "Error!!!!!!!!!!" << endl;
        }
    }

    void rotateLeft(Node*& root, Node*& node)
    {
        Node* rightChild = node->rightchild;
        node->rightchild = rightChild->leftchild;

        if (node->rightchild)
            node->rightchild->parent = node;

        rightChild->parent = node->parent;

        if (!node->parent)
            root = rightChild;
        else if (node == node->parent->leftchild)
            node->parent->leftchild = rightChild;
        else
            node->parent->rightchild = rightChild;

        rightChild->leftchild = node;
        node->parent = rightChild;
    }

    void rotateRight(Node*& root, Node*& node)
    {
        Node* leftChild = node->leftchild;
        node->leftchild = leftChild->rightchild;

        if (node->leftchild)
            node->leftchild->parent = node;

        leftChild->parent = node->parent;

        if (!node->parent)
            root = leftChild;
        else if (node == node->parent->leftchild)
            node->parent->leftchild = leftChild;
        else
            node->parent->rightchild = leftChild;

        leftChild->rightchild = node;
        node->parent = leftChild;
    }

    void fixViolation(Node*& root, Node*& node)
    {
        Node* parent = nullptr;
        Node* grandparent = nullptr;

        while ((node != root) && (node->color != 'B') && (node->parent->color == 'R'))
        {
            parent = node->parent;
            grandparent = parent->parent;

            if (parent == grandparent->leftchild)
            {
                Node* uncle = grandparent->rightchild;

                if (uncle && uncle->color == 'R')
                {
                    grandparent->color = 'R';
                    parent->color = 'B';
                    uncle->color = 'B';
                    node = grandparent;
                }
                else
                {
                    if (node == parent->rightchild)
                    {
                        rotateLeft(root, parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(root, grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else
            {
                Node* uncle = grandparent->leftchild;

                if (uncle && uncle->color == 'R')
                {
                    grandparent->color = 'R';
                    parent->color = 'B';
                    uncle->color = 'B';
                    node = grandparent;
                }
                else
                {
                    if (node == parent->leftchild)
                    {
                        rotateRight(root, parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(root, grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = 'B';
    }

public:
    RedBlackTree() : root(nullptr) {}
    char* extractColumnValue(char* row, int col)
    {
        int k = 0;
        int colcount = 0;
        while (row[k] != '\0')
        {
            if (colcount == col - 1)
            {
                break;
            }

            if (row[k] == ',')
            {
                colcount++;
            }
            k++;
        }
        if (colcount != col - 1)
        {
            return nullptr;
        }
        int start = k;

        while (row[k] != ',' && row[k] != '\0')
        {
            k++;
        }

        int length = k - start;
        char* columnValue = new char[length + 1];
        strncpy_s(columnValue, length + 1, &row[start], length);
        columnValue[length] = '\0';

        return columnValue;
    }
    int instructorHashInteger(int number) {
        int product = 1;

        // Multiply all digits
        while (number > 0) {
            int digit = number % 10;  // Get the last digit
            product *= digit;         // Multiply with the current product
            number /= 10;             // Remove the last digit
        }

        // Return result modulo 29
        return product % 29;
    }

    // Function to generate Instructor Hash for a String
    int instructorHashString(const string& str) {
        int product = 1;

        // Multiply the ASCII values of all characters
        for (char ch : str) {
            product *= static_cast<int>(ch);  // Get ASCII value and multiply
        }

        // Return result modulo 29
        return product % 29;
    }


    bool isdigi(char c)
    {
        return c >= '0' && c <= '9';
    }
    void insert(Node*& root, char* data, int column, int rownum)
    {
        char* newValue = extractColumnValue(data, column);
        if (!newValue) return;
        Node* node = new Node(data, rownum, newValue);
        Node* parent = nullptr;
        Node* current = root;
        int hashValue = 0;
        bool isDigit = isdigi(newValue[0]);

        if (isDigit) {
            int num = atoi(newValue);
            hashValue = instructorHashInteger(num); // Integer hash if it's a number
        }
        else {
            string strValue(newValue);
            hashValue = instructorHashString(strValue); // String hash if it's a string
        }
        //cout << "Generated Hash for column " << column << ": " << hashValue << endl;

        while (current)
        {
            parent = current;

            char* currentValue = extractColumnValue(current->data, column);
            if (!currentValue)
            {
                delete[] newValue;
                return;
            }

            bool newValueIsDigit = isdigi(newValue[0]);
            bool currentValueIsDigit = isdigi(currentValue[0]);

            bool comparison;
            if (newValueIsDigit && currentValueIsDigit)
            {
                int newValueInt = atoi(newValue);
                int currentValueInt = atoi(currentValue);
                comparison = newValueInt < currentValueInt;
            }
            else
            {
                comparison = strcmp(newValue, currentValue) < 0;
            }

            if (comparison) {
                current = current->leftchild;
            }
            else {
                current = current->rightchild;
            }

            delete[] currentValue;
        }

        node->parent = parent;

        if (!parent) {
            root = node;
        }
        else {
            char* parentValue = extractColumnValue(parent->data, column);
            if (!parentValue) {
                delete[] newValue;
                return;
            }

            bool newValueIsDigit = isdigi(newValue[0]);
            bool parentValueIsDigit = isdigi(parentValue[0]);

            if (newValueIsDigit && parentValueIsDigit) {
                int newValueInt = atoi(newValue);
                int parentValueInt = atoi(parentValue);
                if (newValueInt < parentValueInt) {
                    parent->leftchild = node;
                }
                else {
                    parent->rightchild = node;
                }
            }
            else {
                if (strcmp(newValue, parentValue) < 0) {
                    parent->leftchild = node;
                }
                else {
                    parent->rightchild = node;
                }
            }

            delete[] parentValue;
        }

        delete[] newValue;
        saveNodeToFile(node);
        fixViolation(root, node);
    }
    Node* minimum(Node* node)
    {
        while (node->leftchild)
            node = node->leftchild;
        return node;
    }

    void deleteFixUp(Node*& root, Node*& node)
    {
        while (node != root && (!node || node->color == 'B'))
        {
            if (node == node->parent->leftchild)
            {
                Node* sibling = node->parent->rightchild;
                if (sibling->color == 'R')
                {
                    sibling->color = 'B';
                    node->parent->color = 'R';
                    rotateLeft(root, node->parent);
                    sibling = node->parent->rightchild;
                }
                if ((!sibling->leftchild || sibling->leftchild->color == 'B') &&
                    (!sibling->rightchild || sibling->rightchild->color == 'B'))
                {
                    sibling->color = 'R';
                    node = node->parent;
                }
                else
                {
                    if (!sibling->rightchild || sibling->rightchild->color == 'B')
                    {
                        sibling->leftchild->color = 'B';
                        sibling->color = 'R';
                        rotateRight(root, sibling);
                        sibling = node->parent->rightchild;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = 'B';
                    if (sibling->rightchild)
                        sibling->rightchild->color = 'B';
                    rotateLeft(root, node->parent);
                    node = root;
                }
            }
            else
            {
                Node* sibling = node->parent->leftchild;
                if (sibling->color == 'R')
                {
                    sibling->color = 'B';
                    node->parent->color = 'R';
                    rotateRight(root, node->parent);
                    sibling = node->parent->leftchild;
                }
                if ((!sibling->rightchild || sibling->rightchild->color == 'B') &&
                    (!sibling->leftchild || sibling->leftchild->color == 'B'))
                {
                    sibling->color = 'R';
                    node = node->parent;
                }
                else
                {
                    if (!sibling->leftchild || sibling->leftchild->color == 'B')
                    {
                        sibling->rightchild->color = 'B';
                        sibling->color = 'R';
                        rotateLeft(root, sibling);
                        sibling = node->parent->leftchild;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = 'B';
                    if (sibling->leftchild)
                        sibling->leftchild->color = 'B';
                    rotateRight(root, node->parent);
                    node = root;
                }
            }
        }
        if (node)
            node->color = 'B';
    }
    string extractColumnValue(const char* data, int columnIndex)
    {
        string row(data);
        size_t pos = 0;
        int currentColumn = 0;
        while (currentColumn < columnIndex && pos != string::npos)
        {
            pos = row.find(',', pos + 1);
            currentColumn++;
        }

        if (pos != string::npos) {
            size_t endPos = row.find(',', pos + 1);
            if (endPos == string::npos) {
                endPos = row.size();
            }
            return row.substr(pos + 1, endPos - pos - 1);
        }
        return "";
    }



    void deleteNode(Node*& root, int columnIndex, const string& thingToDelete, char* fileName)
    {
        Node* node = root;
        Node* target = nullptr;
        Node* child = nullptr;
        char* nodeValue;

        cout << "Entering deleteNode function." << endl;
        if (!root)
        {
            cout << "Root is null. The tree is empty." << endl;
            return;
        }

        while (node)
        {
            nodeValue = extractColumnValue(node->data, columnIndex);
            if (!nodeValue)
            {
                cout << "Error: nodeValue is null for column index: " << columnIndex << endl;
                return;
            }
            else
            {
                cout << "Extracted node value: " << nodeValue << endl;
            }

            int comparison = strcmp(thingToDelete.c_str(), nodeValue);
            delete[] nodeValue;

            if (comparison == 0)
            {
                target = node;  // Found the node to delete
                cout << "Node found. Preparing to delete." << endl; // Debugging statement
                break;
            }
            else if (comparison < 0)
            {
                node = node->leftchild;
                cout << "Moving left in the tree." << endl; // Debugging statement
            }
            else
            {
                node = node->rightchild;
                cout << "Moving right in the tree." << endl; // Debugging statement
            }
        }

        // Step 2: If the node is not found
        if (!target)
        {
            cout << "Node not found." << endl;
            return;
        }

        Node* temp = target;
        char originalColor = temp->color;
        //cout << "Original color of the target node: " << originalColor << endl; // Debugging statement

        // Step 3: Case 1 - Node has no left child
        if (!target->leftchild)
        {
            child = target->rightchild;
            if (child)
                child->parent = target->parent;
            if (!target->parent)
                root = child;  // If the node is the root
            else if (target == target->parent->leftchild)
                target->parent->leftchild = child;
            else
                target->parent->rightchild = child;
            cout << "Node has no left child, adjusting pointers." << endl; // Debugging statement
        }
        // Step 4: Case 2 - Node has no right child
        else if (!target->rightchild)
        {
            child = target->leftchild;
            if (child)
                child->parent = target->parent;
            if (!target->parent)
                root = child;  // If the node is the root
            else if (target == target->parent->leftchild)
                target->parent->leftchild = child;
            else
                target->parent->rightchild = child;
            cout << "Node has no right child, adjusting pointers." << endl; // Debugging statement
        }
        // Step 5: Case 3 - Node has both left and right children
        else
        {
            Node* successor = minimum(target->rightchild);  // Find the successor
            originalColor = successor->color;
            child = successor->rightchild;

            if (successor->parent == target)
            {
                if (child)
                    child->parent = successor;
            }
            else
            {
                if (child)
                    child->parent = successor->parent;
                successor->parent->leftchild = child;
                successor->rightchild = target->rightchild;
                target->rightchild->parent = successor;
            }

            if (!target->parent)
                root = successor;  // If the target is the root
            else if (target == target->parent->leftchild)
                target->parent->leftchild = successor;
            else
                target->parent->rightchild = successor;

            successor->leftchild = target->leftchild;
            target->leftchild->parent = successor;
            successor->color = target->color;
            target = successor;  // Move the target to the successor
            cout << "Node has both children, using successor." << endl; // Debugging statement
        }

        delete temp;  // Delete the original target node
        cout << "Deleted target node." << endl; // Debugging statement

        // Step 6: Fix the Red-Black Tree properties after deletion
        if (originalColor == 'B')  // If the deleted node was black, fix up the tree
        {
           // cout << "Fixing Red-Black Tree after deletion." << endl; // Debugging statement
            deleteFixUp(root, child);
        }
        ifstream file(fileName);
        ofstream tempFile("C:\\Users\\USR\\OneDrive\\Desktop\\redblacktree\\redblacktree\\temp.csv");

        string line;
        bool deleted = false;
        while (getline(file, line)) {
            stringstream ss(line);
            string columnValue;
            int currentColumnIndex = 0;

            // Assuming the data is comma-separated, find the column we need to match
            while (getline(ss, columnValue, ',')) {
                if (currentColumnIndex == columnIndex) {
                    if (columnValue == thingToDelete) {
                        deleted = true;  // Skip this row
                        break;
                    }
                }
                currentColumnIndex++;
            }

            if (!deleted) {
                tempFile << line << "\n";  // Write the row to the temporary file if it's not deleted
            }

            deleted = false;  // Reset for the next row
        }

        file.close();
        tempFile.close();

        // Replace the original file with the temporary one
        remove(fileName);
        rename("C:\\Users\\USR\\OneDrive\\Desktop\\redblacktree\\redblacktree\\temp.csv", fileName);

        inOrder(root);
        cout << "Tree displayed after deletion." << endl;
    }

    void updateNode(Node*& root, int columnIndex, const string& valueToUpdate, int columnToUpdate, const string& newValue, char* fileName)
    {
        Node* node = root;
        Node* target = nullptr;
        char* nodeValue;

        // Step 1: Search for the node that matches the valueToUpdate
        while (node)
        {
            nodeValue = extractColumnValue(node->data, columnIndex);  // Get the value from the given column
            if (!nodeValue)
            {
                cout << "Error: nodeValue is null for column index: " << columnIndex << endl;
                return;
            }

            int comparison = strcmp(valueToUpdate.c_str(), nodeValue);
            delete[] nodeValue;

            if (comparison == 0)
            {
                target = node;  // Found the node to update
                break;
            }
            else if (comparison < 0)
            {
                node = node->leftchild;
            }
            else
            {
                node = node->rightchild;
            }
        }

        // Step 2: If the node is not found
        if (!target)
        {
            cout << "Node with value '" << valueToUpdate << "' not found." << endl;
            return;
        }

        // Step 3: Update the column value in the node's data
        // Calculate the new length for the updated data
        string updatedData = target->data;
        stringstream newDataStream;
        int currentColumn = 0;
        stringstream originalDataStream(updatedData);
        string cell;

        while (getline(originalDataStream, cell, ','))
        {
            if (currentColumn == columnToUpdate - 1)  // Matching the column to update (0-indexed)
            {
                // Replace the current column with the new value
                newDataStream << newValue;
            }
            else
            {
                newDataStream << cell;
            }

            // Add comma separator between columns
            if (originalDataStream.peek() != EOF)
                newDataStream << ",";

            currentColumn++;
        }

        // Convert the new data into a string
        updatedData = newDataStream.str();

        // Replace the old data with the updated data
        delete[] target->data;
        target->data = new char[updatedData.length() + 1];
        strcpy_s(target->data, updatedData.length() + 1, updatedData.c_str());

        // Step 4: Update the node's file
        saveNodeToFile(target);

        // Step 5: Update the CSV file with the new value
        string csvFilename = fileName;
        ifstream inputFile(csvFilename);
        ofstream outputFile("C:\\Users\\USR\\OneDrive\\Desktop\\redblacktree\\redblacktree\\temp1.csv");

        string line;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string cell;
            int currentColumn = 0;
            bool matchFound = false;

            // Iterate through the columns to find the match in the 'WHERE' clause
            while (getline(ss, cell, ','))
            {
                if (currentColumn == columnIndex && cell == valueToUpdate)
                {
                    matchFound = true;
                    break;
                }
                currentColumn++;
            }

            if (matchFound)
            {
                // Replace the line with the updated value
                string updatedLine;
                stringstream newLineStream;

                currentColumn = 0;
                stringstream originalLineStream(line);
                while (getline(originalLineStream, cell, ','))
                {
                    if (currentColumn == columnToUpdate - 1)
                    {
                        newLineStream << newValue;  // Update the specified column
                    }
                    else
                    {
                        newLineStream << cell;
                    }

                    // Add comma separator if not at the end
                    if (originalLineStream.peek() != EOF)
                        newLineStream << ",";
                    currentColumn++;
                }

                updatedLine = newLineStream.str();
                outputFile << updatedLine << endl;
            }
            else
            {
                outputFile << line << endl;
            }
        }

        // After the update, rewrite the CSV file with the changes
        ifstream file(fileName);
        ofstream tempFile("C:\\Users\\USR\\OneDrive\\Desktop\\redblacktree\\redblacktree\\temp1.csv");

        //string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string columnValue;
            int currentColumnIndex = 0;

            // Find the column to update
            string updatedLine = line;
            while (getline(ss, columnValue, ',')) {
                if (currentColumnIndex == columnIndex && columnValue == valueToUpdate) {
                    // Update the line with the new value
                    updatedLine.replace(updatedLine.find(valueToUpdate), valueToUpdate.length(), newValue);
                    break;
                }
                currentColumnIndex++;
            }

            tempFile << updatedLine << "\n";  // Write the (possibly updated) row to the temporary file
        }

        file.close();
        tempFile.close();

        // Replace the original file with the updated one
        remove(fileName);
        rename("C:\\Users\\USR\\OneDrive\\Desktop\\redblacktree\\redblacktree\\temp.csv", fileName);

    }



    void inOrder(Node* node)
    {
        if (!node)
            return;
        inOrder(node->leftchild);
        cout << node->data << " (" << node->filepath << ")" << endl;
        inOrder(node->rightchild);
    }

    void displayTree()
    {
        inOrder(root);
    }
};

//-------------------------------------------------------------------------------------------------------------------------------------------
class Repository
{

public:

    char* treeType;
    char* fileName;

    Repository()
    {
        treeType = new char;
        fileName = new char;

    }

    ~Repository() {
        delete[] treeType;
        delete[] fileName;
    }
    void getColumnNames(const char* filename, char*** columnNames, int& columnCount)
    {
        ifstream file(fileName);
        columnCount = 0;
        string line;
        if (getline(file, line))
        {
            stringstream ss(line);
            string column;
            *columnNames = new char* [1000];

            while (getline(ss, column, ',')) {
                (*columnNames)[columnCount] = new char[column.length() + 1];
                strncpy_s((*columnNames)[columnCount], column.length() + 1, column.c_str(), column.length() + 1);
                columnCount++;
            }
        }
        file.close();
        for (int i = 0; i < columnCount; i++) {
            cout << i + 1 << ". " << (*columnNames)[i] << endl;
        }

    }

    void cleanupColumnNames(char*** columnNames, int columnCount)
    {
        for (int i = 0; i < columnCount; ++i) {
            delete[](*columnNames)[i];
        }
        delete[] * columnNames;
    }

    int countLinesInCSV(const string& filePath) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Error: Unable to open file!" << endl;
            return -1;
        }

        string line;
        int lineCount = 0;

        while (getline(file, line)) {
            lineCount++;
        }

        file.close();
        return lineCount;
    }
    void furthercommands(RedBlackTree& rbt, Node* root)
    {
        char userChoice;
        while (true)
        {
            cout << "Do you want to make changes in this repository? (y/n): ";
            cin >> userChoice;
            cin.ignore();

            if (userChoice == 'y' || userChoice == 'Y')
            {
                string repocommand;
                cout << "->";
                getline(cin, repocommand);
                string commitData = repocommand.substr(6);  // Extract the string after "COMMIT"
                char* commitString = nullptr;
               // char* commitString = new char[commitData.length() + 1];  // +1 for the null-terminator

                if (repocommand.substr(0, 6) == "DELETE")
                {
                    cout << "DELETE command detected." << endl; // Debugging statement
                    string columnName;

                    // Extracting the part after "DELETE WHERE"
                    size_t posWhere = repocommand.find("WHERE");
                    if (posWhere != string::npos)
                    {
                        cout << "Found WHERE clause." << endl; // Debugging statement

                        // Extract the part after "WHERE"
                        string condition = repocommand.substr(posWhere + 6); // 6 is the length of "WHERE "

                        // Find the position of the '=' sign
                        size_t posEqual = condition.find("=");
                        if (posEqual != string::npos)
                        {
                            cout << "Found '=' sign. Extracting column index and value." << endl; // Debugging statement

                            // Extract the column index (before "=")
                            string columnIndexStr = condition.substr(0, posEqual);
                            columnIndexStr = columnIndexStr.substr(columnIndexStr.find_first_not_of(" \t")); // Remove leading whitespace
                            int columnIndex = stoi(columnIndexStr); // Convert column index to integer
                            cout << "Column index: " << columnIndex << endl;

                            // Extract the value (after "=")
                            string thingToDelete = condition.substr(posEqual + 1);
                            thingToDelete = thingToDelete.substr(thingToDelete.find_first_not_of(" \t")); // Remove leading whitespace
                            cout << "Value to delete: " << thingToDelete << endl;

                            // Remove the trailing single quote if it exists
                            if (thingToDelete.back() == '\'')
                            {
                                thingToDelete.pop_back();
                                cout << "Removed trailing quote." << endl;
                            }

                            // Remove the leading single quote if it exists
                            if (thingToDelete.front() == '\'')
                            {
                                thingToDelete.erase(thingToDelete.begin());
                                cout << "Removed leading quote." << endl; // Debugging statement
                            }
                            rbt.deleteNode(root, columnIndex, thingToDelete, fileName);////////////////////////////////////////////////
                        }
                    }
                }
                else if (repocommand.substr(0, 10) == "SHOWCOMMIT")
                {
                    cout << commitString;
                }
                else if ((repocommand.substr(0, 6) == "COMMIT"))
                {
                    cout << "Enter commit message: "; 
                    string commitData;                
                    cin.ignore();                     // Ignore the newline character from previous input
                    getline(cin, commitData);         // Read the line into the string

                    // Allocate memory for commitString and copy commitData
                    commitString = new char[commitData.length() + 1]; // Dynamically allocate memory
                    strcpy_s(commitString, commitData.length() + 1, commitData.c_str()); // Copy the string

                    cout << "Stored COMMIT string: " << commitString << endl; // Debugging output

                    // You may release the memory when it is no longer needed:
                    // delete[] commitString;
                }

               
                else if (repocommand.substr(0, 6) == "DISPLAY")
                {
                    rbt.inOrder(root);  // Call the function to display the CSV contents
                }
                else if (repocommand.substr(0, 6) == "UPDATE")
                {
                    cout << "UPDATE command detected." << endl; // Debugging statement
                    string columnName;

                    // Extracting the part after "UPDATE Students SET"
                    size_t posSet = repocommand.find("SET");
                    if (posSet != string::npos)
                    {
                        cout << "Found SET clause." << endl; // Debugging statement

                        // Extract the part after "SET"
                        string setClause = repocommand.substr(posSet + 4); // 4 is the length of "SET "

                        // Extract the column and new value from the SET clause
                        size_t posEquals = setClause.find("=");
                        if (posEquals != string::npos)
                        {
                            // Extract the column name (before '=')

                            size_t posSet = repocommand.find("SET");
                            size_t posWhere = repocommand.find("WHERE");

                            string setClause = repocommand.substr(posSet + 4, posWhere - posSet - 4); // Extract part between SET and WHERE

                            // Extract the column index to update (from SET clause before the '=')
                            size_t posEquals = setClause.find("=");
                            string columnToUpdateStr = setClause.substr(0, posEquals);  // Column index (as string initially)
                            columnToUpdateStr = columnToUpdateStr.substr(columnToUpdateStr.find_first_not_of(" \t")); // Remove leading whitespace
                            cout << "Column to update (as string): " << columnToUpdateStr << endl; // Debugging statement

                            // Convert columnToUpdate to int
                            int columnToUpdate = stoi(columnToUpdateStr);
                            cout << "Column to update (as int): " << columnToUpdate << endl; // Debugging statement

                            // Extract the new value (after '=')
                            string newValue = setClause.substr(posEquals + 1);
                            newValue = newValue.substr(newValue.find_first_not_of(" \t")); // Remove leading whitespace
                            cout << "New value: " << newValue << endl; // Debugging statement

                            // Remove the trailing single quote if it exists
                            if (newValue.back() == '\'')
                            {
                                newValue.pop_back();
                                cout << "Removed trailing quote." << endl;
                            }

                            // Remove the leading single quote if it exists
                            if (newValue.front() == '\'')
                            {
                                newValue.erase(newValue.begin());
                                cout << "Removed leading quote." << endl; // Debugging statement
                            }

                            // Extract the condition in WHERE clause
                            string condition = repocommand.substr(posWhere + 6); // 6 is the length of "WHERE "

                            // Find the position of the '=' sign in the WHERE clause
                            size_t posWhereEqual = condition.find("=");
                            if (posWhereEqual != string::npos)
                            {
                                cout << "Found '=' sign in WHERE clause. Extracting column index and value." << endl; // Debugging statement

                                // Extract the column index (before "=") in the WHERE clause
                                string columnIndexStr = condition.substr(0, posWhereEqual);
                                columnIndexStr = columnIndexStr.substr(columnIndexStr.find_first_not_of(" \t")); // Remove leading whitespace
                                int columnIndex = stoi(columnIndexStr); // Convert column index to integer
                                cout << "WHERE column index: " << columnIndex << endl;

                                // Extract the value (after "=")
                                string valueToUpdate = condition.substr(posWhereEqual + 1);
                                valueToUpdate = valueToUpdate.substr(valueToUpdate.find_first_not_of(" \t")); // Remove leading whitespace
                                cout << "WHERE value to match: " << valueToUpdate << endl;

                                // Remove the leading and trailing single quotes if they exist
                                if (valueToUpdate.front() == '\'')
                                    valueToUpdate.erase(valueToUpdate.begin());
                                if (valueToUpdate.back() == '\'')
                                    valueToUpdate.pop_back();

                                // Step 1: Update the tree
                                rbt.updateNode(root, columnIndex, valueToUpdate, columnToUpdate, newValue, fileName); // Method to be implemented for updating nodes in the tree
                            }
                        }
                    }
                }

                else
                {
                    cout << "\nCommand unidentified" << endl;
                }
            }
            else if (userChoice == 'n' || userChoice == 'N')
            {
                cout << "No changes made to the repository." << endl;
                break;
            }
            else
            {
                cout << "Invalid input. Please enter 'y' or 'n'." << endl; // Handle invalid input
            }
        }
    }


    void makefile()
    {
        // cout << "in make file";
        ifstream file(fileName);
        cout << fileName;
        //  cout << "ifstream done";
        if (!file.is_open()) {
            cout << "Error!" << endl;
            return;
        }
        cout << "File opened" << endl;

        int totalLines = countLinesInCSV(fileName);
        cout << "Total Lines: " << totalLines << endl;;
        cout << "Columns:" << endl;
        char** columnNames = nullptr;
        int columnCount = 0;
        getColumnNames(fileName, &columnNames, columnCount);
        cout << "Enter the column number to make the tree:" << endl;
        int column;
        cin >> column;
        Node* root = nullptr;
        string tempRow1;
        getline(file, tempRow1);
        RedBlackTree rbt;
        for (int i = 0; i < 34; i++)
        {
            getline(file, tempRow1);

            char* row1 = new char[tempRow1.size() + 1];
            strcpy_s(row1, tempRow1.size() + 1, tempRow1.c_str());
            //cout << row1 << endl;
            rbt.insert(root, row1, column, i);
            //cout << "Inserting " << i + 1 << endl << endl << endl;
            delete[] row1;
        }
        if (treeType[0] == 'r' || treeType[0] == 'R') {

            for (int i = 0; i < 34; i++)
            {
                getline(file, tempRow1);

                char* row1 = new char[tempRow1.size() + 1];
                strcpy_s(row1, tempRow1.size() + 1, tempRow1.c_str());
                cout << row1 << endl;
                rbt.insert(root, row1, column, i);
                cout << "Inserting " << i + 1 << endl << endl << endl;
                delete[] row1;
            }
            rbt.inOrder(root);
            furthercommands(rbt, root);
        }
        else if (treeType[0] == 'B' || treeType[0] == 'b')
        {
            int a;
            cout << "\n enter order of b tree: ";
            cin >> a;
            cout << "\ncreated b tree";
            rbt.inOrder(root);
            furthercommands(rbt, root);
        }
        else if (treeType[0] == 'A' || treeType[0] == 'a')
        {

            char* root1 = new char[100];
            root1[0] = '\0';
            char* tempRow1 = new char[1000];
            AVL avlTree(column);
            file.getline(tempRow1, 1000);
            for (int i = 0; i < 34; i++) {

                file.getline(tempRow1, 1000);

                cout << tempRow1 << endl;


                avlTree.insert(root1, getWord(column, tempRow1), i, tempRow1);

                avlTree.inorder(root1);
                cout << "Inserting " << i + 1 << endl << endl << endl;
            }
            furthercommands(rbt, root);
        }
        else
        {
            cout << "Invalid tree type !!!!" << endl;
        }
        file.close();
        cleanupColumnNames(&columnNames, columnCount);

        return;
    }
    void init(char* file)
    {
        string fullFilePath = "C:\\Users\\USR\\OneDrive\\Desktop\\redblacktree\\redblacktree\\" + string(file);
        this->fileName = new char[fullFilePath.length() + 1];
        strcpy_s(this->fileName, fullFilePath.length() + 1, fullFilePath.c_str());

        cout << "Enter tree type (AVL/B/Red-Black): ";
        cin >> this->treeType;
        this->makefile();
    }
};

//----------------------------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------//
void welcomeAndRequestCommand() 
{
    Repository repo;
    cout << "                                               Welcome to the repository management system!" << endl;
    string command;
   
    while (true)
    {
        cout << "\n>";
        getline(cin, command);

        if (command.substr(0, 5) == "innit")
        {
            string fileName = command.substr(5);
            char* fileCharArray = new char[fileName.length() + 1];
            strcpy_s(fileCharArray, fileName.length() + 1, fileName.c_str());
            cout << "File path: " << fileName << endl;
            repo.init(fileCharArray);
            delete[] fileCharArray; 
        }
        
        else
        {
            cout << "\ncommand unidentified" << endl;
        }
    }
}

int main() {
    welcomeAndRequestCommand();
    return 0;
}