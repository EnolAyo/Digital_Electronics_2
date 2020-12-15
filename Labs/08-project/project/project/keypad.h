
#ifndef KEYPAD_H_
#define KEYPAD_H_

/* Function prototypes ----------------------------------------------*/
uint8_t read_digits(void);
uint8_t code_analyzer(char code[4],char correct_code1[4],char correct_code2[4],char correct_code3[4]);
#endif /* KEYPAD_H_ */