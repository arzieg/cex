#include <stdio.h>
#include <string.h>

unsigned
decode_roman (const char *roman_number)
{
  int value = 0;
  int predecesor = 0;
  int number = 0;
  int j = (int)strlen (roman_number);
  for (int i = j; i >= 0; i--)
    {
      switch (roman_number[i])
        {
        case 'M':
          number = 1000;
          break;
        case 'D':
          number = 500;
          break;
        case 'C':
          number = 100;
          break;
        case 'L':
          number = 50;
          break;
        case 'X':
          number = 10;
          break;
        case 'V':
          number = 5;
          break;
        case 'I':
          number = 1;
          break;
        }
      if (predecesor > number)
        {
          value -= number;
        }
      else
        {
          value += number;
        }

      predecesor = number;
    }
  return value;
}

//    MCDXCII 1492
//    MCDLIII 1453

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