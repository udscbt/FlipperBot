/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/fbcp.string.h
 * New file name: ./main/fbcp.string.h
 * Identified jobs: 0
 *   Named: 0
 *   Unnamed: 0
 * Identified tasks: 0
 *   Named: 0
 *   Unnamed: 0
 */

#ifndef FBCP_STRING_H
#define FBCP_STRING_H

#include <cstddef>

namespace fbcp
{
  class string
  {
    public:  
      static const size_t npos = -1;
    
            string ();
      string (const string&);
      string (const char*, size_t size=npos);
      string (const char&);
      ~string ();
      
            string& operator=  (const string&);
      string& operator=  (const char*  );
      string& operator+= (const string&);
      string& operator+= (const char*  );
      char&   operator[] (size_t       );
      const char& operator[] (size_t) const;
      
            class iterator;
      class const_iterator;
      iterator       begin ();
      const_iterator begin () const;
      iterator       end   ();
      const_iterator end   () const;
      
            int compare (const string&) const;
      int compare (const char*  ) const;
      
            const char* c_str () const;
      operator const char* () const;
      
            bool empty () const;
      size_t length () const;
      size_t find (const string&, size_t pos = 0) const;
      size_t find (const char*  , size_t pos = 0) const;
      string substr (size_t pos = 0, size_t len = npos) const;
      bool startsWith (const string&, const string& separators = "", bool useSeparator = false) const;
      
    private:
      char* buf;
      size_t str_size;
      size_t buf_size;
      
      string (size_t);
      
      static string void_str;
  };
  
  class string::iterator
  {
    public:
      iterator ();
      iterator (string&, size_t);
      iterator (const iterator&);
      
      iterator& operator++ ();
      iterator  operator++ (int);
      iterator& operator-- ();
      iterator  operator-- (int);
      
      iterator& operator= (const iterator&);
      
      bool operator== (const iterator&) const;
      bool operator!= (const iterator&) const;
      
      char& operator* () const;
      
    private:
      string* str;
      size_t  pos;
  };
  
  class string::const_iterator
  {
    public:
      const_iterator ();
      const_iterator (const string&, size_t);
      const_iterator (const const_iterator&);
      
      const_iterator& operator++ ();
      const_iterator  operator++ (int);
      const_iterator& operator-- ();
      const_iterator  operator-- (int);
      
      const_iterator& operator= (const const_iterator&);
      
      bool operator== (const const_iterator&) const;
      bool operator!= (const const_iterator&) const;
      
      const char operator* () const;
      
    private:
      const string* str;
      size_t  pos;
  };
  
  string operator+ (const fbcp::string&, const fbcp::string&);
  string operator+ (const fbcp::string&, const char*        );
  string operator+ (const char*        , const fbcp::string&);

  bool operator== (const fbcp::string&, const fbcp::string&);
  bool operator== (const char*        , const fbcp::string&);
  bool operator== (const fbcp::string&, const char*        );

  bool operator!= (const fbcp::string&, const fbcp::string&);
  bool operator!= (const char*        , const fbcp::string&); 
  bool operator!= (const fbcp::string&, const char*        );

  bool operator<  (const fbcp::string&, const fbcp::string&);
  bool operator<  (const char*        , const fbcp::string&);
  bool operator<  (const fbcp::string&, const char*        );

  bool operator<= (const fbcp::string&, const fbcp::string&);
  bool operator<= (const char*        , const fbcp::string&);
  bool operator<= (const fbcp::string&, const char*        );

  bool operator>  (const fbcp::string&, const fbcp::string&);
  bool operator>  (const char*        , const fbcp::string&);
  bool operator>  (const fbcp::string&, const char*        );

  bool operator>= (const fbcp::string&, const fbcp::string&);
  bool operator>= (const char*        , const fbcp::string&);
  bool operator>= (const fbcp::string&, const char*        );
}

#endif 