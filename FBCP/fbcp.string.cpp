#include "fbcp.string.h"

namespace fbcp
{
  /* CLASS STRING */
  string string::void_str("");

  string::string (size_t len) : str_size(len), buf_size(len+1)
  {
    buf = new char[buf_size];
    buf[str_size] = '\0';
  }
  
  string::string () : string((size_t) 0) {}
  
  string::string (const string& str) : string(str.buf) {}

  string::string (const char* c_str, size_t size) : string(size)
  {
    if (size == npos)
    {
      const char *p1;
      char *p2;
      for (str_size = 0, p1 = c_str; *p1; ++str_size, ++p1);
      delete[] buf;
      buf_size = str_size+1;
      buf = new char[buf_size];
      for (p1 = c_str, p2 = buf; *p2 = *p1; ++p1, ++p2);
    }
    else
    {
      int i;
      for (i = 0; i < size && c_str[i]; i++)
      {
        buf[i] = c_str[i];
      }
      buf[i] = '\0';
      str_size = i;
    }
  }
  
  string::string (const char& c) : string((size_t) 1)
  {
    buf[0] = c;
  }

  string::~string ()
  {
    delete[] buf;
  }

  string& string::operator= (const string& str)
  {
    if (this != &str)
    {
      *this = str.buf;
    }
    
    return *this;
  }

  string& string::operator= (const char* c_str)
  {
    for (str_size = 0; c_str[str_size]; ++str_size);
    
    if (str_size > buf_size+1)
    {
      delete[] buf;
      buf_size = str_size+1;
      buf = new char[buf_size];
    }
    
    for (int i = 0; c_str[i]; ++i)
    {
      buf[i] = c_str[i];
    }
    buf[str_size] = '\0';
    
    return *this;
  }

  string& string::operator+= (const string& str)
  {
    return *this += str.buf;
  }
  
  string& string::operator+= (const char* c_str)
  {
    size_t c_str_size;
    bool resize = false;
    for (c_str_size = 0; c_str[c_str_size]; ++c_str_size);
    
    char* new_buf = buf;
    if (str_size + c_str_size > buf_size-1)
    {
      resize = true;
      buf_size = str_size+c_str_size+1;
      new_buf = new char[buf_size];
      for (int i=0; i < str_size; i++)
      {
        new_buf[i] = buf[i];
      }
    }
    
    for (int i = 0; i < c_str_size; i++)
    {
      new_buf[str_size+i] = c_str[i];
    }
    str_size += c_str_size;
    new_buf[str_size] = '\0';
    
    if (resize)
    {
      delete[] buf;
      buf = new_buf;
    }
    
    return *this;
  }

  char& string::operator[] (size_t pos)
  {
    if (pos > str_size) pos = str_size;
    return buf[pos];
  }

  const char& string::operator[] (size_t pos) const
  {
    if (pos > str_size) pos = str_size;
    return buf[pos];
  }

  string::iterator string::begin ()
  {
    iterator it(*this, 0);
    return it;
  }

  string::const_iterator string::begin () const
  {
    const_iterator it(*this, 0);
    return it;
  }

  string::iterator string::end ()
  {
    iterator it(*this, length());
    return it;
  }

  string::const_iterator string::end () const
  {
    const_iterator it(*this, length());
    return it;
  }
  
  int string::compare (const string& str) const
  {
    return compare(str.buf);
  }

  int string::compare (const char* c_str) const
  {
    const char *p1, *p2;
    p1 = this->buf;
    p2 = c_str;
    while (*p1 && *p2)
    {
      if (*p1 < *p2) return -2;
      if (*p1 > *p2) return 2;
      ++p1;
      ++p2;
    }
    if (*p2) return -1;
    if (*p1) return 1;
    return 0;
  }
  
  const char* string::c_str () const
  {
    buf[str_size] = '\0';
    return buf;
  }
  
  string::operator const char* () const
  {
    return c_str();
  }

  bool string::empty () const
  {
    return str_size == 0;
  }

  size_t string::length () const
  {
    return str_size;
  }

  size_t string::find (const string& str, size_t pos) const
  {
    while (true)
    {
      for (; pos < length() && buf[pos] != str[0]; ++pos);
      if (pos >= length()) break;
      int i, j;
      for (i = pos, j = 0; i < length() && j < str.length() && buf[i] == str[j]; ++i, ++j);
      if (j == str.length()) return pos;
      else ++pos;
    }
    return npos;
  }
  
  size_t string::find (const char* c_str, size_t pos) const
  {
    string str(c_str);
    return find(str, pos);
  }

  string string::substr (size_t pos, size_t len) const
  {
    if (pos >= str_size) return "";
    if (len > str_size) len = str_size - pos;
    if (pos + len > str_size) len = str_size - pos;
    string str(len);
    for (int i = 0; i < len; i++)
    {
      str.buf[i] = buf[pos+i];
    }
    
    return str;
  }
  
  bool string::startsWith (const string& other, const string& separators, bool useSeparator) const
  {
    if (other.length() > length()) return false;
  
    int i;
    for (i = 0; i < length() && i < other.length() && buf[i] == other[i]; ++i);

    if (i != other.length()) return false;
    else if (useSeparator)
    {
      int j;
      for (j = 0; j < separators.length(); j++)
      {
        if (buf[i] == separators[j]) return true;
      }
      return false;
    }
    else return true;
  }
  /* END CLASS STRING */


  /* CLASSES (CONST_)ITERATOR */
  string::iterator& string::iterator::operator++ ()
  {
    if (++pos > str->length()) pos = str->length();
    return *this;
  }

  string::iterator string::iterator::operator++ (int)
  {
    iterator it(*this);
    ++(*this);
    return it;
  }

  string::iterator& string::iterator::operator-- ()
  {
    if (!(pos--)) pos = 0;
    return *this;
  }

  string::iterator string::iterator::operator-- (int)
  {
    iterator it(*this);
    --(*this);
    return it;
  }

  string::iterator& string::iterator::operator= (const iterator& other)
  {
    str = other.str;
    pos = other.pos;
    return *this;
  }

  bool string::iterator::operator== (const iterator& other) const
  {
    return str == other.str && pos == other.pos;
  }

  bool string::iterator::operator!= (const iterator& other) const
  {
    return !(*this == other);
  }

  char& string::iterator::operator* () const
  {
    return (*str)[pos];
  }

  string::iterator::iterator () : str(&string::void_str), pos(0) {}

  string::iterator::iterator (string& s, size_t p) : str(&s), pos(p) {}

  string::iterator::iterator (const iterator& it) : str(it.str), pos(it.pos) {}

  /* ITERATOR -> CONST_ITERATOR */

  string::const_iterator& string::const_iterator::operator++ ()
  {
    if (++pos > str->length()) pos = str->length();
    return *this;
  }

  string::const_iterator string::const_iterator::operator++ (int)
  {
    const_iterator it(*this);
    ++(*this);
    return it;
  }

  string::const_iterator& string::const_iterator::operator-- ()
  {
    if (!(pos--)) pos = 0;
    return *this;
  }

  string::const_iterator string::const_iterator::operator-- (int)
  {
    const_iterator it(*this);
    --(*this);
    return it;
  }

  string::const_iterator& string::const_iterator::operator= (const const_iterator& other)
  {
    str = other.str;
    pos = other.pos;
    return *this;
  }

  bool string::const_iterator::operator== (const const_iterator& other) const
  {
    return str == other.str && pos == other.pos;
  }

  bool string::const_iterator::operator!= (const const_iterator& other) const
  {
    return !(*this == other);
  }

  const char string::const_iterator::operator* () const
  {
    return (*str)[pos];
  }

  string::const_iterator::const_iterator () : str(&string::void_str), pos(0) {}

  string::const_iterator::const_iterator (const string& s, size_t p) : str(&s), pos(p) {}

  string::const_iterator::const_iterator (const const_iterator& it) : str(it.str), pos(it.pos) {}
  /* END CLASSES (CONST_)ITERATOR */


  /* GLOBAL OPERATOR OVERLOADING */
  string operator+ (const string& lhs, const string& rhs)
  {
    return string(lhs) += rhs;
  }

  string operator+ (const string& lhs, const char* rhs)
  {
    return string(lhs) += rhs;
  }

  string operator+ (const char* lhs, const string& rhs)
  {
    return string(lhs) += rhs;
  }

  bool operator== (const string& lhs, const string& rhs)
  {
    return lhs.length() != rhs.length() ? false : lhs.compare(rhs) == 0;
  }

  bool operator== (const char* lhs, const string& rhs)
  {
    return rhs.compare(lhs) == 0;
  }

  bool operator== (const string& lhs, const char* rhs)
  {
    return lhs.compare(rhs) == 0;
  }

  bool operator!= (const string& lhs, const string& rhs)
  {
      return !(lhs == rhs);
  }

  bool operator!= (const char* lhs, const string& rhs)
  {
    return !(lhs == rhs);
  }

  bool operator!= (const string& lhs, const char* rhs)
  {
    return !(lhs == rhs);
  }

  bool operator< (const string& lhs, const string& rhs)
  {
    return lhs.compare(rhs) < 0;
  }

  bool operator< (const char* lhs, const string& rhs)
  {
    return rhs.compare(lhs) < 0;
  }

  bool operator< (const string& lhs, const char* rhs)
  {
    return lhs.compare(rhs) < 0;
  }

  bool operator<= (const string& lhs, const string& rhs)
  {
    return lhs.compare(rhs) <= 0;
  }

  bool operator<= (const char* lhs, const string& rhs)
  {
    return rhs.compare(lhs) <= 0;
  }

  bool operator<= (const string& lhs, const char* rhs)
  {
    return lhs.compare(rhs) <= 0;
  }

  bool operator> (const string& lhs, const string& rhs)
  {
    return !(lhs <= rhs);
  }

  bool operator> (const char* lhs, const string& rhs)
  {
    return !(lhs <= rhs);
  }

  bool operator> (const string& lhs, const char* rhs)
  {
    return !(lhs <= rhs);
  }

  bool operator>= (const string& lhs, const string& rhs)
  {
    return !(lhs < rhs);
  }

  bool operator>= (const char* lhs, const string& rhs)
  {
    return !(lhs < rhs);
  }

  bool operator>= (const string& lhs, const char* rhs)
  {
    return !(lhs < rhs);
  }
}
