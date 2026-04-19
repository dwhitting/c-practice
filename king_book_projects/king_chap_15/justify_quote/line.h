#ifndef LINE_H
#define LINE_H

/*clear_line: clears the current line*/
void clear_line(void);

/*add_word adds word to current line*/
void add_word(const char *word);

/*space_remaining returns number of chars left in current line*/
int space_remaining(void);

/*write_line writes current line with justification*/
void write_line(void);

/*flush_line writes current line without justification*/
void flush_line(void);

#endif