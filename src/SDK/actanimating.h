#include "SDK.h"

#ifndef ACTANIMATING_H
#define ACTANIMATING_H

class CActAnimating
{
public:

	void			SetActivity( Activity act );
	inline Activity	GetActivity( void ) { return m_Activity; }

private:
	Activity	m_Activity;
};



#endif // ACTANIMATING_H
