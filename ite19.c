#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function Prototypes
int romanToDecimal(const char *roman);
int isValidRoman(const char *roman);
char *numberToWords(int number);
void processLine(const char *line, FILE *outputFile);
void performArithmetic(const char *inputFile, const char *outputFile);
char *trimWhitespace(char *str);

// Main Function
int main() {
    const char *inputFile = "input.txt";
    const char *outputFile = "output.txt";

    // Perform arithmetic operations
    performArithmetic(inputFile, outputFile);

    printf("Processing completed. Check '%s' for results.\n", outputFile);
    return 0;
}

// Convert Roman numeral to decimal
int romanToDecimal(const char *roman) {
    int values[256] = {0};
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0, prev = 0;

    for (int i = strlen(roman) - 1; i >= 0; i--) {
        int curr = values[roman[i]];
        if (curr < prev) {
            total -= curr;
        } else {
            total += curr;
        }
        prev = curr;
    }
    return total;
}

// Check if a string is a valid Roman numeral
int isValidRoman(const char *roman) {
    while (*roman) {
        if (!strchr("IVXLCDM", *roman)) {
            return 0;
        }
        roman++;
    }
    return 1;
}

// Convert a number to its word representation
char *numberToWords(int number) {
    static char buffer[1024];
    buffer[0] = '\0';

    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million"};

    if (number == 0) {
        strcpy(buffer, "Zero");
        return buffer;
    }

    if (number < 0) {
        strcat(buffer, "Negative ");
        number = -number;
    }

    int parts[4] = {0}; // Holds the thousand groups (units, thousands, millions, etc.)
    int i = 0;

    // Break the number into groups of three digits
    while (number > 0) {
        parts[i++] = number % 1000;
        number /= 1000;
    }

    for (int j = i - 1; j >= 0; j--) {
        if (parts[j] == 0) {
            continue;
        }

        char temp[256] = "";

        int hundreds = parts[j] / 100;
        int remainder = parts[j] % 100;

        if (hundreds > 0) {
            sprintf(temp + strlen(temp), "%s Hundred ", ones[hundreds]);
        }

        if (remainder >= 10 && remainder < 20) {
            sprintf(temp + strlen(temp), "%s ", teens[remainder - 10]);
        } else {
            int tensPlace = remainder / 10;
            int onesPlace = remainder % 10;

            if (tensPlace > 0) {
                sprintf(temp + strlen(temp), "%s ", tens[tensPlace]);
            }
            if (onesPlace > 0) {
                sprintf(temp + strlen(temp), "%s ", ones[onesPlace]);
            }
        }

        // Add thousands place (if applicable)
        if (j > 0) {
            sprintf(temp + strlen(temp), "%s ", thousands[j]);
        }

        strcat(buffer, temp);
    }

    return trimWhitespace(buffer);
}

// Process a line of input and write the result to the output file
void processLine(const char *line, FILE *outputFile) {
    char roman1[50], roman2[50], operation;
    int num1, num2, result;

    // Parse the input
    if (sscanf(line, "%s %c %s", roman1, &operation, roman2) != 3) {
        fprintf(outputFile, "Invalid input\n");
        return;
    }

    // Validate Roman numerals
    if (!isValidRoman(roman1) || !isValidRoman(roman2)) {
        fprintf(outputFile, "Invalid Roman numeral\n");
        return;
    }

    // Convert Roman numerals to decimal
    num1 = romanToDecimal(roman1);
    num2 = romanToDecimal(roman2);

    // Perform the arithmetic operation
    switch (operation) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if (num2 == 0) {
                fprintf(outputFile, "Division by zero error\n");
                return;
            }
            result = num1 / num2;
            break;
        default:
            fprintf(outputFile, "Invalid operation\n");
            return;
    }

    // Convert the result to words and write to output
    fprintf(outputFile, "%s\n", numberToWords(result));
}

// Read input file and process each line
void performArithmetic(const char *inputFile, const char *outputFile) {
    FILE *inFile = fopen(inputFile, "r");
    FILE *outFile = fopen(outputFile, "w");

    if (!inFile || !outFile) {
        perror("Error opening file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), inFile)) {
        trimWhitespace(line);
        if (strlen(line) > 0) {
            processLine(line, outFile);
        }
    }

    fclose(inFile);
    fclose(outFile);
}

// Trim leading and trailing whitespace from a string
char *trimWhitespace(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing spaces
    if (*str == '\0') return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
    return str;
}
