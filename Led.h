#include "SerialOutput.h"
#include "Constants.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef ARDUINO_ARCH_ESP32
#include "analogWrite.h"
#endif // ARDUINO_ARCH_ESP32


int closestNumber(int n, int m)
{
	// find the quotient 
	int q = n / m;

	// 1st possible closest number 
	int n1 = m * q;

	// 2nd possible closest number 
	int n2 = (n * m) > 0 ? (m * (q + 1)) : (m * (q - 1));

	// if true, then n1 is the required closest number 
	if (abs(n - n1) < abs(n - n2))
		return n1;

	// else n2 is the required closest number     
	return n2;
}

class Led
{
	friend class RGBLed;
public:
	
	 enum class Brightness { _20, _50, _100 };
	
	Led( PIN i_ledPin = LED_BUILTIN , bool i_negative = false ) :m_pin(i_ledPin)  , m_negative (i_negative)
	{ 
		pinMode( m_pin, OUTPUT );
		turn_off();
	}

	void turn_on(Brightness i_brtness = Brightness::_100 );
	void turn_off()
	{
		( m_negative ) ? digitalWrite(m_pin, HIGH) : digitalWrite(m_pin, LOW);
		m_status = LedStS::OFF;
	}
	void blynk(Brightness i_brtness = Brightness::_100 )
	{
		(m_status == LedStS::OFF) ? turn_on(i_brtness) : turn_off();
	}

	void rapid_blynk( const unsigned long& i_time_ms );
	void fade(const unsigned long&  i_time_ms);

protected:
	enum class LedStS { ON, OFF, BLYNK };
	PIN m_pin;

	LedStS m_status;
	bool   m_negative;
};

class WimosBuildInLed : public Led
{
public:
	WimosBuildInLed(): Led(LED_BUILTIN){}

	void turn_on( Brightness i_brtness = Brightness::_100 )
	{
		switch (i_brtness)
		{
		case Brightness::_20:
			analogWrite(m_pin, (int)::map( 20 , 0, 100, 255, 0 ));
			break;
		case Brightness::_50:
			analogWrite(m_pin, (int)::map( 50, 0, 100, 255, 0 ));
			break;
		case Brightness::_100:
			digitalWrite(m_pin, LOW);
			break;
		}

		m_status = LedStS::ON;
	}

	void turn_off()
	{
		digitalWrite(m_pin, HIGH);
		m_status = LedStS::OFF;
	}
};

class RGBLed
{
public:
	enum class  LedType: char { Red = 0b001 ,Green = 0b010 , Blue = 0b100 };

	RGBLed(PIN i_green, PIN i_blue, PIN i_red , Led::Brightness i_brtns = Led::Brightness::_100 , bool i_negative = false ) :
		m_green(i_green, i_negative ), 
		m_blue(i_blue, i_negative ), 
		m_red(i_red, i_negative ) ,
		m_brightness(i_brtns)
	{
		m_lastUsedLed = (char)LedType::Green;
	}

	void turn_on( LedType i_type  )
	{
		turn_off();
		getLed(i_type).turn_on(m_brightness);
	}

	void turn_off ()
	{
		m_red.turn_off();
		m_blue.turn_off();
		m_green.turn_off();
	}

	void blynk ()
	{
		turn_on( (LedType) ( ( m_lastUsedLed == static_cast<char>(LedType::Blue) ) ? static_cast<char>(LedType::Red) : m_lastUsedLed << 1 ) ) ;
	}

	void blynk( LedType i_type )
	{		
		if ( m_lastUsedLed != static_cast<char>(i_type) ) 
			turn_off();

		getLed(i_type).blynk();
	}

	void rapid_blynk(const unsigned long& i_time_ms);

	void fade(const unsigned long&  i_time_ms, LedType i_type);

	void fade(const unsigned long&  i_time_ms);

protected:

	Led& getLed( LedType i_type )
	{
		m_lastUsedLed = static_cast<char>(i_type);

		switch (i_type)
		{
		case LedType::Red:
			return m_red;
		case LedType::Blue:
			return m_blue;
		case LedType::Green:
			return m_green;
		}
	}

	Led m_green;
	Led m_blue;
	Led m_red;
	
	Led::Brightness m_brightness;
	char m_lastUsedLed;
};

void Led::turn_on( Brightness i_brtness )
{
	if ( ! m_negative )
	{
		switch (i_brtness)
		{
			case Brightness::_20:
				analogWrite( m_pin, (int)::map(20, 0, 100, 0, 255) );
				break;
			case Brightness::_50:
				analogWrite( m_pin, (int)::map(50, 0, 100, 0, 255) );
				break;
			case Brightness::_100:
				digitalWrite( m_pin, HIGH );
				break;
		}
	}
	else
	{
		switch (i_brtness)
		{
			case Brightness::_20:
				analogWrite( m_pin, (int)::map(20, 0, 100, 255, 0) );
				break;
			case Brightness::_50:
				analogWrite( m_pin, (int)::map(50, 0, 100, 255, 0) );
				break;
			case Brightness::_100:
				digitalWrite( m_pin, LOW );
				break;
			}
	}
	
	m_status = LedStS::ON;
}

void Led::rapid_blynk( const unsigned long &i_time_ms )
{
	long iter = i_time_ms;
	auto sleep = 40;

	do
	{
		blynk( Brightness::_100 );
		delay((iter - sleep < 0) ? iter : sleep);
	} while ((iter -= sleep) > 0);

	turn_off();
}

void Led::fade ( const unsigned long &i_time_ms )
{
	
	long iter = i_time_ms;//closestNumber(i_time_ms,30);

    short brightness = 0;

	auto fadeAmount = 5;

	auto sleep = closestNumber( fadeAmount * i_time_ms, 512 /*256 * 2 */ ) / 512;

	this->turn_off();
	while( (iter -= sleep ) >= 0 )
	{
		brightness = brightness + fadeAmount;
		analogWrite( m_pin, constrain( brightness, 0 , 255 ) );
		// reverse the direction of the fading at the ends of the fade:
		if (brightness <= 0 || brightness >= 255) 
			fadeAmount = -fadeAmount;
		
		delay( sleep );
	}
	this->turn_off();
}

void RGBLed::fade(const unsigned long  &i_time_ms, LedType i_type)
{
	turn_off();

	this->getLed( i_type ).fade(i_time_ms);
}

void RGBLed::fade( const unsigned long  &i_time_ms )
{
	this->fade( i_time_ms / 3 , (LedType)((m_lastUsedLed == static_cast<char>(LedType::Blue)) ? static_cast<char>(LedType::Red) : m_lastUsedLed << 1));
	this->fade( i_time_ms / 3 , (LedType)((m_lastUsedLed == static_cast<char>(LedType::Blue)) ? static_cast<char>(LedType::Red) : m_lastUsedLed << 1));
	this->fade( i_time_ms / 3 , (LedType)((m_lastUsedLed == static_cast<char>(LedType::Blue)) ? static_cast<char>(LedType::Red) : m_lastUsedLed << 1));
}

void RGBLed::rapid_blynk(const unsigned long &i_time_ms)
{
	long iter = i_time_ms;
	auto sleep = 40; // to make it 25Hz

	do
	{
		blynk();
		delay((iter - sleep < 0) ? iter : sleep);
	} while ((iter -= sleep) > 0);

	turn_off();
}
