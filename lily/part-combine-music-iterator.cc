/*   
  part-combine-music-iterator.cc -- implement  Part_combine_music_iterator

  source file of the GNU LilyPond music typesetter
  
  (c) 2000 Jan Nieuwenhuizen <janneke@gnu.org>
 */

#include "part-combine-music.hh"
#include "part-combine-music-iterator.hh"
#include "translator-group.hh"
#include "musical-request.hh"
#include "warn.hh"

Part_combine_music_iterator::Part_combine_music_iterator ()
{
  combined_b_ = false;

  first_iter_p_ = 0;
  second_iter_p_ = 0;
}

Part_combine_music_iterator::~Part_combine_music_iterator ()
{
  delete second_iter_p_;
  delete first_iter_p_;
}

Moment
Part_combine_music_iterator::next_moment () const
{
  Moment first_next = first_iter_p_->next_moment ();
  Moment second_next = second_iter_p_->next_moment ();
  return first_next <? second_next;
}

bool
Part_combine_music_iterator::ok () const
{
  //hmm
  return first_iter_p_->ok ();
}

void
Part_combine_music_iterator::do_print () const
{
  first_iter_p_->print ();
  second_iter_p_->print ();
}

void
Part_combine_music_iterator::construct_children ()
{
  Part_combine_music const * m = dynamic_cast<Part_combine_music const*> (music_l_);
  
  first_iter_p_ = get_iterator_p (m->first_l ());
  second_iter_p_ = get_iterator_p (m->second_l ());
}

void
Part_combine_music_iterator::change_to (Music_iterator *it, String to_type,
					String to_id)
{
  Translator_group * current = it->report_to_l ();
  Translator_group * last = 0;

  /*
    Cut & Paste from from Auto_change_iterator from Change_iterator (ugh).

    TODO: abstract this function 
   */
  
  /* find the type  of translator that we're changing.
     
     If \translator Staff = bass, then look for Staff = *
   */
  while  (current && current->type_str_ != to_type)
    {
      last = current;
      current = current->daddy_trans_l_;
    }

  if (current && current->id_str_ == to_id)
    {
      String msg;
      msg += _ ("Can't switch translators, I'm there already");
    }
  
  if (current) 
    if (last)
      {
	Translator_group * dest = 
	  it->report_to_l ()->find_create_translator_l (to_type, to_id);
	current->remove_translator_p (last);
	dest->add_translator (last);
      }
    else
      {
	/*
	  We could change the current translator's id, but that would make 
	  errors hard to catch
	  
	   last->translator_id_str_  = change_l ()->change_to_id_str_;
	*/
	error (_f ("I'm one myself: `%s'", to_type.ch_C ()));
      }
  else
    error (_f ("none of these in my family: `%s'", to_id.ch_C ()));
}

Pitch_interrogate_req* first_spanish_inquisition; // nobody expects it
Pitch_interrogate_req* second_spanish_inquisition; // won't strike twice

void
Part_combine_music_iterator::do_process_and_next (Moment m)
{
  Moment first_next = first_iter_p_->next_moment ();
  Moment second_next = second_iter_p_->next_moment ();

#if 0
  if (first_next > m || second_next > m)
    return;
#endif

  /*
    different rhythm for combined voices: separate 
    same rhythm for separated voices: combine
  */
  if ((first_next != second_next && combined_b_)
      || (first_next == second_next && !combined_b_))
    {
      combined_b_ = !combined_b_;
      String to_id =  combined_b_ ? "first" : "second";

      Part_combine_music const * p = dynamic_cast<Part_combine_music const* > (music_l_);

      change_to (second_iter_p_, p->what_str_, to_id);

      /*
	A quick ugly hack to see if it works..
       */
#if 0
      Translator_group * fd = 
	first_iter_p_->report_to_l ()->find_create_translator_l (p->what_str_,
								 "first");
      Translator_group * sd = 
	second_iter_p_->report_to_l ()->find_create_translator_l (p->what_str_,
								  to_id);
#else
      Translator_group * fd = first_iter_p_->report_to_l ();
      Translator_group * sd = second_iter_p_->report_to_l ();
#endif
      
      if (combined_b_)
	{
	  fd->set_property ("verticalDirection", gh_int2scm (0));
	  fd->set_property ("noteHeadStyle", ly_symbol2scm ("default"));
	}
      else
	{
	  fd->set_property ("verticalDirection", gh_int2scm (1));
	  fd->set_property ("noteHeadStyle", ly_symbol2scm ("diamond"));
	  sd->set_property ("verticalDirection", gh_int2scm (-1));
	}
    }

  first_iter_p_->process_and_next (m);
  second_iter_p_->process_and_next (m);

  Music_iterator::do_process_and_next (m);


#if 0 

  /*
    TODO:

    * when combining two threads: check pitches, request stem directions

    * a2-engraver to put texts a2/Solo/SoloII as appropriate.

    * maybe the a2-engraver should set the stem directions, iso us?
    

    Maybe different modes exist?

    * Wind instruments (Flute I/II)
    * Hymnals:  


      Rules for Hymnals/SATB (John Henckel <henckel@iname.com>):

      1. if S and A differ by less than a third, the stems should be up/down.
      2. else if S and A have different values, the stems should be up/down.
      3. else if A sings "la" or higher, both S and A stems should be down.
      4. else both stems should be up

    * This may get really tricky: combining voices/staffs: string instruments

   */
  
  if (!first_spanish_inquisition)
    first_spanish_inquisition = new Pitch_interrogate_req;
  Music_iterator* fit = first_iter_p_->try_music (first_spanish_inquisition);

  if (!second_spanish_inquisition)
    second_spanish_inquisition = new Pitch_interrogate_req;
  Music_iterator* sit = second_iter_p_->try_music (second_spanish_inquisition);

  if (fit && first_spanish_inquisition->pitch_arr_.size ())
    {
      Musical_pitch p = spanish_inquisition->pitch_arr_[0];
      Direction s = Direction (sign(p.steps ()));
      if (s != where_dir_)
	{
	  where_dir_ = s;
	  String to_id =  (s >= 0) ?  "up" : "down";
	  Part_combine_music const * auto_mus = dynamic_cast<Part_combine_music const* > (music_l_);

	  change_to (it, auto_mus->what_str_, to_id);	  
	}
    }

  first_spanish_inquisition->pitch_arr_.clear ();
  second_spanish_inquisition->pitch_arr_.clear ();
#endif
}

Music_iterator*
Part_combine_music_iterator::try_music_in_children (Music *m) const
{
  Music_iterator * i =  first_iter_p_->try_music (m);
  if (i)
    return i;
  else
    return second_iter_p_->try_music (m);
}

