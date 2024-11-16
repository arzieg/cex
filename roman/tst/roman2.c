#include <stdio.h>

unsigned
decode_roman (const char *roman_string)
{
  static const short roman_numbers[]
      = { ['I'] = 1,   ['V'] = 5,   ['X'] = 10,  ['L'] = 50,
          ['C'] = 100, ['D'] = 500, ['M'] = 1000 };
  int number = 0;

  for (; *roman_string; roman_string++)
    {
      unsigned cur_value = roman_numbers[(unsigned char)(roman_string[0])];
      unsigned next_value = roman_numbers[(unsigned char)(roman_string[1])];

      // roman_numbers['\0'] will be 0, so this works
      number += (next_value > cur_value) ? -cur_value : +cur_value;
    }
  return number;
}

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      printf ("\Error, call prg string");
      return (1);
    }
  printf ("\nValue = %u", decode_roman (argv[1]));
  return 0;
}