// -*- C++ -*-

//=============================================================================
/**
 *  @file    Synch_Invocation.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_SYNCH_INVOCATION_H
#define TAO_SYNCH_INVOCATION_H
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Remote_Invocation.h"

class TAO_Operation_Deatils;
class TAO_Synch_Reply_Dispatcher;

namespace TAO
{
  class Profile_Transport_Resolver;

  class TAO_Export Synch_Twoway_Invocation: public Remote_Invocation
  {
  public:
    Synch_Twoway_Invocation (Profile_Transport_Resolver &resolver,
                             TAO_Operation_Details &detail);

    int communicate (Argument **args,
                     int args_number
                     ACE_ENV_ARG_DECL);

  private:

    void check_reply_status (TAO_Synch_Reply_Dispatcher &rd,
                             Argument **args,
                             int args_number
                             ACE_ENV_ARG_DECL);
  };

  class TAO_Export Synch_Oneway_Invocation: public Synch_Twoway_Invocation
  {
  public:
    Synch_Oneway_Invocation (Profile_Transport_Resolver &resolver,
                             TAO_Operation_Details &detail);

    int communicate (Argument **args,
                     int args_number
                     ACE_ENV_ARG_DECL);

  };
}

#endif /*TAO_SYNCH_INVOCATION_H*/
