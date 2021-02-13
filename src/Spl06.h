#ifndef SPL06_H_INCLUDED
#define SPL06_H_INCLUDED

#include "DpsClass.h"
#include "util/dps310_config.h"
#include "Dps310.h"

class Spl06 : public Dps310
{

protected:
  void init(void);
};

#endif