#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Function that gets an int passed by value, changes the local copy to 10
 */
void pass_by_value(int value)
{
  value = 10;
}
/**
 * Function that gets an int passed by reference, dereferences the pointer and changes its value
 */
void pass_by_reference(int *value)
{
  *value = 10;
}
/**
 * A simple string_copy function
 * Does not protect against buffer-overflow
 */
char *string_copy(char *destination, char *source)
{
  char *destination_pointer;

  destination_pointer = destination;

  while(*source) {
    *destination = *source;
    *destination++;
    *source++;
  }
  *destination++ = 0;
  return destination_pointer;
}

/**
 * A simple C struct
 * A bullet with damage, and a boolean to indicate hits
 */
struct Bullet
{
  char didItHit; // 1 if it hit, 0 if not (1 byte BOOLEAN :D)
  int damage;
};

/**
 * This function is ultimatly pointless, because we pass a pointer by value
 * and change the address it points to.
 * This pointer like any other value is a local copy to the function
 * and is destroyed when the function returns/end
 */
void does_nothing(int *intPointer)
{
  int pointless;
  pointless = 0;

  intPointer = &pointless;
}
/**
 * Passes a pointer by reference 
 * This is very useful in a lot of situations
 * but here its just doing unwise things
 */
void not_wise(int **intPointer)
{
  int z;
  z = 20;

  *intPointer = &z;
}
/** 
 * This is a wise way of doing it, since we return the memory address it can be
 * assigned to a value in the calling function
 */
int *this_is_wise(int *intPointer)
{
  int goodIdea;

  goodIdea = 30;

  *intPointer = goodIdea;

  return intPointer;
}

/**
 * This function is safer than string_copy
 * always nullterminates even if it means splitting the string
 * The length must be the actual length of the destination buffer,
 * else the function may write to invalid memory
 */
char *safer_string_copy(char *destination, char *source, int length)
{
  // if length provided was zero, return a null-pointer
  if (length <= 0)
    return NULL;
  // points to the first element in the character array.
  char *destination_pointer;
  int i;
  // both now point to the same address in memory
  destination_pointer = destination;
  i = 0;
  //increase the pointervalue to get the next element in the character array
  while ((i < (length - 1)) && (*destination++ = *source++))
    i++;
  // null terminating the string
  *destination++ = 0;
  i++;
  // zeroing out the rest of the string if 'i' is still smaller than the length provided
  if (i < length) {
    for ( ; i < length; i++)
      *destination++ = 0;
  }
  // returns a pointer to the first element in the copy
  return destination_pointer;
}

int main(void)
{

  int *intPointer;

  int getsPointedAt = 1;

  intPointer = &getsPointedAt;

  // prints the value printf points to
  printf("Printing the value intPointer points to: %d\n", *intPointer);

  //prints the value of the actual pointer, the address of the value it points to
  // prints the value in hexadecimal

  printf("Printing the value intPointer: %p\n", intPointer);

  printf("Printing the address of getsPointedAt: %p\n", &getsPointedAt);


  // pass by reference, pass by value function calls
  int x = 0;
  pass_by_value(x);
  printf("%d\n", x);


  int y = 0;
  pass_by_reference(&y);
  printf("%d\n", y);



  if (1) {
    int x2 = 255;
    intPointer = &x;
  }



  printf("main function: after if statement: %d\n",  *intPointer);

  /*
   * Create a new int b, make intPonter point to that
   */
  int b = 150;
  intPointer = &b; // change the value of the int, intPointer points at

  // calls a not wise function, sends the address of the pointer to the function
  not_wise(&intPointer);

  printf("main function: called not_wise function(&intPointer): %d\n",  *intPointer);


  //calls the wise function
  printf("main function: called wise(intPointer) %d\n",  *(this_is_wise(intPointer)));


  // the compiler creates an array that will fit the string provided
  // the length will in this case be 22, 21 characters + the nullbyte '\0'


  // this is a char pointer, it can point to a variable of type char
  char *charPointer;
  char string[] = "this is a string in c";

  // this will fail horribly, uncomment to see the result
  //printf("safer stringcopy: %s\n",   safer_string_copy(charPointer, string, 22) );

  // we said to copy 22 bytes, but the charPointer only points to one char (1 byte)
  // when we then increment the charPointer it will point to unallocated memory
  // the operating system sees the pointer tries to access this invalid memory
  // and terminates the program

  // solution?
  // allocated more memory to our charPointer

  // charPointer now points to at least 22 bytes of memory
  charPointer = malloc(22);

  printf("safer stringcopy: %s\n",   safer_string_copy(charPointer, string, 22) );

  // when we have allocated memory and are done using it, we must free it
  free(charPointer);

  

  // but wait, there is more
  // this can also sometimes work
  char *stringPointer;
  stringPointer = malloc(1);
  printf("safer stringcopy with crazy malloc: %s\n",   safer_string_copy(stringPointer, string, 22) );
  free(stringPointer);
  /*
   * malloc returns blocks of memory, because this is more efficient use of memory
   * even if it means some memory ulitmatly gets waisted.
   * so malloc(1) worked because the block was big enough, but it is unsafe and undefined to
   * do this, and it will probably cause a nasty bug if the program is larger.
   */


  // some struct demonstrations, that didn't make it into the final lecture due to time
  struct Bullet bullets[30];

  struct Bullet *bulletPointer = bullets; // bulletPointer now points to the first element in bullets


  // creazy casting, this does something useful!
  ((struct Bullet *)((char *)bulletPointer + 30*sizeof(struct Bullet)))->damage = 100;



  return 0;
}
