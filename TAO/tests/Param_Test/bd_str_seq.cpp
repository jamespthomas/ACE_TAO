// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_str_seq.cpp
//
// = DESCRIPTION
//    tests bounded string sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"

#include "bd_str_seq.h"

ACE_RCSID(Param_Test, bd_str_seq, "$Id$")

// ************************************************************************
//               Test_Bounded_String_Sequence
// ************************************************************************

Test_Bounded_String_Sequence::Test_Bounded_String_Sequence (void)
  : opname_ (CORBA::string_dup ("test_bounded_strseq")),
    in_ (new Param_Test::Bounded_StrSeq),
    inout_ (new Param_Test::Bounded_StrSeq),
    out_ (0),
    ret_ (0)
{
}

Test_Bounded_String_Sequence::~Test_Bounded_String_Sequence (void)
{
}

const char *
Test_Bounded_String_Sequence::opname (void) const
{
  return this->opname_;
}

int
Test_Bounded_String_Sequence::init_parameters (Param_Test_ptr objref,
                                               CORBA::Environment &env)
{
  // Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  const char *choiceList[] =
  {
    "one",
    "two",
    "three"
  };

  CORBA::ULong len = this->in_->maximum ();

  // set the length of the sequence
  this->in_->length (len);
  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      //      char *str = gen->gen_string ();
      //this->in_[i] = str;
      this->in_[i] = choiceList[i%3];

    }
  return 0;
}

int
Test_Bounded_String_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::Bounded_StrSeq; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_Bounded_String_Sequence::run_sii_test (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Param_Test::Bounded_StrSeq_out out (this->out_.out ());
  this->ret_ = objref->test_bounded_strseq (this->in_.in (),
                                           this->inout_.inout (),
                                           out,
                                           env);
  return (env.exception () ? -1:0);
}

int
Test_Bounded_String_Sequence::add_args (CORBA::NVList_ptr param_list,
                                        CORBA::NVList_ptr retval,
                                                                                CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_Bounded_StrSeq, (void *) &this->in_.in (), 0);
  CORBA::Any inout_arg (Param_Test::_tc_Bounded_StrSeq, &this->inout_.inout (), 0);
  // ORB will allocate
  CORBA::Any out_arg (Param_Test::_tc_Bounded_StrSeq, 0, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value type
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_Bounded_StrSeq,
                                                  0,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Bounded_String_Sequence::check_validity (void)
{
  CORBA::Boolean flag = 0;
  if ((this->in_->length () == this->inout_->length ()) &&
      (this->in_->length () == this->out_->length ()) &&
      (this->in_->length () == this->ret_->length ()))
    {
      flag = 1; // assume all are equal
      // lengths are same. Now compare the contents
      for (CORBA::ULong i=0; i < this->in_->length () && flag; i++)
        {
          if (ACE_OS::strcmp (this->in_[i], this->inout_[i]) ||
              ACE_OS::strcmp (this->in_[i], this->out_[i]) ||
              ACE_OS::strcmp (this->in_[i], this->ret_[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Bounded_String_Sequence::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;

  *req->arguments ()->item (2, env)->value () >>= this->out_.out ();
  *req->result ()->value () >>= this->ret_.out ();

  return this->check_validity ();
}

void
Test_Bounded_String_Sequence::print_values (void)
{
  CORBA::ULong i;
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->in_.ptr () && (i < this->in_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  this->in_[i]? (const char *)this->in_[i]:"<nul>"));
    }
  if (!this->in_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->inout_.ptr () && (i < this->inout_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->inout_[i]? (const char *)this->inout_[i]:"<nul>")));
    }
  if (!this->inout_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\ninout sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->out_.ptr () && (i < this->out_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->out_[i]? (const char *)this->out_[i]:"<nul>")));
    }
  if (!this->out_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nout sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->ret_.ptr () && (i < this->ret_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->ret_[i]? (const char *)this->ret_[i]:"<nul>")));
    }
  if (!this->ret_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}

