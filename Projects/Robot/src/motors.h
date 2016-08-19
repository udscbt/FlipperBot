#ifndef MOTORS_H
#define MOTORS_H

class Motor
{
  public:  
  static const unsigned char LEFT;
  static const unsigned char RIGHT;
  static const unsigned char OLD;
  static const unsigned char NEW;

  static constexpr unsigned char PIN(const unsigned char);
  static inline const unsigned char pin(const unsigned char);
  
  Motor(const unsigned char config);
  
  void configure(const unsigned char config);
  
  void forward() const;
  void stop() const;
  void backward() const;
  void value(const char dir) const;

  protected:
  static const unsigned char STOP;
  static const unsigned char OLD_CW;
  static const unsigned char NEW_CW;
  static const unsigned char OLD_CCW;
  static const unsigned char NEW_CCW;
  
  unsigned char config;
  inline const unsigned char getPin() const;
  inline const bool checkAny(const unsigned char flags) const;
  inline const bool checkAll(const unsigned char flags) const;
};

constexpr unsigned char Motor::PIN(const unsigned char pin)
{
  return pin-1;
}

const unsigned char Motor::pin(const unsigned char pin)
{
  return pin-1;
}

inline const unsigned char Motor::getPin() const
{
  return (this->config%(1<<4))+1;
}

inline const bool Motor::checkAny(const unsigned char flags) const
{
  return this->config & flags;
}

inline const bool Motor::checkAll(const unsigned char flags) const
{
  return (this->config & flags) == flags;
}

#endif // MOTORS_H
