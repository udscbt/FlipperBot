#ifdef DEBUG
#define DEBUG_PRINT(str) \
  Serial.print(str);

#define DEBUG_PRINTLN(str) \
  Serial.println(str);

#define DEBUG_NEWLINE \
  Serial.println();
#else
#define DEBUG_PRINT(str)
#define DEBUG_PRINTLN(str)
#define DEBUG_NEWLINE
#endif

unsigned long interDigitDelay = 1000ULL;
unsigned long interCharacterDelay = 500;
unsigned long interBlinkDelay = 0;
const unsigned int max_length = 20;

String text[max_length]; 
int text_length = 0;
int index = 0;

unsigned long int tScroll;
unsigned long int tBlink;
bool shown = true;

int pins(char c)
{
  if (c == '0') return 2;
  if (c == '1') return 3;
  if (c == '2') return 4;
  if (c == '3') return 5;
  if (c == 'a') return 6;
  if (c == 'b') return 7;
  if (c == 'c') return 8;
  if (c == 'd') return 9;
  if (c == 'e') return 10;
  if (c == 'f') return 11;
  if (c == 'g') return 12;
  if (c == '.') return 13;
  return 0;
}

String valuemap(String s)
{
  bool combined = false;
  String sub = "";
  String ret = "";
  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == '+')
    {
      combined = true;
      ret += valuemap(sub);
      sub = "";
    }
    else
    {
      sub += s[i];
    }
  }

  if (combined)
  {
    ret += valuemap(sub);
    return ret;
  }
  
  if (s == "0")   return "abcdef";
  if (s == "0.")  return "abcdef."; //dotted variant
  if (s == "1")   return "bc";
  if (s == "1.")  return "bc."; //dotted variant
  if (s == "2")   return "abedg";
  if (s == "2.")  return "abedg."; //dotted variant
  if (s == "3")   return "abcdg";
  if (s == "3.")  return "abcdg."; //dotted variant
  if (s == "4")   return "bcfg";
  if (s == "4.")  return "bcfg."; //dotted variant
  if (s == "5")   return "acdfg";
  if (s == "5.")  return "acdfg."; //dotted variant
  if (s == "6")   return "acdefg";
  if (s == "6.")  return "acdefg."; //dotted variant
  if (s == "7")   return "abc";
  if (s == "7.")  return "abc."; //dotted variant
  if (s == "8")   return "abcdefg";
  if (s == "8.")  return "abcdefg."; //dotted variant
  if (s == "9")   return "abcdfg";
  if (s == "9.")  return "abcdfg."; //dotted variant
  if (s == "A")   return "abcefg";
  if (s == "A.")  return "abcefg."; //dotted variant
  if (s == "a")   return "abcdeg";
  if (s == "a.")  return "abcdeg."; //dotted variant
  if (s == "B")   return "abcdefg";
  if (s == "B.")  return "abcdefg."; //dotted variant
  if (s == "b")   return "cdefg";
  if (s == "b.")  return "cdefg."; //dotted variant
  if (s == "C")   return "adef";
  if (s == "C.")  return "adef."; //dotted variant
  if (s == "c")   return "deg";
  if (s == "c.")  return "deg."; //dotted variant
  if (s == "D")   return "abcdef"; //hardly recognizable
  if (s == "D.")  return "abcdef."; //dotted variant
  if (s == "d")   return "bcdeg";
  if (s == "d.")  return "bcdeg."; //dotted variant
  if (s == "E")   return "adefg";
  if (s == "E.")  return "adefg."; //dotted variant
  if (s == "e")   return "abgdef";
  if (s == "e.")  return "abgdef."; //dotted variant
  if (s == "F")   return "aefg";
  if (s == "F.")  return "aefg."; //dotted variant
  if (s == "f")   return "aefg"; //actually uppercase
  if (s == "f.")  return "aefg."; //dotted variant
  if (s == "G")   return "abcdfg"; //actually lowercase
  if (s == "G.")  return "abcdfg."; //dotted variant
  if (s == "g")   return "abcdfg";
  if (s == "g.")  return "abcdfg."; //dotted variant
  if (s == "H")   return "bcefg";
  if (s == "H.")  return "bcefg."; //dotted variant
  if (s == "h")   return "cefg";
  if (s == "h.")  return "cefg."; //dotted variant
  if (s == "I")   return "ef";
  if (s == "I.")  return "ef."; //dotted variant
  if (s == "I>")  return "bc"; //right variant
  if (s == "I>.") return "bc."; //dotted variant
  if (s == "I<")  return "ef"; //left variant
  if (s == "I<.") return "ef."; //dotted variant
  if (s == "II")  return "bcef";
  if (s == "II.") return "bcef."; //dotted variant
  if (s == "Il")  return "bcef";
  if (s == "Il.") return "bcef."; //dotted variant
  if (s == "i")   return "c";
  if (s == "i.")  return "c."; //dotted variant
  if (s == "i>")  return "c"; //right variant
  if (s == "i>.") return "c."; //dotted variant
  if (s == "i<")  return "e"; //left variant
  if (s == "i<.") return "e."; //dotted variant
  if (s == "ii")  return "ce";
  if (s == "ii.") return "ce."; //dotted variant
  if (s == "il")  return "bce";
  if (s == "il.") return "bce."; //dotted variant
  if (s == "J")   return "bcd";
  if (s == "J.")  return "bcd."; //dotted variant
  if (s == "j")   return "cd";
  if (s == "j.")  return "cd."; //dotted variant
  if (s == "K")   return "bcefg"; //hardly recognizable
  if (s == "K.")  return "bcefg."; //dotted variant
  if (s == "k")   return "bcefg"; //actually uppercase
  if (s == "k.")  return "bcefg."; //dotted variant
  if (s == "L")   return "def";
  if (s == "L.")  return "def."; //dotted variant
  if (s == "l")   return "bc";
  if (s == "l.")  return "bc."; //dotted variant
  if (s == "l>")  return "bc"; //right variant
  if (s == "l>.") return "bc."; //dotted variant
  if (s == "l<")  return "ef"; //left variant
  if (s == "l<.") return "ef."; //dotted variant
  if (s == "lI")  return "bcef";
  if (s == "lI.") return "bcef."; //dotted variant
  if (s == "li")  return "cef";
  if (s == "li.") return "cef."; //dotted variant
  if (s == "ll")  return "bcef";
  if (s == "ll.") return "bcef."; //dotted variant
  if (s == "N")   return "ceg"; //actually lowercase
  if (s == "N.")  return "ceg."; //dotted variant
  if (s == "n")   return "ceg";
  if (s == "n.")  return "ceg."; //dotted variant
  if (s == "O")   return "abcdef";
  if (s == "O.")  return "abcdef."; //dotted variant
  if (s == "o")   return "cdeg";
  if (s == "o.")  return "cdeg."; //dotted variant
  if (s == "P")   return "abefg";
  if (s == "P.")  return "abefg."; //dotted variant
  if (s == "p")   return "abefg"; //actually uppercase
  if (s == "p.")  return "abefg."; //dotted variant
  if (s == "R")   return "abcefg"; //hardly recognizable
  if (s == "R.")  return "abcefg."; //dotted variant
  if (s == "r")   return "eg";
  if (s == "r.")  return "eg."; //dotted variant
  if (s == "S")   return "afgcd";
  if (s == "S.")  return "afgcd."; //dotted variant
  if (s == "s")   return "afgcd"; //actually uppercase
  if (s == "s.")  return "afgcd."; //dotted variant
  if (s == "T")   return "defg"; //actually lowercase
  if (s == "T.")  return "defg."; //dotted variant
  if (s == "t")   return "defg";
  if (s == "t.")  return "defg."; //dotted variant
  if (s == "U")   return "bcdef";
  if (s == "U.")  return "bcdef."; //dotted variant
  if (s == "u")   return "cde";
  if (s == "u.")  return "cde."; //dotted variant
  if (s == "Y")   return "befg"; //hardly recognizable
  if (s == "Y.")  return "befg."; //dotted variant
  if (s == "y")   return "bcdfg";
  if (s == "y.")  return "bcdfg."; //dotted variant
  if (s == "-")   return "g";
  if (s == "-.")  return "g."; //dotted variant
  if (s == "_")   return "d";
  if (s == "_.")  return "d."; //dotted variant
  if (s == "^")   return "a";
  if (s == "^.")  return "a."; //dotted variant
  if (s == " ")   return "";
  if (s == ".")   return ".";
  if (s == "|")   return "ef";
  if (s == "|.")  return "ef."; //dotted variant
  if (s == "|>")  return "bc"; //right variant
  if (s == "|>.") return "bc."; //dotted variant
  if (s == "|<")  return "ef"; //left variant
  if (s == "|<.") return "ef."; //dotted variant
  if (s == "?")   return "abeg"; //needs . in previous symbol
  if (s == "?.")  return "abeg."; //dotted variant
  if (s == "!")   return "b.";
  if (s == "[")   return "adef";
  if (s == "[.")  return "adef-"; //dotted variant
  if (s == "]")   return "abcd";
  if (s == "].")  return "abcd."; //dotted variant
  if (s == "°")   return "abfg";
  if (s == "°.")  return "abfg."; //dotted variant
  if (s == "'")   return "f";
  if (s == "'.")  return "f."; //dotted variant
  if (s == "'>")  return "b"; //right variant
  if (s == "'>.") return "b."; //dotted variant
  if (s == "'<")  return "f"; //left variant
  if (s == "'<.") return "f."; //dotted variant
  return "";
}

void clearDigits()
{
  for (char c = '0'; c <= '3'; c++)
  {
    digitalWrite(pins(c), HIGH);
  } 
}

void clearSegments()
{
  for (char c = 'a'; c <= 'g'; c++)
  {
    digitalWrite(pins(c), LOW);
  }
  digitalWrite(pins('.'), LOW);
}

void showDigit(char digit, String symbol)
{
  String segments = valuemap(symbol);
  clearDigits();
  clearSegments();
  for (int i = 0; i < segments.length(); i++)
  {
    digitalWrite(pins(segments[i]), HIGH);
  }
  digitalWrite(pins(digit), LOW);
}

void showString(String syms[], int len, int start, bool wrap)
{
  int end = start + 4;
  if (!wrap)
  {
    if (len < end) end = len;
  }
  
  for (int i = start; i < end; i++)
  {
    char digit = '3'-i+start;
    showDigit(digit, syms[i%len]);
    if (interDigitDelay > 10000)
    {
      delay(interDigitDelay/1000);
    }
    else
    {
      delayMicroseconds(interDigitDelay);
    }
  }
}

void setText(String t[], int len)
{
  DEBUG_PRINT("....")
  for (int i = 0; i < len; ++i)
  {
    DEBUG_PRINT(t[i])
  }
  DEBUG_PRINT(";;;;;")
  DEBUG_NEWLINE
  text_length = len;
  for (int i = 0; i < text_length; ++i)
  {
    text[i] = t[i];
  }
  index = 0;
}

void setRefreshRate(unsigned long freq)
{
  interDigitDelay = freq?250000/freq:0;
}

void setScrollSpeed(unsigned long speed)
{
  interCharacterDelay = speed?1000/speed:0;
}

void setBlinkFreq(unsigned long freq)
{
  interBlinkDelay = freq?1000/freq:0;
}

void setup()
{
  for (int i = 2; i <= 13; ++i)
    pinMode(i, OUTPUT);
  
  Serial.begin(9600);
  
  clearDigits();
  
  String msg[] = {};
  setText(msg, sizeof(msg)/sizeof(String));
  t = millis();
}


String msg[max_length];
String symbol = "";
int n = 0;
int base;

bool option = false;
int optIndex;
const int optNum = 3;
long optVal[optNum];
const long NOT_SET = -1;

void loop()
{
  int nc = Serial.available();
  if (nc > 0) {
    DEBUG_PRINTLN("Received:")
    for (int i = 0; i < nc; ++i)
    {
      char c = Serial.read();
      DEBUG_PRINT(c)

      // Options
      if (c == '(')
      {
        option = true;
        optIndex = 0;
        for (int j = 0; j < optNum; j++)
        {
          optVal[j] = NOT_SET;
        }
        base = 10;
      }
      else if (option)
      {
        if (c == ';')
        {
          ++optIndex;
          base = 10;
        }
        else if (c == ')')
        {
          option = false;
          if (optVal[0] != NOT_SET) setRefreshRate(optVal[0]);
          if (optVal[1] != NOT_SET) setScrollSpeed(optVal[1]);
          if (optVal[2] != NOT_SET) setScrollSpeed(optVal[2]);
        }
        else if (optVal[optIndex]==NOT_SET && c == 'b')
        {
          base = 2;
          optVal[optIndex] = 0;
        }
        else if (optVal[optIndex]==NOT_SET && c == 'o')
        {
          base = 8;
          optVal[optIndex] = 0;
        }
        else if (optVal[optIndex]==NOT_SET && c == 'd')
        {
          base = 10;
          optVal[optIndex] = 0;
        }
        else if (optVal[optIndex]==NOT_SET && c == 'x')
        {
          base = 16;
          optVal[optIndex] = 0;
        }
        else if (optIndex < optNum)
        {
          int val;
          if (c >= '0' && c <= '9') val = c-'0';
          else if (c >= 'A' && c <= 'F') val = c-'A'+11;
          else if (c >= 'a' && c <= 'f') val = c-'A'+11;
          else continue;
          if (optVal[optIndex] == NOT_SET) optVal[optIndex] = 0;
          optVal[optIndex] *= base;
          optVal[optIndex] += val;
        }
      }
      // Message
      else
      {      
        if (c == ':' || c == ';')
        {
          msg[n] = symbol;
          n++;
          symbol = "";

          if (c == ';')
          {
            setText(msg, n);
            n = 0;
            symbol = "";
            break;
          }
        }
        else
        {
          symbol += c;
        }
      }
    }
    DEBUG_NEWLINE
  }

  if (shown && interDigitDelay)
  {
    showString(text, text_length, index, text_length > 4);
  }
  else
  {
    clearDigits();
  }
  // Scroll
  if (interCharacterDelay && text_length > 4 && millis() - tScroll > interCharacterDelay)
  {
    ++index;
    if (index >= text_length) index = 0;
    tScroll = millis();
  }
  // Blink
  if (interBlinkDelay && millis() - tBlink > interBlinkDelay)
  {
    shown = !shown;
  }
}
