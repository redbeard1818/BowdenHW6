/*
 * Austin Bowden
 * adb160230@utdallas.edu
 * CS3377.002
 */

#include <iostream>
#include "cdk.h"
#include <stdint.h>
#include <fstream>
#include <string>
#include <sstream>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;



class BinaryFileHeader
{
public:
  uint32_t magicNumber; /* Should be 0xFEEDFACE */
  uint32_t versionNumber;
  uint64_t numRecords;
};



/*
 * Records in the file have a fixed length buffer
 * that will hold a C-Style string. This is the
 * size of the fixed length buffer.
 */
  const int maxRecordStringLength = 25;
class BinaryFileRecord
{
public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};



int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix
  BinaryFileHeader *t1 = new BinaryFileHeader();
  
  ifstream input ("cs3377.bin", ios::in | ios::binary);
  if(!input.is_open())
    {
      cout << "Could not open file.\n";
      exit(-1);
    }
  input.read((char *) t1, sizeof(BinaryFileHeader));
  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"C0", "a", "b", "c", "C4", "C5"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);


  string s1 = "Magic: 0x";
  stringstream ss1;
  ss1 << hex << uppercase << t1->magicNumber;
  s1 += ss1.str();

  string s2 = "Version: ";
  stringstream ss2;
  ss2 << t1->versionNumber;
  s2 += ss2.str();

  string s3 = "NumRecords: ";
  stringstream ss3;
  ss3 << t1->numRecords;
  s3 += ss3.str();

  setCDKMatrixCell(myMatrix, 1, 1, s1.c_str());
  setCDKMatrixCell(myMatrix, 1, 2, s2.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, s3.c_str());

  drawCDKMatrix(myMatrix, true);

  int i = 2;

  BinaryFileRecord *t2 = new BinaryFileRecord();
  while(input.read((char *) t2, sizeof(BinaryFileRecord)) && i <= 5)
    {
      string str1, str2;
      stringstream ss1, ss2;
      ss2 << t2->stringBuffer;
      str2 += ss2.str();
      ss1 << "strlen: " << static_cast<uint16_t> (t2->strLength);
      str1 += ss1.str();

      setCDKMatrixCell(myMatrix, i, 1, str1.c_str());
      setCDKMatrixCell(myMatrix, i, 2, str2.c_str());
      i++;
    }

  /*
   * Dipslay a message
   */
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;
  input.close();
  // Cleanup screen
  endCDK();
}
