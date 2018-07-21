#ifndef KTELEPHONEUA_H
#define KTELEPHONEUA_H

#include <pjsua2.hpp>

using namespace pj;

class UserAgent : public Account
{
 public:
  virtual void onRegState(OnRegStateParam &prm);
  
};

#endif // KTELEPHONEUA_H
