
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
using namespace std;

bool checkHyphen(char source[])
{
	
	int i;
	if (strlen(source) == 0)
		return false;
	for (i = 0; source[i] != '\0'; i++)
	{
		if (source[i] == '-')
			return true;
	}
	return false;
}

bool checkPortion(int wordLimit, int& letterCount, int& spaceTotal, char source[])
{
	for (int i = strlen(source) - 1; i >= 0 && source[i] != '\0'; i--)
	{
		if (source[i] == '-' && letterCount + spaceTotal + i + 1 <= wordLimit)
			return true;
	}
	return false;
}

bool checkPunc(char source[])
{
	if (strlen(source) == 0)
		return false;
	if (source[strlen(source) - 1] == '.' || source[strlen(source) - 1] == '?')
		return true;
	else 
		return false;
}

bool checkParBreak(char source[], char source2[])
{
	if (strcmp(source, source2) == 0)
		return true;
	else return false;
}

void letterCountDash(char source[], int& startAfterDash, int& endAfterDash)
{
	int i;
	
	for (i = 0; source[i] != '\0'; i++)
	{
		if (source[i] == '-')
		{
			startAfterDash = i + 1;
			endAfterDash = strlen(source) - 1;
		}
	}
}
void letterCountSplit()
{

}
void newLines(istream& inf, ostream& outf, int& letterCount, int& spaceCount, int& puncCount, int& spaceTotal) 
{
	outf << '\n';
	letterCount = 0;
	spaceCount = 0;
	puncCount = 0;
	spaceTotal = 0;
}

void paragraphBreak(char source[], istream& inf, ostream& outf, int& letterCount, int& spaceCount, int& puncCount, int& spaceTotal)
{
	outf << "\n\n";
	
	letterCount = 0;
	spaceCount = 0;
	puncCount = 0;
	spaceTotal = 0;
}
void writeText(istream& inf, ostream& outf, char source[])
{
	int i;
	for (i = 0; source[i] != '\0'; i++)
	{
		outf << source[i];
		//cerr << source[i] << endl;
	}
}

void writePortion(istream& inf, ostream& outf, char source[], int wordLimit, int& letterCount, int& puncCount, int& spaceCount, int& spaceTotal)
{
	int i;
	int j;
	int k;
	for (i = strlen(source) - 1; i >= 0 && source[i]!='\0'; i--)
	{
		if (source[i] == '-' && letterCount+spaceTotal+i+1<=wordLimit)
		{
			//cerr << "Legit Split" << endl;
			for (j = 0; j <= i; j++)
			{
				outf << source[j];
				//cerr << "WritePortion Before -" << source[j];
			}
			newLines(inf, outf, letterCount, spaceCount, puncCount, spaceTotal);
			for (k = j; source[k] != '\0'; k++)
			{
				outf << source[k]; 
				//cerr << "WritePortion After -" << source[k];
			}
			break;
		}
	}
	
}

void writeTextTooBig(istream& inf, ostream& outf, char source[], int& letterCount, int wordLimit, int& spaceCount, int& puncCount, int& spaceTotal)
{
	int i;
	for (i = 0; source[i] != '\0'; i++)
	{
		if (letterCount >= wordLimit)
		{
			newLines(inf, outf, letterCount, spaceCount, puncCount, spaceTotal);
		}
		letterCount++;
		outf << source[i];
	}
}
int stuff(int lineLength, istream& inf, ostream& outf)
{
	//if (lineLength < 1 || lineLength > 300)
		//return 2;

	const int MAX_LINELENGTH = lineLength;
	const int wordsGood = 0;
	const int wordTooBig = 1;
	char c;
	char paragraphBreaks[4] = "#P#";
	char emptyOut[2] = "";
	char letter[3] = "";
	char wordHolder[141] = "";
	int spaceCount = 0;
	int puncCount = 0;
	int letterCount = 0;
	int spaceTotal = 0;
	int startAfterDash = 0;
	int endAfterDash = 0;
	bool puncEnd = false;
	bool wordExceed = false;
	bool PortionCall = false;
	bool begin = false; bool parTrigger = false; int breakCounter = 0;

	while (inf.get(c))
	{
		if (!isspace(c))
		{
			if (c == '.' || c == '?')
			{
				puncCount++;
			
			}
			letter[0] = c;
			strcat(wordHolder, letter);
			spaceCount = 0;
			
		}
		else
		{
			if (begin && parTrigger && breakCounter <=0 )
			{
				//cerr << "paragraphBreak CALLED; reset parTrigger" << endl;
				paragraphBreak(wordHolder, inf, outf, letterCount, spaceCount, puncCount, spaceTotal);
				breakCounter++;
				parTrigger = false;
			}
			if (checkParBreak(wordHolder, paragraphBreaks)) //process paragraph break
			{
				//cerr << "paragraph break detected" << endl;
				parTrigger = true;
				strcpy(wordHolder, emptyOut);
				continue;
			}
			if (letterCount + strlen(wordHolder) + spaceTotal + 1> MAX_LINELENGTH && letterCount > 0 && !checkHyphen(wordHolder))
			{
				//cerr << "newline from word not fitting in line" << endl;
				newLines(inf, outf, letterCount, spaceCount, puncCount, spaceTotal);
			}
			else
			{
				if (letterCount + strlen(wordHolder) + spaceTotal + 1> MAX_LINELENGTH && letterCount > 0 && checkHyphen(wordHolder))
				{
					//cerr << "hyphen detected AND too many words in a line" << endl;
					
					
					if (checkPortion(MAX_LINELENGTH, letterCount, spaceTotal, wordHolder))
					{
						//cerr << "writePortion called" << endl;
						if (letterCount > 0 && strlen(wordHolder) > 0)
						{
							outf << ' ';
							//cerr << "space with NO c left" << endl;
							spaceTotal++;
						}
						writePortion(inf, outf, wordHolder, MAX_LINELENGTH, letterCount, puncCount, spaceCount, spaceTotal);
						letterCountDash(wordHolder, startAfterDash, endAfterDash);
						letterCount += endAfterDash - startAfterDash + 1;
						spaceCount++;
						strcpy(wordHolder, emptyOut);
						continue;
					}
					
				}
				else
				{
					if (letterCount + strlen(wordHolder) + spaceTotal + 1 > MAX_LINELENGTH && letterCount > 0)
					{
						//cerr << "ELSE: newline from word not fitting in line" << endl;
						newLines(inf, outf, letterCount, spaceCount, puncCount, spaceTotal);
					}
				}
			}
			if (letterCount > 0 && spaceCount <=0 && !parTrigger) //space shouldn't start in a new line
			{
				outf << ' ';
				//cerr << "space with c left" << endl;
				spaceTotal++;
			}
			if (puncCount > 0 && letterCount > 0)
			{
				outf << ' ';
				//cerr << "space with c from punc" << endl;
				spaceTotal++;
				puncCount = 0;
			}
			if (strlen(wordHolder) > MAX_LINELENGTH)
			{
				if (checkHyphen(wordHolder))
				{
					//cerr << "checkHyphen" << endl;
					if (checkPortion(MAX_LINELENGTH, letterCount, spaceTotal, wordHolder))
					{
						//cerr << "writePortion called WHEN ONE WORD TOO BIG" << endl;
						writePortion(inf, outf, wordHolder, MAX_LINELENGTH, letterCount, puncCount, spaceCount, spaceTotal);
						letterCountDash(wordHolder, startAfterDash, endAfterDash);
						letterCount += endAfterDash - startAfterDash + 1;
						spaceCount++;
						strcpy(wordHolder, emptyOut);
						continue;
					}
				}
				else
				{
					wordExceed = true;
					//cerr << "wordExceed" << endl;
					writeTextTooBig(inf, outf, wordHolder, letterCount, MAX_LINELENGTH, spaceCount, puncCount, spaceTotal);
					spaceCount++;
					strcpy(wordHolder, emptyOut);
					continue;
				}
			}
			if (!checkParBreak(wordHolder, paragraphBreaks) && strlen(wordHolder)>0)
			{
				//cerr << "WordHolder not #P#" << endl;
				writeText(inf, outf, wordHolder);
				letterCount += strlen(wordHolder);
				spaceCount++;
				begin = true;
				breakCounter = 0;
			}
			strcpy(wordHolder, emptyOut);
		}
	}
	if (!inf.get(c))
	{
		if (checkParBreak(wordHolder, paragraphBreaks))
		{
			strcpy(wordHolder, emptyOut);
		}
		if (letterCount + strlen(wordHolder) + spaceTotal + 1> MAX_LINELENGTH && letterCount > 0 && !checkHyphen(wordHolder))
		{
			//cerr << "NO C LEFT: newline from word not fitting in line" << endl;
			newLines(inf, outf, letterCount, spaceCount, puncCount, spaceTotal);
		}
		else
		{
			if (checkHyphen(wordHolder))
			{
				//cerr << "NO C LEFT: hyphen detected" << endl;
				if (checkPortion(MAX_LINELENGTH, letterCount, spaceTotal, wordHolder))
				{
					//cerr << "writePortion Called " << endl;
					
					writePortion(inf, outf, wordHolder, MAX_LINELENGTH, letterCount, puncCount, spaceCount, spaceTotal);
					letterCountDash(wordHolder, startAfterDash, endAfterDash);
					letterCount += endAfterDash - startAfterDash + 1;
					strcpy(wordHolder, emptyOut);
				}
			}
			else
			{
				if (letterCount + strlen(wordHolder) + spaceTotal + 1 > MAX_LINELENGTH && letterCount > 0)
				{
					//cerr << "NO C LEFT: ELSE: newline from word not fitting in line" << endl;
					//cerr << "newLines called" << endl;
					newLines(inf, outf, letterCount, spaceCount, puncCount, spaceTotal);
				}
			}
		}
		if (letterCount > 0 && strlen(wordHolder) > 0)
		{
			outf << ' ';
			//cerr << "space with NO c left" << endl;
		}
		if (puncCount > 0 && !checkPunc(wordHolder))
		{
			outf << ' ';
			//cerr << "NO c: puncSpace" << endl;
		}
		if (strlen(wordHolder) > MAX_LINELENGTH)
		{
			if (checkHyphen(wordHolder))
			{
				//cerr << "NO C LEFT: hyphen detected AND WORDTOOLONG" << endl;
				writePortion(inf, outf, wordHolder, MAX_LINELENGTH, letterCount, puncCount, spaceCount, spaceTotal);
				
			}
			else
			{
				wordExceed = true;
				//cerr << "NO C: WordExceed" << endl;
				writeTextTooBig(inf, outf, wordHolder, letterCount, MAX_LINELENGTH, spaceCount, puncCount, spaceTotal);
				strcpy(wordHolder, emptyOut);
			}
		}
		writeText(inf, outf, wordHolder);
		letterCount += strlen(wordHolder);
	}
	if (letterCount > 0)
	{
		outf << '\n';
		//cerr << "final new line" << endl;
	}
	if (wordExceed)
	{
		return wordTooBig;
	}
	return wordsGood;
}

int main2() //use front slashes for infile and outfile
{
	ifstream infile("C:/Users/Charlie Tong/Desktop/Charlie's College Stuff/P5/test.txt");
	if (!infile)
	{
		cerr << "Error: Cannot open test.txt!" << endl;
	}
	ofstream outfile("C:/Users/Charlie Tong/Desktop/Charlie's College Stuff/P5/result.txt");
	if (!outfile)
	{
		cerr << "Error: Cannot create result.txt!" << endl;
	}

	cout << stuff(10, infile, outfile);
	return 0;
}


#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <cassert>
using namespace std;

class limit_output_streambuf : public streambuf
{
public:
limit_output_streambuf(streambuf* sb, size_t lim)
: real_streambuf(sb), limit(lim)
{
static char dummy;
setp(&dummy, &dummy);
}
private:
streambuf* real_streambuf;
size_t limit;

int overflow(int c)
{
if (limit == 0)
return traits_type::eof();
limit--;
return real_streambuf->sputc(c);
}
};

class streambuf_switcher
{
public:
streambuf_switcher(ios& dest, streambuf* srcbuf)
: dest_stream(dest), saved_streambuf(dest.rdbuf(srcbuf))
{}
~streambuf_switcher()
{
dest_stream.rdbuf(saved_streambuf);
}
private:
ios& dest_stream;
streambuf* saved_streambuf;
};

const string repeat(size_t n, string s)
{
string result;
result.reserve(n * s.size());
for ( ; n > 0; n--)
result += s;
return result;
}

void testone(int n)
{
istringstream iss;
streambuf_switcher ssi(cin, iss.rdbuf());

ostringstream oss;
limit_output_streambuf lob(oss.rdbuf(), 1000*1000+10);
streambuf_switcher sso(cout, &lob);

switch (n)
{
default: {
cout << "Bad argument" << endl;
} break; case  1: {
iss.str("hello there\n\nthis is a test\n");
assert(stuff(0, iss, oss) == 2);
} break; case  2: {
iss.str("hello there\n\nthis is a test\n");
stuff(0, iss, oss);
assert(oss.str().empty());
} break; case  3: {
iss.str("hello there\n\nthis is a test\n");
ostringstream oss2;
streambuf_switcher sso2(cout, oss2.rdbuf());
stuff(11, iss, oss);
assert(oss2.str().empty());
} break; case  4: {
iss.str("hello there\n\nthis is a test\n");
istringstream iss2("Z Y X W V\n");
streambuf_switcher ssi2(cin, iss2.rdbuf());
stuff(11, iss, oss);
string s = oss.str();
assert(s.find_first_of("ZYXWV") == string::npos  &&
s.find("hello") != string::npos);
} break; case  5: {
iss.str("");
assert(stuff(10, iss, oss) == 0  &&  oss.str().empty());
} break; case  6: {
iss.str("     \n\n    \n\n");
assert(stuff(10, iss, oss) == 0  &&  oss.str().empty());
} break; case  7: {
iss.str("abcdefghi abcdefghijk\n");
assert(stuff(10, iss, oss) == 1);
} break; case  8: {
iss.str("abcdefghi abcdefghijk abc abc abc abc abc abc abc\n");
assert(stuff(10, iss, oss) == 1);
} break; case  9: {
iss.str("abcdefghi abcdefgh-ijk\n");
assert(stuff(10, iss, oss) == 0);
} break; case 10: {
iss.str("abcdefghi abcdefghij\n");
assert(stuff(10, iss, oss) == 0);
} break; case 11: {
iss.str("hello there\n");
stuff(30, iss, oss);
assert(oss.str().find("hello there") != string::npos);
} break; case 12: {
iss.str("hello       there\n");
stuff(30, iss, oss);
assert(oss.str().find("hello there") != string::npos);
} break; case 13: {
iss.str("hello? there\n");
stuff(30, iss, oss);
assert(oss.str().find("hello?  there") != string::npos);
} break; case 14: {
iss.str("hello. abcdefghi\n");
stuff(10, iss, oss);
assert(oss.str().find("hello.\n") != string::npos);
} break; case 15: {
iss.str("hello.there\n");
stuff(30, iss, oss);
assert(oss.str().find("hello.there") != string::npos);
} break; case 16: {
iss.str("hello'3*%there\n");
stuff(30, iss, oss);
assert(oss.str().find("hello'3*%there") != string::npos);
} break; case 17: {
iss.str("hello-there\n");
stuff(30, iss, oss);
assert(oss.str().find("hello-there") != string::npos);
} break; case 18: {
iss.str("hello\nthere\n");
stuff(30, iss, oss);
assert(oss.str().find("hello there") != string::npos);
} break; case 19: {
iss.str("abcdefg hijkl\n");
stuff(10, iss, oss);
assert(oss.str().find("abcdefg\n") != string::npos);
} break; case 20: {
iss.str("abcdefg hijkl\n");
stuff(10, iss, oss);
assert(oss.str().find("\nhijkl") != string::npos);
} break; case 21: {
iss.str("hellohello abcdef ghi jklmnop\n");
stuff(10, iss, oss);
assert(oss.str().find("\nabcdef ghi\n") != string::npos);
} break; case 22: {
iss.str("hellohello abcdef ghi jklmnop qrst\n");
stuff(10, iss, oss);
assert(oss.str().find("\nqrst") != string::npos);
} break; case 23: {
iss.str("abcdefg x-ray\n");
stuff(10, iss, oss);
assert(oss.str().find("x-\nray") != string::npos);
} break; case 24: {
iss.str("abcdefg abc xyz-pq-rs\n");
stuff(10, iss, oss);
assert(oss.str().find("xyz-\npq-rs") != string::npos);
} break; case 25: {
iss.str("abcdefg abc x-yz-pqr\n");
stuff(10, iss, oss);
assert(oss.str().find("x-yz-\npqr") != string::npos);
} break; case 26: {
iss.str("abcdefg abc xyz----yz\n");
stuff(10, iss, oss);
assert(oss.str().find("xyz---\n-yz") != string::npos);
} break; case 27: {
iss.str("abcdefg -xyz\n");
stuff(10, iss, oss);
assert(oss.str().find(" -\nxyz") != string::npos);
} break; case 28: {
iss.str("abcdefg abc abcdefghijklmnopqrstuvw\n");
stuff(10, iss, oss);
assert(oss.str().find("\nabcdefghij\nklmnopqrst\nuvw") != string::npos);
} break; case 29: {
iss.str("abc\n");
stuff(10, iss, oss);
assert(oss.str() == "abc\n");
} break; case 30: {
iss.str("abc #P# def\n");
stuff(10, iss, oss);
assert(oss.str().find("abc\n\ndef") != string::npos);
} break; case 31: {
iss.str("abc #P#  #P# def\n");
stuff(10, iss, oss);
assert(oss.str().find("abc\n\ndef") != string::npos);
} break; case 32: {
iss.str("abc\n#P#\ndef\n");
stuff(10, iss, oss);
assert(oss.str().find("abc\n\ndef") != string::npos);
} break; case 33: {
iss.str("#P# abc #P# def\n");
stuff(10, iss, oss);
string result("abc\n\ndef\n");
assert(oss.str() == result  ||
result.compare(0, 8, oss.str()) == 0);
} break; case 34: {
iss.str("abc #P# def\n#P#\n");
stuff(10, iss, oss);
assert(oss.str() == "abc\n\ndef\n");
} break; case 35: {
string s23 = "abcdefghijklmnopqrstuvw";
string s138(repeat(6, s23));
iss.str(s138 + "\n");
assert(stuff(139, iss, oss) == 0);
string result(s138 + "\n");
assert(oss.str() == result  ||
result.compare(0, 138, oss.str()) == 0);
} break; case 36: {
string s23 = "abcdefghijklmnopqrstuvw";
string s138(repeat(6, s23));
iss.str(s138 + "\n" + s138 + "\n");
assert(stuff(279, iss, oss) == 0);
string result(s138 + " " + s138 + "\n");
assert(oss.str() == result  ||
result.compare(0, 277, oss.str()) == 0);
} break; case 37: {
string s10 = "a\na\na\na\na\na\na\na\na\na\n";
string s100(repeat(10, s10));
string s20000(repeat(200, s100)); // For some people, 200 was 20000
iss.str(s20000);
stuff(2, iss, oss);
assert(oss.str() == s20000  ||
s20000.compare(0, 19999, oss.str()) == 0);
} break; case 38: {
string s23 = "abcdefghijklmnopqrstuvw";
string s138(repeat(6, s23));
iss.str(repeat(7, s138 + "\n"));
int n = stuff(980, iss, oss);
string result(repeat(6, s138 + " ") + s138 + "\n");
assert(n == 2  ||  (n == 0  &&
(oss.str() == result  ||
result.compare(0, 972, oss.str()) == 0)));
} break; case 39: {
string s23 = "abcdefghijklmnopqrstuvw";
string s138(repeat(6, s23));
iss.str(repeat(700, s138 + "\n"));
assert(stuff(98000, iss, oss) == 0);
string result(repeat(699, s138 + " ") + s138 + "\n");
assert(oss.str() == result  ||
result.compare(0, 97299, oss.str()) == 0);
} break;
}
}

int main()
{
cout << "Enter test number: ";
int n;
cin >> n;
testone(n);
cout << "Passed" << endl;
}

